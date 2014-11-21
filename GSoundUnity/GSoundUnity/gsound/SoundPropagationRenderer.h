/*
 * Project:     GSound
 * 
 * File:        gsound/SoundPropagationRenderer.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundPropagationRenderer class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_PROPAGATION_RENDERER_H
#define INCLUDE_GSOUND_SOUND_PROPAGATION_RENDERER_H


#include "GSoundBase.h"


#include "GSoundDSP.h"
#include "FrequencyPartition.h"
#include "SoundPropagationPathBuffer.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which renders audio based on the output of a sound propagation system.
class SoundPropagationRenderer : public dsp::SoundOutput
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			SoundPropagationRenderer( const dsp::SpeakerConfiguration& speakerConfiguration );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			~SoundPropagationRenderer();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Propagation Path Update Method
			
			
			
			
			/// Update the propagation paths currently being rendered.
			void updatePropagationPaths( const SoundPropagationPathBuffer& newPathBuffer );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Frequency Band Partition Accessor Methods
			
			
			
			
			/// Get the frequency partition for this sound listener.
			/**
			  * This object denotes how many frequency bands that a sound source's
			  * input audio gets split into when sent to the sound renderer. More
			  * frequency bands will take more time to render but will result in higher
			  * quality audio. Less frequency bands will be more efficient but will
			  * also produce lower quality audio.
			  */
			GSOUND_INLINE const FrequencyPartition& getFrequencyPartition() const
			{
				return frequencyPartition;
			}
			
			
			
			
			/// Set the frequency partition for this sound source.
			/**
			  * This object denotes how many frequency bands that the sound source's
			  * input audio gets split into when sent to the sound renderer. More
			  * frequency bands will take more time to render but will result in higher
			  * quality audio. Less frequency bands will be more efficient but will
			  * also produce lower quality audio.
			  */
			void setFrequencyPartition( const FrequencyPartition& newFrequencyPartition );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Channel Spatializer Accessor Methods
			
			
			
			
			/// Get the speaker configuration for which this sound propagation renderer is generating audio.
			GSOUND_INLINE const dsp::SpeakerConfiguration& getSpeakerConfiguration() const
			{
				return speakerConfiguration;
			}
			
			
			
			
			/// Set the speaker configuration for which this sound propagation renderer is generating audio.
			void setSpeakerConfiguration( const dsp::SpeakerConfiguration& newSpeakerConfiguration );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Reverb Enabling Methods
			
			
			
			
			/// Get whether or not reverb rendering is enabled.
			GSOUND_INLINE Bool getReverbIsEnabled() const
			{
				return reverbIsEnabled;
			}
			
			
			
			
			/// Set whether or not reverb rendering is enabled.
			GSOUND_INLINE void setReverbIsEnabled( Bool newReverbIsEnabled )
			{
				reverbIsEnabled = newReverbIsEnabled;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Reverb Enabling Methods
			
			
			
			
			/// Get the maximum allowed delay time for a propagation path being rendered.
			/**
			  * All propagation paths that have delay times greater than this value will have
			  * their delay times set to be the maximum delay time. This value directly influences
			  * the amount of memory required by the delay buffers used in this renderer.
			  * The total number of samples in the delay buffer for each sound source will be
			  * equal to c*fb*fs*mdt*2, where c is the number of channels,  fb is the number of
			  * frequency bands, fs is the sample rate, and mdt is the maximum delay time.
			  * A recommended value for most simulations is 0.5s.
			  * 
			  * @return the maximum allowed delay time for a propagation path.
			  */
			GSOUND_INLINE Real getMaximumDelayTime() const
			{
				return maxDelayTime;
			}
			
			
			
			
			/// Set the maximum allowed delay time for a propagation path being rendered.
			/**
			  * All propagation paths that have delay times greater than this value will have
			  * their delay times set to be the maximum delay time. This value directly influences
			  * the amount of memory required by the delay buffers used in this renderer.
			  * The total number of samples in the delay buffer for each sound source will be
			  * equal to c*fb*fs*mdt*2, where c is the number of channels,  fb is the number of
			  * frequency bands, fs is the sample rate, and mdt is the maximum delay time.
			  * A recommended value for most simulations is 0.5s.
			  * 
			  * @param newMaxDelayTime - the new maximum allowed delay time for a propagation path.
			  */
			GSOUND_INLINE void setMaximumDelayTime( Real newMaxDelayTime )
			{
				maxDelayTime = newMaxDelayTime;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Maximum Path Count Accessor Methods
			
			
			
			
			/// Get the maximum allowed number of propagation paths that can be rendered at once.
			/**
			  * If the number of paths that this renderer is asked to render exceeds this amount, the
			  * renderer will sort the paths by decreasing intensity and only render the loudest paths
			  * up to this limit.
			  * 
			  * @return the maximum allowed number of propagation paths per sound source.
			  */
			GSOUND_INLINE Size getMaximumNumberOfPaths() const
			{
				return maxNumberOfPropagationPaths;
			}
			
			
			
			
			/// Set the maximum allowed number of propagation paths that can be rendered at once.
			/**
			  * If the number of paths that this renderer is asked to render exceeds this amount, the
			  * renderer will sort the paths by decreasing intensity and only render the loudest paths
			  * up to this limit.
			  * 
			  * @param newMaxNumberOfPaths - the new maximum allowed number of paths per sound source.
			  */
			GSOUND_INLINE void setMaximumNumberOfPaths( Size newMaxNumberOfPaths )
			{
				maxNumberOfPropagationPaths = newMaxNumberOfPaths;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Maximum Path Count Accessor Methods
			
			
			
			
			/// Get the maximum number of frames that a propagation path can go without an update and not be removed.
			/**
			  * After a propagation path goes without an update for 1 frame, the propagation renderer
			  * automatically fades out that path until it reaches this age, at which point it is
			  * removed from the rendering system. This facility is provided in order to improve audio
			  * quality when propagation paths become invalid and then becomd valid again, causing a 
			  * momentary gap in the audio.
			  * 
			  * @return the maximum number of frames that a propagation path can go without an update before it is removed.
			  */
			GSOUND_INLINE Size getMaximumPathAge() const
			{
				return maxPathAge;
			}
			
			
			
			
			/// Set the maximum number of frames that a propagation path can go without an update and not be removed.
			/**
			  * After a propagation path goes without an update for 1 frame, the propagation renderer
			  * automatically fades out that path until it reaches this age, at which point it is
			  * removed from the rendering system. This facility is provided in order to improve audio
			  * quality when propagation paths become invalid and then becomd valid again, causing a 
			  * momentary gap in the audio.
			  * 
			  * @param newMaxPathAge - the maximum number of frames that a path can go without an update.
			  */
			GSOUND_INLINE void setMaximumPathAge( Size newMaxPathAge )
			{
				maxPathAge = newMaxPathAge;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			
			
			/// Get the sample rate of the audio which this sound propagation renderer is producing.
			virtual Float getSampleRate() const;
			
			
			
			
			/// Set the sample rate of the audio which this sound propagation renderer is producing.
			/**
			  * The provided sample rate is clamped to the range of [0, +infinity]. 
			  * If necessary, rendered audio from each sound source will be converted to the desired
			  * sample rate. Thus, sound sources can have input audio of different sample rates than
			  * each other and the global sample rate (specified here). However, there will be a runtime
			  * cost involved in performing the conversion. There is no additional cost when all audio
			  * has the same sample rate.
			  * 
			  * @param newSampleRate - the new sample rate to use for audio rendering in this sound propagation renderer.
			  */
			void setSampleRate( Float newSampleRate );
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Format Accessor Methods
			
			
			
			
			/// Get the number of channels that this sound propagation renderer is outputing.
			virtual Size getNumberOfChannels() const;
			
			
			
			
			/// Get the number of outputs that this sound propagation renderer has. This method always returns 1.
			virtual Size getNumberOfOutputs() const;
			
			
			
			
			/// Return whether or not there is any output remaining from this sound propagation renderer.
			/**
			  * This method will always return TRUE. In the event that there are no sound sources
			  * to render, zeros will be written to the output audio stream.
			  */
			virtual Bool hasOutputRemaining() const;
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Class Declarations
			
			
			
			
			/// A class which mixes down an arbitrary number of input channels to a single output channel.
			class MonoMixer;
			
			
			
			
			/// A class which splits a single input channel into an arbitrary number of output channels.
			class MonoSplitter;
			
			
			
			
			/// A class used internally when sorting sound source contributions by intensity.
			class Impulse;
			
			
			
			
			/// A class used internally to keep track of the delay interpolation parameters for a propagation path.
			class InterpolationState;
			
			
			
			
			class PropagationPathRenderState;
			
			
			
			
			/// A class which contains all rendering state for a single sound source.
			class SoundSourceRenderState;
			
			
			
			
			/// A class which contains the state for the reverb rendering of a single sound source.
			class ReverbRenderState;
			
			
			
			
#if GSOUND_USE_SIMD
			/// Define the type to use for a set of interleaved samples.
			typedef SIMDFloat SIMDAmplitude;
			
			
			/// Define the type to use for a set of interleaved samples.
			typedef SIMDFloat SIMDSample;
#endif
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Main Rendering Methods
			
			
			
			
			virtual Size fillBuffer( dsp::SoundStream& stream, Index startIndex, Size numSamples );
			
			
			
			
			void renderSoundSource( dsp::SoundBuffer& outputBuffer, Index startIndex, Size numSamples, 
									SoundSourceRenderState& renderState );
			
			
			
#if GSOUND_USE_SIMD
			
			void fillBufferDelaysEqual( dsp::Sample* output, const dsp::Sample* const outputEnd,
										const dsp::Sample* const delayBufferStart, const dsp::Sample* const delayBufferEnd,
										const dsp::Sample* delay,
										const SIMDAmplitude& startingAmplitude, const SIMDAmplitude& amplitudeChangePerSample );
			

			
			
			void fillBufferDelayChanges( dsp::Sample* output, const dsp::Sample* const outputEnd,
										const dsp::Sample* const delayBufferStart, const dsp::Sample* const delayBufferEnd,
										const dsp::Sample* delay,
										Float fractionalSampleDelay, Float delayChangePerSample,
										const SIMDAmplitude& startingAmplitude, const SIMDAmplitude& amplitudeChangePerSample );
			
			
			
			void renderCombFilter( dsp::Sample* output, const dsp::Sample* const outputEnd,
									const dsp::Sample* const inputStart, const dsp::Sample* const inputEnd,
									const dsp::Sample* input,
									dsp::Sample* const delayBufferStart, const dsp::Sample* const delayBufferEnd,
									dsp::Sample* delay, 
									const SIMDAmplitude& currentAmplitude, const SIMDAmplitude& amplitudeChangePerSample,
									const SIMDAmplitude& feedbackGain );
			
			
#else
			
			void fillBufferDelaysEqual( dsp::Sample* output, const dsp::Sample* const outputEnd,
										const dsp::Sample* const delayBufferStart, const dsp::Sample* const delayBufferEnd,
										const dsp::Sample* delay,
										Float currentAmplitude, Float newAmplitude );
			
			
			
			
			void fillBufferDelayChanges( dsp::Sample* output, const dsp::Sample* const outputEnd,
										const dsp::Sample* const delayBufferStart, const dsp::Sample* const delayBufferEnd,
										const dsp::Sample* delay,
										Float fractionalSampleDelay, Float delayChangePerSample,
										Float currentAmplitude, Float amplitudeChangePerSample );
			
			
			
			void renderCombFilter( dsp::Sample* output, const dsp::Sample* const outputEnd,
									const dsp::Sample* const inputStart, const dsp::Sample* const inputEnd,
									const dsp::Sample* input,
									dsp::Sample* const delayBufferStart, const dsp::Sample* const delayBufferEnd,
									dsp::Sample* delay,
									Float currentAmplitude, Float amplitudeChangePerSample,
									Float feedbackGain );
			
#endif
			
			
			
			
			void renderAllPassFilter( dsp::Sample* output, const dsp::Sample* const outputEnd,
									const dsp::Sample* input,
									dsp::Sample* const delayBufferStart, const dsp::Sample* const delayBufferEnd,
									dsp::Sample* delay, Float feedbackGain );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Source Propagation Path Update Method
			
			
			
			void updateSourcePropagationPaths( const SoundSourcePropagationPathBuffer& pathBuffer,
												SoundSourceRenderState& renderState, Size maxNumberOfSourcePaths );
			
			
			
			void updatePropagationPathRenderState( PropagationPathRenderState& renderState,
													const Impulse& impulse, Index timeStamp );
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Helper Methods
			
			
			
			
			/// Compare the two pointers to impulses by order of intensity.
			static int compareImpulsesByIntensity( const void* a, const void* b );
			
			
			
			
			/// Sort the specified list of sound source impulses in order of decreasing intensity.
			GSOUND_INLINE static void sortImpulsesByDecreasingIntensity( ArrayList<Impulse>& list );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Rendering Data Members
			
			
			
			
			/// A map from each sound source being rendered to that source's rendering state object.
			HashMap<SoundSource*,SoundSourceRenderState*> sourceRenderStates;
			
			
			
			
			/// The current time stamp (frame index) for this sound propagation renderer.
			Index timeStamp;
			
			
			
#if GSOUND_USE_SIMD
			/**
			  * A sound stream used to get the frequency band separated output from each sound source
			  * before it is written to the interleaved delay buffer.
			  */
			dsp::SoundStream inputStream;
			
			
			
			/// The number of SIMD iterations that must be performed on the audio per sample.
			/**
			  * This is on most platforms the numbered multiple of 4 that is greater than or equal to the number of
			  * frequency bands being rendered. So, 1 == 1 to 4 frequency bands, 2 == 5 to 8 frequency bands, etc.
			  */
			Size numSIMDIterations;
			
			
			
			
			/// The number of interleaved frequency band samples per actual sample.
			Size sampleFrameWidth;
			
			
#else // GSOUND_USE_SIMD
			
			
			/// A buffer used to hold the intermediate output of reverb rendering before it is mixed with the output buffer.
			dsp::SoundBuffer reverbBuffer;
			
#endif
			
			
			
			/// A temporary object (stored here to reduce reallocations) that holds panning information.
			dsp::ChannelGainArray channelGainArray;
			
			
			
			
			/// A list of impulses that is used as scratch when sorting impulses by decreasing intensity.
			ArrayList<Impulse> impulseSortList;
			
			
			
			
			/// The number of samples required for each channel of the delay buffer for each frequency band.
			Size delayBufferSize;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Renderer Configuration Data Members
			
			
			
			
			/// The speaker configuration for which this sound propagation renderer is generating audio.
			dsp::SpeakerConfiguration speakerConfiguration;
			
			
			
			
			/// An object which specifies how incoming audio will be split into frequency bands for rendering.
			FrequencyPartition frequencyPartition;
			
			
			
			
			/// The output sample rate for this sound propagation renderer.
			Float sampleRate;
			
			
			
			
			/// The maximum allowed delay time for any propagation path in seconds.
			/**
			  * This value directly influences the amount of memory required by the delay
			  * buffers used in this sound source renderer. The total number of samples in the
			  * delay buffers will be equal to c*fb*fs*mdt*2, where c is the number of channels,
			  * fb is the number of frequency bands, fs is the sample rate, and mdt is the maximum
			  * delay time. A recommended value for most simulations is 0.5s.
			  */
			Real maxDelayTime;
			
			
			
			
			/// Whether or not reverb rendering is enabled for this sound propagation renderer.
			Bool reverbIsEnabled;
			
			
			
			
			/// The maximum number of propagation paths that this sound propagation renderer should render.
			/**
			  * If the number of paths that this renderer is asked to render exceeds this amount, the
			  * renderer will sort the paths by decreasing intensity and only render the loudest paths
			  * up to this limit.
			  */
			Size maxNumberOfPropagationPaths;
			
			
			
			
			/// The maximum number of frames that a propagation path can be without an update and still be kept.
			Size maxPathAge;
			
			
			
			
			/// A mutex which indicates that rendering parameters are either being used or changed.
			mutable Mutex renderMutex;
			
			
			
			
};



//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_PROPAGATION_RENDERER_H
