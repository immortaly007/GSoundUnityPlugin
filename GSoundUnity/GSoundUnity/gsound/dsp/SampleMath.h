/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SampleMath.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    Functions for mixing and converting audio samples of various types.
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


#ifndef INCLUDE_GSOUND_SAMPLE_MATH_H
#define INCLUDE_GSOUND_SAMPLE_MATH_H


#include "GSoundDSPConfig.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A non-instantiable class used to encapsulate operations on sound samples.
/**
  * The class, better thought of as a namespace, contains only static methods
  * for performing math operations on sound samples.
  *
  * Methods are provided which perform the following operations:
  * - Sample Type Conversion: the convert() methods provide conversion between any two sample types.
  * - Sample Mixing: the mix() methods provide a way to add two samples of any type together without overflow.
  * - Sample Scaling: the scale() methods provide a way for a sample of any type to scaled by a
  *   floating-point gain multiplier.
  */
class sample
{
	public:
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Convert Methods for 8-bit aSamples
			
			
			
			
			template < typename T >
			GSOUND_FORCE_INLINE static T convert( Sample8 aSample )
			{
				return T( aSample );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Convert Methods for 16-bit aSamples
			
			
			
			
			template < typename T >
			GSOUND_FORCE_INLINE static T convert( Sample16 aSample )
			{
				return T( aSample );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Convert Methods for 24-bit aSamples
			
			
			
			
			template < typename T >
			GSOUND_FORCE_INLINE static T convert( Sample24 aSample )
			{
				return T( aSample );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Convert Methods for 32-bit aSamples
			
			
			
			
			template < typename T >
			GSOUND_FORCE_INLINE static T convert( Sample32 aSample )
			{
				return T( aSample );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Convert Methods for 64-bit aSamples
			
			
			
			
			template < typename T >
			GSOUND_FORCE_INLINE static T convert( Sample64 aSample )
			{
				return T( aSample );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Convert Methods for 32-bit floating point aSamples
			
			
			
			
			template < typename T >
			GSOUND_FORCE_INLINE static T convert( Sample32f aSample )
			{
				return T( aSample );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Convert Methods for 64-bit floating point aSamples
			
			
			
			
			template < typename T >
			GSOUND_FORCE_INLINE static T convert( Sample64f aSample )
			{
				return T( aSample );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Mix Methods
			
			
			
			
			/// Mix two 8-bit signed integral samples, saturating at minimum and maximum sample values.
			GSOUND_FORCE_INLINE static Sample8 mix( Sample8 aSample1, Sample8 aSample2 )
			{
				// Clamp the output to [-128,127]
				static const Int32 sampleMin = -128;
				static const Int32 sampleMax = 127;
				
				Int32 result = (Int32)aSample1 + (Int32)aSample2;
				
				return (Sample8)math::clamp( result, sampleMin, sampleMax );
			}
			
			
			
			
			/// Mix two 16-bit signed integral samples, saturating at minimum and maximum sample values.
			GSOUND_FORCE_INLINE static Sample16 mix( Sample16 aSample1, Sample16 aSample2 )
			{
				// Clamp the output to [-32768,32767]
				static const Int32 sampleMin = -32768;
				static const Int32 sampleMax = 32767;
				
				Int32 result = (Int32)aSample1 + (Int32)aSample2;
				
				return (Sample16)math::clamp( result, sampleMin, sampleMax );
			}
			
			
			
			
			/// Mix two 24-bit signed integral samples, saturating at minimum and maximum sample values.
			GSOUND_FORCE_INLINE static Sample24 mix( Sample24 aSample1, Sample24 aSample2 )
			{
				Int32 result = (Int32)aSample1 + (Int32)aSample2;
				
				return Sample24(result);
			}
			
			
			
			
			/// Mix two 32-bit signed integral samples, saturating at minimum and maximum sample values.
			GSOUND_FORCE_INLINE static Sample32 mix( Sample32 aSample1, Sample32 aSample2 )
			{
				// Clamp the output to [-2147483648,2147483647]
				static const Int64 sampleMin = -2147483648LL;
				static const Int64 sampleMax = 2147483647LL;
				
				Int64 result = (Int64)aSample1 + (Int64)aSample2;
				
				return (Sample32)math::clamp( result, sampleMin, sampleMax );
			}
			
			
			
			
			/// Mix two 64-bit signed integral samples, saturating at minimum and maximum sample values.
			GSOUND_FORCE_INLINE static Sample64 mix( Sample64 aSample1, Sample64 aSample2 )
			{
				// Clamp the output to [-2^63,2^63-1]
				static const Double sampleMin = -9.223372036855e+18;
				static const Double sampleMax = 9.223372036855e+18;
				
				Double result = (Double)aSample1 + (Double)aSample2;
				
				return (Sample64)math::clamp( result, sampleMin, sampleMax );
			}
			
			
			
			
			/// Mix two 32-bit floating-point samples without performing any saturation.
			GSOUND_FORCE_INLINE static Sample32f mix( Sample32f aSample1, Sample32f aSample2 )
			{
				// Don't clamp the output to [-1,1]. Allow floating point aSamples
				// to exceed the maximum representable amplitude for other aSamples.
				// This is useful for HDR sound, etc. and will be clipped upon output
				// anyway.
				return aSample1 + aSample2;
			}
			
			
			
			
			/// Mix two 64-bit floating-point samples without performing any saturation.
			GSOUND_FORCE_INLINE static Sample64f mix( Sample64f aSample1, Sample64f aSample2 )
			{
				// Don't clamp the output to [-1,1]. Allow floating point aSamples
				// to exceed the maximum representable amplitude for other aSamples.
				// This is useful for HDR sound, etc. and will be clipped upon output
				// anyway.
				return aSample1 + aSample2;
			}
			
			
			
			
			/// Mix two 32-bit floating-point sample vectors without performing any saturation.
			GSOUND_FORCE_INLINE static SIMDSample32f mix( const SIMDSample32f& aSample1, const SIMDSample32f& aSample2 )
			{
				// Don't clamp the output to [-1,1]. Allow floating point aSamples
				// to exceed the maximum representable amplitude for other aSamples.
				// This is useful for HDR sound, etc. and will be clipped upon output
				// anyway.
				return aSample1 + aSample2;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Scale Methods
			
			
			
			
			/// Scale an 8-bit signed integer sample by a floating-point linear gain factor.
			GSOUND_FORCE_INLINE static Sample8 scale( Sample8 aSample, Float gain )
			{
				// Clamp the output to [-128,127]
				static const Float sampleMin = -128.0f;
				static const Float sampleMax = 127.0f;
				
				Float result = (Float)aSample*gain;
				
				return (Sample8)math::clamp( result, sampleMin, sampleMax );
			}
			
			
			
			
			/// Scale an 16-bit signed integer sample by a floating-point linear gain factor.
			GSOUND_FORCE_INLINE static Sample16 scale( Sample16 aSample, Float gain )
			{
				// Clamp the output to [-32768,32767]
				static const Float sampleMin = -32768.0f;
				static const Float sampleMax = 32767.0f;
				
				Float result = (Float)aSample*gain;
				
				return (Sample16)math::clamp( result, sampleMin, sampleMax );
			}
			
			
			
			
			/// Scale an 24-bit signed integer sample by a floating-point linear gain factor.
			GSOUND_FORCE_INLINE static Sample24 scale( Sample24 aSample, Float gain )
			{
				// Clamp the output to [-2147483648,2147483647]
				static const Double sampleMin = math::min<Sample32>();
				static const Double sampleMax = math::max<Sample32>();
				
				Double result = (Double)((Sample32)aSample)*(Double)gain;
				
				return Sample24( (Sample32)math::clamp( result, sampleMin, sampleMax ) );
			}
			
			
			
			
			/// Scale an 32-bit signed integer sample by a floating-point linear gain factor.
			GSOUND_FORCE_INLINE static Sample32 scale( Sample32 aSample, Float gain )
			{
				// Clamp the output to [-2147483648,2147483647]
				static const Double sampleMin = math::min<Sample32>();
				static const Double sampleMax = math::max<Sample32>();
				
				Double result = (Double)aSample*(Double)gain;
				
				return (Sample32)math::clamp( result, sampleMin, sampleMax );
			}
			
			
			
			
			/// Scale an 64-bit signed integer sample by a floating-point linear gain factor.
			GSOUND_FORCE_INLINE static Sample64 scale( Sample64 aSample, Float gain )
			{
				// Clamp the output to [-2^63,2^63-1]
				static const Double sampleMin = -9.223372036855e+18;
				static const Double sampleMax = 9.223372036855e+18;
				
				Double result = (Double)aSample*(Double)gain;
				
				return (Sample64)math::clamp( result, sampleMin, sampleMax );
			}
			
			
			
			
			/// Scale an 32-bit floating point sample by a floating-point linear gain factor.
			GSOUND_FORCE_INLINE static Sample32f scale( Sample32f aSample, Float gain )
			{
				return aSample*gain;
			}
			
			
			
			
			/// Scale an 64-bit floating point sample by a floating-point linear gain factor.
			GSOUND_FORCE_INLINE static Sample64f scale( Sample64f aSample, Float gain )
			{
				return aSample*gain;
			}
			
			
			
			
			/// Scale a SIMD vector of 32-bit floating point samples by a vector of floating-point linear gain factors.
			GSOUND_FORCE_INLINE static SIMDSample32f scale( const SIMDSample32f& aSample, const SIMDFloat& gain )
			{
				return aSample*gain;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Contructor
			
			
			
			
			GSOUND_FORCE_INLINE sample()
			{
			}
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		8-bit Integer Conversion Methods
//############		
//##########################################################################################
//##########################################################################################





template <>
GSOUND_FORCE_INLINE Sample8 sample:: convert<Sample8>( Sample8 aSample )
{
	return aSample;
}




template <>
GSOUND_FORCE_INLINE Sample16 sample:: convert<Sample16>( Sample8 aSample )
{
	return (Sample16)aSample << 8;
}




template <>
GSOUND_FORCE_INLINE Sample24 sample:: convert<Sample24>( Sample8 aSample )
{
	return Sample24( (Sample32)aSample << 16 );
}




template <>
GSOUND_FORCE_INLINE Sample32 sample:: convert<Sample32>( Sample8 aSample )
{
	return (Sample32)aSample << 24;
}




template <>
GSOUND_FORCE_INLINE Sample64 sample:: convert<Sample64>( Sample8 aSample )
{
	return (Sample64)aSample << 56;
}




template <>
GSOUND_FORCE_INLINE Sample32f sample:: convert<Sample32f>( Sample8 aSample )
{
	if ( aSample > 0 )
		return (Sample32f)aSample / Sample32f(127.0);
	else
		return (Sample32f)aSample / Sample32f(128.0);
}




template <>
GSOUND_FORCE_INLINE Sample64f sample:: convert<Sample64f>( Sample8 aSample )
{
	if ( aSample > 0 )
		return (Sample64f)aSample / Sample64f(127.0);
	else
		return (Sample64f)aSample / Sample64f(128.0);
}




//##########################################################################################
//##########################################################################################
//############		
//############		16-bit Integer Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
GSOUND_FORCE_INLINE Sample8 sample:: convert<Sample8>( Sample16 aSample )
{
	return Sample8(aSample >> 8);
}




template <>
GSOUND_FORCE_INLINE Sample16 sample:: convert<Sample16>( Sample16 aSample )
{
	return aSample;
}




template <>
GSOUND_FORCE_INLINE Sample24 sample:: convert<Sample24>( Sample16 aSample )
{
	return Sample24( (Sample32)aSample << 8 );
}




template <>
GSOUND_FORCE_INLINE Sample32 sample:: convert<Sample32>( Sample16 aSample )
{
	return (Sample32)aSample << 16;
}




template <>
GSOUND_FORCE_INLINE Sample64 sample:: convert<Sample64>( Sample16 aSample )
{
	return (Sample64)aSample << 48;
}




template <>
GSOUND_FORCE_INLINE Sample32f sample:: convert<Sample32f>( Sample16 aSample )
{
	if ( aSample > 0 )
		return (Sample32f)aSample / Sample32f(32767);
	else
		return (Sample32f)aSample / Sample32f(32768);
}




template <>
GSOUND_FORCE_INLINE Sample64f sample:: convert<Sample64f>( Sample16 aSample )
{
	if ( aSample > 0 )
		return (Sample64f)aSample / Sample64f(32767);
	else
		return (Sample64f)aSample / Sample64f(32768);
}




//##########################################################################################
//##########################################################################################
//############		
//############		24-bit Integer Conversion Methods
//############		
//##########################################################################################
//##########################################################################################





template <>
GSOUND_FORCE_INLINE Sample8 sample:: convert<Sample8>( Sample24 aSample )
{
	return Sample8((Sample32)aSample >> 16);
}




template <>
GSOUND_FORCE_INLINE Sample16 sample:: convert<Sample16>( Sample24 aSample )
{
	return Sample16((Sample32)aSample >> 8);
}




template <>
GSOUND_FORCE_INLINE Sample24 sample:: convert<Sample24>( Sample24 aSample )
{
	return aSample;
}




template <>
GSOUND_FORCE_INLINE Sample32 sample:: convert<Sample32>( Sample24 aSample )
{
	return (Sample32)aSample << 8;
}




template <>
GSOUND_FORCE_INLINE Sample64 sample:: convert<Sample64>( Sample24 aSample )
{
	return ((Sample64)(Sample32)aSample) << 40;
}




template <>
GSOUND_FORCE_INLINE Sample32f sample:: convert<Sample32f>( Sample24 aSample )
{
	Sample32 value = aSample;
	if ( value > 0 )
		return (Sample32f)value / Sample32f(8388607);
	else
		return (Sample32f)value / Sample32f(8388608);
}




template <>
GSOUND_FORCE_INLINE Sample64f sample:: convert<Sample64f>( Sample24 aSample )
{
	Sample32 value = aSample;
	if ( value > 0 )
		return (Sample64f)value / Sample64f(8388607);
	else
		return (Sample64f)value / Sample64f(8388608);
}




//##########################################################################################
//##########################################################################################
//############		
//############		32-bit Integer Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
GSOUND_FORCE_INLINE Sample8 sample:: convert<Sample8>( Sample32 aSample )
{
	return Sample8(aSample >> 24);
}




template <>
GSOUND_FORCE_INLINE Sample16 sample:: convert<Sample16>( Sample32 aSample )
{
	return Sample16(aSample >> 16);
}




template <>
GSOUND_FORCE_INLINE Sample24 sample:: convert<Sample24>( Sample32 aSample )
{
	return Sample24( aSample );
}




template <>
GSOUND_FORCE_INLINE Sample32 sample:: convert<Sample32>( Sample32 aSample )
{
	return aSample;
}




template <>
GSOUND_FORCE_INLINE Sample64 sample:: convert<Sample64>( Sample32 aSample )
{
	return (Sample64)aSample << 32;
}




template <>
GSOUND_FORCE_INLINE Sample32f sample:: convert<Sample32f>( Sample32 aSample )
{
	if ( aSample > 0 )
		return (Sample32f)aSample / Sample32f(2147483647.0);
	else
		return (Sample32f)aSample / Sample32f(2147483648.0);
}




template <>
GSOUND_FORCE_INLINE Sample64f sample:: convert<Sample64f>( Sample32 aSample )
{
	if ( aSample > 0 )
		return (Sample64f)aSample / Sample64f(2147483647.0);
	else
		return (Sample64f)aSample / Sample64f(2147483648.0);
}




//##########################################################################################
//##########################################################################################
//############		
//############		64-bit Integer Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
GSOUND_FORCE_INLINE Sample8 sample:: convert<Sample8>( Sample64 aSample )
{
	return Sample8(aSample >> 56);
}




template <>
GSOUND_FORCE_INLINE Sample16 sample:: convert<Sample16>( Sample64 aSample )
{
	return Sample16(aSample >> 48);
}




template <>
GSOUND_FORCE_INLINE Sample24 sample:: convert<Sample24>( Sample64 aSample )
{
	return Sample24( Sample32(aSample >> 40) );
}




template <>
GSOUND_FORCE_INLINE Sample32 sample:: convert<Sample32>( Sample64 aSample )
{
	return Sample32(aSample >> 32);
}




template <>
GSOUND_FORCE_INLINE Sample64 sample:: convert<Sample64>( Sample64 aSample )
{
	return aSample;
}




template <>
GSOUND_FORCE_INLINE Sample32f sample:: convert<Sample32f>( Sample64 aSample )
{
	return (Sample32f)aSample / Sample32f(9.223372036855e+18);
}




template <>
GSOUND_FORCE_INLINE Sample64f sample:: convert<Sample64f>( Sample64 aSample )
{
	return (Sample64f)aSample / Sample64f(9.223372036855e+18);
}




//##########################################################################################
//##########################################################################################
//############		
//############		32-bit Float Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
GSOUND_FORCE_INLINE Sample8 sample:: convert<Sample8>( Sample32f aSample )
{
	if ( aSample >= Sample32f(1) )
		return math::max<Sample8>();
	else if ( aSample <= Sample32f(-1) )
		return math::min<Sample8>();
	else
		return Sample8( aSample*math::max<Sample8>() );
}




template <>
GSOUND_FORCE_INLINE Sample16 sample:: convert<Sample16>( Sample32f aSample )
{
	if ( aSample >= Sample32f(1) )
		return math::max<Sample16>();
	else if ( aSample <= Sample32f(-1) )
		return math::min<Sample16>();
	else
		return Sample16( aSample*math::max<Sample16>() );
}




template <>
GSOUND_FORCE_INLINE Sample24 sample:: convert<Sample24>( Sample32f aSample )
{
	if ( aSample >= Sample32f(1) )
		return Sample24(8388607);
	else if ( aSample <= Sample32f(-1) )
		return Sample24(-8388608);
	else
		return Sample24( Sample32(aSample*Sample32f(8388607)) );
}




template <>
GSOUND_FORCE_INLINE Sample32 sample:: convert<Sample32>( Sample32f aSample )
{
	if ( aSample >= Sample32f(1) )
		return math::max<Sample32>();
	else if ( aSample <= Sample32f(-1) )
		return math::min<Sample32>();
	else
		return Sample32( aSample*math::max<Sample32>() );
}




template <>
GSOUND_FORCE_INLINE Sample64 sample:: convert<Sample64>( Sample32f aSample )
{
	if ( aSample >= Sample32f(1) )
		return math::max<Sample64>();
	else if ( aSample <= Sample32f(-1) )
		return math::min<Sample64>();
	else
		return Sample64( aSample*(Sample32f)math::max<Sample64>() );
}




template <>
GSOUND_FORCE_INLINE Sample32f sample:: convert<Sample32f>( Sample32f aSample )
{
	return aSample;
}




template <>
GSOUND_FORCE_INLINE Sample64f sample:: convert<Sample64f>( Sample32f aSample )
{
	return (Sample64f)aSample;
}




//##########################################################################################
//##########################################################################################
//############		
//############		64-bit Float Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
GSOUND_FORCE_INLINE Sample8 sample:: convert<Sample8>( Sample64f aSample )
{
	if ( aSample >= Sample64f(1) )
		return math::max<Sample8>();
	else if ( aSample <= Sample64f(-1) )
		return math::min<Sample8>();
	else
		return Sample8( aSample*math::max<Sample8>() );
}




template <>
GSOUND_FORCE_INLINE Sample16 sample:: convert<Sample16>( Sample64f aSample )
{
	if ( aSample >= Sample64f(1) )
		return math::max<Sample16>();
	else if ( aSample <= Sample64f(-1) )
		return math::min<Sample16>();
	else
		return Sample16( aSample*math::max<Sample16>() );
}




template <>
GSOUND_FORCE_INLINE Sample24 sample:: convert<Sample24>( Sample64f aSample )
{
	if ( aSample >= Sample64f(1) )
		return Sample24(8388607);
	else if ( aSample <= Sample64f(-1) )
		return Sample24(-8388608);
	else
		return Sample24( Sample32(aSample*Sample64f(8388607)) );
}




template <>
GSOUND_FORCE_INLINE Sample32 sample:: convert<Sample32>( Sample64f aSample )
{
	if ( aSample >= Sample64f(1) )
		return math::max<Sample32>();
	else if ( aSample <= Sample64f(-1) )
		return math::min<Sample32>();
	else
		return Sample32( aSample*math::max<Sample32>() );
}




template <>
GSOUND_FORCE_INLINE Sample64 sample:: convert<Sample64>( Sample64f aSample )
{
	if ( aSample >= Sample64f(1) )
		return math::max<Sample64>();
	else if ( aSample <= Sample64f(-1) )
		return math::min<Sample64>();
	else
		return Sample64( aSample*math::max<Sample64>() );
}




template <>
GSOUND_FORCE_INLINE Sample32f sample:: convert<Sample32f>( Sample64f aSample )
{
	return (Sample32f)aSample;
}




template <>
GSOUND_FORCE_INLINE Sample64f sample:: convert<Sample64f>( Sample64f aSample )
{
	return aSample;
}




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SAMPLE_MATH_H
