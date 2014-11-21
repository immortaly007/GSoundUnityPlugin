/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/Crossover.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::Crossover class implementation
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


#include "Crossover.h"


#include "SampleMath.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//##########################################################################################
//##########################################################################################
//############		
//############		Crossover Filter Class Definition
//############		
//##########################################################################################
//##########################################################################################



class Crossover:: SecondOrderFilter
{
	public:
		
		GSOUND_INLINE SecondOrderFilter()
			:	numChannels( 0 ),
				channels( NULL )
		{
		}
		
		
		GSOUND_INLINE SecondOrderFilter( const SecondOrderFilter& other )
			:	a0( other.a0 ),
				a1( other.a1 ),
				a2( other.a2 ),
				b1( other.b1 ),
				b2( other.b2 ),
				numChannels( other.numChannels )
		{
			if ( other.channels != NULL )
				channels = util::constructArray<FilterSampleBuffer>( other.numChannels );
			else
				channels = NULL;
		}
		
		
		GSOUND_INLINE ~SecondOrderFilter()
		{
			if ( channels != NULL )
				util::destructArray( channels, numChannels );
		}
		
		
		
		GSOUND_INLINE SecondOrderFilter& operator = ( const SecondOrderFilter& other )
		{
			if ( this != &other )
			{
				if ( channels != NULL )
					util::destructArray( channels, numChannels );
				
				a0 = other.a0;
				a1 = other.a1;
				a2 = other.a2;
				b1 = other.b1;
				b2 = other.b2;
				numChannels = other.numChannels;
				
				if ( other.channels != NULL )
					channels = util::constructArray<FilterSampleBuffer>( other.numChannels );
				else
					channels = NULL;
			}
			
			return *this;
		}
		
		
		
		void performFilter( const SoundBuffer& inputBuffer, Index inputStartIndex, 
							SoundBuffer& outputBuffer, Index outputStartIndex, Size numSamples );
		
		
		
		Float a0;
		Float a1;
		Float a2;
		Float b1;
		Float b2;
		
		
		
	private:
		
		typedef struct FilterSampleBuffer
		{
			GSOUND_INLINE FilterSampleBuffer()
			{
				for ( Index i = 0; i < 4; i++ )
				{
					inputs[i] = Sample(0);
					outputs[i] = Sample(0);
				}
			}
			
			Sample inputs[4];
			Sample outputs[4];
		};
		
		
		Size numChannels;
		
		
		FilterSampleBuffer* channels;
		
		
		
};




void Crossover:: SecondOrderFilter:: performFilter( const SoundBuffer& inputBuffer, Index inputStartIndex,
													SoundBuffer& outputBuffer, Index outputStartIndex, Size numSamples )
{
	// Check some conditions that should be always true if the crossover class doesn't mess things up.
	GSOUND_DEBUG_ASSERT( inputBuffer.getNumberOfChannels() <= outputBuffer.getNumberOfChannels() );
	GSOUND_DEBUG_ASSERT( inputBuffer.getSize() + inputStartIndex >= numSamples );
	GSOUND_DEBUG_ASSERT( outputBuffer.getSize() + outputStartIndex >= numSamples );
	
	// Make sure that the sample buffers in use by this filter have the correct number of channels.
	if ( numChannels != inputBuffer.getNumberOfChannels() )
	{
		if ( channels != NULL )
			util::destructArray( channels, numChannels );
		
		numChannels = inputBuffer.getNumberOfChannels();
		channels = util::constructArray<FilterSampleBuffer>( inputBuffer.getNumberOfChannels() );
	}
	
	
	for ( Index i = 0; i < numChannels; i++ )
	{
		const Sample* source = inputBuffer.getChannelStart(i) + inputStartIndex;
		const Sample* const sourceEnd = source + numSamples;
		Sample* destination = outputBuffer.getChannelStart(i) + outputStartIndex;
		
		Sample* lastInput = channels[i].inputs;
		Sample* lastOutput = channels[i].outputs;
		
		while ( source != sourceEnd )
		{
			Sample input = *source;
			
			for ( Index j = 0; j < 4; j += 2 )
			{
				// Calculate the value of the current sample.
				*destination = sample::mix( input*a0,
								sample::mix( sample::scale( lastInput[j], a1 ),
								sample::mix( sample::scale( lastInput[j + 1], a2 ),
								sample::mix( sample::scale( lastOutput[j], b1 ),
											sample::scale( lastOutput[j + 1], b2 ) ) ) ) );
				
				// Buffer the last 2 input and output samples
				lastInput[j + 1] = lastInput[j];
				lastInput[j] = input;
				
				lastOutput[j + 1] = lastOutput[j];
				lastOutput[j] = *destination;
				
				input = *destination;
			}
			
			source++;
			destination++;
		}
	}
}




class Crossover:: CrossoverFilter
{
	public:
		
		GSOUND_INLINE CrossoverFilter( Float newFrequency )
			:	frequency( newFrequency )
		{
		}
		
		
		GSOUND_INLINE CrossoverFilter( Float newFrequency, Float sampleRate )
		{
			updateFilterCoefficients( newFrequency, sampleRate );
		}
		
		
		
		void updateFilterCoefficients( Float newFrequency, Float sampleRate );
		
		
		
		Float frequency;
		
		SecondOrderFilter highPass;
		SecondOrderFilter lowPass;
		
};




void Crossover:: CrossoverFilter:: updateFilterCoefficients( Float newFrequency, Float sampleRate )
{
	// This method will break unless we detect a zero sample rate as a special case.
	if ( sampleRate == Float(0) )
	{
		// Set all filter coefficients to zero and save the crossover frequency.
		lowPass.a0 = highPass.a0 = Float(0);
		lowPass.a1 = highPass.a1 = Float(0);
		lowPass.a2 = highPass.a2 = Float(0);
		lowPass.b1 = highPass.b1 = Float(0);
		lowPass.b2 = highPass.b2 = Float(0);
		frequency = newFrequency;
		
		return;
	}
	
	frequency = newFrequency;
	Float sanitizedFrequency = math::clamp( newFrequency, 0.0f, sampleRate * 0.5f );
	
	// Coefficients of the butterworth polynomial.
	Float g = 1.0f;
	Float p = math::sqrt(2.0f);
	
	// Calculate the filter coefficients for the low pass filter.
	{
		Float correctedFrequency = sanitizedFrequency/sampleRate;
		
		Float w0 = math::tan( math::pi<Float>()*correctedFrequency );
		
		Float k1 = p*w0;
		Float k2 = g*w0*w0;
		
		lowPass.a0 = k2/(1.0f + k1 + k2);
		lowPass.a1 = 2.0f*lowPass.a0;
		lowPass.a2 = lowPass.a0;
		lowPass.b1 = 2.0f*lowPass.a0*(1.0f/k2 - 1.0f);
		lowPass.b2 = 1.0f - (lowPass.a0 + lowPass.a1 + lowPass.a2 + lowPass.b1);
	}
	{
		// Calculate the corrected sample freqency of the high pass filter
		Float correctedFrequency = 0.5f - sanitizedFrequency/sampleRate;
		
		Float w0 = math::tan( math::pi<Float>()*correctedFrequency );
		
		Float k1 = p*w0;
		Float k2 = g*w0*w0;
		
		highPass.a0 = k2/(1.0f + k1 + k2);
		highPass.a1 = -2.0f*highPass.a0;
		highPass.a2 = highPass.a0;
		highPass.b1 = -2.0f*highPass.a0*(1.0f/k2 - 1.0f);
		highPass.b2 = 1.0f - (highPass.a0 - highPass.a1 + highPass.a2 - highPass.b1);
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Crossover:: Crossover()
	:	input( NULL ),
		inputStream(),
		sampleRate( 0 )
{
}




Crossover:: Crossover( SoundOutput* newInput )
	:	input( newInput ),
		inputStream(),
		sampleRate( newInput == NULL ? 0 : newInput->getSampleRate() )
{
}




Crossover:: Crossover( SoundOutput* newInput, const ArrayList<Float>& newCrossoverFrequencies )
	:	input( newInput ),
		inputStream(),
		sampleRate( newInput == NULL ? 0 : newInput->getSampleRate() ) 
{
	initializeCrossoverFilters( newCrossoverFrequencies );
}




Crossover:: Crossover( const Crossover& other )
	:	input( other.input ),
		inputStream( other.inputStream ),
		sampleRate( other.sampleRate ) 
{
	for ( Index i = 0; i < other.getNumberOfCrossoverFrequencies(); i++ )
		this->addCrossoverFrequency( other.getCrossoverFrequency(i) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Crossover:: ~Crossover()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




Crossover& Crossover:: operator = ( const Crossover& other )
{
	if ( this != &other )
	{
		// Acquire the mutex which indicates that rendering parameters are either being used or changed.
		renderMutex.acquire();
		
		input = other.input;
		filters = other.filters;
		inputStream = other.inputStream;
		sampleRate = other.sampleRate;
		
		// Relase the mutex which indicates that rendering parameters are either being used or changed.
		renderMutex.release();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Crossover Frequency Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Float Crossover:: getCrossoverFrequency( Index crossoverFrequencyIndex ) const
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	GSOUND_DEBUG_ASSERT_MESSAGE( crossoverFrequencyIndex < filters.getSize(),
								"Cannot access crossover frequency at invalid index." );
	
	Float frequency = filters[crossoverFrequencyIndex].frequency;
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
	
	return frequency;
}




void Crossover:: setCrossoverFrequencies( const ArrayList<Float>& newCrossoverFrequencies )
{
	this->initializeCrossoverFilters( newCrossoverFrequencies );
}




Bool Crossover:: addCrossoverFrequency( Float newCrossoverFrequency )
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	// Make sure that the crossover frequency is valid before adding it.
	if ( input == NULL || (newCrossoverFrequency > 0.0f && newCrossoverFrequency < 0.5f*sampleRate) )
	{
		if ( filters.getSize() > 0 )
		{
			// Should we insert the new crossover frequency?
			for ( Index i = 0; i < filters.getSize(); i++ )
			{
				if ( filters[i].frequency > newCrossoverFrequency )
				{
					filters.insert( CrossoverFilter( newCrossoverFrequency, sampleRate ), i );
					
					// Relase the mutex which indicates that rendering parameters are either being used or changed.
					renderMutex.release();
					
					return true;
				}
			}
		}
		
		// If not, then add the crossover filter to the end.
		filters.add( CrossoverFilter( newCrossoverFrequency, sampleRate ) );
		
		// Relase the mutex which indicates that rendering parameters are either being used or changed.
		renderMutex.release();
		
		return true;
	}
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
	
	return false;
}




void Crossover:: removeCrossoverFrequency( Index crossoverFrequencyIndex )
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	GSOUND_DEBUG_ASSERT_MESSAGE( crossoverFrequencyIndex < filters.getSize(),
								"Cannot remove crossover frequency at invalid index." );
	
	filters.removeAtIndex( crossoverFrequencyIndex );
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
}




void Crossover:: initializeCrossoverFilters( const ArrayList<Float>& newCrossoverFrequencies )
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	filters.clear();
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
	
	for ( Index i = 0; i < newCrossoverFrequencies.getSize(); i++ )
		this->addCrossoverFrequency( newCrossoverFrequencies[i] );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Input Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundOutput* Crossover:: getInput() const
{
	return input;
}




void Crossover:: setInput( SoundOutput* newInput )
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	input = newInput;
	
	if ( input != NULL )
	{
		sampleRate = input->getSampleRate();
		
		for ( Index i = 0; i < filters.getSize(); i++ )
			filters[i].updateFilterCoefficients( filters[i].frequency, sampleRate );
	}
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
}




void Crossover:: removeInput()
{
	this->setInput( NULL );
}




Bool Crossover:: hasInput() const
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




Float Crossover:: getSampleRate() const
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	// Get the sample rate from the input if it is not NULL, otherwise return 0;
	Float sampleRate = input == NULL ? Float(0) : input->getSampleRate();
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
	
	return sampleRate;
}




Size Crossover:: getNumberOfChannels() const
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	// Get the number of channels from the input if it is not NULL, otherwise return 0;
	Size numChannels = input == NULL ? Size(0) : input->getNumberOfChannels();
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
	
	return numChannels;
}




Size Crossover:: getNumberOfOutputs() const
{
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	Size numOutputs = filters.getSize() + 1;
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
	
	return numOutputs;
}




Bool Crossover:: hasOutputRemaining() const
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
//############		Main Crossover Filter Rendering Method
//############		
//##########################################################################################
//##########################################################################################




Size Crossover:: fillBuffer( SoundStream& outputStream, Index startIndex, Size numSamples )
{
	if ( input == NULL || !input->hasOutputRemaining() )
		return 0;
	
	// Acquire the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	// If the input sample rate has changed, reset the filter coefficients.
	if ( sampleRate != input->getSampleRate() )
	{
		sampleRate = input->getSampleRate();
		
		for ( Index i = 0; i < filters.getSize(); i++ )
			filters[i].updateFilterCoefficients( filters[i].frequency, sampleRate );
	}
	
	// If there are no crossover filters, pass input directly to the output.
	if ( filters.getSize() == 0 )
	{
		// Relase the mutex which indicates that rendering parameters are either being used or changed.
		renderMutex.release();
		
		return input->getSamples( outputStream, startIndex, numSamples );
	}
	
	// Get the input audio for the crossover.
	Size numRead = input->getSamples( inputStream, numSamples );
	
	// Perform the low pass filter for the lowest frequency band.
	filters[0].lowPass.performFilter( inputStream.getBuffer(0), 0, outputStream.getBuffer(0), startIndex, numRead );
	
	Size numFilters = filters.getSize();
	
	for ( Index i = 1; i < numFilters; i++ )
	{
		filters[i - 1].highPass.performFilter( inputStream.getBuffer(0), 0, outputStream.getBuffer(i), startIndex, numRead );
		filters[i].lowPass.performFilter( outputStream.getBuffer(i), startIndex, outputStream.getBuffer(i), startIndex, numRead );
	}
	
	// Perform the high pass filter for the highest frequency band.
	filters.getLast().highPass.performFilter( inputStream.getBuffer(0), 0, outputStream.getBuffer(numFilters), startIndex, numRead );
	
	// Relase the mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
	
	return numRead;
}




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
