/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/GSoundDSPConfig.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    Configuration for the dsp namespace of the GSound library.
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


#ifndef INCLUDE_GSOUND_DSP_CONFIG_H
#define INCLUDE_GSOUND_DSP_CONFIG_H


#include "../GSoundConfig.h"


#include "../GSoundBase.h"


//##########################################################################################
//##########################################################################################
//############		
//############		GSound Internal Namespace Configuration
//############		
//##########################################################################################
//##########################################################################################




/// Define the namespace to use for library DSP functionality.
#ifndef GSOUND_DSP_NAMESPACE
	#define GSOUND_DSP_NAMESPACE dsp
#endif




/// Define a macro which indicates the start of the gsound::dsp namespace.
#ifndef GSOUND_DSP_NAMESPACE_START
	#define GSOUND_DSP_NAMESPACE_START		GSOUND_NAMESPACE_START namespace GSOUND_DSP_NAMESPACE {
#endif




/// Define a macro which indicates the end of the gsound::dsp namespace.
#ifndef GSOUND_DSP_NAMESPACE_END
	#define GSOUND_DSP_NAMESPACE_END		}; GSOUND_NAMESPACE_END
#endif




GSOUND_NAMESPACE_START
/// A namespace containing library DSP functionality.
/**
  * This namespace consists of classes and functions that perform
  * tasks related to digital signal processing of audio data and
  * its output to system audio devices.
  */
namespace GSOUND_DSP_NAMESPACE {
};
GSOUND_NAMESPACE_END




//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// Define the type used to indicate the index of a sample within a sound.
typedef UInt64 SampleIndex;




/// Define the type used to indicate the size in samples of a sound.
typedef UInt64 SoundSize;




/// Define the type to use for 8-bit signed integer sound samples.
typedef Int8 Sample8;




/// Define the type to use for 16-bit signed integer sound samples.
typedef Int16 Sample16;




/// Define the type to use for 32-bit signed integer sound samples.
typedef Int32 Sample32;




/// Define the type to use for 64-bit signed integer sound samples.
typedef Int64 Sample64;




/// Define the type to use for 32-bit floating-point sound samples.
typedef Float32 Sample32f;




/// Define the type to use for 64-bit floating-point sound samples.
typedef Float64 Sample64f;




/// Define the type to use for a SIMD vector of 32-bit floating point samples.
typedef math::SIMDScalar<float,4> SIMDSample32f;




/// A class used to represent a 24-bit integer sound sample.
/**
  * This class helps facilitate operations on 24-bit sound samples
  * by providing methods which pack and unpack a 24-bit sound sample
  * from/to a 32-bit integer sound sample. This class should be exactly
  * 24 bits wide, allowing its use in arrays of samples.
  */
class Sample24
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a 24-bit sample equal to 0.
			GSOUND_INLINE Sample24()
			{
				data[0] = data[1] = data[2] = 0;
			}
			
			
			
			
			/// Create a 24-bit sample from the specified 32-bit integer sample, discarding the lower bits.
			GSOUND_INLINE Sample24( Sample32 value )
			{
				pack( value );
			}
			
			
			
			
			/// Cast this 24-bit sample to a 32-bit integer sample.
			GSOUND_INLINE operator Sample32 () const
			{
				return unpack();
			}
			
			
			
			
	private:
		
		/// Unpack this 24-bit sample into a 32-bit integer sample.
		GSOUND_INLINE Int32 unpack() const
		{
			return Int32(data[0] << 24) | 
					Int32(data[1] << 16) | 
					Int32(data[2] << 8);
		}
		
		
		
		
		/// Pack the specified 32-bit integer sample into this 24-bit sample, discarding the lower bits.
		GSOUND_INLINE void pack( Int32 value )
		{
			value = math::clamp( value, -Int32(1 << 23), Int32(1 << 23) - 1 );
			
			data[0] = (UInt8)((value & 0xFF000000) >> 24);
			data[1] = (UInt8)((value & 0x00FF0000) >> 16);
			data[2] = (UInt8)((value & 0x0000FF00) >> 8);
		}
		
		
		
		
		/// An array of 3 bytes representing the 3 bytes of a 24-bit word.
		UInt8 data[3];
		
		
		
		
};




/// Define the type of sample to use for rendering in the GSound library.
typedef Sample32f Sample;




/// Define the type to use for a SIMD vector of samples.
typedef SIMDSample32f SIMDSample;




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_DSP_CONFIG_H
