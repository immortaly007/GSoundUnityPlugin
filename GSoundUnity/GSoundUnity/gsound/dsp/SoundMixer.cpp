/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundMixer.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundMixer class implementation
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


#include "SoundMixer.h"


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
//############		Sound Mixer Input Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundMixer:: SoundMixerInput
{
	public:
		
		GSOUND_INLINE SoundMixerInput( SoundOutput* newInput, Float newSampleRate )
			:	input( newInput ),
				sampleRateConverter( newInput, newSampleRate )
		{
		}
		
		
		
		
		GSOUND_INLINE void setInput( SoundOutput* newInput )
		{
			input = newInput;
			sampleRateConverter.setInput( newInput );
		}
		
		
		
		SoundOutput* input;
		
		
		
		SampleRateConverter sampleRateConverter;
		
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundMixer:: SoundMixer()
	:	sampleRate( Float(0) ),
		sampleRateConversionIsEnabled( true ),
		numOutputs( 0 ),
		numChannels( 0 )
{
}




SoundMixer:: SoundMixer( Float newSampleRate )
	:	sampleRate( math::max( newSampleRate, Float(0) ) ),
		sampleRateConversionIsEnabled( true ),
		numOutputs( 0 ),
		numChannels( 0 )
{
}




SoundMixer:: SoundMixer( const SoundMixer& other )
	:	inputs( other.inputs ),
		sampleRate( other.sampleRate ),
		sampleRateConversionIsEnabled( other.sampleRateConversionIsEnabled ),
		numOutputs( other.numOutputs ),
		numChannels( other.numChannels )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundMixer:: ~SoundMixer()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundMixer& SoundMixer:: operator = ( const SoundMixer& other )
{
	if ( this != &other )
	{
		inputs = other.inputs;
		sampleRate = other.sampleRate;
		sampleRateConversionIsEnabled = other.sampleRateConversionIsEnabled;
		numOutputs = other.numOutputs;
		numChannels = other.numChannels;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Sample Rate Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void SoundMixer:: setSampleRate( Float newSampleRate )
{
	// Acquire the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.acquire();
	
	// Set the sample rate.
	sampleRate = math::max( newSampleRate, Float(0) );
	
	// Tell each sample rate converter that the sample rate has changed.
	for ( Index i = 0; i < inputs.getSize(); i++ )
		inputs[i].sampleRateConverter.setSampleRate( sampleRate );
	
	// Release the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.release();
}




Float SoundMixer:: getSampleRate() const
{
	return sampleRate;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Rate Conversion Enabling Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMixer:: getSampleRateConversionIsEnabled() const
{
	return sampleRateConversionIsEnabled;
}




void SoundMixer:: setSampleRateConversionIsEnabled( Bool newSampleRateConversionIsEnabled )
{
	// Acquire the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.acquire();
	
	sampleRateConversionIsEnabled = newSampleRateConversionIsEnabled;
	
	// Release the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.release();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Input Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundMixer:: getNumberOfInputs() const
{
	return inputs.getSize();
}




SoundOutput* SoundMixer:: getInput( Index inputIndex ) const
{
	GSOUND_DEBUG_ASSERT( inputIndex < inputs.getSize() );
	
	return inputs[inputIndex].input;
}




void SoundMixer:: setInput( Index inputIndex, SoundOutput* newInput )
{
	// Acquire the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.acquire();
	
	GSOUND_DEBUG_ASSERT( inputIndex < inputs.getSize() );
	
	// If the new input is NULL, interpret it as removing the input at the specified index.
	if ( newInput == NULL )
		inputs.removeAtIndex( inputIndex );
	else
		inputs[inputIndex].setInput( newInput );
	
	// Recalculate the number of outputs and number of channels for the mixer.
	numOutputs = numChannels = 0;
	
	for ( Index i = 0; i < inputs.getSize(); i++ )
	{
		numOutputs = math::max( numOutputs, inputs[i].input->getNumberOfOutputs() );
		numChannels = math::max( numChannels, inputs[i].input->getNumberOfChannels() );
	}
	
	// Release the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.release();
}




void SoundMixer:: addInput( SoundOutput* newInput )
{
	// Ignore inputs that are NULL.
	if ( newInput == NULL )
		return;
	
	// Acquire the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.acquire();
	
	// If the sample rate has not been set yet (or is zero), set the sample rate to be the 
	// sample rate of this input.
	if ( sampleRate == Float(0) )
		sampleRate = newInput->getSampleRate();
	
	inputs.add( SoundMixerInput( newInput, sampleRate ) );
	
	// Update the output and channel count for the mixer.
	numOutputs = math::max( numOutputs, newInput->getNumberOfOutputs() );
	numChannels = math::max( numChannels, newInput->getNumberOfChannels() );
	
	// Release the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.release();
}




void SoundMixer:: removeInput( Index inputIndex )
{
	// Acquire the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.acquire();
	
	GSOUND_DEBUG_ASSERT( inputIndex < inputs.getSize() );
	
	// Remove the input at the specified index.
	inputs.removeAtIndex( inputIndex );
	
	// Recalculate the number of outputs and number of channels for the mixer.
	numOutputs = numChannels = 0;
	
	for ( Index i = 0; i < inputs.getSize(); i++ )
	{
		numOutputs = math::max( numOutputs, inputs[i].input->getNumberOfOutputs() );
		numChannels = math::max( numChannels, inputs[i].input->getNumberOfChannels() );
	}
	
	// Release the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.release();
}




Bool SoundMixer:: removeInput( SoundOutput* input )
{
	// Acquire the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.acquire();
	
	Bool foundInput = false;
	
	// Find the input.
	for ( Index i = 0; i < inputs.getSize(); i++ )
	{
		if ( inputs[i].input == input )
		{
			inputs.removeAtIndex( i );
			foundInput = true;
			
			// Recalculate the number of outputs and number of channels for the mixer.
			numOutputs = numChannels = 0;
			
			for ( Index i = 0; i < inputs.getSize(); i++ )
			{
				numOutputs = math::max( numOutputs, inputs[i].input->getNumberOfOutputs() );
				numChannels = math::max( numChannels, inputs[i].input->getNumberOfChannels() );
			}
			
			break;
		}
	}
	
	// Release the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.release();
	
	return foundInput;
}




void SoundMixer:: clearInputs()
{
	// Acquire the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.acquire();
	
	inputs.clear();
	numOutputs = 0;
	numChannels = 0;
	
	// Release the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.release();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundMixer:: getNumberOfChannels() const
{
	return numChannels;
}




Size SoundMixer:: getNumberOfOutputs() const
{
	return numOutputs;
}




Bool SoundMixer:: hasOutputRemaining() const
{
	// Acquire the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.acquire();
	
	Bool hasOutput = false;
	
	for ( Index i = 0; i < inputs.getSize() && !hasOutput; i++ )
	{
		hasOutput |= inputs[i].input->hasOutputRemaining();
	}
	
	// Release the mutex used to synchonize changes to rendering parameters and the rendering itself.
	renderMutex.release();
	
	return hasOutput;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Private Mixing Helper Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundMixer:: mixWithOutput( const Sample* input, const Sample* const inputEnd, Sample* output )
{
	while ( input < inputEnd )
	{
		*output = sample::mix( *input, *output );
		input++;
		output++;
	}
}




void SoundMixer:: mixWithOutputSIMD( const Sample* input, const Sample* const inputEnd, Sample* output )
{
	while ( input < inputEnd )
	{
		sample::mix( SIMDSample(input), SIMDSample(input) ).store( output );
		input += SIMDSample::getWidth();
		output += SIMDSample::getWidth();
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Rendering Method
//############		
//##########################################################################################
//##########################################################################################





Size SoundMixer:: fillBuffer( SoundStream& outputStream, Index startIndex, Size numSamples )
{
	// Zero the output stream.
	outputStream.zero( startIndex, numSamples );
	
	Size numInputs = inputs.getSize();
	Size maxNumSamplesRead = 0;
	
	for ( Index i = 0; i < numInputs; i++ )
	{
		SoundMixerInput& input = inputs[i];
		
		Size numSamplesRead;
		
		// If sample rate conversion is enabled, get sound from the sample rate converter.
		// Otherwise, get the sound directly from the input.
		if ( sampleRateConversionIsEnabled )
			numSamplesRead = input.sampleRateConverter.getSamples( inputStream, 0, numSamples );
		else
			numSamplesRead = input.input->getSamples( inputStream, 0, numSamples );
		
		// Update the maximum number of samples that have been read so far.
		maxNumSamplesRead = math::max( maxNumSamplesRead, numSamplesRead );
		
		// Get the number of channels and outputs for this input.
		Size numInputChannels = input.input->getNumberOfChannels();
		Size numInputOutputs = input.input->getNumberOfOutputs();
		Size numSIMDSamples = math::previousMultiple( numSamplesRead, SIMDSample::getWidth() );
		
		// Mix the sound for this input with the output stream.
		for ( Index i = 0; i < numInputOutputs; i++ )
		{
			const SoundBuffer& inputBuffer = inputStream.getBuffer(i);
			SoundBuffer& outputBuffer = outputStream.getBuffer(i);
			
			for ( Index c = 0; c < numInputChannels; c++ )
			{
				const Sample* input = inputBuffer.getChannelStart(c);
				const Sample* const inputEndSIMD = input + numSIMDSamples;
				const Sample* const inputEndScalar = input + numSamplesRead;
				Sample* outputSIMD = outputBuffer.getChannelStart(c) + startIndex;
				
				mixWithOutputSIMD( input, inputEndSIMD, outputSIMD );
				mixWithOutput( inputEndSIMD, inputEndScalar, outputSIMD + numSIMDSamples );
			}
		}
	}
	
	return maxNumSamplesRead;
}




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
