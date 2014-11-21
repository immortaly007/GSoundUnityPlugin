/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundInput.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundInput class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_INPUT_H
#define INCLUDE_GSOUND_SOUND_INPUT_H


#include "GSoundDSPConfig.h"


#include "SoundOutput.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// An interface representing an object which gets audio data from a SoundOutput object.
class SoundInput
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy a SoundInput object.
			virtual ~SoundInput()
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Input Accessor Methods
			
			
			
			
			/// Get a pointer to the SoundOutput object which is providing audio to this SoundInput object.
			/**
			  * If the SoundInput object doesn't have an input, NULL should be returned.
			  */
			virtual SoundOutput* getInput() const = 0;
			
			
			
			
			/// Set a pointer to the SoundOutput object which should provide audio to this SoundInput object.
			/**
			  * Any implementor of this interface should be able to cope with a NULL pointer being passed
			  * to this method with no il-effects.
			  * 
			  * @param newInput - the new SoundOutput object to use as input for this SoundInput object.
			  */
			virtual void setInput( SoundOutput* newInput ) = 0;
			
			
			
			
			/// Remove any previously present input object from this SoundInput.
			virtual void removeInput() = 0;
			
			
			
			
			/// Return whether or not this SoundInput object has a valid input oject.
			virtual Bool hasInput() const = 0;
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_INPUT_H
