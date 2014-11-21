/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundOutput.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundOutput class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_OUTPUT_H
#define INCLUDE_GSOUND_SOUND_OUTPUT_H


#include "GSoundDSPConfig.h"


#include "SoundStream.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// An interface which is used to abstract an object which produces sound data.
/**
  * A SoundOutput represents a stream of sound data. It provides methods which
  * read data from that stream. It should be noted that data can only be read
  * from the stream once before it is discarded and the current read position
  * is advanced by the number of samples read.
  * 
  * A class which derives from SoundOutput must truthfully provide all data
  * returned from the pure-virtual methods declared here.
  */
class SoundOutput
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy a sound output object.
			virtual ~SoundOutput()
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Accessor Methods
			
			
			
			
			/// Fill the entire specified stream with data from this sound output.
			/**
			  * The SoundOutput will attempt to place as many samples as will fit
			  * in the specified sound stream, starting at the first sample. The
			  * number of samples actually placed in the stream is returned.
			  * 
			  * @param stream - the stream which will be filled with samples.
			  * @return the number of samples placed in the stream.
			  */
			GSOUND_INLINE Size getSamples( SoundStream& stream )
			{
				return this->getSamples( stream, 0, stream.getSize() );
			}
			
			
			
			
			/// Fill the specified stream with the specified number of samples from this SoundOutput.
			/**
			  * The SoundOutput will attempt to place up to numSamples samples in the
			  * specified stream, starting with the first sample in the stream.
			  * The stream object's internal buffers will be automatically 
			  * enlarged if they are not large enough to hold the output format of this
			  * SoundOutput object or the specified number of samples. The number of
			  * samples written to the stream is returned.
			  * 
			  * @param stream - the stream which will be filled with samples.
			  * @param numSamples - the number of samples to attempt to put in the stream.
			  * @return the number of samples placed in the stream.
			  */
			GSOUND_INLINE Size getSamples( SoundStream& stream, Size numSamples )
			{
				return this->getSamples( stream, 0, numSamples );
			}
			
			
			
			
			/// Fill the specified stream with the specified number of samples from this SoundOutput.
			/**
			  * The SoundOutput will attempt to place up to numSamples samples in the
			  * specified stream starting at the specified start index.
			  * The stream object's internal buffers will be automatically 
			  * enlarged if they are not large enough to hold the output format of this
			  * SoundOutput object or the specified number of samples plus the start index + 1.
			  * The number of samples written to the stream is
			  * returned.
			  * 
			  * @param stream - the stream which will be filled with samples.
			  * @param startIndex - the index within the stream to begin writing output samples.
			  * @param numSamples - the number of samples to attempt to put in the stream.
			  * @return the number of samples placed in the stream.
			  */
			Size getSamples( SoundStream& stream, Size startIndex, Size numSamples );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Format Accessor Methods
			
			
			
			
			/// Get the current sample rate of audio being produced by this SoundOutput.
			/**
			  * It is possible that the value returned by this method can change between
			  * successive calls to getSamples(), however it is guaranteed that the sample
			  * rate will not change during a call to getSamples().
			  */
			virtual Float getSampleRate() const = 0;
			
			
			
			
			/// Get the current number of channels that this SoundOutput produces.
			/**
			  * @return the number of channels that this SoundOutput produces.
			  */
			virtual Size getNumberOfChannels() const = 0;
			
			
			
			
			/// Get the number of outputs that this SoundOutput has.
			/**
			  * Each output corresponds to a buffer within an output SoundStream
			  * object with the same index (up to the number of output minus 1).
			  * 
			  * @return the number of outputs that this SoundOutput has.
			  */
			virtual Size getNumberOfOutputs() const = 0;
			
			
			
			
			/// Return whether or not a SoundOutput has any sound remaining to produce.
			/**
			  * If this method returns TRUE, there is still sound remaining to be produced
			  * by this SoundOutput object. Calls to getSamples() may return 0 initially but
			  * will eventually return a non-zero result. Otherwise, if the method
			  * returns FALSE, calls to getSamples() will return 0 and there will not
			  * be any more audio to output unless the state of the SoundOutput is changed
			  * via external means.
			  */
			virtual Bool hasOutputRemaining() const = 0;
			
			
			
			
	protected:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Main Rendering Method
			
			
			
			
			/// Fill the specified sound stream with the specified number of channels starting at the given index.
			/**
			  * This is the main sound output method for which derived classes of the SoundOutput interface
			  * will provide an implementation. Upon calling of this method, the output sound stream
			  * will have been already enlarged so that it is at least large enough to hold data of
			  * of the format produced by this sound output (as reported by the other pure-virtual methods here),
			  * as well as long enough to hold the specified number of samples plus the start index plus
			  * one.
			  * 
			  * Within this function, the implementor must fill the stream with the specified number
			  * of samples, starting at the specified sample start index within the sound stream.
			  * Upon exiting the method, the number of samples that were actually written to the
			  * stream should be returned.
			  * 
			  * @param stream - the stream in which to place output audio data.
			  * @param startIndex - the position in the stream where the first sample should be written.
			  * @param numSamples - the number of samples to write to the stream.
			  * @return the number of samples actually written to the stream.
			  */
			virtual Size fillBuffer( SoundStream& stream, Index startIndex, Size numSamples ) = 0;
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_OUTPUT_H
