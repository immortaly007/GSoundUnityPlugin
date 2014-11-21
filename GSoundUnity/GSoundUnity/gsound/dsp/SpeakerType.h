/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SpeakerType.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SpeakerType class declaration
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


#ifndef INCLUDE_GSOUND_SPEAKER_TYPE_H
#define INCLUDE_GSOUND_SPEAKER_TYPE_H


#include "GSoundDSPConfig.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// An enum wrapper class which specifies the logical function of a speaker.
class SpeakerType
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Speaker Type Enum Definition
			
			
			
			
			/// The underlying enum type which specifies the logical function of a speaker.
			typedef enum Enum
			{
				/// A speaker which produces non-directional sound for the listener.
				MONO,
				
				/// A speaker which is positioned to the left of the listener.
				LEFT,
				
				/// A speaker which is positioned to the front and left of the listener.
				FRONT_LEFT,
				
				/// A speaker which is the left side of a pair of headphones.
				HEADPHONE_LEFT = LEFT,
				
				/// A speaker which is positioned directly in front of the listener.
				CENTER,
				
				/// A speaker which is positioned directly in front of the listener.
				FRONT_CENTER = CENTER,
				
				/// A speaker which is positioned to the right of the listener.
				RIGHT,
				
				/// A speaker which is positioned to the front and right of the listener.
				FRONT_RIGHT,
				
				/// A speaker which is the right side of a pair of headphones.
				HEADPHONE_RIGHT = RIGHT,
				
				/// A speaker which is positioned behind and to the left of the listener.
				BACK_LEFT,
				
				/// A speaker which is positioned directly behind the listener.
				BACK_CENTER,
				
				/// A speaker which is positioned behind and to the right of the listener.
				BACK_RIGHT,
				
				/// A speaker which is positioned to the left side of the listener.
				SIDE_LEFT = LEFT,
				
				/// A speaker which is positioned to the right side of the listener.
				SIDE_RIGHT = RIGHT,
				
				/// A speaker which caries low-frequency audio information only.
				SUBWOOFER,
				
				/// An undefined/unsupported speaker type.
				UNDEFINED
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a new SpeakerType with the specified speaker type enum value.
			GSOUND_INLINE SpeakerType( Enum newType )
				:	type( newType )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			
			
			/// Convert this SpeakerType to an enum value.
			/**
			  * This operator is provided so that the SpeakerType object can be used
			  * directly in a switch statement without the need to explicitly access
			  * the underlying enum value.
			  * 
			  * @return the enum representation of this SpeakerType.
			  */
			GSOUND_INLINE operator Enum () const
			{
				return type;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The underlying enum which specifies the function of a speaker.
			Enum type;
			
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SPEAKER_TYPE_H
