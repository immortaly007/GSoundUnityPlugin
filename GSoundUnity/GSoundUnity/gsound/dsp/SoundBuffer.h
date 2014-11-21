/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundBuffer.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundBuffer class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_BUFFER_H
#define INCLUDE_GSOUND_SOUND_BUFFER_H


#include "GSoundDSPConfig.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class representing a buffer of multichannel audio data.
/**
  * A SoundBuffer class contains zero or more non-interleaved channels of audio data.
  * All channels are the same length. The class will not allocate any memory for audio
  * data unless the number of channels and length of the buffer are non-zero, saving
  * unnecessary allocations.
  */
class SoundBuffer
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a SoundBuffer object with zero channels of zero samples each.
			/**
			  * No memory is allocated until the number of channels is set to a non-zero value
			  * using the setNumberOfChannels() method.
			  */
			GSOUND_INLINE SoundBuffer()
			{
				initializeChannels( 0, 0 );
			}
			
			
			
			
			/// Create a SoundBuffer object with the specified number of channels.
			/**
			  * If the number of channels is non-zero, an internal array of pointers
			  * to channel buffers is allocated but no channel data is allocated
			  * until a buffer size is specified using the setSize() method. If the number
			  * of channels is zero, no memory is allocated until the number of channels
			  * is set to a non-zero value using the setNumberOfChannels() method.
			  * 
			  * @param newNumChannels - the number of channels that this sound buffers should have.
			  */
			GSOUND_INLINE SoundBuffer( Size newNumChannels )
			{
				initializeChannels( newNumChannels, 0 );
			}
			
			
			
			
			/// Create a SoundBuffer object with the specified number of channels and buffer size.
			/**
			  * If the number of channels is non-zero, an internal array of pointers
			  * to channel buffers is allocated. Otherwise, if the number of channels is 0,
			  * no allocations are performed until a non-zero number of channels is specified
			  * using the setNumberOfChannels() method.
			  * 
			  * Similarly, if the buffer size is zero, no channel buffers are allocated until
			  * a non-zero buffer size is specified using the setSize() method.
			  * 
			  * @param newNumChannels - the number of channels that this sound buffers should have.
			  * @param bufferSize - the length in samples that this buffer should have.
			  */
			GSOUND_INLINE SoundBuffer( Size newNumChannels, Size bufferSize )
			{
				initializeChannels( newNumChannels, bufferSize );
			}
			
			
			
			
			/// Create an exact copy of the specified SoundBuffer.
			/**
			  * @param other - the SoundBuffer object whose state is to be copied.
			  */
			SoundBuffer( const SoundBuffer& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy a SoundBuffer object.
			/**
			  * All internally allocated data is released.
			  */
			GSOUND_INLINE ~SoundBuffer()
			{
				releaseChannels();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Copy the contents of one SoundBuffer object into this SoundBuffer object.
			/**
			  * All previously existing internal state is released and new state
			  * allocated to exactly replicate the specified SoundBuffer object.
			  * 
			  * @param other - the SoundBuffer object whose state should replace this SoundBuffer's.
			  * @return a reference to this SoundBuffer object, allowing assignment chaining.
			  */
			SoundBuffer& operator = ( const SoundBuffer& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Start/End Accessor Methods
			
			
			
			
			/// Get a pointer to the first sample within the channel with the specified index.
			/**
			  * If the specified channel index is not within the valid bounds of channel
			  * indices, an assertion is raised.
			  * 
			  * @param channelIndex - the channel whose start pointer should be accessed.
			  * @return a pointer to the first sample of the specified channel within this SoundBuffer.
			  */
			GSOUND_INLINE Sample* getChannelStart( Index channelIndex )
			{
				GSOUND_DEBUG_ASSERT( channelIndex < numChannels );
				
				return channels[channelIndex];
			}
			
			
			
			
			/// Get a pointer to the first sample within the channel with the specified index.
			/**
			  * If the specified channel index is not within the valid bounds of channel
			  * indices, an assertion is raised.
			  * 
			  * @param channelIndex - the channel whose start pointer should be accessed.
			  * @return a pointer to the first sample of the specified channel within this SoundBuffer.
			  */
			GSOUND_INLINE const Sample* getChannelStart( Index channelIndex ) const
			{
				GSOUND_DEBUG_ASSERT( channelIndex < numChannels );
				
				return channels[channelIndex];
			}
			
			
			
			
			/// Get a pointer to just past the last sample within the channel with the specified index.
			/**
			  * If the specified channel index is not within the valid bounds of channel
			  * indices, an assertion is raised.
			  * 
			  * @param channelIndex - the channel whose end pointer should be accessed.
			  * @return a pointer to the first sample after the end of the specified channel.
			  */
			GSOUND_INLINE Sample* getChannelEnd( Index channelIndex )
			{
				GSOUND_DEBUG_ASSERT( channelIndex < numChannels );
				
				return channels[channelIndex] + size;
			}
			
			
			
			
			/// Get a pointer to just past the last sample within the channel with the specified index.
			/**
			  * If the specified channel index is not within the valid bounds of channel
			  * indices, an assertion is raised.
			  * 
			  * @param channelIndex - the channel whose end pointer should be accessed.
			  * @return a pointer to the first sample after the end of the specified channel.
			  */
			GSOUND_INLINE const Sample* getChannelEnd( Index channelIndex ) const
			{
				GSOUND_DEBUG_ASSERT( channelIndex < numChannels );
				
				return channels[channelIndex] + size;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			
			
			/// Get the number of samples that this SoundBuffer object can hold.
			/**
			  * @return the number of samples that this SoundBuffer object can hold.
			  */
			GSOUND_INLINE Size getSize() const
			{
				return size;
			}
			
			
			
			
			/// Set the number of samples that this SoundBuffer object can hold.
			/**
			  * This method reallocates the internal channel buffers if necessary
			  * to hold the specified number of samples. If the specified size is less
			  * than the current size, no memory may be reallocated if not necessary.
			  * 
			  * @param newSize - the new number of samples that this SoundBuffer should hold.
			  */
			void setSize( Size newSize );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Channel Count Accessor Methods
			
			
			
			
			/// Get the number of channels that this SoundBuffer object has.
			/**
			  * @return the number of channels that this sound buffer has.
			  */
			GSOUND_INLINE Size getNumberOfChannels() const
			{
				return numChannels;
			}
			
			
			
			
			/// Set the number of channels that this SoundBuffer object has.
			/**
			  * Changing the number of channels entails reallocating the internal
			  * array of channels and is a potentially expensive operation.
			  * 
			  * @param newNumberOfChannels - the new number of channels that this SoundBuffer object should have.
			  */
			GSOUND_INLINE void setNumberOfChannels( Size newNumberOfChannels )
			{
				Size currentSize = this->getSize();
				
				releaseChannels();
				initializeChannels( newNumberOfChannels, currentSize );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Buffer Zero Methods
			
			
			
			
			/// Zero-out the entire contents of this SoundBuffer.
			GSOUND_INLINE void zero()
			{
				this->zero( 0, size );
			}
			
			
			
			
			/// Zero-out a range of the contents of this SoundBuffer.
			/**
			  * This method writes zero to every sample of every channel in this SoundBuffer
			  * starting at the specified sample start index and ending numSamples past
			  * that index.
			  * 
			  * @param startIndex - the first sample to be zero-d out in the SoundBuffer object.
			  * @param numSamples - the number of samples to be zero-d out after the start index.
			  */
			void zero( Index startIndex, Size numSamples );
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Methods
			
			
			
			
			/// Initialize the specified number of channels of the SoundBuffer to have the given size.
			void initializeChannels( Size newNumChannels, Size newSize );
			
			
			
			
			/// Release the data allocated to all channels in this SoundBuffer object.
			void releaseChannels();
			
			
			
			
			/// Copy the specified number of samples from the source pointer to the destination pointer.
			GSOUND_INLINE static void copy( Sample* destination, const Sample* source, Size number );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A variable-length array of pointers to channel sample buffers.
			Sample** channels;
			
			
			
			
			/// The number of channels in this SoundBuffer.
			Size numChannels;
			
			
			
			
			/// The number of samples in each channel of this SoundBuffer.
			Size size;
			
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_BUFFER_H
