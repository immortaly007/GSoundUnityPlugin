/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SampleType.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SampleType class declaration
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


#ifndef INCLUDE_GSOUND_SAMPLE_TYPE_H
#define INCLUDE_GSOUND_SAMPLE_TYPE_H


#include "GSoundDSPConfig.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// An enum wrapper class which specifies the type of a sample of audio data.
/**
  * In addition to providing conversion operator to and from the underlying enum type,
  * the class also provides a way to query the size in bytes of a given sample type,
  * avoiding the need for an external switch statement.
  */
class SampleType
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Type Enum Definition
			
			
			
			
			/// The underlying enum type which specifies the type of a sample of audio data.
			typedef enum Enum
			{
				/// An 8-bit signed integer sample, stored in native endian format.
				SAMPLE_8,
				
				/// A 16-bit signed integer sample, stored in native endian format.
				SAMPLE_16,
				
				/// A 24-bit signed integer sample, stored in native endian format.
				SAMPLE_24,
				
				/// A 32-bit signed integer sample, stored in native endian format.
				SAMPLE_32,
				
				/// A 64-bit signed integer sample, stored in native endian format.
				SAMPLE_64,
				
				/// A 32-bit floating point sample, stored in native endian format.
				SAMPLE_32F,
				
				/// A 64-bit floating point sample, stored in native endian format.
				SAMPLE_64F,
				
				/// An undefined/unsupported sample type.
				UNDEFINED
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a new sample type with the undefined sample type enum value.
			GSOUND_INLINE SampleType()
				:	type( UNDEFINED )
			{
			}
			
			
			
			
			/// Create a new sample type with the specified sample type enum value.
			GSOUND_INLINE SampleType( Enum newType )
				:	type( newType )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			
			
			/// Convert this sample type to an enum value.
			/**
			  * This operator is provided so that the SampleType object can be used
			  * directly in a switch statement without the need to explicitly access
			  * the underlying enum value.
			  * 
			  * @return the enum representation of this sample type.
			  */
			GSOUND_INLINE operator Enum () const
			{
				return type;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Size Accessor Methods
			
			
			
			
			/// Get the size in bytes that this sample type occupies.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				switch ( type )
				{
					case SAMPLE_8:		return 1;
					case SAMPLE_16:		return 2;
					case SAMPLE_24:		return 3;
					case SAMPLE_32:		return 4;
					case SAMPLE_64:		return 8;
					case SAMPLE_32F:	return 4;
					case SAMPLE_64F:	return 8;
					default:			return 0;
				};
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The underlying enum representing the type of sample for this SampleType object.
			Enum type;
			
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SAMPLE_TYPE_H
