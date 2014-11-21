/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundMixer.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundMixer class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_MIXER_H
#define INCLUDE_GSOUND_SOUND_MIXER_H


#include "GSoundDSPConfig.h"


#include "SoundStream.h"
#include "SoundOutput.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// A class which provides facilities to mix incoming audio from an arbitrary number of input sources.
/**
  * The SoundMixer class takes the input audio from an arbitrary number of sources and mixes
  * it together into a single output stream. Sample rate conversion of the input audio can be
  * optionally performed in order to consume audio from the input sources at the same rate in
  * seconds, avoiding pitch shifting artifacts where input audio of a different sample rate
  * is interpreted as being of a different sample rate. The SoundMixer class allows the user
  * to set the output sample rate of the mixer and enable/disable sample rate conversion (enabled
  * by default).
  *
  * When mixing together inputs that have different numbers of channels/outputs, the SoundMixer
  * produces a stream which is as wide as the input with the most channels/outputs. Input channels
  * and outputs are mixed with output channels and outputs that have the same indices in the audio
  * stream.
  */
class SoundMixer : public SoundOutput
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a SoundMixer with no specified output sample rate.
			/**
			  * The output sample rate of the SoundMixer is initially 0 until
			  * an input is added to the mixer, at which point the output sample rate
			  * of the SoundMixer is set to the sample rate of the first input.
			  * Sample rate conversion is enabled by default.
			  */
			SoundMixer();
			
			
			
			
			/// Create a SoundMixer which uses the specified output sample rate.
			/**
			  * The audio from each input of the SoundMixer will be automatically converted
			  * to the specified sample rate (if necessary) before it is mixed with the
			  * other inputs.
			  * 
			  * @param newSampleRate - the output sample rate to use for this SoundMixer.
			  */
			SoundMixer( Float newSampleRate );
			
			
			
			
			
			/// Create a complete copy of the specified SoundMixer object.
			/**
			  * All internal state will be replicated (inputs, sample rate, etc.).
			  * 
			  * @param other - the SoundMixer object to be copied.
			  */
			SoundMixer( const SoundMixer& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy this SoundMixer and release all internal state.
			~SoundMixer();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Replace the internal state of this SoundMixer object with that of another.
			/**
			  * All previously existing state is replaced by the state copied from the
			  * other SoundMixer object.
			  * 
			  * @param other - the SoundMixer object to replace the contents of this object.
			  * @return a reference to this SoundMixer object in order to enable assignment chaining.
			  */
			SoundMixer& operator = ( const SoundMixer& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Sample Rate Accessor Method
			
			
			
			
			/// Get the current sample rate of audio being produced by this SoundMixer.
			/**
			  * It is possible that the value returned by this method can change between
			  * successive calls to getSamples(), however it is guaranteed that the sample
			  * rate will not change during a call to getSamples().
			  */
			virtual Float getSampleRate() const;
			
			
			
			
			/// Set the sample rate of the output audio of this SoundMixer.
			/**
			  * If sample rate conversion is enabled, the audio from each input
			  * to this SoundMixer is converted to the specified sample rate (if necessary)
			  * and then mixed with the other inputs. The sample rate value is
			  * clamped to the range of [0,+infinity] when it is set.
			  * 
			  * @param newSampleRate - the sample rate that input audio will be converted to.
			  */
			void setSampleRate( Float newSampleRate );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Rate Conversion Enabling Methods
			
			
			
			
			/// Get whether or not automatic sample rate conversion is enabled.
			/**
			  * If automatic sample rate conversion is enabled, The audio from each
			  * input of the SoundMixer will be automatically converted to the
			  * output sample rate (if necessary) of the SoundMixer before being mixed
			  * with the other inputs. If sample rate conversion is not enabled, 
			  * no conversion will be performed, resulting in potential pitch shifting
			  * if all inputs are not of the same sample rate as the output sample rate
			  * of the SoundMixer.
			  * 
			  * @return whether or not sample rate conversion is enabled.
			  */
			Bool getSampleRateConversionIsEnabled() const;
			
			
			
			
			/// Set whether or not automatic sample rate conversion is enabled.
			/**
			  * If automatic sample rate conversion is enabled, The audio from each
			  * input of the SoundMixer will be automatically converted to the
			  * output sample rate (if necessary) of the SoundMixer before being mixed
			  * with the other inputs. If sample rate conversion is not enabled, 
			  * no conversion will be performed, resulting in potential pitch shifting
			  * if all inputs are not of the same sample rate as the output sample rate
			  * of the SoundMixer.
			  * 
			  * @param newSampleRateConversionIsEnabled - whether or not sample rate conversion should be enabled.
			  */
			void setSampleRateConversionIsEnabled( Bool newSampleRateConversionIsEnabled );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Input Accessor Methods
			
			
			
			
			/// Get the number of inputs that this SoundMixer is mixing.
			/**
			  * @return the number of input that this SoundMixer has.
			  */
			Size getNumberOfInputs() const;
			
			
			
			
			/// Get the SoundMixer's input which has the specified index.
			/**
			  * If the specified index is outside the valid range of indices of
			  * [0,getNumberOfInputs()], an assertion is raised.
			  * 
			  * @param inputIndex - the index of the input to query.
			  * @return the SoundMixer's input with the specified index.
			  */
			SoundOutput* getInput( Index inputIndex ) const;
			
			
			
			
			/// Set the input of the SoundMixer with the specified index.
			/**
			  * If the specified index is outside the valid range of indices of
			  * [0,getNumberOfInputs()], an assertion is raised. If the new input
			  * object is NULL, the input at the specified index is removed. Otherwise, 
			  * the input for the sound mixer at the specified index is replaced with
			  * the new input.
			  * 
			  * @param inputIndex - the index of the input to set.
			  * @param newInput - the new input to use at the specified index in this SoundMixer.
			  */
			void setInput( Index inputIndex, SoundOutput* newInput );
			
			
			
			
			/// Add the specified input to the SoundMixer at the end of its list of inputs.
			/**
			  * If the pointer to the new input is NULL, the call to this method has no
			  * effect. Otherwise, the new input is added to the end of the SoundMixer's
			  * list of inputs.
			  * 
			  * @param newInput - the input to be added to the end of the SoundMixer's list of inputs.
			  */
			void addInput( SoundOutput* newInput );
			
			
			
			
			/// Remove the input of this sound mixer at the specified index.
			/**
			  * If the specified index is outside the valid range of indices of
			  * [0,getNumberOfInputs()], an assertion is raised. Removing an input
			  * causes all inputs after the specified index to be shifted by one
			  * index to fill the removed input's place.
			  * 
			  * @param inputIndex - the index of the input to remove.
			  */
			void removeInput( Index inputIndex );
			
			
			
			
			/// Remove the specified input from this SoundMixer.
			/**
			  * This method searches through the internal array of inputs of this
			  * SoundMixer for the specified input object. If it is found, the input
			  * is removed and TRUE is returned. Otherwise, the method call has no
			  * effect and FALSE is returned.
			  * 
			  * @param input - the input to try to remove from this SoundMixer.
			  * @return whether or not the remove operation was successful.
			  */
			Bool removeInput( SoundOutput* input );
			
			
			
			
			/// Clear all inputs from this SoundMixer.
			void clearInputs();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Format Accessor Methods
			
			
			
			
			/// Get the current number of channels that this SoundMixer produces.
			/**
			  * The number of channels that this SoundMixer has is computed
			  * to be the maximum number of channels of any input.
			  * 
			  * @return the number of channels that this SoundMixer produces.
			  */
			virtual Size getNumberOfChannels() const;
			
			
			
			
			/// Get the number of outputs that this SoundMixer has.
			/**
			  * The number of outputs that this SoundMixer has is computed
			  * to be the maximum number of outputs of any input.
			  * 
			  * @return the number of outputs that this SoundMixer has.
			  */
			virtual Size getNumberOfOutputs() const;
			
			
			
			
			/// Return whether or not a SoundMixer has any sound remaining to produce.
			/**
			  * This method will return TRUE if any of the SoundMixer's inputs have audio
			  * to produce. If none of the inputs to this SoundMixer have any output
			  * remaining, FALSE will be returned.
			  * 
			  * @return whether or not this SoundMixer has any output remaining.
			  */
			virtual Bool hasOutputRemaining() const;
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Sound Mixer Input Class Declaration
			
			
			
			
			/// A class used to encapsulate data needed by each input of this SoundMixer.
			class SoundMixerInput;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Mixing Helper Methods
			
			
			
			
			/// Mix samples from the input buffer into the output buffer until the input's end is reached.
			GSOUND_FORCE_INLINE void mixWithOutput( const Sample* input, const Sample* const inputEnd, Sample* output );
			
			
			
			
			/// Mix SIMD-width samples from the input buffer into the output buffer until the input's end is reached.
			GSOUND_FORCE_INLINE void mixWithOutputSIMD( const Sample* input, const Sample* const inputEnd, Sample* output );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Main Rendering Method
			
			
			
			
			/// Fill the specified sound stream with the specified number of channels starting at the given index.
			virtual Size fillBuffer( SoundStream& stream, Index startIndex, Size numSamples );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The inputs of this SoundMixer.
			ArrayList<SoundMixerInput> inputs;
			
			
			
			
			/// The target output sample rate to which all input audio is converted.
			Float sampleRate;
			
			
			
			
			/// Whether or not automatic sample rate conversion is enabled.
			Bool sampleRateConversionIsEnabled;
			
			
			
			
			/// The number of outputs that this SoundMixer has.
			Size numOutputs;
			
			
			
			
			/// The number of channels that this SoundMixer produces.
			Size numChannels;
			
			
			
			
			/// An intermediate buffer for audio data used when requesting audio from inputs.
			SoundStream inputStream;
			
			
			
			
			/// A mutex used to synchonize changes to rendering parameters and the rendering itself.
			mutable Mutex renderMutex;
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_MIXER_H
