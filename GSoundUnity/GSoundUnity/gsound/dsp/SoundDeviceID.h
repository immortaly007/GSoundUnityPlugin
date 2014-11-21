/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundDeviceID.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundDeviceID class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_DEVICE_ID_H
#define INCLUDE_GSOUND_SOUND_DEVICE_ID_H


#include "GSoundDSPConfig.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which is used to encapsulate a unique identifier for a system sound device.
/**
  * An unsigned integer is used as the underlying type for this SoundDeviceID. Conversion
  * operators are provided to facilitate converting SoundDeviceID objects to the underlying
  * type.
  */
class SoundDeviceID
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Constants
			
			
			
			
			/// Define the reserved ID to use to indicate an invalid device.
			static const UInt INVALID_DEVICE_ID = 0xFFFFFFFF;
			
			
			
			
			/// Define an instance of SoundDeviceID that represents an invalid device.
			static const SoundDeviceID INVALID_DEVICE;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Comparison Operators
			
			
			
			
			/// Return whether or not this device ID represents the same device as another.
			GSOUND_INLINE Bool operator == ( const SoundDeviceID& other ) const
			{
				return deviceID == other.deviceID;
			}
			
			
			
			
			/// Return whether or not this device ID represents a different device than another.
			GSOUND_INLINE Bool operator != ( const SoundDeviceID& other ) const
			{
				return deviceID != other.deviceID;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Device Status Accessor Method
			
			
			
			
			/// Return whether or not this SoundDeviceID represents a valid device.
			GSOUND_INLINE Bool isValid() const
			{
				return deviceID != INVALID_DEVICE_ID;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Conversion Operator
			
			
			
			
			/// Convert this SoundDeviceID object to an unsigned integer which uniquely represents a sound device.
			GSOUND_INLINE operator UInt () const
			{
				return deviceID;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Constructor
			
			
			
			
			/// Create a SoundDeviceID object which represents the device with the specified device ID.
			GSOUND_INLINE explicit SoundDeviceID( UInt newDeviceID )
				:	deviceID( newDeviceID )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The underlying representation of a SoundDeviceID.
			UInt deviceID;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Friend Declaration
			
			
			
			
			/// Declare the SoundDeviceManager a friend so that it can construct SoundDeviceID objects.
			friend class SoundDeviceManager;
			friend class SoundDeviceManagerWrapper;
			
			
			
			
			/// Declare the SoundOutputDevice a friend so that it can construct SoundDeviceID objects.
			friend class SoundOutputDevice;
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_DEVICE_ID_H
