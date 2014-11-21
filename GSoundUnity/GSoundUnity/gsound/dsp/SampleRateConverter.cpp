/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SampleRateConverter.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SampleRateConverter class implementation
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


#include "SampleRateConverter.h"


#include "SampleMath.h"


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




SampleRateConverter:: SampleRateConverter()
	:	input( NULL ),
		sampleRate( 0 ),
		subSampleOffset( 0 )
{
}




SampleRateConverter:: SampleRateConverter( Float newSampleRate )
	:	input( NULL ),
		sampleRate( math::max( newSampleRate, Float(0) ) ),
		subSampleOffset( 0 )
{
}




SampleRateConverter:: SampleRateConverter( SoundOutput* newInput )
	:	input( newInput ),
		subSampleOffset( 0 )
{
	if ( input != NULL )
		sampleRate = input->getSampleRate();
	else
		sampleRate = 0;
}




SampleRateConverter:: SampleRateConverter( SoundOutput* newInput, Float newSampleRate )
	:	input( newInput ),
		sampleRate( math::max( newSampleRate, Float(0) ) ),
		subSampleOffset( 0 )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Rate Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Float SampleRateConverter:: getSampleRate() const
{
	return sampleRate;
}




void SampleRateConverter:: setSampleRate( Float newSampleRate )
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	// Set the target sample rate, ensuring that it is positive.
	sampleRate = math::max( newSampleRate, Float(0) );
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Input Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundOutput* SampleRateConverter:: getInput() const
{
	return input;
}




void SampleRateConverter:: setInput( SoundOutput* newInput )
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	// Set the input.
	input = newInput;
	
	// Reset the sub-sample offset value for linear sample interpolation to 0.
	subSampleOffset = Float(0);
	
	// If the target sample rate was previously zero, use the input's sample rate as the output sample rate.
	if ( sampleRate == Float(0) && input != NULL )
		sampleRate = input->getSampleRate();
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
}




void SampleRateConverter:: removeInput()
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	// Set the input to NULL.
	input = NULL;
	
	// Reset the sub-sample offset value for linear sample interpolation to 0.
	subSampleOffset = Float(0);
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
}




Bool SampleRateConverter:: hasInput() const
{
	return input != NULL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SampleRateConverter:: getNumberOfChannels() const
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	// Get the number of channels from the input if it is not NULL, otherwise return 0;
	Size numChannels = input == NULL ? Size(0) : input->getNumberOfChannels();
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
	
	return numChannels;
}




Size SampleRateConverter:: getNumberOfOutputs() const
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	// Get the number of channels from the input if it is not NULL, otherwise return 0;
	Size numOutputs = input == NULL ? Size(0) : input->getNumberOfOutputs();
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
	
	return numOutputs;
}




Bool SampleRateConverter:: hasOutputRemaining() const
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	if ( input == NULL )
	{
		// Relase the mutex which indicates that rendering parameters are either being used or changed.
		renderMutex.release();
		
		return false;
	}
	else
	{
		Bool hasOutputRemaining = input->hasOutputRemaining();
		
		// Relase the mutex which indicates that rendering parameters are either being used or changed.
		renderMutex.release();
		
		return hasOutputRemaining;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Rendering Methods
//############		
//##########################################################################################
//##########################################################################################




Size SampleRateConverter:: fillBuffer( SoundStream& outputStream, Index startIndex, Size numSamples )
{
	// Write no samples to the output stream if the input is either NULL or has no audio data remaining.
	if ( input == NULL || !input->hasOutputRemaining() )
		return 0;
	
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	Float inputSampleRate = input->getSampleRate();
	
	// If the input sample rate is the same as the output sample rate,
	// pass the sound directly to the output and return.
	if ( sampleRate == inputSampleRate )
	{
		Size numSamplesRead = input->getSamples( outputStream, startIndex, numSamples );
		
		// Relase the mutex which indicates that rendering parameters are either being used or changed.
		renderMutex.release();
		
		return numSamplesRead;
	}
	
	// If the input or output sample rates are zero, return that no samples were written.
	// This is done to protect the rest of the sample interpolation method from divide by zeros/
	// infinite loops and is physically correct.
	if ( sampleRate == Float(0) || inputSampleRate == Float(0) )
	{
		// Relase the mutex which indicates that rendering parameters are either being used or changed.
		renderMutex.release();
		
		return 0;
	}
	
	// Get the number of outputs and channels from the input object.
	Size numOutputs = input->getNumberOfOutputs();
	Size numChannels = input->getNumberOfChannels();
	
	// Compute the ratio of the input to output sample rates.
	Float sampleRateRatio = inputSampleRate/sampleRate;
	
	// Compute the number of samples to read from the input (to maintain real-time performance).
	Size numSamplesToRead = Size(Float(numSamples)*sampleRateRatio + subSampleOffset);
	
	// Get the audio from the input object in an internal audio stream.
	Size numSamplesRead = input->getSamples( inputStream, 2, numSamplesToRead );
	
	// Compute the number of samples that will be placed in the output stream.
	Size numOutputSamples;
	
	if ( numSamplesRead == numSamplesToRead )
		numOutputSamples = numSamples;
	else
		numOutputSamples = Size(Float(numSamplesRead)/sampleRateRatio);
	
	for ( Index i = 0; i < numOutputs; i++ )
	{
		SoundBuffer& inputBuffer = inputStream.getBuffer(i);
		SoundBuffer& outputBuffer = outputStream.getBuffer(i);
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			Sample* firstInputSample = inputBuffer.getChannelStart(c);
			Sample* secondInputSample = firstInputSample + 1;
			const Sample* inputSample = secondInputSample;
			const Sample* const inputEnd = firstInputSample + 2 + numSamplesRead;
			Sample* outputSample = outputBuffer.getChannelStart(c) + startIndex;
			const Sample* const outputEnd = outputSample + numOutputSamples;
			
			// The interpolated input sample offset which will probably not be a whole number.
			Float currentInputSample = subSampleOffset;
			
			// The mathematical floor of the currentInputSample value.
			Float currentInputSampleIndex = Float(0);
			
			// Setup the input sample rate state.
			Sample lastInputSample = *firstInputSample;
			
			while ( outputSample != outputEnd )
			{
				// A value indicating how far the interpolation is between the last and current input samples.
				Float a = currentInputSample - currentInputSampleIndex;
				
				// Compute the output sample by linearly interpolating between the current and
				// previous input samples.
				*outputSample = sample::mix( sample::scale( *inputSample, a ), sample::scale( lastInputSample, (Float(1) - a) ) );
				
				// Increment the input sample position.
				currentInputSample += sampleRateRatio;
				
				// Update the current input sample state if necessary.
				while ( currentInputSample - currentInputSampleIndex >= Float(1) && inputSample + 1 < inputEnd )
				{
					currentInputSampleIndex += 1.0f;
					lastInputSample = *inputSample;
					inputSample++;
				}
				
				outputSample++;
			}
			
			*firstInputSample = lastInputSample;
			*secondInputSample = *inputSample;
		}
	}
	
	// Update the sub-sample offset value and normalize it to between 0 and 1.
	Float sampleOffset = subSampleOffset + Float(numOutputSamples)*sampleRateRatio;
	subSampleOffset = sampleOffset - math::floor(sampleOffset);
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
	
	return numOutputSamples;
}




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
