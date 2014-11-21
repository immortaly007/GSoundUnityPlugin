/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundOutputDevice.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundOutputDevice class implementation
 * 
 * License:
 * 
 *     Copyright (C) 2010-12 Carl Schissler, University of North Carolina at Chapel Hill.
 *     All rights reserved.
 *     
 *     Permission to use, copy, modify, and distribute this software and its
 *     documentation for educational, research, and non-profit purposes, without
 *     fee, and without a written agreement is hereby granted, provided that the
 *     above copyright notice, this paragraph, and the following four paragraphs
 *     appear in all copies.
 *     
 *     Permission to incorporate this software into commercial products may be
 *     obtained by contacting the University of North Carolina at Chapel Hill.
 *     
 *     This software program and documentation are copyrighted by Carl Schissler and
 *     the University of North Carolina at Chapel Hill. The software program and
 *     documentation are supplied "as is", without any accompanying services from
 *     the University of North Carolina at Chapel Hill or the authors. The University
 *     of North Carolina at Chapel Hill and the authors do not warrant that the
 *     operation of the program will be uninterrupted or error-free. The end-user
 *     understands that the program was developed for research purposes and is advised
 *     not to rely exclusively on the program for any reason.
 *     
 *     IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR ITS
 *     EMPLOYEES OR THE AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 *     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
 *     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE
 *     UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED
 *     OF THE POSSIBILITY OF SUCH DAMAGE.
 *     
 *     THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 *     DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY
 *     STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS
 *     ON AN "AS IS" BASIS, AND THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND
 *     THE AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 *     ENHANCEMENTS, OR MODIFICATIONS.
 * 
 * 
 * Contact Information:
 *     
 *     Please send all bug reports and other contact to:
 *     Carl Schissler
 *     carl.schissler@gmail.com
 * 
 *     Updates and downloads are available at the main GSound web page:
 *     http://gamma.cs.unc.edu/GSOUND/
 * 
 */


#include "SoundOutputDevice.h"


#include "SampleMath.h"


// Include platform-specific headers
#if defined(GSOUND_PLATFORM_APPLE)
	#undef __APPLE_ALTIVEC__
	#include <CoreServices/CoreServices.h>
	#include <CoreAudio/CoreAudio.h>
	#include <unistd.h>
	#include <AudioUnit/AudioUnit.h>

#elif defined(GSOUND_PLATFORM_WINDOWS)
	#include <Windows.h>
	#include <MmDeviceapi.h>
	#include <Audioclient.h>
	#include <Audiopolicy.h>
	#include <Functiondiscoverykeys_devpkey.h>
#endif


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Output Device Wrapper Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundOutputDevice:: SoundOutputDeviceWrapper
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			GSOUND_INLINE SoundOutputDeviceWrapper( const SoundDeviceID& newDeviceID )
				:	internalDeviceID( newDeviceID ),
					isRunning( false )
			{
				// If the device ID is not valid, do nothing to initialize the device and go into 'dummy' mode.
				if ( internalDeviceID == SoundDeviceID::INVALID_DEVICE_ID )
					return;
				
#if defined(GSOUND_PLATFORM_APPLE)
				
				// Register the audio output callback function
				OSStatus error = AudioDeviceCreateIOProcID( internalDeviceID,
															SoundOutputDeviceWrapper::audioOutputCallback,
															this, &ioProcID );
				
				// Make sure that there was no error.
				GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
						"An error was encountered while registering the audio output callback function.",
						error );
				
#elif defined(GSOUND_PLATFORM_WINDOWS)
				
				//****************************************************************
				// Create an object that enumerates the connected devices.
				
				CoInitialize( NULL );
				
				const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
				const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
				
				IMMDeviceEnumerator* deviceEnumerator;
				
				HRESULT result = CoCreateInstance( CLSID_MMDeviceEnumerator, NULL,
													CLSCTX_ALL, IID_IMMDeviceEnumerator,
													(void**)&deviceEnumerator );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while creating an audio device enumerator object.",
							result );
				
				// Get the collection of output devices.
				IMMDeviceCollection* deviceCollection;
				
				result = deviceEnumerator->EnumAudioEndpoints( eRender, DEVICE_STATE_ACTIVE, &deviceCollection );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying the connected audio output devices.",
							result );
				
				//****************************************************************
				// Get the output device at the index of this output device.
				
				result = deviceCollection->Item( (UINT)internalDeviceID, &device );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying an audio output device.",
							result );

				//****************************************************************
				// Get the audio client interface for this output device.
				
				result = device->Activate( __uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&audioClient );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while creating an audio client for an audio output device.",
							result );
				
				//****************************************************************
				// Initialize the audio client object.
				
				// Get the current stream format for this output device.
				WAVEFORMATEX* streamFormat;
				
				result = audioClient->GetMixFormat( (WAVEFORMATEX**)&streamFormat );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying an audio output device's format.",
							result );
				
				// Get the length of the audio device's minimum period in reference time units (100's of nanoseconds).
				REFERENCE_TIME devicePeriod;
				
				audioClient->GetDevicePeriod( NULL, &devicePeriod );
				
				// Initialize audio client object to use shared mode with the above parameters.
				audioClient->Initialize( AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
										devicePeriod, 0, streamFormat, NULL );
				
				// Free the stream format structure.
				CoTaskMemFree( streamFormat );
				
				//****************************************************************
				// Create an event object which signals when an output buffer is free.
				
				bufferFreeEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
				
				GSOUND_ASSERT_MESSAGE_CODE( bufferFreeEvent != NULL,
							"An error was encountered while creating a signaling event object.",
							result );
				
				// Register the event with the audio client so that it signals the event whenever a buffer
				// become available.
				result = audioClient->SetEventHandle( bufferFreeEvent );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while registering an audio output buffer free signal event.",
							result );
				
				//****************************************************************
				// Get an interface to the rendering system from the audio client interface.
				
				result = audioClient->GetService( __uuidof(IAudioRenderClient), (void**)&audioRenderClient );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while getting an interface to the audio output system",
							result );
				
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			GSOUND_INLINE ~SoundOutputDeviceWrapper()
			{
				// If the device is invalid, don't do any cleanup because none is necessary.
				if ( internalDeviceID == SoundDeviceID::INVALID_DEVICE_ID )
					return;
				
#if defined(GSOUND_PLATFORM_APPLE)
				
				// Unregister the audio output callback function.
				OSStatus error = AudioDeviceDestroyIOProcID( internalDeviceID, ioProcID );
				
				// Make sure that there was no error.
				GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
							"An error was encountered while destroying the audio output callback ID object.",
							error );
				
#elif defined(GSOUND_PLATFORM_WINDOWS)
				
				// Release the audio render client object.
				audioRenderClient->Release();
				
				// Release the audio client object.
				audioClient->Release();

				// Release the device.
				device->Release();

				// Destroy the driver thread object.
				CloseHandle( driverThread );
				
				// Destroy the buffer-free event object.
				CloseHandle( bufferFreeEvent );
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Audio Stream Start Method
			
			
			
			
#if defined(GSOUND_PLATFORM_WINDOWS)
			GSOUND_INLINE void startStream()
			{
				if ( !isRunning )
				{
					// Create the driver thread.
					driverThread = CreateThread( NULL, 0, driverThreadEntry, (LPVOID)this, 0, NULL );
					
					// Start the output stream.
					HRESULT result = audioClient->Start();
					
					GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
								"An error was encountered while tring to start audio output for device.",
								result );
					
					isRunning = true;
				}
			}
#endif
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Audio Stream Stop Method
			
			
			
			
#if defined(GSOUND_PLATFORM_WINDOWS)
			GSOUND_INLINE void stopStream()
			{
				if ( isRunning )
				{
					WaitForSingleObject( driverThread, 100 );
					
					// Stop the output stream.
					HRESULT result = audioClient->Stop();
					
					GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
								"An error was encountered while tring to stop audio output for device.",
								result );
					
					isRunning = false;
				}
			}
#endif
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			SoundStream inputStream;
			
			SampleRateConverter sampleRateConverter;
			
			
			ChannelIOMap channelMap;
			
			
			Bool isRunning;
			
			mutable Mutex outputMutex;
			
			
			Size numChannels;
			
			
#if defined(GSOUND_PLATFORM_APPLE)
			
			AudioDeviceIOProcID ioProcID;
			
			AudioDeviceID internalDeviceID;
			
#elif defined(GSOUND_PLATFORM_WINDOWS)
			
			SoundDeviceID internalDeviceID;
			
			IMMDevice* device;
			IAudioClient* audioClient;
			IAudioRenderClient* audioRenderClient;
			
			// A handle to the thread which drives the audio rendering pipeline.
			HANDLE driverThread;
			
			// A handle to an event object which signals when an output buffer is available.
			HANDLE bufferFreeEvent;
			
#endif
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Audio Output Callback Method
			
			
#if defined(GSOUND_PLATFORM_APPLE)
			static OSStatus audioOutputCallback( AudioDeviceID deviceID, const AudioTimeStamp* currentTime,
												const AudioBufferList* inputData, const AudioTimeStamp* inputTime,
												AudioBufferList* outputData, const AudioTimeStamp* outputTime,
												void* clientData );
#elif defined(GSOUND_PLATFORM_WINDOWS)
			static DWORD WINAPI driverThreadEntry( LPVOID lpParam );
#endif
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Audio Output Callback Method
//############		
//##########################################################################################
//##########################################################################################




#if defined(GSOUND_PLATFORM_APPLE)

OSStatus SoundOutputDevice:: SoundOutputDeviceWrapper:: audioOutputCallback( AudioDeviceID deviceID,
											const AudioTimeStamp* currentTime,
											const AudioBufferList* inputData, const AudioTimeStamp* inputTime,
											AudioBufferList* outputData, const AudioTimeStamp* outputTime,
											void* clientData )
{
	if ( clientData != NULL )
	{
		// Get a pointer to the device object for which we are producing audio.
		SoundOutputDeviceWrapper* wrapper = (SoundOutputDeviceWrapper*)clientData;
		
		// If the device isn't currently running, do nothing and return from the function.
		if ( !wrapper->isRunning )
			return noErr;
		
		// Acquire the mutex which indicates that audio is currently being output.
		wrapper->outputMutex.acquire();
		
		// Calculate the number of samples to read from the input source.
		Size numSamplesToRead = outputData->mBuffers[0].mDataByteSize /
									(outputData->mBuffers[0].mNumberChannels*sizeof(Sample32f));
		
		Size totalSamplesRead = 0;
		
		// Get the audio data from the device's audio source, after being potentially converted
		// to a different sample rate.
		while ( wrapper->sampleRateConverter.hasOutputRemaining() && totalSamplesRead < numSamplesToRead )
		{
			totalSamplesRead += wrapper->sampleRateConverter.getSamples( 
												wrapper->inputStream, totalSamplesRead, numSamplesToRead - totalSamplesRead );
		}
		
		// Get the nubmer of input channels of audio.
		Size numInputChannels = wrapper->sampleRateConverter.getNumberOfChannels();
		
		
		for ( Index i = 0; i < numInputChannels; i++ )
		{
			// There is an output channel mapping for this input channel. Put the channel's
			// data in that output channel.
			
			// Get the output channel index for this input channel.
			Index outputChannelIndex;
			
			if ( wrapper->channelMap.inputChannelHasMapping(i) )
				outputChannelIndex = wrapper->channelMap.getInputChannelMapping(i);
			else
			{
				// If the channel mapping has the 1-to-1 input to output mapping output channel
				// unassigned, then use it for the input channel. If a mapping exists for that
				// output channel, don't use this input channel.
				if ( !wrapper->channelMap.outputChannelHasMapping(i) )
					outputChannelIndex = i;
				else
					continue;
			}
			
			// Find the buffer corresponding to this output channel index.
			for ( Index bufferIndex = 0, currentChannelCount = 0; bufferIndex < outputData->mNumberBuffers; bufferIndex++ )
			{
				Size numBufferChannels = outputData->mBuffers[bufferIndex].mNumberChannels;
				
				// If the specified output channel is not within this buffer, then try the next buffer.
				if ( currentChannelCount + numBufferChannels < outputChannelIndex )
				{
					// Update the current number of channels encountered so far.
					currentChannelCount += numBufferChannels;
					continue;
				}
				else
				{
					// Calculate the index of the output channel within the buffer.
					Index bufferChannelIndex = outputChannelIndex - currentChannelCount;
					
					const Sample* input = wrapper->inputStream.getBuffer(0).getChannelStart(i);
					const Sample* const inputEnd = input + totalSamplesRead;
					Sample32f* output = (Sample32f*)outputData->mBuffers[bufferIndex].mData + bufferChannelIndex;
					
					while ( input != inputEnd )
					{
						*output = sample::convert<Sample32f>(*input);
						output += numBufferChannels;
						input++;
					}
					
					break;
				}
			}
		}
		
		
		// Release the mutex which indicates that audio is currently being output.
		wrapper->outputMutex.release();
	}
	
	return noErr;
}




#elif defined(GSOUND_PLATFORM_WINDOWS)

DWORD WINAPI SoundOutputDevice:: SoundOutputDeviceWrapper:: driverThreadEntry( LPVOID lpParam )
{
	SoundOutputDeviceWrapper* wrapper = (SoundOutputDeviceWrapper*)lpParam;
	Bool isRunning = true;
	SampleType sampleType;
	Size numChannels;
	
	// Get the current stream format for this output device.
	WAVEFORMATEX* streamFormat;
	
	HRESULT result = wrapper->audioClient->GetMixFormat( (WAVEFORMATEX**)&streamFormat );
	
	GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
				"An error was encountered while querying an audio output device's format.",
				result );
	
	numChannels = streamFormat->nChannels;
	
	if ( streamFormat->wFormatTag == WAVE_FORMAT_IEEE_FLOAT ||
		(streamFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE &&
		((WAVEFORMATEXTENSIBLE*)streamFormat)->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT) )
	{
		sampleType = SampleType::SAMPLE_32F;
	}
	else
	{
		switch ( streamFormat->wBitsPerSample )
		{
			case 8: sampleType = SampleType::SAMPLE_8; break;
			case 16: sampleType = SampleType::SAMPLE_16; break;
			case 24: sampleType = SampleType::SAMPLE_24; break;
			case 32: sampleType = SampleType::SAMPLE_32; break;
			case 64: sampleType = SampleType::SAMPLE_64; break;
			default:
				GSOUND_ASSERT_MESSAGE( 0, "Unsupported sample type for sound output device." );
		}
	}
	
	// Free the stream format structure.
	CoTaskMemFree( streamFormat );
	

	while ( isRunning )
	{
		// Wait until there is a buffer available for output.
		DWORD waitStatus = WaitForSingleObject( wrapper->bufferFreeEvent, INFINITE );
		
		GSOUND_ASSERT_MESSAGE( waitStatus == WAIT_OBJECT_0,
					"An error was encountered while waiting for a free audio output buffer." );
		
		// Acquire the mutex which indicates that audio is currently being output.
		wrapper->outputMutex.acquire();
		
		//*******************************************************************************
		
		// Get the size of the output buffer.
		// This is the number of samples in the buffer times the number of channels.
		UINT32 bufferSize;
		UINT32 bufferPadding;
		
		result = wrapper->audioClient->GetBufferSize( &bufferSize );
		
		GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
					"An error was encountered while querying the size of an audio output buffer.",
					result );
		
		result = wrapper->audioClient->GetCurrentPadding( &bufferPadding );
		
		GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
					"An error was encountered while querying the current padding of an audio output buffer.",
					result );
		
		UINT32 bufferSizeInSamples = (bufferSize - bufferPadding) / numChannels;
		
		// Get the output buffer.
		BYTE* bufferData;
		
		result = wrapper->audioRenderClient->GetBuffer( bufferSizeInSamples, &bufferData );
		
		GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
					"An error was encountered while getting an audio output buffer from the device.",
					result );
		
		//*******************************************************************************
		
		// Calculate the number of samples to read from the input source.
		Size numSamplesToRead = bufferSizeInSamples;
		Size totalSamplesRead = 0;
		
		// Get the audio data from the device's audio source, after being potentially converted
		// to a different sample rate.
		while ( wrapper->sampleRateConverter.hasOutputRemaining() && totalSamplesRead < numSamplesToRead )
		{
			totalSamplesRead += wrapper->sampleRateConverter.getSamples( wrapper->inputStream, totalSamplesRead,
																		numSamplesToRead - totalSamplesRead );
		}
		
		if ( totalSamplesRead != 0 )
		{
			// Get the nubmer of input channels of audio.
			Size numInputChannels = wrapper->sampleRateConverter.getNumberOfChannels();
			Size numOutputChannels = numChannels;
			
			for ( Index i = 0; i < numOutputChannels; i++ )
			{
				// Get the input channel index for this output channel.
				Index inputChannelIndex;
				Bool shouldWriteSilence = false;
				
				if ( wrapper->channelMap.outputChannelHasMapping(i) )
				{
					inputChannelIndex = wrapper->channelMap.getOutputChannelMapping(i);
					
					// Make sure that the channel mapping is valid.
					if ( inputChannelIndex >= numInputChannels )
						shouldWriteSilence = true;
				}
				else
				{
					// If the channel mapping has the 1-to-1 input to output mapping output channel
					// unassigned, then use it for the input channel. If a mapping exists for that
					// output channel, don't use this input channel.
					if ( !wrapper->channelMap.inputChannelHasMapping(i) )
						inputChannelIndex = i;
					else
						shouldWriteSilence = true;
				}
				
				if ( shouldWriteSilence )
				{
					// Write silence to the unused output channel.
					Sample32f* output = (Sample32f*)bufferData + i;
					Sample32f* outputEnd = output + numOutputChannels*totalSamplesRead;
					
					while ( output != outputEnd )
					{
						*output = sample::convert<Sample32f>( 0 );
						output += numOutputChannels;
					}
				}
				else
				{
					// Write the input channel's audio to the current output channel.
					const Sample* input = wrapper->inputStream.getBuffer(0).getChannelStart(inputChannelIndex);
					const Sample* const inputEnd = input + totalSamplesRead;
					Sample32f* output = (Sample32f*)bufferData + i;
					
					while ( input != inputEnd )
					{
						*output = sample::convert<Sample32f>(*input);
						output += numOutputChannels;
						input++;
					}
				}
			}
			
			//*******************************************************************************
			
			// Release the output buffer.
			result = wrapper->audioRenderClient->ReleaseBuffer( UINT32(totalSamplesRead), 0 );
			
		}
		else
		{
			// No samples were read from the input, tell the system to interpret the output
			// as a buffer full of zeros.
			
			// Release the output buffer.
			result = wrapper->audioRenderClient->ReleaseBuffer( bufferSizeInSamples, AUDCLNT_BUFFERFLAGS_SILENT );
		}
		
		GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
					"An error was encountered while releasing an audio output buffer to the device.",
					result );
		
		// Release the mutex which indicates that audio is currently being output.
		wrapper->outputMutex.release();
		
		isRunning = wrapper->isRunning;
	}
	
	return 0;
}


#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundOutputDevice:: SoundOutputDevice( const SoundDeviceID& newDeviceID )
	:	wrapper( util::construct<SoundOutputDeviceWrapper>( newDeviceID ) )
{
	initializeDeviceData();
	wrapper->sampleRateConverter.setSampleRate( this->getSampleRate() );
}




SoundOutputDevice:: SoundOutputDevice( const SoundOutputDevice& other )
	:	wrapper( util::construct<SoundOutputDeviceWrapper>( other.getID() ) )
{
	// Initialize data about this device.
	initializeDeviceData();
	
	// Use the same source of input audio for this device object.
	this->setInput( other.getInput(), other.getChannelMap() );
	wrapper->sampleRateConverter.setSampleRate( this->getSampleRate() );
	
	// If the other device object was outputing audio, start outputing audio from this device.
	if ( other.isRunning() )
		this->start();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundOutputDevice:: ~SoundOutputDevice()
{
	// Stop outputing audio if the device is currently running.
	if ( wrapper->isRunning )
		this->stop();
	
	// Destroy the sound output device wrapper object.
	util::destruct( wrapper );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundOutputDevice& SoundOutputDevice:: operator = ( const SoundOutputDevice& other )
{
	if ( this != &other )
	{
		// Stop outputing audio if the device is currently running.
		if ( wrapper->isRunning )
			this->stop();
		
		// Destroy the sound output device wrapper object.
		util::destruct( wrapper );
		
		// Clear the list of native sample rates.
		nativeSampleRates.clear();
		
		//*********************************************************************
		
		// Create a copy of the output device wrapper object.
		wrapper = util::construct<SoundOutputDeviceWrapper>( other.getID() );
		
		// Initialize data about the device.
		initializeDeviceData();
			
		// Copy the input source and channel mapping.
		this->setInput( other.getInput(), other.getChannelMap() );
		
		// If the other device object was outputing audio, start outputing audio from this device.
		if ( other.isRunning() )
			this->start();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Output Start/Stop Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundOutputDevice:: start()
{
	// If the device is already running or is invalid, there is nothing to be done.
	if ( wrapper->isRunning || (UInt)wrapper->internalDeviceID == SoundDeviceID::INVALID_DEVICE_ID )
		return;
	
#if defined(GSOUND_PLATFORM_APPLE)
	
	// Start outputing audio.
	OSStatus error = AudioDeviceStart( wrapper->internalDeviceID, wrapper->ioProcID );
	
	// Make sure that no error has occurred.
	GSOUND_ASSERT_MESSAGE( error == noErr, "An error was encountered while trying to start audio output for device." );
	
	// Set a flag which indicates that the device is currently running.
	wrapper->isRunning = true;
	
#elif defined(GSOUND_PLATFORM_WINDOWS)
	
	wrapper->startStream();
	
#endif
}




void SoundOutputDevice:: stop()
{
	// If the device isn't currently running or is invalid, there is nothing to be done.
	if ( !wrapper->isRunning || (UInt)wrapper->internalDeviceID == SoundDeviceID::INVALID_DEVICE_ID )
		return;
	
#if defined(GSOUND_PLATFORM_APPLE)
	
	// Stop outputing audio.
	OSStatus error = AudioDeviceStop( wrapper->internalDeviceID, wrapper->ioProcID );
	
	// Make sure that no error has occurred.
	GSOUND_ASSERT_MESSAGE( error == noErr, "An error was encountered while trying to stop audio output for device." );
	
#elif defined(GSOUND_PLATFORM_WINDOWS)
	
	wrapper->stopStream();
	
#endif
	
	// Set a flag which indicates that the device is currently running.
	wrapper->isRunning = false;
}




Bool SoundOutputDevice:: isRunning() const
{
	return wrapper->isRunning;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Count Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Size SoundOutputDevice:: getNumberOfChannels() const
{
	return wrapper->numChannels;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Rate Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Float SoundOutputDevice:: getSampleRate() const
{
	// If the device is invalid, do nothing and return a sample rate of 0.
	if ( (UInt)wrapper->internalDeviceID == SoundDeviceID::INVALID_DEVICE_ID )
		return Float(0);
	
#if defined(GSOUND_PLATFORM_APPLE)
	
	::UInt32 propertySize = sizeof(::Float64);
	::Float64 sampleRate;
	
	// Query the current sample rate of the device.
	OSStatus error = AudioDeviceGetProperty( wrapper->internalDeviceID, 0, false,
											kAudioDevicePropertyNominalSampleRate,
											&propertySize, &sampleRate );
	
	// Make sure that no error has occurred.
	GSOUND_ASSERT_MESSAGE( error == noErr, "An error was encountered while querying an audio output device's current sample rate." );
	
	return Float(sampleRate);
	
#elif defined(GSOUND_PLATFORM_WINDOWS)
	
	// Get the current stream format for this output device.
	WAVEFORMATEXTENSIBLE* streamFormat;
	
	wrapper->audioClient->GetMixFormat( (WAVEFORMATEX**)&streamFormat );
	
	Float sampleRate = Float(streamFormat->Format.nSamplesPerSec);
	
	// Release the stream format object.
	CoTaskMemFree( streamFormat );
	
	return sampleRate;
	
#endif
}




Bool SoundOutputDevice:: setSampleRate( Float newSampleRate )
{
	// If the device is invalid, do nothing and return FALSE.
	if ( (UInt)wrapper->internalDeviceID == SoundDeviceID::INVALID_DEVICE_ID )
		return false;
	
	// Acquire the mutex which indicates that audio is currently being output.
	wrapper->outputMutex.acquire();
	
#if defined(GSOUND_PLATFORM_APPLE)
	
	// Make sure that the sample rate of the device can be changed.
	::Boolean isWritable;
	
	OSStatus error = AudioDeviceGetPropertyInfo( wrapper->internalDeviceID, 0, false,
													kAudioDevicePropertyNominalSampleRate, NULL, &isWritable );
	
	// Make sure that no error has occurred.
	GSOUND_ASSERT_MESSAGE( error == noErr, "An error was encountered while determining whether or not the sample\
 rate for an audio output device can be changed." );
	
	// If the sample rate of the device cannot be changed, return false.
	if ( !isWritable )
		return false;
	
	// Convert the incoming sample rate to the necessary type.
	::Float64 sampleRate = ::Float64(newSampleRate);
	
	// Try setting the sample rate of the device to the new value.
	error = AudioDeviceSetProperty( wrapper->internalDeviceID, NULL, 0, false,
									kAudioDevicePropertyNominalSampleRate, sizeof(::Float64), &sampleRate );
	
	// Release the mutex which indicates that audio is currently being output.
	wrapper->outputMutex.release();
	
	if ( error == noErr )
	{
		// The sample rate change operation was successful, inform the sample rate converter object.
		wrapper->sampleRateConverter.setSampleRate( newSampleRate );
		
		return true;
	}
	else
		return false;
	
#elif defined(GSOUND_PLATFORM_WINDOWS)
	
	
	return false;
	
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device ID Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundDeviceID SoundOutputDevice:: getID() const
{
	return SoundDeviceID(wrapper->internalDeviceID);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Validity Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundOutputDevice:: isValid() const
{
	return wrapper->internalDeviceID != SoundDeviceID::INVALID_DEVICE_ID;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Input Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundOutput* SoundOutputDevice:: getInput() const
{
	return input;
}




void SoundOutputDevice:: setInput( SoundOutput* newInput )
{
	// Acquire the mutex which indicates that audio is currently being output.
	wrapper->outputMutex.acquire();
	
	input = newInput;
	wrapper->sampleRateConverter.setInput( newInput );
	wrapper->channelMap = ChannelIOMap();
	
	// Release the mutex which indicates that audio is currently being output.
	wrapper->outputMutex.release();
}




void SoundOutputDevice:: setInput( SoundOutput* newInput, const ChannelIOMap& newChannelMap )
{
	// Acquire the mutex which indicates that audio is currently being output.
	wrapper->outputMutex.acquire();
	
	input = newInput;
	wrapper->sampleRateConverter.setInput( newInput );
	wrapper->channelMap = newChannelMap;
	
	// Release the mutex which indicates that audio is currently being output.
	wrapper->outputMutex.release();
}




void SoundOutputDevice:: removeInput()
{
	// Acquire the mutex which indicates that audio is currently being output.
	wrapper->outputMutex.acquire();
	
	input = NULL;
	wrapper->sampleRateConverter.removeInput();
	
	// Release the mutex which indicates that audio is currently being output.
	wrapper->outputMutex.release();
}




Bool SoundOutputDevice:: hasInput() const
{
	return input != NULL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Map Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




const ChannelIOMap& SoundOutputDevice:: getChannelMap() const
{
	return wrapper->channelMap;
}




void SoundOutputDevice:: setChannelMap( const ChannelIOMap& newChannelMap )
{
	// Acquire the mutex which indicates that audio is currently being output.
	wrapper->outputMutex.acquire();
	
	wrapper->channelMap = newChannelMap;
	
	// Release the mutex which indicates that audio is currently being output.
	wrapper->outputMutex.release();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Data Initialization Method
//############		
//##########################################################################################
//##########################################################################################




void SoundOutputDevice:: initializeDeviceData()
{
	// If the device is invalid, do nothing and return a sample rate of 0.
	if ( (UInt)wrapper->internalDeviceID == SoundDeviceID::INVALID_DEVICE_ID )
		return;
	
#if defined(GSOUND_PLATFORM_APPLE)
	
	// Declare a variable to hold the error status for each call to a Core Audio function.
	OSStatus error;
	
	//**************************************************************************
	// Get the name of this output device.
	
	// Get the length of the device's name string.
	::UInt32 nameLength;
	
	error = AudioDeviceGetPropertyInfo( wrapper->internalDeviceID, 0, false,
										kAudioDevicePropertyDeviceName, &nameLength, NULL );
	
	// Make sure that there was no error.
	GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
				"An error was encountered while querying the name of an audio output device.",
				error );
	
	// Allocate an array of characters which will hold the device's name.
	Char* cString = util::allocate<Char>( nameLength / sizeof(char) );
	
	// Get the device's name string.
	error = AudioDeviceGetProperty( wrapper->internalDeviceID, 0, false, 
									kAudioDevicePropertyDeviceName, &nameLength, cString );
	
	// Set the name string object to contain the contents of this device name string.
	name = String( cString );
	
	// Deallocate the temporary array of characters which held the name string.
	util::deallocate( cString );
	
	
	//**************************************************************************
	// Get the native sample rates for this output device
	
	// Get the number of samples rates.
	::UInt32 propertySize;
	
	error = AudioDeviceGetPropertyInfo( wrapper->internalDeviceID, 0, false,
										kAudioDevicePropertyAvailableNominalSampleRates,
										&propertySize, NULL );
	
	// Make sure that there was no error.
	GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
		"An error was encountered while querying an audio output device for the number of available sample rates.",
		error );
	
	Size numSamplesRates = propertySize / sizeof(AudioValueRange);
	
	// Allocate an array of value range structures to hold the valid device sample rates.
	AudioValueRange* sampleRates = util::allocate<AudioValueRange>( numSamplesRates );
	
	// Query the native sample rates for the device.
	error = AudioDeviceGetProperty( wrapper->internalDeviceID, 0, false,
									kAudioDevicePropertyAvailableNominalSampleRates,
									&propertySize, sampleRates );
	
	// Make sure that there was no error.
	GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
				"An error was encountered while querying an audio output device for available sample rates.",
				error );
	
	// Make sure that the list of native sample rates is empty.
	nativeSampleRates.clear();
	
	// Add all sample rates in the temporary array to the list of native sample rates.
	for ( Index i = 0; i < numSamplesRates; i++ )
	{
		// Use the minimum sample rate value, not sure why these are specified as a range.
		nativeSampleRates.add( sampleRates[i].mMinimum );
	}
	
	// Deallocate the array of sample rates.
	util::deallocate( sampleRates );
	
	
	//**************************************************************************
	// Get the number of channels that the output device has.
	
	// Allocate a temporary audio buffer list to hold the stream configuration.
	AudioBufferList bufferList;
	::UInt32 bufferListSize = sizeof(AudioBufferList);
	
	// Get the output stream configuration.
	error = AudioDeviceGetProperty( wrapper->internalDeviceID, 0, false, kAudioDevicePropertyStreamConfiguration,
									&bufferListSize, &bufferList );
	
	// Make sure that an error has not been encountered.
	GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
				"An error was encountered while querying an audio output device for its audio stream configuration.",
				error );
	
	wrapper->numChannels = 0;
	
	// For each stream of the device, add the number of channels in that stream to
	// the total number of channels.
	for ( Index i = 0; i < bufferList.mNumberBuffers; i++ )
		wrapper->numChannels += bufferList.mBuffers[i].mNumberChannels;
	
#elif defined(GSOUND_PLATFORM_WINDOWS)
	
	//**************************************************************************
	// Open a way to access properties of the device.
	
	IPropertyStore* deviceProperties;
	
	HRESULT result = wrapper->device->OpenPropertyStore( STGM_READ, &deviceProperties );
	
	GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
				"An error was encountered while querying an audio output device property store.", result );
	
	//**************************************************************************
	// Get the name of this output device.
	
	// Create a structure to hold the name of the device.
	PROPVARIANT propertyValue;
	PropVariantInit( &propertyValue );
	
	// Get the name of the device.
	result = deviceProperties->GetValue( PKEY_Device_FriendlyName, &propertyValue );
	
	GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
				"An error was encountered while querying an audio output device name.", result );
	
	// Get the length of the name of the device.
	int nameLength = WideCharToMultiByte( CP_ACP, 0, propertyValue.pwszVal, -1, 0, 0, NULL, NULL );
	
	// Allocate an array of characters which will hold the device's name.
	Char* cString = util::allocate<Char>( nameLength / sizeof(char) );
	
	// Get the device's name string in the temporary buffer.
	int bytesWritten = WideCharToMultiByte( CP_ACP, 0, propertyValue.pwszVal, -1, cString, nameLength, NULL, NULL );
	
	// Make sure that the name string is null-terminated.
	if ( bytesWritten == 0 )
		cString[0] = '\0';
	else
		cString[nameLength - 1] = '\0';
	
	// Set the name string object to contain the contents of this device name string.
	name = String( cString );
	
	// Deallocate the temporary array of characters which held the name string.
	util::deallocate( cString );
	
	// Clear the structure that was used to query the name string.
	PropVariantClear( &propertyValue );
	
	//**************************************************************************
	// Get the stream format for this device.
	
	// We are using 'shared' mode in the WASAPI, so there is only one valid
	// output format.
	
	// Get the current stream format for this output device.
	WAVEFORMATEXTENSIBLE* streamFormat;
	
	result = wrapper->audioClient->GetMixFormat( (WAVEFORMATEX**)&streamFormat );
	
	GSOUND_ASSERT_MESSAGE_CODE( result == S_OK, 
				"An error was encountered while querying an audio output device's format.", result );
	
	// Make sure that the list of native sample rates is empty.
	nativeSampleRates.clear();
	
	// Add the current sample rate to the list of native sample rates.
	nativeSampleRates.add( Float(streamFormat->Format.nSamplesPerSec) );
	
	// Get the number of channels that this device has.
	wrapper->numChannels = streamFormat->Format.nChannels;
	
	// Release the stream format object.
	CoTaskMemFree( streamFormat );
	
#endif
	
}




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
