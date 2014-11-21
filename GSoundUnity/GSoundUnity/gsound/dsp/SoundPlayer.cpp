/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundPlayer.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundPlayer class implementation
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


#include "SoundPlayer.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################





SoundPlayer:: SoundPlayer()
	:	input( NULL ),
		currentSampleIndex( 0 ),
		isPlaying( false ),
		isLooping( false )
{
}




SoundPlayer:: SoundPlayer( SeekableSoundOutput* newInput )
	:	input( newInput ),
		currentSampleIndex( 0 ),
		isPlaying( false ),
		isLooping( false )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Playing State Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPlayer:: getIsPlaying() const
{
	return isPlaying;
}




void SoundPlayer:: setIsPlaying( Bool newIsPlaying )
{
	isPlaying = newIsPlaying;
}




void SoundPlayer:: play()
{
	isPlaying = true;
}




void SoundPlayer:: pause()
{
	isPlaying = false;
}




void SoundPlayer:: stop()
{
	isPlaying = false;
	currentSampleIndex = 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Looping State Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPlayer:: getIsLooping() const
{
	return isLooping;
}




void SoundPlayer:: setIsLooping( Bool newIsLooping )
{
	isLooping = newIsLooping;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Current Time Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundSize SoundPlayer:: getLengthInSamples() const
{
	if ( input != NULL )
		return input->getLengthInSamples();
	else
		return SoundSize(0);
}




Double SoundPlayer:: getLengthInSeconds() const
{
	if ( input != NULL )
		return input->getLengthInSeconds();
	else
		return Double(0);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Current Time Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SampleIndex SoundPlayer:: getCurrentSampleIndex() const
{
	return currentSampleIndex;
}




Double SoundPlayer:: getCurrentTime() const
{
	if ( input != NULL )
		return Double(currentSampleIndex) / input->getSampleRate();
	else
		return Double(0);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Seek Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPlayer:: seek( Double timeOffset )
{
	if ( input != NULL && input->seek( timeOffset ) )
	{
		currentSampleIndex = input->getCurrentSampleIndex();
		
		return true;
	}
	else
		return false;
}




Bool SoundPlayer:: seek( Int64 sampleOffset )
{
	if ( input != NULL && input->seek( sampleOffset ) )
	{
		currentSampleIndex = input->getCurrentSampleIndex();
		
		return true;
	}
	else
		return false;
}




Bool SoundPlayer:: seekTo( Double time )
{
	if ( input != NULL && input->seekTo( time ) )
	{
		currentSampleIndex = input->getCurrentSampleIndex();
		
		return true;
	}
	else
		return false;
}




Bool SoundPlayer:: seekTo( SampleIndex sampleIndex )
{
	if ( input != NULL && input->seekTo( sampleIndex ) )
	{
		currentSampleIndex = input->getCurrentSampleIndex();
		
		return true;
	}
	else
		return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Float SoundPlayer:: getSampleRate() const
{
	if ( input != NULL )
		return input->getSampleRate();
	else
		return Float(0);
}




Size SoundPlayer:: getNumberOfChannels() const
{
	if ( input != NULL )
		return input->getNumberOfChannels();
	else
		return Size(0);
}




Size SoundPlayer:: getNumberOfOutputs() const
{
	if ( input != NULL )
		return input->getNumberOfOutputs();
	else
		return Size(0);
}




Bool SoundPlayer:: hasOutputRemaining() const
{
	if ( input != NULL && isPlaying )
	{
		SoundSize lengthInSamples = input->getLengthInSamples();
		
		if ( lengthInSamples != SoundSize(0) )
			return currentSampleIndex < lengthInSamples || isLooping;
		else
			return false;
	}
	else
		return false;
}



//##########################################################################################
//##########################################################################################
//############		
//############		Input Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SeekableSoundOutput* SoundPlayer:: getInput() const
{
	return input;
}




void SoundPlayer:: setInput( SeekableSoundOutput* newInput )
{
	input = newInput;
	currentSampleIndex = newInput->getCurrentSampleIndex();
}




void SoundPlayer:: removeInput()
{
	input = NULL;
}




Bool SoundPlayer:: hasInput() const
{
	return input != NULL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Rendering Method
//############		
//##########################################################################################
//##########################################################################################




Size SoundPlayer:: fillBuffer( SoundStream& stream, Index startIndex, Size numSamples )
{
	// If there is no sound input or the sound isn't playing, don't produce any sound.
	if ( input == NULL || !isPlaying )
		return 0;
	
	// Seek to the current sample index if the sound isn't currently reading from that location.
	if ( currentSampleIndex != input->getCurrentSampleIndex() )
		input->seekTo( currentSampleIndex );
	
	SoundSize soundLength = input->getLengthInSamples();
	
	// If the end of the sound has been reached, don't produce any sound.
	if ( currentSampleIndex >= soundLength && !isLooping )
		return 0;
	
	// Get the sound from the input object.
	Size numSamplesRead = input->getSamples( stream, startIndex, numSamples );
	
	// Update the current position of the sound player.
	currentSampleIndex += numSamplesRead;
	
	// If the end of the sound has been reached and the sound is looping,
	// put more sound in the output stream.
	if ( currentSampleIndex == soundLength )
	{
		if ( isLooping )
		{
			// The number of samples that have been put into the output stream.
			Size numSamplesFilled = numSamplesRead;
			
			// While the sound output stream hasn't been totally filled, continue
			// to repeat the sound.
			while ( numSamplesFilled < numSamples )
			{
				// Seek to the beginning of the sound.
				input->seekTo( SampleIndex(0) );
				currentSampleIndex = 0;
				
				// Get more sound from the input.
				numSamplesRead = input->getSamples( stream, startIndex + numSamplesFilled, numSamples - numSamplesFilled );
				
				// Update the current position of the sound player.
				currentSampleIndex += numSamplesRead;
				numSamplesFilled += numSamplesRead;
			}
			
			return numSamplesFilled;
		}
		else
			isPlaying = false;
	}
	
	return numSamplesRead;
}




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
