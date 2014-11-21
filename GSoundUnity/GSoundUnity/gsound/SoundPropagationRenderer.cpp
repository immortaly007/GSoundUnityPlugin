/*
 * Project:     GSound
 * 
 * File:        gsound/SoundPropagationRenderer.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundPropagationRenderer class implementation
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


#include "SoundPropagationRenderer.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Mono Mixer Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagationRenderer:: MonoMixer : public dsp::SoundProcessor
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			GSOUND_INLINE MonoMixer()
				:	input( NULL )
			{
			}
			
			
			
			
			GSOUND_INLINE MonoMixer( SoundOutput* newInput )
				:	input( newInput )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Input Accessor Methods
			
			
			
			
			virtual SoundOutput* getInput() const
			{
				return input;
			}
			
			
			
			
			virtual void setInput( SoundOutput* newInput )
			{
				// Acquire the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.acquire();
				
				input = newInput;
				
				// Relase the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.release();
			}
			
			
			
			
			virtual void removeInput()
			{
				// Acquire the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.acquire();
				
				input = NULL;
				
				// Relase the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.release();
			}
			
			
			
			
			virtual Bool hasInput() const
			{
				return input != NULL;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Format Accessor Methods
			
			
			
			
			virtual Float getSampleRate() const
			{
				if ( input == NULL )
					return Float(0);
				else
					return input->getSampleRate();
			}
			
			
			
			
			virtual Size getNumberOfChannels() const
			{
				return Size(1);
			}
			
			
			
			
			virtual Size getNumberOfOutputs() const
			{
				if ( input == NULL )
					return Size(0);
				else
					return input->getNumberOfOutputs();
			}
			
			
			
			
			virtual Bool hasOutputRemaining() const
			{
				if ( input == NULL )
					return false;
				else
					return input->hasOutputRemaining();
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Main Rendering Method
			
			
			
			
			virtual Size fillBuffer( dsp::SoundStream& outputStream, Index startIndex, Size numSamples )
			{
				// Write no samples to the output stream if the input is either NULL or has no audio data remaining.
				if ( input == NULL || !input->hasOutputRemaining() )
					return 0;
				
				// Acquire the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.acquire();
				
				Size numInputChannels = input->getNumberOfChannels();
				
				Size numRead;
				
				if ( numInputChannels == Size(1) )
				{
					// Read sound from the input audio unit directly into the output stream.
					numRead = input->getSamples( outputStream, startIndex, numSamples );
				}
				else
				{
					// Read sound from the input audio unit directly into the output stream.
					numRead = input->getSamples( inputStream, 0, numSamples );
					
					// Zero the output stream.
					outputStream.zero( startIndex, numSamples );
					
					Size numOutputs = input->getNumberOfOutputs();
					
					// Mix the channels together into the output audio stream.
					for ( Index outputIndex = 0; outputIndex < numOutputs; outputIndex++ )
					{
						dsp::SoundBuffer& inputBuffer = inputStream.getBuffer(outputIndex);
						dsp::SoundBuffer& outputBuffer = outputStream.getBuffer(outputIndex);
						
						for ( Index c = 0; c < numInputChannels; c++ )
						{
							dsp::Sample* output = outputBuffer.getChannelStart(0) + startIndex;
							const dsp::Sample* input = inputBuffer.getChannelStart(c);
							const dsp::Sample* const inputEnd = input + numRead;
							
							while ( input != inputEnd )
							{
								*output = dsp::sample::mix( *input, *output );
								
								input++;
								output++;
							}
						}
					}
				}
				
				// Relase the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.release();
				
				return numRead;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			SoundOutput* input;
			
			
			
			
			dsp::SoundStream inputStream;
			
			
			
			
			mutable Mutex renderMutex;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Mono Mixer Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagationRenderer:: MonoSplitter : public dsp::SoundProcessor
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			GSOUND_INLINE MonoSplitter( Size newNumOutputChannels )
				:	input( NULL ),
					numOutputChannels( newNumOutputChannels )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Channel Count Accessor Methods
			
			
			
			
			GSOUND_INLINE Size getNumberOfOutputChannels() const
			{
				return numOutputChannels;
			}
			
			
			
			
			GSOUND_INLINE void setNumberOfOutputChannels( Size newNumOutputChannels )
			{
				// Acquire the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.acquire();
				
				numOutputChannels = newNumOutputChannels;
				
				// Relase the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.release();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Input Accessor Methods
			
			
			
			
			virtual SoundOutput* getInput() const
			{
				return input;
			}
			
			
			
			
			virtual void setInput( SoundOutput* newInput )
			{
				// Acquire the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.acquire();
				
				input = newInput;
				
				// Relase the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.release();
			}
			
			
			
			
			virtual void removeInput()
			{
				// Acquire the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.acquire();
				
				input = NULL;
				
				// Relase the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.release();
			}
			
			
			
			
			virtual Bool hasInput() const
			{
				return input != NULL;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Format Accessor Methods
			
			
			
			
			virtual Float getSampleRate() const
			{
				if ( input == NULL )
					return Float(0);
				else
					return input->getSampleRate();
			}
			
			
			
			
			virtual Size getNumberOfChannels() const
			{
				return numOutputChannels;
			}
			
			
			
			
			virtual Size getNumberOfOutputs() const
			{
				if ( input == NULL )
					return Size(0);
				else
					return input->getNumberOfOutputs();
			}
			
			
			
			
			virtual Bool hasOutputRemaining() const
			{
				if ( input == NULL )
					return false;
				else
					return input->hasOutputRemaining();
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Main Rendering Method
			
			
			
			
			virtual Size fillBuffer( dsp::SoundStream& outputStream, Index startIndex, Size numSamples )
			{
				// Write no samples to the output stream if the input is either NULL or has no audio data remaining.
				if ( input == NULL || !input->hasOutputRemaining() )
					return 0;
				
				// Acquire the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.acquire();
				
				Size numOutputs = input->getNumberOfOutputs();
				
				// Read sound from the input audio unit directly into the output stream.
				Size numRead = input->getSamples( outputStream, startIndex, numSamples );
				
				// Mix the channels together into the output audio stream.
				for ( Index outputIndex = 0; outputIndex < numOutputs; outputIndex++ )
				{
					dsp::SoundBuffer& outputBuffer = outputStream.getBuffer(outputIndex);
					
					const dsp::Sample* const inputStart = outputBuffer.getChannelStart(0) + startIndex;
					const dsp::Sample* const inputEnd = inputStart + numRead;
					
					for ( Index c = 1; c < numOutputChannels; c++ )
					{
						dsp::Sample* output = outputBuffer.getChannelStart(c) + startIndex;
						const dsp::Sample* input = inputStart;
						
						while ( input != inputEnd )
						{
							*output = *input;
							
							input++;
							output++;
						}
					}
				}
				
				// Relase the mutex which indicates that rendering parameters are either being used or changed.
				renderMutex.release();
				
				return numRead;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			SoundOutput* input;
			
			
			
			
			Size numOutputChannels;
			
			
			
			
			mutable Mutex renderMutex;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Reverb Renderer Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagationRenderer:: ReverbRenderState
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			GSOUND_INLINE ReverbRenderState( Size numChannels, Size numFrequencyBands )
			{
				Size numCombFilters = 10;
				AABB1 combFilterDelayRange( Real(0.02), Real(0.05) );
				Size numAllPassFilters = 2;
				AABB1 allPassFilterDelayRange( Real(0.005), Real(0.03) );
				
				for ( Index i = 0; i < numCombFilters; i++ )
					combFilters.add( CombFilter( numChannels, numFrequencyBands, combFilterDelayRange ) );
				
				for ( Index i = 0; i < numAllPassFilters; i++ )
					allPassFilters.add( AllPassFilter( numChannels, allPassFilterDelayRange ) );
			}
			
			
			
			
			GSOUND_INLINE ReverbRenderState( Size numChannels, Size numFrequencyBands,
											Size numCombFilters, Size numAllPassFilters,
											AABB1 combFilterDelayRange, AABB1 allPassFilterDelayRange )
				:	combFilters( numCombFilters ),
					allPassFilters( numAllPassFilters )
			{
				// Make sure that there is always at least one comb and all pass filter.
				numCombFilters = math::max( numCombFilters, Size(1) );
				numAllPassFilters = math::max( numAllPassFilters, Size(1) );
				
				for ( Index i = 0; i < numCombFilters; i++ )
					combFilters.add( CombFilter( numChannels, numFrequencyBands, combFilterDelayRange ) );
				
				for ( Index i = 0; i < numAllPassFilters; i++ )
					allPassFilters.add( AllPassFilter( numChannels, allPassFilterDelayRange ) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Gain Accessor Methods
			
			
			
			
			GSOUND_INLINE Float getOutputGain( Index channelIndex, Index frequencyBandIndex ) const
			{
				return Float(combFilters.getSize())*
						combFilters[0].channels[channelIndex].frequencyBands[frequencyBandIndex].currentAmplitude;
			}
			
			
			
			
			GSOUND_INLINE void setTargetGain( Index channelIndex, Index frequencyBandIndex, Float newTargetGain )
			{
				Float combFilterGain = newTargetGain/Float(combFilters.getSize());
				Size numCombFilters = combFilters.getSize();
				
				for ( Index i = 0; i < numCombFilters; i++ )
					combFilters[i].channels[channelIndex].frequencyBands[frequencyBandIndex].targetAmplitude = combFilterGain;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Reverb Time Accessor Methods
			
			
			
			
			GSOUND_INLINE Real getReverbTime( Index channelIndex, Index frequencyBandIndex ) const
			{
				return combFilters[0].channels[channelIndex].frequencyBands[frequencyBandIndex].decayTime;
			}
			
			
			
			
			GSOUND_INLINE void setReverbTime( Index channelIndex, Index frequencyBandIndex, Real newReverbTime )
			{
				Size numCombFilters = combFilters.getSize();
				
				for ( Index i = 0; i < numCombFilters; i++ )
				{
					combFilters[i].channels[channelIndex].frequencyBands[frequencyBandIndex].decayTime = newReverbTime;
					
					Float feedbackGain;
					
					if ( math::equals( newReverbTime, Real(0) ) )
						feedbackGain = Float(0);
					else
						feedbackGain = math::pow( Float(0.001), combFilters[i].channels[channelIndex].delayTime / newReverbTime );
					
					combFilters[i].channels[channelIndex].frequencyBands[frequencyBandIndex].feedbackGain = feedbackGain;
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Comb Filter Class Declaration
			
			
			
			
			class CombFilter
			{
				public:
					
					GSOUND_INLINE CombFilter( Size numChannels, Size numFrequencyBands, AABB1 delayRange )
					{
						for ( Index i = 0; i < numChannels; i++ )
						{
							channels.add( CombFilterChannel( numFrequencyBands,
															math::random( delayRange.min, delayRange.max ) ) );
						}
					}
					
					
					
					
					class CombFilterChannel
					{
						public:
							
							GSOUND_INLINE CombFilterChannel( Size numFrequencyBands, Float newDelayTime )
								:	currentDelayReadIndex( 0 ),
									delayTime( math::max( newDelayTime, Float(0) ) ),
#if GSOUND_USE_SIMD
									delayBuffer( 1 )
#else
									delayBuffers( 1, 1 )
#endif
							{
								for ( Index i = 0; i < numFrequencyBands; i++ )
									frequencyBands.add( FrequencyBand() );
							}
							
							GSOUND_INLINE Size getNumberOfFrequencyBands() const
							{
								return frequencyBands.getSize();
							}
							
							
							GSOUND_INLINE void setNumberOfFrequencyBands( Size newNumFrequencyBands )
							{
								if ( newNumFrequencyBands < frequencyBands.getSize() )
									frequencyBands.removeLast( frequencyBands.getSize() - newNumFrequencyBands );
								else
								{
									for ( Index i = frequencyBands.getSize(); i < newNumFrequencyBands; i++ )
										frequencyBands.add( FrequencyBand() );
								}
							}
							
							
							class FrequencyBand
							{
								public:
									
									GSOUND_INLINE FrequencyBand()
										:	decayTime( 0 ),
											feedbackGain( 0 ),
											currentAmplitude( 0 ),
											targetAmplitude( 0 )
									{
									}
									
									
									Real decayTime;
									
									Float feedbackGain;
									Float currentAmplitude;
									Float targetAmplitude;
									
							};
							
							
							
							ArrayList<FrequencyBand> frequencyBands;
							
							
							Index currentDelayReadIndex;
							
							
							Float delayTime;
							
							
#if GSOUND_USE_SIMD
							dsp::SoundBuffer delayBuffer;
#else
							dsp::SoundStream delayBuffers;
#endif
							
					};
					
					ArrayList<CombFilterChannel> channels;
					
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	All Pass Filter Class Declaration
			
			
			
			
			class AllPassFilter
			{
				public:
					
					GSOUND_INLINE AllPassFilter( Size numChannels, AABB1 delayRange )
					{
						for ( Index i = 0; i < numChannels; i++ )
							channels.add( AllPassFilterChannel( math::random( delayRange.min, delayRange.max ) ) );
					}
					
					
					
					
					class AllPassFilterChannel
					{
						public:
							
							GSOUND_INLINE AllPassFilterChannel( Float newDelayTime )
								:	currentDelayReadIndex( 0 ),
									delayTime( math::max( newDelayTime, Float(0) ) ),
									decayTime( 0.1f ),
									delayBuffer( 1 )
							{
								feedbackGain = math::pow( Float(0.001), delayTime / decayTime );
							}
							
							
							
							Index currentDelayReadIndex;
							
							
							Float delayTime;
							Real decayTime;
							
							Float feedbackGain;
							
							
							dsp::SoundBuffer delayBuffer;
							
					};
					
					ArrayList<AllPassFilterChannel> channels;
					
			};
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			ArrayList<CombFilter> combFilters;
			
			
			
			
			ArrayList<AllPassFilter> allPassFilters;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Inpulse Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagationRenderer:: Impulse
{
	public:
		
		GSOUND_INLINE Impulse( Real newAmplitude, Real newDelay, Real newDelayChangePerSecond,
								const PropagationPath* newPath )
			:	amplitude( newAmplitude ),
				delay( newDelay ),
				delayChangePerSecond( newDelayChangePerSecond ),
				path( newPath )
		{
		}
		
		
		
		
		Real amplitude;
		
		
		
		
		Real delay;
		
		
		
		
		Real delayChangePerSecond;
		
		
		
		
		const PropagationPath* path;
		
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Interpolation State Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagationRenderer:: InterpolationState
{
	public:
		
		GSOUND_INLINE InterpolationState()
			:	currentAmplitude( 0 ),
				targetAmplitude( 0 )
		{
		}
		
		
		GSOUND_INLINE InterpolationState( Float amplitude )
			:	currentAmplitude( 0 ),
				targetAmplitude( amplitude )
		{
		}
		
		Float currentAmplitude;
		Float targetAmplitude;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Propagation Path Render State
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagationRenderer:: PropagationPathRenderState
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			GSOUND_INLINE PropagationPathRenderState( Size newNumFrequencyBands, Size newNumChannels, Index newTimeStamp )
				:	numFrequencyBands( math::max( newNumFrequencyBands, Size(1) ) ),
					numChannels( math::max( newNumChannels, Size(1) ) ),
					timeStamp( newTimeStamp ),
					currentDelayTime( 0 ),
					targetDelayTime( 0 ),
					delayChangePerSecond( 0 )
			{
				interpolationStates = util::constructArray<InterpolationState>( numFrequencyBands*numChannels );
			}
			
			
			
			
			GSOUND_INLINE PropagationPathRenderState( const PropagationPathRenderState& other )
				:	numFrequencyBands( other.numFrequencyBands ),
					numChannels( other.numChannels ),
					timeStamp( other.timeStamp )
			{
				interpolationStates = util::copyArray( other.interpolationStates, numFrequencyBands*numChannels );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			GSOUND_INLINE ~PropagationPathRenderState()
			{
				util::destructArray( interpolationStates, numFrequencyBands*numChannels );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			GSOUND_INLINE PropagationPathRenderState& operator = ( const PropagationPathRenderState& other )
			{
				if ( this != &other )
				{
					util::destructArray( interpolationStates, numFrequencyBands*numChannels );
					
					numFrequencyBands = other.numFrequencyBands;
					numChannels = other.numChannels;
					timeStamp = other.timeStamp;
					currentDelayTime = other.currentDelayTime;
					targetDelayTime = other.targetDelayTime;
					delayChangePerSecond = other.delayChangePerSecond;
					
					interpolationStates = util::copyArray( other.interpolationStates, numFrequencyBands*numChannels );
				}
				
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Interpolation State Accessor Methods
			
			
			
			
			GSOUND_INLINE InterpolationState& getInterpolationState( Index bandIndex, Index channelIndex )
			{
				GSOUND_DEBUG_ASSERT( bandIndex < numFrequencyBands );
				GSOUND_DEBUG_ASSERT( channelIndex < numChannels );
				
				return interpolationStates[bandIndex*numChannels + channelIndex];
			}
			
			
			GSOUND_INLINE void setInterpolationState( Index bandIndex, Index channelIndex,
														const InterpolationState& newInterpolationState )
			{
				GSOUND_DEBUG_ASSERT( bandIndex < numFrequencyBands );
				GSOUND_DEBUG_ASSERT( channelIndex < numChannels );
				
				interpolationStates[bandIndex*numChannels + channelIndex] = newInterpolationState;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	State Size Accessor Methods
			
			
			
			
			GSOUND_INLINE Size getNumberOfFrequencyBands() const
			{
				return numFrequencyBands;
			}
			
			
			
			
			GSOUND_INLINE Size getNumberOfChannels() const
			{
				return numChannels;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			Index timeStamp;
			
			
			
			Real currentDelayTime;
			
			
			
			
			Real targetDelayTime;
			
			
			
			Real delayChangePerSecond;
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The interpolation states for this propagation path render state.
			InterpolationState* interpolationStates;
			
			
			
			Size numFrequencyBands;
			
			
			
			Size numChannels;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Source Render State Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagationRenderer:: SoundSourceRenderState
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			GSOUND_INLINE SoundSourceRenderState( SoundOutput* newInput, Size numOutputChannels,
												const FrequencyPartition& newFrequencyPartition,
												Size newTimeStamp, Float newSampleRate )
				:	input( newInput ),
					sampleRateConverter( util::construct<dsp::SampleRateConverter>( newInput, newSampleRate ) ),
					monoMixer( util::construct<MonoMixer>() ),
					crossover( util::construct<dsp::Crossover>() ),
					monoSplitter( util::construct<MonoSplitter>( numOutputChannels ) ),
					timeStamp( newTimeStamp ),
					currentDelayWriteIndex( 0 ),
					reverbRenderState( numOutputChannels, newFrequencyPartition.getNumberOfFrequencyBands() )
			{
				monoMixer->setInput( sampleRateConverter );
				crossover->setInput( monoMixer );
				monoSplitter->setInput( crossover );
				setFrequencyPartition( newFrequencyPartition );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			GSOUND_INLINE ~SoundSourceRenderState()
			{
				util::destruct( sampleRateConverter );
				util::destruct( monoMixer );
				util::destruct( crossover );
				util::destruct( monoSplitter );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Input Accessor Methods
			
			
			
			
			GSOUND_INLINE SoundOutput* getInput() const
			{
				return input;
			}
			
			
			
			
			GSOUND_INLINE void setInput( SoundOutput* newInput )
			{
				input = newInput;
				sampleRateConverter->setInput( newInput );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Accessor Methods
			
			
			
			
			GSOUND_INLINE SoundOutput* getOutput() const
			{
				return monoSplitter;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Frequency Partition Accessor Method
			
			
			
			GSOUND_INLINE void setFrequencyPartition( const FrequencyPartition& newFrequencyPartition )
			{
				crossover->setCrossoverFrequencies( newFrequencyPartition.getSplitFrequencies() );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Channel Count Accessor Method
			
			
			
			
			GSOUND_INLINE Size getNumberOfOutputChannels() const
			{
				return monoSplitter->getNumberOfOutputChannels();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Rate Accessor Method
			
			
			
			
			GSOUND_INLINE void setSampleRate( Float newSampleRate )
			{
				sampleRateConverter->setSampleRate( newSampleRate );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// An object which contains the rendering state for the reverb for this sound source.
			ReverbRenderState reverbRenderState;
			
			
			
			
			HashMap<PropagationPathID,PropagationPathRenderState> propagationPaths;
			
			
			
			
#if GSOUND_USE_SIMD
			/// A delay buffer with interleaved frequency band samples, aligned to a multiple of 4 samples.
			dsp::SoundBuffer delayBuffer;
#else
			/// A stream of buffers, one for each frequency band, that hold a set of delayed samples.
			dsp::SoundStream delayBuffers;
#endif
			
			
			
			/// An integer representing the frame index when the sound source's rendering information was last updated.
			Index timeStamp;
			
			
			
			
			/// The current position being written to in the delay buffer.
			Index currentDelayWriteIndex;
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A pointer to the output unit which provides input audio for this sound source renderer.
			SoundOutput* input;
			
			
			
			/// An object which converts the input audio to a particular output sample rate.
			dsp::SampleRateConverter* sampleRateConverter;
			
			
			
			
			/// A class which mixes every input channel for a sound source into a single output channel.
			MonoMixer* monoMixer;
			
			
			
			
			/// An object used to split incoming source audio into multiple frequency bands for rendering.
			dsp::Crossover* crossover;
			
			
			
			
			/// A class which splits a single input channel for a sound source into multiple output output channels.
			MonoSplitter* monoSplitter;
			
			
			
};





//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundPropagationRenderer:: SoundPropagationRenderer( const dsp::SpeakerConfiguration& newSpeakerConfiguration )
	:	speakerConfiguration( newSpeakerConfiguration ),
		timeStamp( 0 ),
		reverbIsEnabled( true ),
		maxNumberOfPropagationPaths( math::max<Size>() ),
		maxPathAge( 10 ),
		sampleRate( Float(44100) ),
		maxDelayTime( Real(0.5) )
{
#if GSOUND_USE_SIMD
	numSIMDIterations = 1;
	sampleFrameWidth = numSIMDIterations*SIMDSample::getWidth();
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundPropagationRenderer:: ~SoundPropagationRenderer()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Propagation Path Update Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagationRenderer:: updatePropagationPaths( const SoundPropagationPathBuffer& newPathBuffer )
{
	const Size numSources = newPathBuffer.getNumberOfSources();
	const Size totalNumPaths = newPathBuffer.getTotalNumberOfPropagationPaths();
	
	// Whether or not culling of quieter paths should be performed.
	const Bool shouldCullPaths = totalNumPaths > maxNumberOfPropagationPaths;
	
	// The percentage of the incoming propagation paths that should be discarded if there are too many.
	const Float percentValidPaths = Float(maxNumberOfPropagationPaths) / Float(totalNumPaths);
	
	// Acquire a mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	for ( Index s = 0; s < numSources; s++ )
	{
		const SoundSourcePropagationPathBuffer& sourcePathBuffer = newPathBuffer.getSourceBuffer(s);
		SoundSource* source = sourcePathBuffer.getSource();
		
		// Ignore this buffer if its source is NULL.
		if ( source == NULL )
			continue;
		
		// Calculate the number of paths that should be kept for the sound source if 
		// it is necessary to cull propagation paths.
		Size maxNumberOfSourcePaths;
		
		if ( shouldCullPaths )
			maxNumberOfSourcePaths = Size(percentValidPaths * sourcePathBuffer.getNumberOfPropagationPaths());
		else
			maxNumberOfSourcePaths = sourcePathBuffer.getNumberOfPropagationPaths();
		
		SoundSourceRenderState** renderState;
		
		// Is there already a rendering state for this source?
		if ( sourceRenderStates.find( source->getHashCode(), source, renderState ) )
		{
			// If the source is disabled, clear its render state and continue to the next source.
			if ( !source->getIsEnabled() )
			{
				(*renderState)->propagationPaths.clear();
				continue;
			}
			
			// If we have already encountered a path buffer for this sound source,
			// skip this path buffer. This should not happen in practice.
			if ( (*renderState)->timeStamp == timeStamp )
				continue;
			
			// Update the time stamp of the source's render state.
			(*renderState)->timeStamp = timeStamp;
			
			updateSourcePropagationPaths( sourcePathBuffer, **renderState, maxNumberOfSourcePaths );
		}
		else
		{
			// We didn't find a source render state for this sound source.
			// Create a new one.
			SoundSourceRenderState* newRenderState = util::construct<SoundSourceRenderState>( source->getSoundInput(), 
																			speakerConfiguration.getNumberOfChannels(),
																			frequencyPartition,
																			timeStamp,
																			sampleRate );
			
			// Add the new source render state to the set of source render states.
			sourceRenderStates.add( source->getHashCode(), source, newRenderState );
			
			// Update the paths for that render state.
			updateSourcePropagationPaths( sourcePathBuffer, *newRenderState, maxNumberOfSourcePaths );
		}
	}
	
	
	//****************************************************************************
	// Iterate over the sound source render states and remove ones that no
	// longer correspond to a sound source.
	
	HashMap<SoundSource*,SoundSourceRenderState*>::Iterator i = sourceRenderStates.getIterator();
	
	while ( i )
	{
		if ( (*i)->timeStamp < timeStamp )
		{
			util::destruct( *i );
			i.remove();
			continue;
		}
		
		i++;
	}
	
	// Increment the current time stamp.
	timeStamp++;
	
	
	// Release a mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
}





//##########################################################################################
//##########################################################################################
//############		
//############		Sound Source Propagation Path Update Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagationRenderer:: updateSourcePropagationPaths( const SoundSourcePropagationPathBuffer& pathBuffer,
																SoundSourceRenderState& renderState,
																Size maxNumberOfSourcePaths )
{
	SoundSource* source = pathBuffer.getSource();
	
	const Size numChannels = speakerConfiguration.getNumberOfChannels();
	const Size numFrequencyBands = frequencyPartition.getNumberOfFrequencyBands();
	
	if ( source == NULL )
	{
		// If the source that the renderer is supposed to be rendering is NULL,
		// clear all rendering state.
		renderState.setInput( NULL );
		renderState.propagationPaths.clear();
	}
	
	// Make sure that the renderer is using the correct audio input.
	if ( source->getSoundInput() != renderState.getInput() )
		renderState.setInput( source->getSoundInput() );
	
	
	//****************************************************************************
	
	// Clear the impulse sort list.
	impulseSortList.clear();
	
	Size numPropagationPaths = pathBuffer.getNumberOfPropagationPaths();
	
	// For each propagation path, compute the intensity of the source's sound and
	// add an impulse to a list which is used to sort paths by intensity.
	for ( Index i = 0; i < numPropagationPaths; i++ )
	{
		const PropagationPath& path = pathBuffer.getPropagationPath(i);
		
		const Real distance = path.getDistance();
		const Real distanceAttenuation = source->getDistanceAttenuation( distance );
		
		const Real inverseSpeedOfSound = Real(1) / path.getSpeedOfSound();
		
		const Real delayChangePerSecond = path.getRelativeSpeed()*inverseSpeedOfSound;
		const Real delay = math::min( distance*inverseSpeedOfSound, maxDelayTime );
		
		const Float amplitude = source->getIntensity()*distanceAttenuation;
		
		impulseSortList.add( Impulse( amplitude, delay, delayChangePerSecond, &path ) );
	}
	
	
	//****************************************************************************
	
	Size numValidImpulses;
	
	// If necessary, sort the lists of impulses by decreasing intensity so that
	// we can cull out the quietest impulses.
	if ( impulseSortList.getSize() > maxNumberOfSourcePaths )
	{
		sortImpulsesByDecreasingIntensity( impulseSortList );
		numValidImpulses = maxNumberOfSourcePaths;
	}
	else
		numValidImpulses = impulseSortList.getSize();
	
	
	//****************************************************************************
	
	
	for ( Index i = 0; i < numValidImpulses; i++ )
	{
		const Impulse& impulse = impulseSortList[i];
		const PropagationPath& path = *impulse.path;
		const PropagationPathID& pathID = path.getID();
		
		// Compute the gain for each channel based on the direction of the path.
		speakerConfiguration.spatializeDirection( path.getDirection(), channelGainArray );
		
		PropagationPathRenderState* pathRenderState;
		
		// Is there already a propagation path render state for this propagation path?
		// If so, update the render state for that path.
		
		if ( renderState.propagationPaths.find( pathID.getHashCode(), pathID, pathRenderState ) )
		{
			// If the number of frequency bands or number of channels in the path render state
			// is not the same as the current number of bands or channels, reset the path's render state.
			if ( pathRenderState->getNumberOfChannels() != numChannels ||
				pathRenderState->getNumberOfFrequencyBands() != numFrequencyBands )
			{
				*pathRenderState = PropagationPathRenderState( numFrequencyBands, numChannels, renderState.timeStamp );
			}
			
			// Update the target delay time and doppler delay change. These are constant for all channels/bands.
			pathRenderState->targetDelayTime = impulse.delay;
			pathRenderState->delayChangePerSecond = impulse.delayChangePerSecond;
			
			for ( Index bandIndex = 0; bandIndex < numFrequencyBands; bandIndex++ )
			{
				Real bandGain = path.getFrequencyAttenuation().getBandAverageGain( 
												frequencyPartition.getFrequencyBandRange( bandIndex ) )*impulse.amplitude;
				
				for ( Index c = 0; c < numChannels; c++ )
				{
					InterpolationState& interpolationState = pathRenderState->getInterpolationState( bandIndex, c );
					
					// Update the target ampltiude, delay time, and delay change per second of this interpolation state.
					interpolationState.targetAmplitude = bandGain*channelGainArray.getGain(c);
				}
			}
			
			// Update the time stamp for this path render state.
			pathRenderState->timeStamp = renderState.timeStamp;
		}
		else
		{
			// This is a new propagation path. Add a propagation path render state to the
			// hash map of render states.
			renderState.propagationPaths.add( pathID.getHashCode(), pathID,
											PropagationPathRenderState( numFrequencyBands, numChannels, renderState.timeStamp ) );
			
			// Get a pointer to the new propagation path render state. TODO: this is inefficient.
			renderState.propagationPaths.find( pathID.getHashCode(), pathID, pathRenderState );
			
			// Set the delay time and doppler delay change. These are constant for all channels/bands.
			pathRenderState->currentDelayTime = impulse.delay;
			pathRenderState->targetDelayTime = impulse.delay;
			pathRenderState->delayChangePerSecond = impulse.delayChangePerSecond;
			
			for ( Index bandIndex = 0; bandIndex < numFrequencyBands; bandIndex++ )
			{
				Real bandGain = path.getFrequencyAttenuation().getBandAverageGain( 
												frequencyPartition.getFrequencyBandRange( bandIndex ) )*impulse.amplitude;
				
				for ( Index c = 0; c < numChannels; c++ )
				{
					InterpolationState& interpolationState = pathRenderState->getInterpolationState( bandIndex, c );
					
					// Update the target ampltiude, delay time, and delay change per second of this interpolation state.
					interpolationState = InterpolationState( bandGain*channelGainArray.getGain(c) );
				}
			}
		}
	}
	
	
	//****************************************************************************
	// Iterate over the data structure of propagation path render states and prepare aging
	// states for removal and remove states that are older than the maximum allowed value.
	
	{
		HashMap<PropagationPathID,PropagationPathRenderState>::Iterator i = renderState.propagationPaths.getIterator();
		
		while ( i )
		{
			if ( i->timeStamp < renderState.timeStamp )
			{
				// If the number of frequency bands or number of channels is not equal to the current
				// number of bands or channels, remove this propagation path render state immediately
				// because there is no way we can gracefully perform this operation.
				if ( i->getNumberOfFrequencyBands() != numFrequencyBands || i->getNumberOfChannels() != numChannels )
				{
					i.remove();
					continue;
				}
				
				// Compute the age of the path.
				Size pathAge = renderState.timeStamp - i->timeStamp;
				
				// Remove paths that are older than the maximum allowed value.
				if ( pathAge > maxPathAge )
				{
					i.remove();
					continue;
				}
				
				// If the path is aging but not ready to be removed yet, fade it out.
				Float lastGain = math::square(1.0f - (Float)(pathAge - 1) / (Float)maxPathAge);
				Float gain = math::square(1.0f - (Float)pathAge / (Float)maxPathAge);
				
				for ( Index bandIndex = 0; bandIndex < numFrequencyBands; bandIndex++ )
				{
					for ( Index c = 0; c < numChannels; c++ )
					{
						InterpolationState& interpolationState = i->getInterpolationState( bandIndex, c );
						
						Float originalAmplitude = interpolationState.currentAmplitude/lastGain;
						
						// Update the target amplitude so that it will fade out exponentially.
						interpolationState.targetAmplitude = originalAmplitude*gain;
					}
				}
			}
			
			i++;
		}
	}
	
	//****************************************************************************
	
	ReverbRenderState& reverbRenderState = renderState.reverbRenderState;
	
	
	if ( reverbIsEnabled )
	{
		const SoundSourceReverbResponse& reverbResponse = pathBuffer.getReverbResponse();
		
		// Update the reverb times and amplitudes for all frequency bands.
		for ( Index bandIndex = 0; bandIndex < numFrequencyBands; bandIndex++ )
		{
			// Calculate the reverb time for this frequency band.
			const AABB1 bandRange = frequencyPartition.getFrequencyBandRange( bandIndex );
			Real bandAttenuation = reverbResponse.averageSurfaceAttenuation.getBandAverageGain( bandRange );
			
			const Real c = (-Real(4)*math::ln(Real(1.0e-6))/Real(343));
			
			Real reverbTime;
			
			if ( math::equals( reverbResponse.surfaceArea, Real(0) ) )
				reverbTime = Real(0);
			else
				reverbTime = c*reverbResponse.volume/(-reverbResponse.surfaceArea*math::ln(bandAttenuation));
			
			Float bandAmplitude = reverbResponse.distanceAttenuation.getBandAverageGain( bandRange )*source->getIntensity();
			
			for ( Index i = 0; i < numChannels; i++ )
			{
				reverbRenderState.setReverbTime( i, bandIndex, reverbTime );
				reverbRenderState.setTargetGain( i, bandIndex, bandAmplitude ); // *reverbGain
			}
		}
	}
	else
	{
		// Make sure that the reverb output gain and reverb times are zero.
		for ( Index bandIndex = 0; bandIndex < numFrequencyBands; bandIndex++ )
		{
			for ( Index i = 0; i < numChannels; i++ )
			{
				reverbRenderState.setReverbTime( bandIndex, i, 0 );
				reverbRenderState.setTargetGain( bandIndex, i, 0 );
			}
		}
	}
}





//##########################################################################################
//##########################################################################################
//############		
//############		Frequency Partition Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagationRenderer:: setFrequencyPartition( const FrequencyPartition& newFrequencyPartition )
{
	// Acquire a mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	frequencyPartition = newFrequencyPartition;
	
#if GSOUND_USE_SIMD
	// Compute the number of SIMD words that are necessary to encompass all frequency bands in an interleaved sample stream.
	sampleFrameWidth = math::nextMultiple( frequencyPartition.getNumberOfFrequencyBands(), SIMDSample::getWidth() );
	numSIMDIterations = sampleFrameWidth / SIMDSample::getWidth();
	
#endif
	
	for ( HashMap<SoundSource*,SoundSourceRenderState*>::Iterator i = sourceRenderStates.getIterator(); i; i++ )
	{
		(*i)->setFrequencyPartition( frequencyPartition );
	}
	
	// Release a mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Spatializer Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagationRenderer:: setSpeakerConfiguration( const dsp::SpeakerConfiguration& newSpeakerConfiguration )
{
	// Acquire a mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	if ( speakerConfiguration.getNumberOfChannels() != newSpeakerConfiguration.getNumberOfChannels() )
	{
		// Clear all sound source render states from the renderer. This is costly and will interupt
		// playback of propagated sound, but is necessary due to the difficulty in updating the new number
		// of channels any other way. This operation shouldn't need to be performed in regular use of the 
		// library anyway.
		sourceRenderStates.clear();
	}
	
	// Set the new speaker configuration to use.
	speakerConfiguration = newSpeakerConfiguration;
	
	// Release a mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Sample Rate Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Float SoundPropagationRenderer:: getSampleRate() const
{
	return sampleRate;
}




void SoundPropagationRenderer:: setSampleRate( Float newSampleRate )
{
	// Acquire a mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	sampleRate = math::max( newSampleRate, Float(0) );
	
	for ( HashMap<SoundSource*,SoundSourceRenderState*>::Iterator i = sourceRenderStates.getIterator(); i; i++ )
	{
		(*i)->setSampleRate( sampleRate );
	}
	
	// Release a mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundPropagationRenderer:: getNumberOfChannels() const
{
	return speakerConfiguration.getNumberOfChannels();
}




Size SoundPropagationRenderer:: getNumberOfOutputs() const
{
	return Size(1);
}




Bool SoundPropagationRenderer:: hasOutputRemaining() const
{
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Rendering Method
//############		
//##########################################################################################
//##########################################################################################




Size SoundPropagationRenderer:: fillBuffer( dsp::SoundStream& outputStream, Index startIndex, Size numSamples )
{
	// Acquire a mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.acquire();
	
	// Zero the output stream.
	outputStream.zero( startIndex, numSamples );
	
	// Update the delay buffer size to use.
	delayBufferSize = Size(Real(2)*sampleRate*maxDelayTime);
	
	// For each sound source, render the audio to the output stream.
	for ( HashMap<SoundSource*,SoundSourceRenderState*>::Iterator i = sourceRenderStates.getIterator(); i; i++ )
	{
		renderSoundSource( outputStream.getBuffer(0), startIndex, numSamples, **i );
	}
	
	// Release a mutex which indicates that rendering parameters are either being used or changed.
	renderMutex.release();
	
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Source Rendering Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagationRenderer:: renderSoundSource( dsp::SoundBuffer& outputBuffer, Index startIndex, Size numSamples, 
													SoundSourceRenderState& renderState )
{
	//****************************************************************************
	// Compute some constants that are used in both the SIMD and non-SIMD versions of this method.
	
	// Calculate the length of the output buffer in seconds.
	const Float outputBufferLength = Float(numSamples)/sampleRate;
	
	// Calculate the length in seconds of half a sample.
	const Float halfSampleLength = Float(0.5)/sampleRate;
	const Float inverseNumSamples = 1.0f / (Float)(numSamples);
	
	const Size numChannels = renderState.getNumberOfOutputChannels();
	const Size numFrequencyBands = frequencyPartition.getNumberOfFrequencyBands();
	
#if GSOUND_USE_SIMD
	
	//****************************************************************************
	//****************************************************************************
	//****************************************************************************
	//****************************************************************************
	// Get the output audio of the crossover and write it to the delay buffer
	// over several steps if necessary.
	
	const Size actualDelayBufferSize = delayBufferSize*sampleFrameWidth;
	
	dsp::SoundBuffer& delayBuffer = renderState.delayBuffer;
	
	// Make sure that the delay buffer has the right number of channels.
	// This is necessary because we are not passing this buffer to a standard audio
	// component which would normally handle this task.
	if ( delayBuffer.getNumberOfChannels() < numChannels )
		delayBuffer.setNumberOfChannels( numChannels );
	
	// Make sure that the delay buffer is the right size.
	if ( actualDelayBufferSize > delayBuffer.getSize() )
	{
		delayBuffer.setSize( actualDelayBufferSize );
		delayBuffer.zero();
	}
	
	Size samplesRemaining = numSamples;
	Size totalSamplesRead = 0;
	
	// Save the index of the sample with 0 delay so that we can calculate delay offsets later.
	Index currentDelayReadIndex = renderState.currentDelayWriteIndex;
	
	while ( samplesRemaining > 0 )
	{
		// Only read until the end of the delay buffer is reached.
		Size samplesToRead = math::min( samplesRemaining, delayBufferSize - renderState.currentDelayWriteIndex );
		
		// Write the output of the crossover to an intermediate buffer before it is interleaved.
		Size samplesRead = renderState.getOutput()->getSamples( inputStream, 0, samplesToRead );
		
		// If there was no more input audio, break to avoid an infinite loop.
		if ( samplesRead == Size(0) && !renderState.getOutput()->hasOutputRemaining() )
		{
			// Write zeros to the rest of the delay buffer to avoid anything strange happening.
			renderState.delayBuffer.zero( renderState.currentDelayWriteIndex*sampleFrameWidth, samplesToRead*sampleFrameWidth );
			
			// Pretend that we read the desired number of samples.
			samplesRead = samplesToRead;
		}
		else
		{
			// Write the new samples to the interleaved delay buffer for the sound source.
			for ( Index i = 0; i < numChannels; i++ )
			{
				Index bandIndex = 0;
				
				// Copy the samples to the interleaved delay buffer.
				for ( ; bandIndex < numFrequencyBands; bandIndex++ )
				{
					const dsp::Sample* source = inputStream.getBuffer(bandIndex).getChannelStart(i);
					const dsp::Sample* const sourceEnd = source + samplesRead;
					dsp::Sample* destination = renderState.delayBuffer.getChannelStart(i) + bandIndex + 
												renderState.currentDelayWriteIndex*sampleFrameWidth;
					
					while ( source != sourceEnd )
					{
						*destination = *source;
						source++;
						destination += sampleFrameWidth;
					}
				}
				
				// Write zeros to the unused frequency bands.
				for ( ; bandIndex < sampleFrameWidth; bandIndex++ )
				{
					dsp::Sample* destination = renderState.delayBuffer.getChannelStart(i) + bandIndex + 
												renderState.currentDelayWriteIndex*sampleFrameWidth;
					const dsp::Sample* const destinationEnd = destination + sampleFrameWidth*samplesRead;
					
					while ( destination != destinationEnd )
					{
						*destination = dsp::Sample(0);
						destination += sampleFrameWidth;
					}
				}
				
			}
		}
		
		// Update the current delay write index.
		renderState.currentDelayWriteIndex += samplesRead;
		
		if ( renderState.currentDelayWriteIndex == delayBufferSize )
			renderState.currentDelayWriteIndex = 0;
		
		// Update the number of samples read so far and the number of samples that still need to be read.
		totalSamplesRead += samplesRead;
		samplesRemaining -= samplesRead;
	}
	
#else // GSOUND_USE_SIMD
	
	//****************************************************************************
	// Get the output audio of the crossover and write it directly
	// to the delay buffer over several steps if necessary.
	
	// Make sure that the delay buffer has the right number of channels.
	// This is necessary because we are not passing this buffer to a standard audio
	// component which would normally handle this task.
	if ( renderState.delayBuffers.getNumberOfBuffers() < numFrequencyBands )
		renderState.delayBuffers.setNumberOfBuffers( numFrequencyBands );
	
	if ( renderState.delayBuffers.getNumberOfChannels() < numChannels )
		renderState.delayBuffers.setNumberOfChannels( numChannels );
	
	// Make sure that the delay buffer is the right size.
	if ( delayBufferSize > renderState.delayBuffers.getSize() )
	{
		renderState.delayBuffers.setSize( delayBufferSize );
		renderState.delayBuffers.zero();
	}
	
	Size samplesRemaining = numSamples;
	Size totalSamplesRead = 0;
	
	// Save the index of the sample with 0 delay so that we can calculate delay offsets later.
	Index currentDelayReadIndex = renderState.currentDelayWriteIndex;
	
	while ( samplesRemaining > 0 )
	{
		// Only read until the end of the delay buffer is reached.
		Size samplesToRead = math::min( samplesRemaining, delayBufferSize - renderState.currentDelayWriteIndex );
		
		// Write the output of the crossover directly to the delay buffer.
		Size samplesRead = renderState.getOutput()->getSamples( renderState.delayBuffers,
															renderState.currentDelayWriteIndex,
															samplesToRead );
		
		// If there was no more input audio, break to avoid an infinite loop.
		if ( samplesRead == Size(0) && !renderState.getOutput()->hasOutputRemaining() )
		{
			// Write zeros to the rest of the delay buffer to avoid anything strange happening.
			renderState.delayBuffers.zero( renderState.currentDelayWriteIndex, samplesRemaining );
			
			// Pretend that we read the desired number of samples.
			samplesRead = samplesToRead;
		}
		
		// Update the current delay write index.
		renderState.currentDelayWriteIndex += samplesRead;
		
		if ( renderState.currentDelayWriteIndex == delayBufferSize )
			renderState.currentDelayWriteIndex = 0;
		
		// Update the number of samples read so far and the number of samples that still need to be read.
		totalSamplesRead += samplesRead;
		samplesRemaining -= samplesRead;
	}
	
#endif // !GSOUND_USE_SIMD
	
	
	//****************************************************************************
	//****************************************************************************
	//****************************************************************************
	//****************************************************************************
	// Render every propagation path for the sound source.
	
	HashMap<PropagationPathID,PropagationPathRenderState>::Iterator pathIterator = renderState.propagationPaths.getIterator();
	
	while ( pathIterator )
	{
		PropagationPathRenderState& pathRenderState = *pathIterator;
		
		//****************************************************************************
		// Determine how the delay of the propagation path's delay time should change.
		
		// If the current and target delay times are closer than half a sample,
		// set them to be the same.
		if ( math::fuzzyEquals( pathRenderState.currentDelayTime, pathRenderState.targetDelayTime, halfSampleLength ) )
			pathRenderState.currentDelayTime = pathRenderState.targetDelayTime;
		
		Real newDelayTime;
		
		if ( pathRenderState.currentDelayTime == pathRenderState.targetDelayTime )
			newDelayTime = pathRenderState.targetDelayTime;
		else
		{
			// Calculate the amount of change in the delay time using two different metrics.
			
			// The first uses a delay change based on the doppler shifting calculated by the
			// relative speeds of the source and listener along the propagation path.
			const Real dopplerDelayChange = outputBufferLength*pathRenderState.delayChangePerSecond;
			
			// The second uses a delay change calculated by taking the midpoint of the current
			// and target delay times and calculating the change that it would take to make the
			// delay time that value in 1 second.
			const Real midpointDelayChange = outputBufferLength*((pathRenderState.currentDelayTime +
																	pathRenderState.targetDelayTime)*Float(0.5) -
															pathRenderState.currentDelayTime);
			
			// If the doppler delay change amount is zero, then use the midpoint delay change.
			// Otherwise, use the doppler delay change amount.
			if ( math::fuzzyEquals( dopplerDelayChange, Real(0), math::epsilon<Real>() ) )
				newDelayTime = pathRenderState.currentDelayTime + midpointDelayChange;
			else
				newDelayTime = pathRenderState.currentDelayTime + dopplerDelayChange;
			
		}
		
		const Float delayChangePerSample = (1.0f - (newDelayTime - pathRenderState.currentDelayTime)*
																	inverseNumSamples*sampleRate);
		
		Float delayStart = (Float)currentDelayReadIndex - sampleRate*pathRenderState.currentDelayTime;
		
		if ( delayStart < 0.0f )
			delayStart += (Float)delayBufferSize;
		
		Float fractionalSampleDelay = delayStart - math::floor(delayStart);
		Index delayStartIndex = (Index)delayStart;
		
		//****************************************************************************
		// Render the path for each frequency band and channel.
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			dsp::Sample* const output = outputBuffer.getChannelStart(c) + startIndex;
			const dsp::Sample* const outputEnd = output + numSamples;
			
#if GSOUND_USE_SIMD
			
			const dsp::Sample* delayBufferStart = delayBuffer.getChannelStart(c);
			const dsp::Sample* const delayBufferEnd = delayBufferStart + actualDelayBufferSize;
			const dsp::Sample* delay = delayBufferStart + delayStartIndex*sampleFrameWidth;
			
			// Perform multiple SIMD iterations if there are more than the SIMD width frequency bands.
			for ( Index iteration = 0; iteration < numSIMDIterations; iteration++ )
			{
				SIMDAmplitude currentAmplitude;
				SIMDAmplitude amplitudeChangePerSample;
				
				Size bandIndexStart = iteration*SIMDSample::getWidth();
				
				for ( Index bandIndex = bandIndexStart, i = 0; bandIndex < numFrequencyBands; bandIndex++, i++ )
				{
					InterpolationState& state = pathRenderState.getInterpolationState( bandIndex, c );
					
					currentAmplitude[i] = state.currentAmplitude;
					amplitudeChangePerSample[i] = (state.targetAmplitude - state.currentAmplitude)*inverseNumSamples;
					state.currentAmplitude = state.targetAmplitude;
				}
				
				
#else // GSOUND_USE_SIMD
			
			for ( Index bandIndex = 0; bandIndex < numFrequencyBands; bandIndex++ )
			{
				const dsp::Sample* delayBufferStart = renderState.delayBuffers.getBuffer(bandIndex).getChannelStart(c);
				const dsp::Sample* const delayBufferEnd = delayBufferStart + delayBufferSize;
				const dsp::Sample* delay = delayBufferStart + delayStartIndex;
				
				InterpolationState& state = pathRenderState.getInterpolationState( bandIndex, c );
				
				Float currentAmplitude = state.currentAmplitude;
				Float amplitudeChangePerSample = (state.targetAmplitude - state.currentAmplitude)*inverseNumSamples;
				state.currentAmplitude = state.targetAmplitude;
				
#endif // GSOUND_USE_SIMD
				
				//****************************************************************************
				// Render the audio for this path based on whether or not the delay time changed.
				
				if ( pathRenderState.currentDelayTime == newDelayTime )
				{
					// The delay times are equal, we can use a more simple rendering method
					// to render the audio.
					fillBufferDelaysEqual( output, outputEnd,
											delayBufferStart, delayBufferEnd, delay,
											currentAmplitude, amplitudeChangePerSample );
				}
				else
				{
					// The delay time changes, call a rendering method which is optimized for
					// delay interpolation.
					fillBufferDelayChanges( output, outputEnd,
											delayBufferStart, delayBufferEnd, delay,
											fractionalSampleDelay, delayChangePerSample,
											currentAmplitude, amplitudeChangePerSample );
				}
				
#if GSOUND_USE_SIMD
				// Increment the current delay start pointer for the iteration number.
				delayBufferStart += SIMDSample::getWidth();
				delay += SIMDSample::getWidth();
#endif
			}
		}
		
		// Update the current delay time for the propagation path.
		pathRenderState.currentDelayTime = newDelayTime;
		
		pathIterator++;
	}
	
	
	//****************************************************************************
	//****************************************************************************
	//****************************************************************************
	//****************************************************************************
	// Render the reverb for the sound source.
	
	if ( reverbIsEnabled )
	{
		ReverbRenderState& reverbRenderState = renderState.reverbRenderState;
		
#if GSOUND_USE_SIMD
		// Make sure that an uninitialized input stream has at least 1 buffer.
		if ( inputStream.getNumberOfBuffers() < Size(1) )
			inputStream.setNumberOfBuffers(1);
		
		dsp::SoundBuffer& reverbBuffer = inputStream.getBuffer(0);
#endif
		if ( reverbBuffer.getSize() < numSamples )
			reverbBuffer.setSize( numSamples );
		
		if ( reverbBuffer.getNumberOfChannels() < numChannels )
			reverbBuffer.setNumberOfChannels( numChannels );
		
		
		// Zero the reverb output buffer
		reverbBuffer.zero( 0, numSamples );
		
		
		//****************************************************************************
		// Render every comb filter in parallel and add the results to the reverb buffer.
		
		Size numCombFilters = reverbRenderState.combFilters.getSize();
		
		for ( Index combFilterIndex = 0; combFilterIndex < numCombFilters; combFilterIndex++ )
		{
			ReverbRenderState::CombFilter& combFilter = reverbRenderState.combFilters[combFilterIndex];
			
			for ( Index c = 0; c < numChannels; c++ )
			{
				ReverbRenderState::CombFilter::CombFilterChannel& combFilterChannel = combFilter.channels[c];
				
				dsp::Sample* const output = reverbBuffer.getChannelStart(c);
				const dsp::Sample* const outputEnd = output + numSamples;
				
#if GSOUND_USE_SIMD
				
				Size combFilterDelayBufferSize = Size(sampleRate*combFilterChannel.delayTime)*sampleFrameWidth;
				
				if ( combFilterChannel.delayBuffer.getSize() < combFilterDelayBufferSize )
				{
					combFilterChannel.delayBuffer.setSize( combFilterDelayBufferSize );
					combFilterChannel.delayBuffer.zero();
				}
				
				dsp::Sample* delayBufferStart = combFilterChannel.delayBuffer.getChannelStart(0);
				const dsp::Sample* const delayBufferEnd = delayBufferStart + combFilterDelayBufferSize;
				dsp::Sample* delay = delayBufferStart + combFilterChannel.currentDelayReadIndex*sampleFrameWidth;
				
				const dsp::Sample* inputStart = renderState.delayBuffer.getChannelStart(c);
				const dsp::Sample* inputEnd = inputStart + actualDelayBufferSize;
				const dsp::Sample* input = inputStart + currentDelayReadIndex*sampleFrameWidth;
				
				for ( Index iteration = 0; iteration < numSIMDIterations; iteration++ )
				{
					SIMDAmplitude feedbackGain;
					SIMDAmplitude currentAmplitude;
					SIMDAmplitude amplitudeChangePerSample;
					
					Size bandIndexStart = iteration*SIMDSample::getWidth();
					
					for ( Index bandIndex = bandIndexStart, i = 0; bandIndex < numFrequencyBands; bandIndex++, i++ )
					{
						ReverbRenderState::CombFilter::CombFilterChannel::FrequencyBand& band =
																		combFilterChannel.frequencyBands[bandIndex];
						
						feedbackGain[i] = band.feedbackGain;
						currentAmplitude[i] = band.currentAmplitude;
						amplitudeChangePerSample[i] = (band.targetAmplitude - band.currentAmplitude)*inverseNumSamples;
						band.currentAmplitude = band.targetAmplitude;
					}
					
					renderCombFilter( output, outputEnd, inputStart, inputEnd, input,
									delayBufferStart, delayBufferEnd, delay,
									currentAmplitude, amplitudeChangePerSample, feedbackGain );
					
					input += SIMDSample::getWidth();
					delay += SIMDSample::getWidth();
					delayBufferStart += SIMDSample::getWidth();
				}
				
				combFilterChannel.currentDelayReadIndex = (combFilterChannel.currentDelayReadIndex + numSamples) % 
																			Size(sampleRate*combFilterChannel.delayTime);
				
				
#else // GSOUND_USE_SIMD
				
				Size combFilterDelayBufferSize = Size(sampleRate*combFilterChannel.delayTime);
				
				if ( combFilterChannel.delayBuffers.getSize() < combFilterDelayBufferSize || 
					combFilterChannel.delayBuffers.getNumberOfBuffers() < numFrequencyBands )
				{
					combFilterChannel.delayBuffers.setSize( combFilterDelayBufferSize );
					combFilterChannel.delayBuffers.setNumberOfBuffers( numFrequencyBands );
					combFilterChannel.delayBuffers.zero();
				}
				
				for ( Index bandIndex = 0; bandIndex < numFrequencyBands; bandIndex++ )
				{
					dsp::Sample* delayBufferStart = combFilterChannel.delayBuffers.getBuffer(bandIndex).getChannelStart(0);
					const dsp::Sample* const delayBufferEnd = delayBufferStart + combFilterDelayBufferSize;
					dsp::Sample* delay = delayBufferStart + combFilterChannel.currentDelayReadIndex;
					
					const dsp::Sample* inputStart = renderState.delayBuffers.getBuffer(bandIndex).getChannelStart(c);
					const dsp::Sample* inputEnd = inputStart + delayBufferSize;
					const dsp::Sample* input = inputStart + currentDelayReadIndex;
					
					ReverbRenderState::CombFilter::CombFilterChannel::FrequencyBand& band =
																	combFilterChannel.frequencyBands[bandIndex];
					
					Float amplitudeChangePerSample = (band.targetAmplitude - band.currentAmplitude)*inverseNumSamples;
					
					renderCombFilter( output, outputEnd, inputStart, inputEnd, input,
										delayBufferStart, delayBufferEnd, delay,
										band.currentAmplitude, amplitudeChangePerSample, band.feedbackGain );
					
					band.currentAmplitude = band.targetAmplitude;
				}
				
				combFilterChannel.currentDelayReadIndex = (combFilterChannel.currentDelayReadIndex + numSamples) % 
																							combFilterDelayBufferSize;
				
#endif // !GSOUND_USE_SIMD
			}
		}
		
		
		//****************************************************************************
		// Render every all pass filter in series in the reverb buffer.
		
		Size numAllPassFilters = reverbRenderState.allPassFilters.getSize();
		
		for ( Index allPassFilterIndex = 0; allPassFilterIndex < numAllPassFilters; allPassFilterIndex++ )
		{
			ReverbRenderState::AllPassFilter& allPassFilter = reverbRenderState.allPassFilters[allPassFilterIndex];
			
			for ( Index c = 0; c < numChannels; c++ )
			{
				ReverbRenderState::AllPassFilter::AllPassFilterChannel& allPassFilterChannel = allPassFilter.channels[c];
				
				Size allPassFilterDelayBufferSize = Size(sampleRate*allPassFilterChannel.delayTime);
				
				if ( allPassFilterChannel.delayBuffer.getSize() < allPassFilterDelayBufferSize )
				{
					allPassFilterChannel.delayBuffer.setSize( allPassFilterDelayBufferSize );
					allPassFilterChannel.delayBuffer.zero();
				}
				
				dsp::Sample* const output = reverbBuffer.getChannelStart(c);
				const dsp::Sample* const outputEnd = output + numSamples;
				
				dsp::Sample* delayBufferStart = allPassFilterChannel.delayBuffer.getChannelStart(0);
				const dsp::Sample* const delayBufferEnd = delayBufferStart + allPassFilterDelayBufferSize;
				dsp::Sample* delay = delayBufferStart + allPassFilterChannel.currentDelayReadIndex;
				
				// Write the output of the all pass filter directly to the input buffer,
				// creating a series chain of all pass filters.
				renderAllPassFilter( output, outputEnd, output,
									delayBufferStart, delayBufferEnd, delay,
									allPassFilterChannel.feedbackGain );
				
				allPassFilterChannel.currentDelayReadIndex = (allPassFilterChannel.currentDelayReadIndex + numSamples) % 
																							allPassFilterDelayBufferSize;
				
			}
		}
		
		//****************************************************************************
		// Mix the output of the reverb rendering with the main output.
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			dsp::Sample* output = outputBuffer.getChannelStart(c) + startIndex;
			const dsp::Sample* const outputEnd = output + numSamples;
			const dsp::Sample* reverb = reverbBuffer.getChannelStart(c);
			
			while ( output != outputEnd )
			{
				*output = dsp::sample::mix( *output, *reverb );
				output++;
				reverb++;
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constant Delay Time Path Rendering Method
//############		
//##########################################################################################
//##########################################################################################




#if GSOUND_USE_SIMD
void SoundPropagationRenderer:: fillBufferDelaysEqual( dsp::Sample* output, const dsp::Sample* const outputEnd,
												const dsp::Sample* const delayBufferStart,
												const dsp::Sample* const delayBufferEnd,
												const dsp::Sample* delay,
												const SIMDAmplitude& startingAmplitude, const SIMDAmplitude& amplitudeChangePerSample )
{
	// Get a copy of the sample frame width on the stack, to avoid having to go to the heap every iteration.
	const Size sampleWidth = sampleFrameWidth;

	SIMDAmplitude currentAmplitude = startingAmplitude;
	
	while ( output != outputEnd )
	{
		if ( delay >= delayBufferEnd )
			delay = delayBufferStart;
		
		*output += (SIMDSample(delay)*currentAmplitude).sum();
		
		delay += sampleWidth;
		output++;
		currentAmplitude += amplitudeChangePerSample;
	}
}


#else


void SoundPropagationRenderer:: fillBufferDelaysEqual( dsp::Sample* output, const dsp::Sample* const outputEnd,
												const dsp::Sample* const delayBufferStart,
												const dsp::Sample* const delayBufferEnd,
												const dsp::Sample* delay,
												Float currentAmplitude, Float amplitudeChangePerSample )
{
	while ( output != outputEnd )
	{
		if ( delay >= delayBufferEnd )
			delay = delayBufferStart;
		
		*output = dsp::sample::mix( dsp::sample::scale( *delay, currentAmplitude ), *output );
		
		delay++;
		output++;
		currentAmplitude += amplitudeChangePerSample;
	}
}
#endif



//##########################################################################################
//##########################################################################################
//############		
//############		Changing Delay Time Path Rendering Method
//############		
//##########################################################################################
//##########################################################################################




#if GSOUND_USE_SIMD
void SoundPropagationRenderer:: fillBufferDelayChanges( dsp::Sample* output, const dsp::Sample* const outputEnd,
												const dsp::Sample* const delayBufferStart,
												const dsp::Sample* const delayBufferEnd,
												const dsp::Sample* delay,
												Float fractionalSampleDelay, Float delayChangePerSample,
												const SIMDAmplitude& startingAmplitude, const SIMDAmplitude& amplitudeChangePerSample )
{
	// Get a copy of the sample frame width on the stack, to avoid having to go to the heap every iteration.
	const Size sampleWidth = sampleFrameWidth;

	SIMDAmplitude currentAmplitude = startingAmplitude;
	
	const dsp::Sample* lastDelay = delay;
	SIMDFloat a = fractionalSampleDelay;
	SIMDFloat d = delayChangePerSample;
	SIMDFloat one = 1.0f;
	
	delay += sampleWidth;
	
	while ( output != outputEnd )
	{
		if ( delay >= delayBufferEnd )
			delay = delayBufferStart;
		
		*output += ((SIMDSample(delay)*a + SIMDSample(lastDelay)*(one - a))*currentAmplitude).sum();
		
		output++;
		a += d;
		currentAmplitude += amplitudeChangePerSample;
		
		while ( a.a > 1.0f )
		{
			a -= one;
			lastDelay = delay;
			delay += sampleWidth;
		}
	}
}


#else


void SoundPropagationRenderer:: fillBufferDelayChanges( dsp::Sample* output, const dsp::Sample* const outputEnd,
												const dsp::Sample* const delayBufferStart,
												const dsp::Sample* const delayBufferEnd,
												const dsp::Sample* delay,
												Float fractionalSampleDelay, Float delayChangePerSample,
												Float currentAmplitude, Float amplitudeChangePerSample )
{
	const dsp::Sample* lastDelay = delay;
	Float a = fractionalSampleDelay;
	
	delay++;
	
	while ( output != outputEnd )
	{
		if ( delay >= delayBufferEnd )
			delay = delayBufferStart;
		
		*output = dsp::sample::mix( *output, dsp::sample::scale( dsp::sample::mix( dsp::sample::scale( *delay, a ),
																	dsp::sample::scale( *lastDelay, 1.0f - a ) ),
														currentAmplitude ) );
		
		output++;
		a += delayChangePerSample;
		currentAmplitude += amplitudeChangePerSample;
		
		while ( a > 1.0f )
		{
			a -= 1.0f;
			lastDelay = delay;
			delay++;
		}
	}
}
#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Comb Filter Rendering Method
//############		
//##########################################################################################
//##########################################################################################


#if GSOUND_USE_SIMD


void SoundPropagationRenderer:: renderCombFilter( dsp::Sample* output, const dsp::Sample* const outputEnd,
													const dsp::Sample* const inputStart, const dsp::Sample* const inputEnd,
													const dsp::Sample* input,
													dsp::Sample* const delayBufferStart,
													const dsp::Sample* const delayBufferEnd,
													dsp::Sample* delay,
													const SIMDAmplitude& startingAmplitude, const SIMDAmplitude& amplitudeChangePerSample,
													const SIMDAmplitude& feedbackGain )
{
	const Size sampleWidth = sampleFrameWidth;
	
	SIMDAmplitude currentAmplitude = startingAmplitude;
	
	while ( output != outputEnd )
	{
		if ( delay >= delayBufferEnd )
			delay = delayBufferStart;
		
		if ( input >= inputEnd )
			input = inputStart;
		
		SIMDSample delaySample = SIMDSample(delay)*currentAmplitude;
		
		(SIMDSample(delay)*feedbackGain + SIMDSample(input)).store( delay );
		*output += delaySample.sum();
		
		delay += sampleWidth;
		input += sampleWidth;
		output++;
		currentAmplitude += amplitudeChangePerSample;
	}
}


#else // GSOUND_USE_SIMD


void SoundPropagationRenderer:: renderCombFilter( dsp::Sample* output, const dsp::Sample* const outputEnd,
													const dsp::Sample* const inputStart, const dsp::Sample* const inputEnd,
													const dsp::Sample* input,
													dsp::Sample* const delayBufferStart,
													const dsp::Sample* const delayBufferEnd,
													dsp::Sample* delay,
													Float currentAmplitude, Float amplitudeChangePerSample,
													Float feedbackGain )
{
	while ( output != outputEnd )
	{
		if ( delay >= delayBufferEnd )
			delay = delayBufferStart;
		
		if ( input >= inputEnd )
			input = inputStart;
		
		dsp::Sample delaySample = dsp::sample::scale(*delay,currentAmplitude);
		
		*delay = dsp::sample::mix( dsp::sample::scale(*delay,feedbackGain), *input );
		*output = dsp::sample::mix( *output, delaySample );
		
		delay++;
		input++;
		output++;
		currentAmplitude += amplitudeChangePerSample;
	}
}


#endif // GSOUND_USE_SIMD




//##########################################################################################
//##########################################################################################
//############		
//############		All Pass Filter Rendering Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagationRenderer:: renderAllPassFilter( dsp::Sample* output, const dsp::Sample* const outputEnd,
													const dsp::Sample* input,
													dsp::Sample* const delayBufferStart,
													const dsp::Sample* const delayBufferEnd,
													dsp::Sample* delay,
													Float feedbackGain )
{
	while ( output != outputEnd )
	{
		if ( delay >= delayBufferEnd )
			delay = delayBufferStart;
		
		dsp::Sample delaySample = *delay;
		
		*delay = dsp::sample::mix( dsp::sample::scale(delaySample,feedbackGain), *input );
		*output = dsp::sample::mix( delaySample, -dsp::sample::scale( *delay, feedbackGain ) );
		
		delay++;
		output++;
		input++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Impulse Response Sorting Methods
//############		
//##########################################################################################
//##########################################################################################




int SoundPropagationRenderer:: compareImpulsesByIntensity( const void* a, const void* b )
{
	const Impulse* i1 = (const Impulse*)a;
	const Impulse* i2 = (const Impulse*)b;
	
	if ( i1->amplitude > i2->amplitude )
		return -1;
	else if ( i1->amplitude < i2->amplitude )
		return 1;
	else
		return 0;
}




GSOUND_INLINE void SoundPropagationRenderer:: sortImpulsesByDecreasingIntensity( ArrayList<Impulse>& list )
{
	std::qsort( list.getArrayPointer(), list.getSize(),
				sizeof(Impulse), compareImpulsesByIntensity );
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
