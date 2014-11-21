/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundStream.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundStream class implementation
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


#include "SoundStream.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundStream& SoundStream:: operator = ( const SoundStream& stream )
{
	if ( this != &stream )
	{
		if ( numBuffers != stream.numBuffers )
		{
			if ( buffers != NULL )
				util::destructArray( buffers, numBuffers );
			
			if ( stream.buffers != NULL )
				buffers = util::allocate<SoundBuffer>( stream.numBuffers );
			else
				buffers = NULL;
			
			numBuffers = stream.numBuffers;
		}
		
		numChannels = stream.numChannels;
		size = stream.size;
		
		for ( Index i = 0; i < numBuffers; i++ )
			new (buffers + i) SoundBuffer( stream.buffers[i] );
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Count Modification Method
//############		
//##########################################################################################
//##########################################################################################




void SoundStream:: setNumberOfBuffers( Size newNumberOfBuffers )
{
	if ( newNumberOfBuffers == numBuffers )
		return;
	else if ( newNumberOfBuffers < numBuffers )
	{
		// Since the new number is less than the old, just call the
		// destructors of the unused buffers at the end of the buffer array.
		
		SoundBuffer* buffer = buffers + newNumberOfBuffers;
		const SoundBuffer* const buffersEnd = buffers + numBuffers;
		
		while ( buffer != buffersEnd )
		{
			buffer->~SoundBuffer();
			buffer++;
		}
	}
	else if ( numBuffers == 0 )
	{
		// There were no previously allocated buffers, so we don't have to worry about copying anything.
		numBuffers = newNumberOfBuffers;
		
		buffers = util::constructArray<SoundBuffer>( newNumberOfBuffers, numChannels, size );
	}
	else
	{
		// Reallocate the array of buffers, copying the contents of the previous buffers.
		SoundBuffer* oldBuffers = buffers;
		const SoundBuffer* oldBuffer = buffers;
		const SoundBuffer* const oldBuffersEnd = buffers + numBuffers;
		
		buffers = util::allocate<SoundBuffer>( newNumberOfBuffers );
		
		SoundBuffer* buffer = buffers;
		const SoundBuffer* const buffersEnd = buffers + newNumberOfBuffers;
		
		// Copy the old buffers to the new buffer array, calling destructors of the old buffers along the way.
		while ( oldBuffer != oldBuffersEnd )
		{
			new (buffer) SoundBuffer(*oldBuffer);
			oldBuffer->~SoundBuffer();
			
			oldBuffer++;
			buffer++;
		}
		
		// Initialize the rest of the buffers.
		while ( buffer != buffersEnd )
		{
			new (buffer) SoundBuffer( numChannels, size );
			
			buffer++;
		}
		
		// Deallocate the old buffer array.
		util::deallocate( oldBuffers );
	}
	
	numBuffers = newNumberOfBuffers;
}




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
