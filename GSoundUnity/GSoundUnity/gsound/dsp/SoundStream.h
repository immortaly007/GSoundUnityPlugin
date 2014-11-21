/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundStream.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundStream class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_STREAM_H
#define INCLUDE_GSOUND_SOUND_STREAM_H


#include "GSoundDSPConfig.h"


#include "SoundBuffer.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which represents a collection of SoundBuffer objects.
/**
  * All of a SoundStream's SoundBuffer objects are automatically managed so that
  * they are all the same length and have the same number of channels. This class
  * is provided in order to allow the transfer of multiple versions of the same audio
  * data in one block. For instance, a stream with more than one buffer can be
  * used to hold the output of a crossover filter which splits incoming audio into
  * more than one frequency band. Each buffer in the SoundStream would then represent
  * a particular frequency band of the crossover's output.
  */
class SoundStream
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a SoundStream object with 0 buffers of 0 channels with 0 length.
			/**
			  * No memory is allocated by calling this constructor, leaving the user to
			  * set the size/number of buffers/number of channels of the SoundStream after
			  * is created.
			  */
			GSOUND_INLINE SoundStream()
				:	numBuffers( 0 ),
					numChannels( 0 ),
					size( 0 ),
					buffers( NULL )
			{
			}
			
			
			
			
			/// Create a SoundStream object with the specified number of buffers and channels per buffer.
			/**
			  * This constructor creates a SoundStream with a length of zero and the specified
			  * number of buffers and channels per buffer. Use this constructor when the number of buffers
			  * and channels per buffer are known at object creation time, but the length of the stream
			  * is not known. The size can later be efficiently set using the setSize() method.
			  * 
			  * @param newNumBuffers - the number of buffers that the created SoundStream should have.
			  * @param channelsPerBuffer - the number of channels that each SoundBuffer of this SoundStream should have.
			  */
			GSOUND_INLINE SoundStream( Size newNumBuffers, Size channelsPerBuffer )
				:	numBuffers( newNumBuffers ),
					numChannels( channelsPerBuffer ),
					size( 0 )
			{
				if ( newNumBuffers != 0 )
					buffers = util::constructArray<SoundBuffer>( newNumBuffers, channelsPerBuffer );
				else
					buffers = NULL;
			}
			
			
			
			
			/// Create a SoundStream object with the specified number of buffers, channels per buffer, and size.
			/**
			  * This constructor creates a SoundStream with the specified number of buffers,
			  * channels per buffer, and buffer size.
			  * 
			  * @param newNumBuffers - the number of buffers that the created SoundStream should have.
			  * @param channelsPerBuffer - the number of channels that each SoundBuffer of this SoundStream should have.
			  * @param bufferSize - the size that each buffer in this SoundStream should have.
			  */
			GSOUND_INLINE SoundStream( Size newNumBuffers, Size channelsPerBuffer, Size bufferSize )
				:	numBuffers( newNumBuffers ),
					numChannels( channelsPerBuffer ),
					size( bufferSize )
			{
				if ( newNumBuffers != 0 )
					buffers = util::constructArray<SoundBuffer>( newNumBuffers, channelsPerBuffer, bufferSize );
				else
					buffers = NULL;
			}
			
			
			
			
			/// Create an exact copy of the specified SoundStream object.
			/**
			  * The contents of the other SoundStream are copied to newly allocated
			  * buffers in this SoundStream.
			  * 
			  * @param other - the SoundStream object to be copied.
			  */
			GSOUND_INLINE SoundStream( const SoundStream& other )
				:	numBuffers( other.numBuffers ),
					numChannels( other.numChannels ),
					size( other.size )
			{
				if ( other.buffers != NULL )
				{
					buffers = util::allocate<SoundBuffer>( other.numBuffers );
					
					for ( Index i = 0; i < numBuffers; i++ )
						new (buffers + i) SoundBuffer( other.buffers[i] );
				}
				else
					buffers = NULL;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy all internal state and deallocate all memory allocated by this SoundStream object.
			GSOUND_INLINE ~SoundStream()
			{
				if ( buffers != NULL )
					util::destructArray( buffers, numBuffers );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the contents of the specified SoundStream object to this object.
			/**
			  * All previously present internal state of this SoundStream is deallocated and
			  * replaced with a newly allocated copy of the specified SoundStream object's
			  * internal state.
			  * 
			  * @param other - the SoundStream object to be copied.
			  * @return a reference to this SoundStream object in order to allow assignment chaining.
			  */
			SoundStream& operator = ( const SoundStream& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Buffer Accessor Methods
			
			
			
			
			/// Get the SoundBuffer within this SoundStream at the specified index.
			/**
			  * If the specified buffer index is greater than or equal to the number
			  * of buffers in this SoundStream, an assertion is raised. Otherwise,
			  * a reference to the SoundBuffer at that index is returned.
			  * 
			  * @param bufferIndex - the index of the buffer in this SoundStream to access.
			  * @return a reference to the SoundBuffer at the specified index.
			  */
			GSOUND_INLINE SoundBuffer& getBuffer( Index bufferIndex )
			{
				GSOUND_DEBUG_ASSERT( bufferIndex < numBuffers );
				
				return buffers[bufferIndex];
			}
			
			
			
			
			/// Get the SoundBuffer within this SoundStream at the specified index.
			/**
			  * If the specified buffer index is greater than or equal to the number
			  * of buffers in this SoundStream, an assertion is raised. Otherwise,
			  * a const reference to the SoundBuffer at that index is returned.
			  * 
			  * @param bufferIndex - the index of the buffer in this SoundStream to access.
			  * @return a const reference to the SoundBuffer at the specified index.
			  */
			GSOUND_INLINE const SoundBuffer& getBuffer( Index bufferIndex ) const
			{
				GSOUND_DEBUG_ASSERT( bufferIndex < numBuffers );
				
				return buffers[bufferIndex];
			}
			
			
			
			
			/// Get the number of internal buffers that this SoundStream has.
			GSOUND_INLINE Size getNumberOfBuffers() const
			{
				return numBuffers;
			}
			
			
			
			
			/// Set the number of internal buffers that this SoundStream has.
			/**
			  * This method reallocates the internal array of SoundBuffer objects that this
			  * SoundStream has and allocates a new array of the specified size. The method
			  * copies as many of the buffers as it can from the old buffer array into the
			  * new buffer array.
			  * 
			  * @param newNumberOfBuffers - the number of buffers that this SoundStream should have.
			  */
			void setNumberOfBuffers( Size newNumberOfBuffers );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Channel Count Accessor Methods
			
			
			
			
			/// Get the number of channels that the buffers in this SoundStream have.
			GSOUND_INLINE Size getNumberOfChannels() const
			{
				return numChannels;
			}
			
			
			
			
			/// Set the number of channels that the buffers in this SoundStream should have.
			/**
			  * If the number of channels changes, channels are either removed or added
			  * to each buffer in order to have the specified number of channels.
			  * 
			  * @param newNumberOfChannels - the number of channels that the buffers should have.
			  */
			GSOUND_INLINE void setNumberOfChannels( Size newNumberOfChannels )
			{
				if ( buffers != NULL )
				{
					for ( Index i = 0; i < numBuffers; i++ )
						buffers[i].setNumberOfChannels( newNumberOfChannels );
				}
				
				numChannels = newNumberOfChannels;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			
			
			/// Get the size of the SoundBuffers in this SoundStream.
			/**
			  * This value indicates the number of samples that each channel
			  * of each buffer can hold.
			  * 
			  * @return the size of every SoundBuffer in this SoundStream.
			  */
			GSOUND_INLINE Size getSize() const
			{
				return size;
			}
			
			
			
			
			/// Set the size that every SoundBuffer in this SoundStream should have.
			/**
			  * This method reallocates the memory for each buffer so that it can
			  * hold the specified number of samples per channel.
			  * 
			  * @param newSize - the new size that each SoundBuffer should have.
			  */
			GSOUND_INLINE void setSize( Size newSize )
			{
				if ( buffers != NULL )
				{
					for ( Index i = 0; i < numBuffers; i++ )
						buffers[i].setSize( newSize );
				}
				
				size = newSize;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Buffer Zero Methods
			
			
			
			
			/// Zero the entire contents of every SoundBuffer in this SoundStream.
			GSOUND_INLINE void zero()
			{
				if ( buffers != NULL )
				{
					SoundBuffer* buffer = buffers;
					const SoundBuffer* const buffersEnd = buffers + numBuffers;
					
					while ( buffer != buffersEnd )
					{
						buffer->zero();
						buffer++;
					}
				}
			}
			
			
			
			
			/// Zero the specified number of samples, starting at the specified index, of this SoundStream.
			/**
			  * Every SoundBuffer in this SoundStream is zero-d, starting at the specified sample
			  * index and proceeding for the specified number of samples in the buffer. If the
			  * start index or number of samples causes the zero operation to overrun the end of
			  * a buffer, the invalid portion of the zero operation is ignored.
			  * 
			  * @param startIndex - the sample index within the stream to start writing zeros
			  * @param numSamples - the number of samples to write zeros to, starting at the start index.
			  */
			GSOUND_INLINE void zero( Index startIndex, Size numSamples )
			{
				if ( buffers != NULL )
				{
					SoundBuffer* buffer = buffers;
					const SoundBuffer* const buffersEnd = buffers + numBuffers;
					
					while ( buffer != buffersEnd )
					{
						buffer->zero( startIndex, numSamples );
						buffer++;
					}
				}
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A variable-length array of SoundBuffer objects that represent the buffers in this SoundStream.
			SoundBuffer* buffers;
			
			
			
			
			/// The number of buffers in this SoundStream.
			Size numBuffers;
			
			
			
			
			/// The number of channels in each buffer of this SoundStream.
			Size numChannels;
			
			
			
			
			/// The number of samples that each buffer channel in this SoundStream can hold.
			Size size;
			
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_STREAM_H
