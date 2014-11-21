/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/WaveDecoder.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::WaveDecoder class declaration
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


#ifndef INCLUDE_GSOUND_WAVE_DECODER_H
#define INCLUDE_GSOUND_WAVE_DECODER_H


#include "GSoundDSPConfig.h"


#include "SeekableSoundOutput.h"
#include <cstdio>


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which streams and decodes a WAVE audio file from mass storage.
class WaveDecoder : public SeekableSoundOutput
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a WaveDecoder object which doesn't have a WAVE file to decode.
			WaveDecoder();
			
			
			
			
			/// Create a WaveDecoder which decodes the WAVE file with the specified path.
			WaveDecoder( const String& fileName );
			
			
			
			
			/// Create a copy of the specified WaveDecoder object.
			WaveDecoder( const WaveDecoder& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Release all resources associated with this WaveDecoder object.
			~WaveDecoder();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the current state of another WaveDecoder object to this WaveDecoder object.
			WaveDecoder& operator = ( const WaveDecoder& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	File Name Accessor Methods
			
			
			
			
			/// Get a string which represents the path to the WAVE file being decoded.
			const String& getFileName() const;
			
			
			
			
			/// Set the path to the WAVE file being decoded.
			/**
			  * Changing the file being decoded resets the current read location to the
			  * beginning of the file.
			  * 
			  * @param newFileName - the WAVE file to be decoded.
			  */
			void setFileName( const String& newFileName );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	WAVE File Length Accessor Methods
			
			
			
			
			/// Get the length in samples of the WAVE file which is being decoded.
			virtual SoundSize getLengthInSamples() const;
			
			
			
			
			/// Get the length in seconds of the WAVE file which is being decoded.
			virtual Double getLengthInSeconds() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Current Time Accessor Methods
			
			
			
			
			/// Get the index of the sample currently being read from the WAVE file.
			virtual SampleIndex getCurrentSampleIndex() const;
			
			
			
			
			/// Get the time within the WAVE file of the current read position of this decoder.
			virtual Double getCurrentTime() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Wave Decoder Seek Methods
			
			
			
			
			/// Seek a signed number of seconds relative to the current read position in the WAVE file.
			/**
			  * If the specified time offset added to the current time is outside of
			  * the bounds of the WAVE file, FALSE is returned and the current read position is not
			  * changed. Otherwise, the current read position is shifted by the amount specified by
			  * the time offset and TRUE is returned.
			  * 
			  * @param timeOffset - the signed time in seconds to moved the current read position.
			  * @return whether or not the seek operation was successful.
			  */
			virtual Bool seek( Double timeOffset );
			
			
			
			
			/// Seek a signed number of samples relative to the current read position in the WAVE file.
			/**
			  * If the specified sample offset added to the current sample index is outside of
			  * the bounds of the WAVE file, FALSE is returned and the current read position is not
			  * changed. Otherwise, the current read position is shifted by the amount specified by
			  * the sample offset and TRUE is returned.
			  * 
			  * @param sampleOffset - the signed distance in samples to moved the current read position.
			  * @return whether or not the seek operation was successful.
			  */
			virtual Bool seek( Int64 sampleOffset );
			
			
			
			
			/// Seek to the specified absolute time in seconds within the WAVE file.
			/**
			  * If the specified location is outside of the length of the WAVE file being
			  * decoded, FALSE is returned and the current read position is not changed.
			  * Otherwise, the current read position is changed and TRUE is returned.
			  * 
			  * @param time - the time in seconds to seek to within the WAVE file.
			  * @return whether or not the seek operation was successful.
			  */
			virtual Bool seekTo( Double time );
			
			
			
			
			/// Seek to the specified absolute sample index within the WAVE file.
			/**
			  * If the specified location is outside of the length of the WAVE file being
			  * decoded, FALSE is returned and the current read position is not changed.
			  * Otherwise, the current read position is changed and TRUE is returned.
			  * 
			  * @param sampleIndex - the sample index to seek to within the WAVE file.
			  * @return whether or not the seek operation was successful.
			  */
			virtual Bool seekTo( SampleIndex sampleIndex );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Format Accessor Methods
			
			
			
			
			/// Get the sample rate of the audio being produced by this WaveDecoder.
			/**
			  * This is the sample rate of the WAVE file being decoded.
			  * 
			  * @return the sample rate of the WAVE file being decoded.
			  */
			virtual Float getSampleRate() const;
			
			
			
			
			/// Get the current number of channels that this WaveDecoder produces.
			/**
			  * This is equal to the number of channels that the WAVE file being decoded has.
			  * 
			  * @return the number of channels in the decoded WAVE file.
			  */
			virtual Size getNumberOfChannels() const;
			
			
			
			
			/// Get the number of outputs that this Wave Decoder has.
			/**
			  * Since WAVE files have a single-stream format, this method always returns 1.
			  * 
			  * @return the number of outputs that this WaveDecoder has, always 1.
			  */
			virtual Size getNumberOfOutputs() const;
			
			
			
			
			/// Return whether or not a WaveDecoder has any sound remaining to produce.
			/**
			  * If this method returns TRUE, there is still sound remaining to be read from
			  * the WAVE file being decoded. If this method returns FALSE, the end of the
			  * WAVE file has been reached and no sound is remaining to be produced unless
			  * the current read position within the file is changed.
			  */
			virtual Bool hasOutputRemaining() const;
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Main Sound Output Method
			
			
			
			
			/// Put the specified number of samples from the WAVE file being decoded into the output stream.
			virtual Size fillBuffer( SoundStream& stream, Index startIndex, Size numSamples );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private File Open/Close Methods
			
			
			
			
			void openWaveFile();
			
			
			
			
			void closeWaveFile();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The path to the WAVE file being decoded.
			String fileName;
			
			
			
			
			/// The number of channels in the WAVE file.
			Size numChannels;
			
			
			
			
			/// The sample rate of the WAVE file.
			Float sampleRate;
			
			
			
			
			/// The number of bytes per sample of the WAVE file.
			Size bytesPerSample;
			
			
			
			
			/// The length in samples of the WAVE file.
			SoundSize lengthInSamples;
			
			
			
			
			/// The length in seconds of the WAVE file.
			Double lengthInSeconds;
			
			
			
			
			/// The index within the WAVE file of the current sample being read.
			SampleIndex currentSampleIndex;
			
			
			
			
			/// A handle to the WAVE file being decoded.
			std::FILE* file;
			
			
			
			
			/// A buffer used to hold data read from the WAVE file.
			UByte* inputBuffer;
			
			
			
			
			/// The length of the buffer used to hold data read from the WAVE file.
			Size inputBufferSize;
			
			
			
			
			/// A mutex which indicates that data is being currently decoded from the WAVE file.
			mutable Mutex decodingMutex;
			
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_WAVE_DECODER_H
