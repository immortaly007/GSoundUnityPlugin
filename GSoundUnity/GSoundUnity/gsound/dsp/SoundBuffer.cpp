/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SoundBuffer.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SoundBuffer class implementation
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


#include "SoundBuffer.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundBuffer:: SoundBuffer( const SoundBuffer& buffer )
	:	size( buffer.size ),
		numChannels( buffer.numChannels )
{
	if ( buffer.channels == NULL )
		channels = NULL;
	else
	{
		channels = util::allocate<Sample*>( buffer.numChannels );
		
		for ( Index i = 0; i < numChannels; i++ )
		{
			if ( buffer.channels[i] != NULL )
			{
#if GSOUND_USE_SIMD
				channels[i] = util::allocateAligned<Sample>( size, SIMDSample::getWidth()*sizeof(Sample) );
#else
				channels[i] = util::allocate<Sample>( size );
#endif
				copy( channels[i], buffer.channels[i], size );
			}
			else
				channels[i] = NULL;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundBuffer& SoundBuffer:: operator = ( const SoundBuffer& buffer )
{
	if ( this != &buffer )
	{
		releaseChannels();
		
		numChannels = buffer.numChannels;
		size = buffer.size;
		
		if ( buffer.channels == NULL )
			channels = NULL;
		else
		{
			channels = util::allocate<Sample*>( buffer.numChannels );
			
			for ( Index i = 0; i < numChannels; i++ )
			{
				if ( buffer.channels[i] != NULL )
				{
#if GSOUND_USE_SIMD
					channels[i] = util::allocateAligned<Sample>( size, SIMDSample::getWidth()*sizeof(Sample) );
#else
					channels[i] = util::allocate<Sample>( size );
#endif
					copy( channels[i], buffer.channels[i], size );
				}
				else
					channels[i] = NULL;
			}
		}
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Size Modification Method
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: setSize( Size newSize )
{
	if ( newSize > size && channels != NULL )
	{
		for ( Index i = 0; i < numChannels; i++ )
		{
#if GSOUND_USE_SIMD
			Sample* newChannel = util::allocateAligned<Sample>( newSize, SIMDSample::getWidth()*sizeof(Sample) );
#else
			Sample* newChannel = util::allocate<Sample>( newSize );
#endif
			
			if ( channels[i] != NULL )
			{
				copy( newChannel, channels[i], size );
				util::deallocate( channels[i] );
			}
			
			channels[i] = newChannel;
		}
	}
	
	size = newSize;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Zero Method
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: zero( Index startIndex, Size numSamples )
{
	if ( startIndex >= size || channels == NULL )
		return;
	
	Size endIndex = math::min( startIndex + numSamples, size );
	
	for ( Index i = 0; i < numChannels; i++ )
	{
		Sample* const channel = channels[i];
		
		if ( channel != NULL )
		{
			Sample* destination = channel + startIndex;
			const Sample* const destinationEnd = channel + endIndex;
			
			while ( destination != destinationEnd )
			{
				*destination = Sample(0);
				destination++;
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Initialization Method
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: initializeChannels( Size newNumChannels, Size newSize )
{
	// Set properties of the buffer and allocate the channel array.
	numChannels = newNumChannels;
	size = newSize;
	
	if ( newNumChannels == 0 )
		channels = NULL;
	else
	{
		channels = util::allocate<Sample*>( newNumChannels );
		
		// Prepare to iterate over the channels.
		Sample** channel = channels;
		const Sample* const * const channelsEnd = channels + numChannels;
		
		if ( size == Size(0) )
		{
			// Set all channel pointers to be NULL.
			while ( channel != channelsEnd )
			{
				*channel = NULL;
				channel++;
			}
		}
		else
		{
			// Allocate a buffer of samples for each channel.
			while ( channel != channelsEnd )
			{
#if GSOUND_USE_SIMD
				*channel = util::allocateAligned<Sample>( size, SIMDSample::getWidth()*sizeof(Sample) );
#else
				*channel = util::allocate<Sample>( size );
#endif
				channel++;
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Data Release Method
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: releaseChannels()
{
	if ( channels != NULL )
	{
		Sample** channel = channels;
		const Sample* const * const channelsEnd = channels + numChannels;
		
		while ( channel != channelsEnd )
		{
			if ( *channel != NULL )
#if GSOUND_USE_SIMD
				util::deallocateAligned( *channel );
#else
				util::deallocate( *channel );
#endif
			
			channel++;
		}
		
		util::deallocate( channels );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Data Copy Method
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: copy( Sample* destination, const Sample* source, Size number )
{
	const Sample* const destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		*destination = *source;
		destination++;
		source++;
	}
}




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
