/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SampleRateConverter.h
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


#ifndef INCLUDE_GSOUND_SAMPLE_RATE_CONVERTER_H
#define INCLUDE_GSOUND_SAMPLE_RATE_CONVERTER_H


#include "GSoundDSPConfig.h"


#include "SoundProcessor.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which is used to provide conversion to an arbitrary sample rate for an audio stream.
/**
  * A SampleRateConverter provides a mechanism to dynamically change the sample
  * rate of an incoming audio stream in real time to an arbitrary output sample rate.
  * This can be useful when working with audio from many different sources which may have
  * different sample rates.
  *
  * The sample rate conversion algorithm is implementation defined but should produce
  * acceptable audio for real-time applications when converting between common sample rates.
  * It is however not guaranteed to be of similar quality to offline sample rate conversion
  * algorithms and should not be used as a replacement for such an operation.
  *
  * If the input sample rate is the same as the desired output sample rate, 
  * no conversion is performed and the SampleRateConverter incurrs almost no DSP
  * overhead.
  */
class SampleRateConverter : public SoundProcessor
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a SampleRateConverter that has no input object or output sample rate.
			/**
			  * The target sample rate is initially set to 0 and will be replaced by the sample
			  * rate of the audio input object if it is not otherwise set.
			  */
			SampleRateConverter();
			
			
			
			
			/// Create a SampleRateConverter object with no sound input object and the specified output sample rate.
			/**
			  * The sample rate value is clamped to the range of [0,+infinity]. A value of 0 will produce
			  * no output audio from the sample rate converter.
			  * 
			  * @param newSampleRate - the sample rate of the audio that this SampleRateConverter should produce.
			  */
			SampleRateConverter( Float newSampleRate );
			
			
			
			
			/// Create a SampleRateConverter object with the specified sound input object and default output sample rate.
			/**
			  * If the specified audio input object is NULL, the output sample rate is set to 0 and
			  * the sample rate converter behaves if it had no input. Otherwise, the output sample rate
			  * is set to the sample rate of the audio provided by the input audio unit.
			  * 
			  * @param input - a pointer to an object which should provide the input audio for this converter.
			  */
			SampleRateConverter( SoundOutput* input );
			
			
			
			
			/// Create a SampleRateConverter object with the specified sound input object and output sample rate.
			/**
			  * The sample rate value is clamped to the range of [0,+infinity]. A value of 0 will produce
			  * no output audio from the sample rate converter. NULL is a legal value for the sound input
			  * object, behaving as if the SampleRateConverter had no input.
			  * 
			  * @param input - a pointer to an object which should provide the input audio for this converter.
			  * @param newSampleRate - the sample rate of the audio that this SampleRateConverter should produce.
			  */
			SampleRateConverter( SoundOutput* input, Float newSampleRate );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			
			
			/// Get the output sample rate of this sample rate converter.
			virtual Float getSampleRate() const;
			
			
			
			
			/// Set the output sample rate of this sample rate converter.
			/**
			  * Any input audio will be converted to this sample rate at the converter's output.
			  * The sample rate value is clamped to the range of [0,+infinity]. A sample rate of 
			  * 0 will result in no output audio being produced.
			  * 
			  * @param newSampleRate - the sample rate of the audio that this SampleRateConverter should produce.
			  */
			void setSampleRate( Float newSampleRate );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Input Accessor Methods
			
			
			
			
			/// Get a pointer to the object which is providing input audio for this SampleRateConverter.
			virtual SoundOutput* getInput() const;
			
			
			
			
			/// Set a pointer to the object which is providing input audio for this SampleRateConverter.
			/**
			  * Setting this pointer to NULL effectively removes the input for the SampleRateConverter.
			  * 
			  * @param newInput - a pointer to an object which should provide the input audio for this converter.
			  */
			virtual void setInput( SoundOutput* newInput );
			
			
			
			
			/// If there is an input audio object, remove it from the SampleRateConverter.
			/**
			  * After a call to this method, the getInput() method is guaranteed to return NULL
			  * and the hasInput() method is guaranteed to return FALSE.
			  */
			virtual void removeInput();
			
			
			
			
			/// Return whether or not this SampleRateConverter has an object which is providing input audio.
			/**
			  * If such an object exists, TRUE is returned. Otherwise FALSE is returned.
			  * 
			  * @return whether or not this SampleRateConverter has an object which is providing input audio.
			  */
			virtual Bool hasInput() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Format Accessor Methods
			
			
			
			
			/// Return the number of channels of audio that this SampleRateConverter is producing.
			/**
			  * This will be equal to the number of channels that the input audio object is producing
			  * if the SampleRateConverter has an input. Otherwise, 0 will be returned.
			  * 
			  * @return the number of channels of audio that this SampleRateConverter is producing.
			  */
			virtual Size getNumberOfChannels() const;
			
			
			
			
			/// Return the number of outputs that this SampleRateConverter has.
			/**
			  * This will be equal to the number of outputs that the input audio object has
			  * if the SampleRateConverter has an input. Otherwise, 0 will be returned.
			  * 
			  * @return the number of outputs that this SampleRateConverter is producing.
			  */
			virtual Size getNumberOfOutputs() const;
			
			
			
			
			/// Return whether or not this SampleRateConverter has any more audio to produce.
			/**
			  * If there are still samples to be produced, TRUE will be returned. Otherwise,
			  * FALSE will be returned.
			  * 
			  * @return whether or not this SampleRateConverter has any more audio to produce.
			  */
			virtual Bool hasOutputRemaining() const;
			
			
			
			
	protected:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Main Rendering Method
			
			
			
			
			/// Place the sample rate converted output audio into the specified SoundStream object.
			virtual Size fillBuffer( SoundStream& stream, Index startIndex, Size numSamples );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// An intermediate buffer of audio data used when getting input audio.
			SoundStream inputStream;
			
			
			
			
			/// A pointer to an object which is providing input audio for this SampleRateConverter.
			SoundOutput* input;
			
			
			
			
			/// The output sample rate of this SampleRateConverter.
			Float sampleRate;
			
			
			
			
			/// The position (from 0 to 1) of sample interpolation between the two most recent input samples.
			Float subSampleOffset;
			
			
			
			
			/// A mutex object used to synchronize class parameter changes and audio rendering.
			mutable Mutex renderMutex;
			
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SAMPLE_RATE_CONVERTER_H
