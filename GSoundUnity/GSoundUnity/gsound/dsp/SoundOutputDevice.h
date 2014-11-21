/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundOutputDevice.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundOutputDevice class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_OUTPUT_DEVICE_H
#define INCLUDE_GSOUND_SOUND_OUTPUT_DEVICE_H


#include "GSoundDSPConfig.h"


#include "SoundInput.h"
#include "SoundOutput.h"
#include "SampleType.h"
#include "SoundDeviceID.h"
#include "SampleRateConverter.h"
#include "ChannelIOMap.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which represents a system sound output device.
/**
  * A SoundOutputDevice provides an easy-to-use platform-independent interface for
  * sending audio to an audio output device. It allows the user to access commonly
  * needed parameters such as the device's sample rate, name, and channel IO map.
  *
  * The class also provides automatic sample rate conversion if the input audio
  * sample rate is not the same as the device's current sample rate. If one does not
  * wish to incurr a performance penalty from the sample rate conversion, the class
  * also allows the user to attempt to set the device's sample rate.
  */
class SoundOutputDevice : public SoundInput
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a SoundOutputDevice for the specified SoundDeviceID.
			SoundOutputDevice( const SoundDeviceID& newDeviceID );
			
			
			
			
			/// Create a copy of the specified SoundOutputDevice object.
			SoundOutputDevice( const SoundOutputDevice& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy a SoundOutputDevice object, stopping the output of any audio.
			~SoundOutputDevice();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the state from one SoundOutputDevice to this object.
			SoundOutputDevice& operator = ( const SoundOutputDevice& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Output Start/Stop Methods
			
			
			
			
			/// Start sending audio to the output device.
			/**
			  * If this device has no input, zeroes are sent to the device until
			  * an audio input is bound to the device. If the device is invalid,
			  * this method has no effect.
			  */
			void start();
			
			
			
			
			/// Stop sending audio data to the output device.
			/**
			  * If the device is currently outputing audio, the output of further audio
			  * is stopped. Otherwise, the method has no effect. If the device is invalid,
			  * this method has no effect.
			  */
			void stop();
			
			
			
			
			/// Return whether or not the output device is currently outputing audio.
			/**
			  * If audio is currently being sent to the device, TRUE is returned.
			  * Otherwise, FALSE is returned. If the device is invalid, FALSE is always
			  * returned.
			  * 
			  * @return whether or not the output device is currently outputing audio.
			  */
			Bool isRunning() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Device Output Channel Count Accessor Method
			
			
			
			
			/// Get the number of channels that this output device has.
			/**
			  * If the device is invalid, this method always returns 0.
			  */
			Size getNumberOfChannels() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			
			
			/// Get the current sample rate at which audio is being sent to the device.
			/**
			  * This is the sample rate of the device's clock. Any input audio that doesn't
			  * match this sample rate is automatically converted to this sample rate.
			  * If the device is invalid, a sample rate of 0 is returned.
			  * 
			  * @return the current sample rate of the device's clock.
			  */
			Float getSampleRate() const;
			
			
			
			
			/// Set the current sample rate at which audio should be sent to the device.
			/**
			  * This method sets the sample rate of the device's clock. If the specified
			  * new sample rate is not a natively supported sample rate, FALSE is returned
			  * and this method has no effect on the device's state. Otherwise, if the sample
			  * rate is a native format, TRUE is returned and the device's output sample rate
			  * is changed. If the device is invalid, FALSE is returned and the mehtod
			  * has no effect.
			  * 
			  * @param newSampleRate - the sample rate to which the device's clock will be set to.
			  * @return whether or not the sample rate change operation was successful.
			  */
			Bool setSampleRate( Float newSampleRate );
			
			
			
			
			/// Return whether or not the specified sample rate is a native sample rate for this output device.
			/**
			  * For a sample rate to be native, no sample rate conversion is necessary before
			  * sending the audio to the device if it is of that sampling rate.
			  * 
			  * @param sampleRate - the sample rate to test to see if it is a native format.
			  * @return whether or not the specified sample rate is a native format for this device.
			  */
			GSOUND_INLINE Bool isNativeSampleRate( Float sampleRate )
			{
				return nativeSampleRates.contains( sampleRate );
			}
			
			
			
			
			/// Return a list of the native sampling rates for this output audio device.
			/**
			  * For a sample rate to be native, no sample rate conversion is necessary before
			  * sending the audio to the device if it is of that sampling rate.
			  * 
			  * @param sampleRate - the sample rate to test to see if it is a native format.
			  * @return whether or not the specified sample rate is a native format for this device.
			  */
			GSOUND_INLINE const ArrayList<Float>& getNativeSampleRates() const
			{
				return nativeSampleRates;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Input Accessor Methods
			
			
			
			
			/// Get the audio output that is providing audio to the output device.
			virtual SoundOutput* getInput() const;
			
			
			
			
			/// Set the audio output that will provide audio to the output device.
			/**
			  * A default 1-to-1 channel mapping is used to map input channels to device output
			  * channels, up to the channel count limits of the device/audio output.
			  */
			virtual void setInput( SoundOutput* newInput );
			
			
			
			
			/// Set the audio output that will provide audio to the output device with the specified channel mapping.
			/**
			  * The specified channel mapping will be used to determine which input channels
			  * are sent to each device output channel. If no mapping is specified for a given
			  * input channel, that channel of audio data will be sent to the device output channel
			  * with the same index unless that output channel already has a mapping. If no 
			  * mapping is specified for a given output channel, no sound is output on that channel.
			  */
			void setInput( SoundOutput* newInput, const ChannelIOMap& newInputChannelMap );
			
			
			
			
			/// Clear any object that was previously providing output audio for this device.
			virtual void removeInput();
			
			
			
			
			/// Return whether or not this output device has an object to provide output audio.
			virtual Bool hasInput() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Input Channel Mapping Accessor Methods
			
			
			
			
			/// Get the channel mapping to map input to output channels for this device.
			/**
			  * This channel mapping is be used to determine which input channels
			  * are sent to each device output channel. If no mapping is specified for a given
			  * input channel, that channel of audio data will be sent to the device output channel
			  * with the same index unless that output channel already has a mapping. If no 
			  * mapping is specified for a given output channel, no sound is output on that channel.
			  */
			const ChannelIOMap& getChannelMap() const;
			
			
			
			
			/// Set the channel mapping to map input to output channels for this device.
			/**
			  * The specified channel mapping will be used to determine which input channels
			  * are sent to each device output channel. If no mapping is specified for a given
			  * input channel, that channel of audio data will be sent to the device output channel
			  * with the same index unless that output channel already has a mapping. If no 
			  * mapping is specified for a given output channel, no sound is output on that channel.
			  */
			void setChannelMap( const ChannelIOMap& newInputChannelMap );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Device Name Accessor Method
			
			
			
			
			/// Get a string representing the name of this output device.
			/**
			  * This name is usually specified by the hardware driver as a human-readable
			  * identifier for the device.
			  * 
			  * @return the name of this output device.
			  */
			GSOUND_INLINE const String& getName() const
			{
				return name;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Device ID Accessor Method
			
			
			
			
			/// Return an object which uniquely identifies this sound output device.
			SoundDeviceID getID() const;
			
			
			
			
			/// Return whether or not this output device represents a valid device.
			Bool isValid() const;
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Wrapper Class Declaration
			
			
			
			
			/// A class which encapsulates internal data needed by the SoundOutputDevice object.
			class SoundOutputDeviceWrapper;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Helper Methods
			
			
			
			
			/// Get data from the audio device about its current state.
			void initializeDeviceData();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A pointer to a class which wraps internal state of this SoundOutputDevice.
			SoundOutputDeviceWrapper* wrapper;
			
			
			
			
			/// A list of the natively supported sample rates of this SoundOutputDevice.
			ArrayList<Float> nativeSampleRates;
			
			
			
			
			/// The device-provided name of this SoundOutputDevice.
			String name;
			
			
			
			
			/// A pointer to the object which is producing audio to be output.
			SoundOutput* input;
			
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_OUTPUT_DEVICE_H
