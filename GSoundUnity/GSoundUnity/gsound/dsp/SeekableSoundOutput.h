/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SeekableSoundOutput.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SeekableSoundOutput class declaration
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


#ifndef INCLUDE_GSOUND_SEEKABLE_SOUND_OUTPUT_H
#define INCLUDE_GSOUND_SEEKABLE_SOUND_OUTPUT_H


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
/// An interface which represents a sound output object that allows seeking within the audio stream.
/**
  * Generally, a class which derives from SeekableSoundOutput will read audio data from
  * a buffer of known size, such as a sound file. It can be used to abstract the source
  * of the audio data (disc streamed, memory resident, etc.) so that audio data sources that
  * have a similar functional interface can be used in the same ways.
  */
class SeekableSoundOutput : public SoundOutput
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Length Accessor Methods
			
			
			
			
			/// Get the length of the seekable sound output's sound source in number of samples.
			virtual SoundSize getLengthInSamples() const = 0;
			
			
			
			
			/// Get the length of the seekable sound output's sound source in seconds.
			virtual Double getLengthInSeconds() const = 0;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Current Time Accessor Methods
			
			
			
			
			/// Get the index of the sample currently being read by this seekable sound output.
			virtual SampleIndex getCurrentSampleIndex() const = 0;
			
			
			
			
			/// Get the current position in seconds within the sound source.
			virtual Double getCurrentTime() const = 0;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Seek Methods
			
			
			
			
			/// Seek a signed number of seconds relative to the current read position in the sound stream.
			virtual Bool seek( Double timeOffset ) = 0;
			
			
			
			
			/// Seek a signed number of samples relative to the current read position in the sound stream.
			virtual Bool seek( Int64 sampleOffset ) = 0;
			
			
			
			
			/// Seek to the specified absolute time in seconds within the audio stream.
			virtual Bool seekTo( Double time ) = 0;
			
			
			
			
			/// Seek to the specified absolute sample index within the audio stream.
			virtual Bool seekTo( SampleIndex sampleIndex ) = 0;
			
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SEEKABLE_SOUND_OUTPUT_H
