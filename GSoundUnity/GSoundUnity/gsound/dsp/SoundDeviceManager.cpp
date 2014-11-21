/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundDeviceManager.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundDeviceManager class declaration
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


#include "SoundDeviceManager.h"


// Include platform-specific headers
#ifdef GSOUND_PLATFORM_APPLE
	#undef __APPLE_ALTIVEC__
	#include <CoreServices/CoreServices.h>
	#include <CoreAudio/CoreAudio.h>
	#include <unistd.h>
	#include <AudioUnit/AudioUnit.h>
#endif

#ifdef GSOUND_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <MmDeviceapi.h>
	#include <Audioclient.h>
	#include <Audiopolicy.h>
#endif


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Device Manager Wrapper Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundDeviceManagerWrapper
{
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Device Change Notifier Class
			
			
			
			
#if defined(GSOUND_PLATFORM_WINDOWS)
			class DeviceChangeNotifier : public IMMNotificationClient
			{
				public:
					
					DeviceChangeNotifier()
						:	referenceCount( 1 )
					{
					}

					~DeviceChangeNotifier()
					{
					}
					
					// IUnknown methods -- AddRef, Release, and QueryInterface
					
					ULONG STDMETHODCALLTYPE AddRef()
					{
						return InterlockedIncrement(&referenceCount);
					}
					
					ULONG STDMETHODCALLTYPE Release()
					{
						ULONG ulRef = InterlockedDecrement(&referenceCount);
						
						if ( ulRef == 0 )
							delete this;
						
						return ulRef;
					}
					
					HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, VOID **ppvInterface )
					{
						if (IID_IUnknown == riid)
						{
							AddRef();
							*ppvInterface = (IUnknown*)this;
						}
						else if (__uuidof(IMMNotificationClient) == riid)
						{
							AddRef();
							*ppvInterface = (IMMNotificationClient*)this;
						}
						else
						{
							*ppvInterface = NULL;
							return E_NOINTERFACE;
						}
						
						return S_OK;
					}
					
					// Callback methods for device-event notifications.
					
					HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged( EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId )
					{
						if ( flow == eRender )
							SoundDeviceManagerWrapper::refreshDefaultOutputDevice();
						else if ( flow == eCapture )
							SoundDeviceManagerWrapper::refreshDefaultInputDevice();
						
						return S_OK;
					}
					
					HRESULT STDMETHODCALLTYPE OnDeviceAdded( LPCWSTR pwstrDeviceId )
					{
						SoundDeviceManagerWrapper::refreshDevices();
						
						return S_OK;
					};
					
					HRESULT STDMETHODCALLTYPE OnDeviceRemoved( LPCWSTR pwstrDeviceId )
					{
						SoundDeviceManagerWrapper::refreshDevices();
						
						return S_OK;
					}
					
					HRESULT STDMETHODCALLTYPE OnDeviceStateChanged( LPCWSTR pwstrDeviceId, DWORD dwNewState )
					{
						SoundDeviceManagerWrapper::refreshDevices();
						
						return S_OK;
					}
					
					HRESULT STDMETHODCALLTYPE OnPropertyValueChanged( LPCWSTR pwstrDeviceId, const PROPERTYKEY key )
					{
						return S_OK;
					}
					
				private:
					
					LONG referenceCount;
					
			};
#endif
			
			
			
			
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Device Cache Method
			
			
			
			
			static void cacheDevices()
			{
				// If we haven't registered any device change listeners or cached any devices, do so.
				if ( !hasCachedDevices )
				{
					
#if defined(GSOUND_PLATFORM_APPLE)
					
					// Add the function which listens to changes in the connected audio devices.
					OSStatus error = AudioHardwareAddPropertyListener( kAudioHardwarePropertyDevices, devicesChangeCallback, NULL );
					
					// Make sure that an error has not been encountered.
					GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
								"An error was encountered while registering an audio device change callback.",
								error );
					
					// Add the function which listens to changes in the default audio input device.
					error = AudioHardwareAddPropertyListener( kAudioHardwarePropertyDefaultInputDevice, devicesChangeCallback, NULL );
					
					// Make sure that an error has not been encountered.
					GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
								"An error was encountered while registering an audio input device change callback.",
								error );
					
					// Add the function which listens to changes in the default audio output device.
					error = AudioHardwareAddPropertyListener( kAudioHardwarePropertyDefaultOutputDevice, devicesChangeCallback, NULL );
					
					// Make sure that an error has not been encountered.
					GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
								"An error was encountered while registering an audio output device change callback.",
								error );
					
#elif defined(GSOUND_PLATFORM_WINDOWS)
					
					//****************************************************************
					// Create an object that enumerates the connected devices.
					
					CoInitialize( NULL );
					
					const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
					const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
					
					HRESULT result = CoCreateInstance( CLSID_MMDeviceEnumerator, NULL,
														CLSCTX_ALL, IID_IMMDeviceEnumerator,
														(void**)&deviceEnumerator );
					
					GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
								"An error was encountered while creating an audio device enumerator object.",
								result );
					
					//****************************************************************
					// Create and register an object that receives device change notifications
					
					if ( deviceChangeNotifier != NULL )
					{
						deviceChangeNotifier->Release();
						deviceChangeNotifier = NULL;
					}
					
					deviceChangeNotifier = new DeviceChangeNotifier();
					
					result = deviceEnumerator->RegisterEndpointNotificationCallback( deviceChangeNotifier );
					
					GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
								"An error was encountered while registering an audio device change callback.",
								result );
					
#endif
					
					// Update the data contained by this class.
					refreshDevices();
					refreshDefaultInputDevice();
					refreshDefaultOutputDevice();

					hasCachedDevices = true;
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Static Data Members
			
			
			
			
			static ArrayList<SoundDeviceID> outputDevices;
			static ArrayList<SoundDeviceID> inputDevices;
			
			
			static Index defaultOutputDeviceIndex;
			static Index defaultInputDeviceIndex;
			
			
			static Mutex deviceChangeMutex;
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Device Change Callback Method
			
			
			
			
#if defined(GSOUND_PLATFORM_APPLE)
			static OSStatus devicesChangeCallback( AudioHardwarePropertyID propertyID, void* clientData )
			{
				if ( propertyID == kAudioHardwarePropertyDevices )
					refreshDevices();
				else if ( propertyID == kAudioHardwarePropertyDefaultInputDevice )
					refreshDefaultInputDevice();
				else if ( propertyID == kAudioHardwarePropertyDefaultOutputDevice )
					refreshDefaultOutputDevice();
				
				return noErr;
			}
#endif
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Device Refresh Method
			
			
			
			
			static void refreshDevices()
			{
				// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
				SoundDeviceManagerWrapper::deviceChangeMutex.acquire();
				
#if defined(GSOUND_PLATFORM_APPLE)
				
				OSStatus error;
				
				//****************************************************************
				//	Get handles to all of the connected audio devices
				
				::UInt32 numBytes;
				
				// Get the number of bytes needed to hold the buffer of devices
				error = AudioHardwareGetPropertyInfo( kAudioHardwarePropertyDevices, &numBytes, NULL ); 
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
							"An error was encountered while querying the number of connected audio devices.",
							error );
				
				// Allocate a temporary array of device IDs.
				Size numDevices = numBytes / sizeof(AudioDeviceID);
				AudioDeviceID* devices = util::allocate<AudioDeviceID>( numDevices );
				
				// Get the device IDs in the array.
				error = AudioHardwareGetProperty( kAudioHardwarePropertyDevices, &numBytes, devices );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
							"An error was encountered while querying the connected audio device.",
							error );
				
				// Clear all input and output devices.
				inputDevices.clear();
				outputDevices.clear();
				
				// For each device ID in the device ID array, test to see if that device is an input
				// or output device and add it the array list holding the associated device IDs for inputs or outputs.
				for ( Index i = 0; i < numDevices; i++ )
				{
					if ( deviceIsInputDevice( devices[i] ) )
						inputDevices.add( SoundDeviceID(devices[i]) );
					
					if ( deviceIsOutputDevice( devices[i] ) )
						outputDevices.add( SoundDeviceID(devices[i]) );
				}
				
				// Deallocate the temporary array of device IDs.
				util::deallocate(devices);
				
#elif defined(GSOUND_PLATFORM_WINDOWS)
				
				//****************************************************************
				// Enumerate the connected audio input devices.
				
				// Release the previous device collection if it existed.
				if ( inputDeviceCollection != NULL )
				{
					inputDeviceCollection->Release();
					inputDeviceCollection = NULL;
				}
				
				// Get the collection of input devices.
				HRESULT result = deviceEnumerator->EnumAudioEndpoints( eCapture, DEVICE_STATE_ACTIVE, &inputDeviceCollection );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying the connected audio input devices.",
							result );
				
				// Get the number of input devices.
				UINT numInputDevices;
				result = inputDeviceCollection->GetCount( &numInputDevices );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying the number of connected audio input devices.",
							result );
				
				// Clear all input devices.
				inputDevices.clear();
				
				// Iterate over the connected input devices.
				for ( UINT i = 0; i < numInputDevices; i++ )
					inputDevices.add( SoundDeviceID(i) );
				
				//****************************************************************
				// Enumerate the connected audio output devices.
				
				// Release the previous device collection if it existed.
				if ( outputDeviceCollection != NULL )
				{
					outputDeviceCollection->Release();
					outputDeviceCollection = NULL;
				}
				
				// Get the collection of output devices.
				result = deviceEnumerator->EnumAudioEndpoints( eRender, DEVICE_STATE_ACTIVE, &outputDeviceCollection );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying the connected audio output devices.",
							result );
				
				// Get the number of output devices.
				UINT numOutputDevices;
				result = outputDeviceCollection->GetCount( &numOutputDevices );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying the number of connected audio output devices.",
							result );
				
				// Clear all output devices.
				outputDevices.clear();
				
				// Iterate over the connected output devices.
				for ( UINT i = 0; i < numOutputDevices; i++ )
					outputDevices.add( SoundDeviceID(i) );
				
#endif
				
				// Release a mutex which indicates that the number of devices is currently changing or being queried.
				SoundDeviceManagerWrapper::deviceChangeMutex.release();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Default Input Device Refresh Method
			
			
			
			
			static void refreshDefaultInputDevice()
			{
				// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
				SoundDeviceManagerWrapper::deviceChangeMutex.acquire();
				
#if defined(GSOUND_PLATFORM_APPLE)
				
				//****************************************************************
				//	Get a handle to the default input device
				
				::UInt32 propertySize = sizeof(AudioDeviceID);
				AudioDeviceID defaultInputDevice;
				
				OSStatus error = AudioHardwareGetProperty( kAudioHardwarePropertyDefaultInputDevice,
															&propertySize, &defaultInputDevice );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
							"An error was encountered while getting the default input audio device.",
							error );
				
				// Find the default input device in the cached array of devices.
				for ( Index i = 0; i < inputDevices.getSize(); i++ )
				{
					if ( inputDevices[i] == SoundDeviceID(defaultInputDevice) )
					{
						defaultInputDeviceIndex = i;
						
						// Release a mutex which indicates that the number of devices is currently changing or being queried.
						SoundDeviceManagerWrapper::deviceChangeMutex.release();
						
						return;
					}
				}
				
#elif defined(GSOUND_PLATFORM_WINDOWS)
				
				//****************************************************************
				//	Get a handle to the default input device
				
				IMMDevice* device;
				HRESULT  result = deviceEnumerator->GetDefaultAudioEndpoint( eCapture, eMultimedia, &device );
				
				// If there was an error because no device was found, recover.
				if ( result != S_OK && result == E_NOTFOUND )
				{
					// Set the default input device to be the first if there was no default.
					defaultInputDeviceIndex = 0;
					
					// Release a mutex which indicates that the number of devices is currently changing or being queried.
					SoundDeviceManagerWrapper::deviceChangeMutex.release();
					return;
				}
				
				// Check for other unrecoverable errors.
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying the default audio input device.",
							result );
				
				// Get the number of input devices.
				UINT numInputDevices;
				result = inputDeviceCollection->GetCount( &numInputDevices );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying the number of connected audio input devices.",
							result );
				
				// Get the name of the default input device.
				LPWSTR defaultDeviceName;
				result = device->GetId( &defaultDeviceName );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying the name of an audio input device.",
							result );
				
				// Release the default device.
				device->Release();
				
				// Iterate over the connected input devices.
				for ( UINT i = 0; i < numInputDevices; i++ )
				{
					// Get the device at the current index.
					IMMDevice* otherDevice;
					result = inputDeviceCollection->Item( i, &otherDevice );
					
					GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
								"An error was encountered while querying a connected audio input device.",
								result );
					
					// Get the name of the current device.
					LPWSTR otherDeviceName;
					result = otherDevice->GetId( &otherDeviceName );
					
					GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
								"An error was encountered while querying the name of an audio input device.",
								result );
					
					if ( wcscmp( defaultDeviceName, otherDeviceName ) == 0 )
					{
						// Set the index of the default input device.
						defaultInputDeviceIndex = i;
						
						// Release the current device's name.
						CoTaskMemFree( otherDeviceName );
						
						// Release a mutex which indicates that the number of devices is currently changing or being queried.
						SoundDeviceManagerWrapper::deviceChangeMutex.release();
						
						return;
					}
					else
					{
						// Release the current device.
						otherDevice->Release();
					}
					
					// Release the current device's name.
					CoTaskMemFree( otherDeviceName );
				}
				
				// Release the default device's name.
				CoTaskMemFree( defaultDeviceName );
#endif
				// Set the default input device to be the first if there was no default.
				defaultInputDeviceIndex = 0;
				
				// Release a mutex which indicates that the number of devices is currently changing or being queried.
				SoundDeviceManagerWrapper::deviceChangeMutex.release();
			}
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Default Output Device Refresh Method
			
			
			
			
			static void refreshDefaultOutputDevice()
			{
				// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
				SoundDeviceManagerWrapper::deviceChangeMutex.acquire();
				
#if defined(GSOUND_PLATFORM_APPLE)
				
				//****************************************************************
				//	Get a handle to the default output device
				
				::UInt32 propertySize = sizeof(AudioDeviceID);
				AudioDeviceID defaultOutputDevice;
				
				OSStatus error = AudioHardwareGetProperty( kAudioHardwarePropertyDefaultOutputDevice,
															&propertySize, &defaultOutputDevice );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
							"An error was encountered while getting the default output audio device.",
							error );
				
				// Find the default output device in the cached array of devices.
				for ( Index i = 0; i < outputDevices.getSize(); i++ )
				{
					if ( outputDevices[i] == SoundDeviceID(defaultOutputDevice) )
					{
						defaultOutputDeviceIndex = i;
						
						// Release a mutex which indicates that the number of devices is currently changing or being queried.
						SoundDeviceManagerWrapper::deviceChangeMutex.release();
						
						return;
					}
				}
				
				
#elif defined(GSOUND_PLATFORM_WINDOWS)
				
				//****************************************************************
				//	Get a handle to the default output device
				
				IMMDevice* device;
				HRESULT  result = deviceEnumerator->GetDefaultAudioEndpoint( eRender, eMultimedia, &device );
				
				// If there was an error because no device was found, recover.
				if ( result != S_OK && result == E_NOTFOUND )
				{
					// Set the default output device to be the first if there was no default.
					defaultOutputDeviceIndex = 0;
					
					// Release a mutex which indicates that the number of devices is currently changing or being queried.
					SoundDeviceManagerWrapper::deviceChangeMutex.release();
					return;
				}
				
				// Check for other unrecoverable errors.
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying the default audio output device.",
							result );
				
				// Get the number of output devices.
				UINT numOutputDevices;
				result = outputDeviceCollection->GetCount( &numOutputDevices );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying the number of connected audio output devices.",
							result );
				
				// Get the name of the default output device.
				LPWSTR defaultDeviceName;
				result = device->GetId( &defaultDeviceName );
				
				GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
							"An error was encountered while querying the name of an audio output device.",
							result );
				
				// Release the default device.
				device->Release();
				
				// Iterate over the connected output devices.
				for ( UINT i = 0; i < numOutputDevices; i++ )
				{
					// Get the device at the current index.
					IMMDevice* otherDevice;
					result = outputDeviceCollection->Item( i, &otherDevice );
					
					GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
								"An error was encountered while querying a connected audio output device.",
								result );
					
					// Get the name of the current device.
					LPWSTR otherDeviceName;
					result = otherDevice->GetId( &otherDeviceName );

					GSOUND_ASSERT_MESSAGE_CODE( result == S_OK,
								"An error was encountered while querying the name of an audio output device.",
								result );
					
					if ( wcscmp( defaultDeviceName, otherDeviceName ) == 0 )
					{
						// Set the index of the default output device.
						defaultOutputDeviceIndex = i;
						
						// Release the current device's name.
						CoTaskMemFree( otherDeviceName );
						
						// Release a mutex which indicates that the number of devices is currently changing or being queried.
						SoundDeviceManagerWrapper::deviceChangeMutex.release();
						
						return;
					}
					else
					{
						// Release the current device.
						otherDevice->Release();
					}
					
					// Release the current device's name.
					CoTaskMemFree( otherDeviceName );
				}
				
				// Release the default device's name.
				CoTaskMemFree( defaultDeviceName );
#endif
				// Set the default output device to be the first if there was no default.
				defaultOutputDeviceIndex = 0;
				
				// Release a mutex which indicates that the number of devices is currently changing or being queried.
				SoundDeviceManagerWrapper::deviceChangeMutex.release();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Device Is Input Query Method
			
			
			
			
#if defined(GSOUND_PLATFORM_APPLE)
			static Bool deviceIsInputDevice( AudioDeviceID device )
			{
				// Allocate a temporary audio buffer list to hold the stream configuration.
				AudioBufferList bufferList;
				::UInt32 propertySize = sizeof(AudioBufferList);
				
				// Get the input stream configuration.
				OSStatus error = AudioDeviceGetProperty( device, 0, true, kAudioDevicePropertyStreamConfiguration,
														&propertySize, &bufferList );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE_CODE( error == noErr,
							"An error was encountered while testing to see if an audio device was an output device.",
							error );
				
				// If the number of buffers is zero, the device has no input channels and is not an input device.
				if ( bufferList.mNumberBuffers == 0 )
					return false;
				else
					return true;
			}
#endif
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Device Is Output Query Method
			
			
			
			
#if defined(GSOUND_PLATFORM_APPLE)
			static Bool deviceIsOutputDevice( AudioDeviceID device )
			{
				// Allocate a temporary audio buffer list to hold the stream configuration.
				AudioBufferList bufferList;
				::UInt32 propertySize = sizeof(AudioBufferList);
				
				// Get the input stream configuration.
				OSStatus error = AudioDeviceGetProperty( device, 0, false, kAudioDevicePropertyStreamConfiguration,
														&propertySize, &bufferList );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE( error == noErr,
										"An error was encountered while testing to see if an audio device was an input device." );
				
				// If the number of buffers is zero, the device has no input channels and is not an input device.
				if ( bufferList.mNumberBuffers == 0 )
					return false;
				else
					return true;
			}
#endif
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Data Members
			
			
			
			
#if defined(GSOUND_PLATFORM_WINDOWS)
			
			static DeviceChangeNotifier* deviceChangeNotifier;
			
			static IMMDeviceEnumerator* deviceEnumerator;
			
			static IMMDeviceCollection* inputDeviceCollection;
			static IMMDeviceCollection* outputDeviceCollection;
			
#endif
			
			/// Whether or not the device manager wrapper has cached the available devices yet.
			static Bool hasCachedDevices;
			
			
			
};




Index SoundDeviceManagerWrapper:: defaultInputDeviceIndex = 0;
Index SoundDeviceManagerWrapper:: defaultOutputDeviceIndex = 0;

ArrayList<SoundDeviceID> SoundDeviceManagerWrapper:: inputDevices;
ArrayList<SoundDeviceID> SoundDeviceManagerWrapper:: outputDevices;


Mutex SoundDeviceManagerWrapper:: deviceChangeMutex;


Bool SoundDeviceManagerWrapper:: hasCachedDevices = false;


#if defined(GSOUND_PLATFORM_WINDOWS)

SoundDeviceManagerWrapper:: DeviceChangeNotifier*
			SoundDeviceManagerWrapper:: deviceChangeNotifier = NULL;

IMMDeviceEnumerator* SoundDeviceManagerWrapper:: deviceEnumerator = NULL;

IMMDeviceCollection* SoundDeviceManagerWrapper:: inputDeviceCollection = NULL;
IMMDeviceCollection* SoundDeviceManagerWrapper:: outputDeviceCollection = NULL;

#endif


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundDeviceManager:: SoundDeviceManager()
{
	SoundDeviceManagerWrapper::cacheDevices();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Default Device Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundDeviceID SoundDeviceManager:: getDefaultOutputDeviceID() const
{
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	SoundDeviceManagerWrapper::deviceChangeMutex.acquire();
	
	// Determine whether or not the device at that index is valid.
	Bool validDeviceIndex = SoundDeviceManagerWrapper::defaultOutputDeviceIndex < SoundDeviceManagerWrapper::outputDevices.getSize();
	
	// Determine the device ID for the requested device.
	SoundDeviceID deviceID = validDeviceIndex ? 
				SoundDeviceManagerWrapper::outputDevices[SoundDeviceManagerWrapper::defaultOutputDeviceIndex] : 
				SoundDeviceID::INVALID_DEVICE;
	
	// Release a mutex which indicates that the number of devices is currently changing or being queried.
	SoundDeviceManagerWrapper::deviceChangeMutex.release();
	
	return deviceID;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Device Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundDeviceManager:: getNumberOfOutputDevices() const
{
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	SoundDeviceManagerWrapper::deviceChangeMutex.acquire();
	
	Size numDevices = SoundDeviceManagerWrapper::outputDevices.getSize();
	
	// Release a mutex which indicates that the number of devices is currently changing or being queried.
	SoundDeviceManagerWrapper::deviceChangeMutex.release();
	
	return numDevices;
}




SoundDeviceID SoundDeviceManager:: getOutputDeviceID( Index deviceIndex ) const
{
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	SoundDeviceManagerWrapper::deviceChangeMutex.acquire();
	
	GSOUND_DEBUG_ASSERT( deviceIndex < SoundDeviceManagerWrapper::outputDevices.getSize() );
	
	// Determine whether or not the device at that index is valid.
	Bool validDeviceIndex = deviceIndex < SoundDeviceManagerWrapper::outputDevices.getSize();
	
	// Determine the device ID for the requested device.
	SoundDeviceID deviceID = validDeviceIndex ? SoundDeviceManagerWrapper::outputDevices[deviceIndex] : SoundDeviceID::INVALID_DEVICE;
	
	// Release a mutex which indicates that the number of devices is currently changing or being queried.
	SoundDeviceManagerWrapper::deviceChangeMutex.release();
	
	return deviceID;
}




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
