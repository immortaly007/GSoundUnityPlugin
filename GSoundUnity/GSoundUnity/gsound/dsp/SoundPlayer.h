/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundPlayer.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundPlayer class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_PLAYER_H
#define INCLUDE_GSOUND_SOUND_PLAYER_H


#include "GSoundDSPConfig.h"


#include "SeekableSoundOutput.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which provides a simple interface for playing and looping sounds.
/**
  * 
  */
class SoundPlayer : public SeekableSoundOutput
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			SoundPlayer();
			
			
			
			
			SoundPlayer( SeekableSoundOutput* newInput );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Playing State Accessor Methods
			
			
			
			
			Bool getIsPlaying() const;
			
			
			
			
			void setIsPlaying( Bool newIsPlaying );
			
			
			
			
			void play();
			
			
			
			
			void pause();
			
			
			
			void stop();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Looping State Accessor Methods
			
			
			
			
			/// Get whether or not the SoundPlayer is looping the sound which it is playing.
			Bool getIsLooping() const;
			
			
			
			
			/// Set whether or not the SoundPlayer should loop the sound that it is playing.
			void setIsLooping( Bool newIsLooping );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Length Accessor Methods
			
			
			
			
			/// Get the length of the sound that is being played by this SoundPlayer in samples.
			virtual SoundSize getLengthInSamples() const;
			
			
			
			
			/// Get the length of the sound that is being played by this SoundPlayer in seconds.
			virtual Double getLengthInSeconds() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Current Time Accessor Methods
			
			
			
			
			/// Get the index of the current sample in the sound that is being played.
			virtual SampleIndex getCurrentSampleIndex() const;
			
			
			
			
			/// Get the current time within the sound that is being played.
			virtual Double getCurrentTime() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Seek Methods
			
			
			
			
			/// Seek the SoundPlayer to the specified signed time offset from the current play position.
			/**
			  * If there is no sound being played by this SoundPlayer or the specified seek offset
			  * is outside the length of the sound being played, FALSE is returned and the
			  * method has no effect. Otherwise, TRUE is returned and the seek operation is performed.
			  * 
			  * @param timeOffset - the time offset relative to the current play position to seek to.
			  * @return whether or not the seek operation was successul.
			  */
			virtual Bool seek( Double timeOffset );
			
			
			
			
			/// Seek the SoundPlayer to the specified signed sample offset from the current play position.
			/**
			  * If there is no sound being played by this SoundPlayer or the specified seek offset
			  * is outside the length of the sound being played, FALSE is returned and the
			  * method has no effect. Otherwise, TRUE is returned and the seek operation is performed.
			  * 
			  * @param timeOffset - the sample index offset relative to the current play position to seek to.
			  * @return whether or not the seek operation was successul.
			  */
			virtual Bool seek( Int64 sampleOffset );
			
			
			
			
			/// Seek the SoundPlayer to the specified time within the sound being played.
			/**
			  * If there is no sound being played by this SoundPlayer or the specified seek time
			  * is outside the length of the sound being played, FALSE is returned and the
			  * method has no effect. Otherwise, TRUE is returned and the seek operation is performed.
			  * 
			  * @param timeOffset - the time relative to the start of the sound to seek to.
			  * @return whether or not the seek operation was successul.
			  */
			virtual Bool seekTo( Double time );
			
			
			
			
			/// Seek the SoundPlayer to the specified sample index within the sound being played.
			/**
			  * If there is no sound being played by this SoundPlayer or the specified seek sample index
			  * is outside the length of the sound being played, FALSE is returned and the
			  * method has no effect. Otherwise, TRUE is returned and the seek operation is performed.
			  * 
			  * @param timeOffset - the sample index relative to the start of the sound to seek to.
			  * @return whether or not the seek operation was successul.
			  */
			virtual Bool seekTo( SampleIndex sampleIndex );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Format Accessor Methods
			
			
			
			
			/// Get the sample rate of the audio being currently produced by this SoundPlayer.
			virtual Float getSampleRate() const;
			
			
			
			
			/// Get the number of channels in the output audio stream of this SoundPlayer.
			virtual Size getNumberOfChannels() const;
			
			
			
			
			/// Get the number of outputs that this SoundPlayer has.
			virtual Size getNumberOfOutputs() const;
			
			
			
			
			/// Get whether or not this SoundPlayer has any more audio to produce.
			virtual Bool hasOutputRemaining() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Input Accessor Methods
			
			
			
			
			/// Get a pointer to the object which is producing the sound that this SoundPlayer is playing.
			virtual SeekableSoundOutput* getInput() const;
			
			
			
			
			/// Set a pointer to the object which is producing the sound that this SoundPlayer is playing.
			virtual void setInput( SeekableSoundOutput* newInput );
			
			
			
			
			/// Remove the object which is providing the input audio to this SoundPlayer.
			virtual void removeInput();
			
			
			
			
			/// Return whether or not this SoundPlayer has an object that is providing it's input audio.
			virtual Bool hasInput() const;
			
			
			
			
	protected:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Main Rendering Method
			
			
			
			
			/// Fill the output stream with the specified number of samples of the playing sound.
			virtual Size fillBuffer( SoundStream& stream, Index startIndex, Size numSamples );
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A pointer to a SeekableSoundOutput object which is providing the input audio for this SoundPlayer.
			SeekableSoundOutput* input;
			
			
			
			
			/// The current play position in samples in the SeekableSoundOutput's stream of audio.
			SampleIndex currentSampleIndex;
			
			
			
			
			/// Whether or not the SoundPlayer is currently playing sound.
			Bool isPlaying;
			
			
			
			
			/// Whether or not the SoundPlayer is looping the sound.
			Bool isLooping;
			
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_PLAYER_H
