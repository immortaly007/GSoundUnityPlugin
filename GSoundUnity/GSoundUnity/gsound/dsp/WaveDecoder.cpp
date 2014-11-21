/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/WaveDecoder.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::WaveDecoder class implementation
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


#include "WaveDecoder.h"


#include "SampleMath.h"


#define WAVE_HEADER_SIZE 44


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




WaveDecoder:: WaveDecoder()
	:	fileName(),
		numChannels( Size(0) ),
		sampleRate( Float(0) ),
		bytesPerSample( Size(0) ),
		lengthInSamples( SoundSize(0) ),
		lengthInSeconds( Double(0) ),
		currentSampleIndex( SampleIndex(0) ),
		file( NULL ),
		inputBuffer( NULL ),
		inputBufferSize( 0 )
{
}




WaveDecoder:: WaveDecoder( const String& newFileName )
	:	fileName( newFileName ),
		numChannels( Size(0) ),
		sampleRate( Float(0) ),
		bytesPerSample( Size(0) ),
		lengthInSamples( SoundSize(0) ),
		lengthInSeconds( Double(0) ),
		currentSampleIndex( SampleIndex(0) ),
		file( NULL ),
		inputBuffer( NULL ),
		inputBufferSize( 0 )
{
	// Open the file.
	openWaveFile();
}




WaveDecoder:: WaveDecoder( const WaveDecoder& other )
	:	fileName( other.fileName ),
		numChannels( other.numChannels ),
		sampleRate( other.sampleRate ),
		bytesPerSample( other.bytesPerSample ),
		lengthInSamples( other.lengthInSamples ),
		lengthInSeconds( other.lengthInSeconds ),
		currentSampleIndex( other.currentSampleIndex ),
		file( NULL ),
		inputBuffer( NULL ),
		inputBufferSize( 0 )
{
	// Open the file.
	openWaveFile();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




WaveDecoder:: ~WaveDecoder()
{
	if ( file != NULL )
	{
		std::fclose( file );
		file = NULL;
	}
	
	if ( inputBuffer != NULL )
		util::deallocate( inputBuffer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




WaveDecoder& WaveDecoder:: operator = ( const WaveDecoder& other )
{
	if ( this != &other )
	{
		// Acquire a mutex which indicates that data is being currently decoded from the WAVE file.
		decodingMutex.acquire();
		
		// Close the currently open WAVE file.
		closeWaveFile();
		
		// Get the path to the other decoder's wave file.
		fileName = other.fileName;
		
		// Open the new WAVE file.
		openWaveFile();
		
		// Release a mutex which indicates that data is being currently decoded from the WAVE file.
		decodingMutex.release();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		File Name Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




const String& WaveDecoder:: getFileName() const
{
	return fileName;
}




void WaveDecoder:: setFileName( const String& newFileName )
{
	// Acquire a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.acquire();
	
	// Close the current WAVE file.
	closeWaveFile();
	
	// Set the file name of the WAVE file that we should read.
	fileName = newFileName;
	
	// Open the new WAVE file.
	openWaveFile();
	
	// Release a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.release();
}




//##########################################################################################
//##########################################################################################
//############		
//############		WAVE File Length Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundSize WaveDecoder:: getLengthInSamples() const
{
	return lengthInSamples;
}




Double WaveDecoder:: getLengthInSeconds() const
{
	return lengthInSeconds;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Current Time Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SampleIndex WaveDecoder:: getCurrentSampleIndex() const
{
	// Acquire a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.acquire();
	
	SampleIndex localSampleIndex = currentSampleIndex;
	
	// Release a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.release();
	
	return localSampleIndex;
}




Double WaveDecoder:: getCurrentTime() const
{
	// Acquire a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.acquire();
	
	SampleIndex localSampleIndex = currentSampleIndex;
	
	// Release a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.release();
	
	return Double(localSampleIndex) / Double(sampleRate);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Wave Decoder Seek Methods
//############		
//##########################################################################################
//##########################################################################################




Bool WaveDecoder:: seek( Double timeOffset )
{
	// Make sure that the offset won't seek past the beginning of the file.
	if ( getCurrentTime() + timeOffset < 0.0 )
		return false;
	else
	{
		// Acquire a mutex which indicates that data is being currently decoded from the WAVE file.
		decodingMutex.acquire();
		
		// Compute the desired sample index.
		SampleIndex index = (SampleIndex)((Double)currentSampleIndex + (timeOffset * (Double)sampleRate));
		
		// if the sample index is past the end of the file, return.
		if ( index > lengthInSamples )
		{
			// Release a mutex which indicates that data is being currently decoded from the WAVE file.
			decodingMutex.release();
			
			return false;
		}
		
		currentSampleIndex = index;
		
		// Seek to the desired position within the file.
		std::fseek( file, long(WAVE_HEADER_SIZE + currentSampleIndex*numChannels*bytesPerSample), SEEK_SET );
		
		// Release a mutex which indicates that data is being currently decoded from the WAVE file.
		decodingMutex.release();
		
		return true;
	}
}




Bool WaveDecoder:: seek( Int64 sampleOffset )
{
	// Acquire a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.acquire();
	
	// Make sure that the offset won't seek past the beginning of the file.
	if ( sampleOffset < 0 && (Int64)currentSampleIndex + sampleOffset < 0 )
	{
		// Release a mutex which indicates that data is being currently decoded from the WAVE file.
		decodingMutex.release();
		
		return false;
	}
	else
	{
		// Compute the desired sample index.
		SampleIndex index = (SampleIndex)(currentSampleIndex + sampleOffset);
		
		// if the sample index is past the end of the file, return.
		if ( index > lengthInSamples )
		{
			// Release a mutex which indicates that data is being currently decoded from the WAVE file.
			decodingMutex.release();
			
			return false;
		}
		
		currentSampleIndex = index;
		
		// Seek to the desired position within the file.
		std::fseek( file, long(WAVE_HEADER_SIZE + currentSampleIndex*numChannels*bytesPerSample), SEEK_SET );
		
		// Release a mutex which indicates that data is being currently decoded from the WAVE file.
		decodingMutex.release();
		
		return true;
	}
}




Bool WaveDecoder:: seekTo( Double time )
{
	// Don't seek if the desired time isn't within the bounds of the file.
	if ( time < Double(0) || time > lengthInSeconds )
		return false;
	
	// Acquire a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.acquire();
	
	// Compute the desired sample index.
	currentSampleIndex = (SampleIndex)(time*(Double)sampleRate);
	
	// Seek to the desired position within the file.
	std::fseek( file, long(WAVE_HEADER_SIZE + currentSampleIndex*numChannels*bytesPerSample), SEEK_SET );
	
	// Release a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.release();
	
	return true;
}




Bool WaveDecoder:: seekTo( SampleIndex sampleIndex )
{
	// Don't seek if the desired time isn't within the bounds of the file.
	if ( sampleIndex > lengthInSamples )
		return false;
	
	// Acquire a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.acquire();
	
	// Compute the desired sample index.
	currentSampleIndex = sampleIndex;
	
	// Seek to the desired position within the file.
	std::fseek( file, long(WAVE_HEADER_SIZE + currentSampleIndex*numChannels*bytesPerSample), SEEK_SET );
	
	// Release a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.release();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################





Float WaveDecoder:: getSampleRate() const
{
	return sampleRate;
}




Size WaveDecoder:: getNumberOfChannels() const
{
	return numChannels;
}




Size WaveDecoder:: getNumberOfOutputs() const
{
	return Size(1);
}




Bool WaveDecoder:: hasOutputRemaining() const
{
	// Acquire a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.acquire();
	
	SampleIndex localSampleIndex = currentSampleIndex;
	
	// Release a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.release();
	
	if ( file == NULL )
		return false;
	else if ( localSampleIndex < lengthInSamples )
		return true;
	else
		return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Sound Output Method
//############		
//##########################################################################################
//##########################################################################################




Size WaveDecoder:: fillBuffer( SoundStream& stream, Index startIndex, Size numSamples )
{
	// Acquire a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.acquire();
	
	// If the file was not able to be opened or the end of the file has been reached,
	// return indicating that no samples were read.
	if ( file == NULL || currentSampleIndex >= lengthInSamples )
	{
		// Release a mutex which indicates that data is being currently decoded from the WAVE file.
		decodingMutex.release();
		
		return 0;
	}
	
	Size numBytesToRead = numChannels*math::min( lengthInSamples - currentSampleIndex, SoundSize(numSamples) )*bytesPerSample;
	
	// If the input buffer has not been allocated yet or is too small, increase its size.
	if ( inputBufferSize < numBytesToRead )
	{
		// Deallocate the old buffer if necessary.
		if ( inputBuffer != NULL )
			util::deallocate( inputBuffer );
		
		// Allocate a new buffer and change the buffer size.
		inputBuffer = util::allocate<UByte>( numBytesToRead );
		inputBufferSize = numBytesToRead;
	}
	
	// Read data into the input buffer.
	Size numBytesRead = std::fread( inputBuffer, sizeof(UByte), numBytesToRead, file );
	
	// Compute the number of output samples based on the number of bytes read.
	Size numOutputSamples = numBytesRead / (bytesPerSample*numChannels);
	
	// Get the buffer in which to place all output data.
	SoundBuffer& outputBuffer = stream.getBuffer(0);
	
	for ( Index i = 0; i < numChannels; i++ )
	{
		Sample* destination = outputBuffer.getChannelStart(i) + startIndex;
		const Sample* const destinationEnd = destination + numOutputSamples;
		const UByte* source = inputBuffer + i*bytesPerSample;
		
		// Compute the distance in bytes between successive samples in the same channel.
		Size stride = bytesPerSample*numChannels;
		
		switch ( bytesPerSample )
		{
			case 1:
				while ( destination != destinationEnd )
				{
					*destination = sample::convert<Sample>((Sample8)((Int16)source[0] - 127));
					
					destination++;
					source += stride;
				}
				break;
				
			case 2:
				while ( destination != destinationEnd )
				{
					Sample16 bla = ((Sample16)source[1] << 8) | (Sample16)source[0];
					
					*destination = sample::convert<Sample>( bla );
					
					destination++;
					source += stride;
				}
				break;
				
			case 3:
				while ( destination != destinationEnd )
				{
					static const Sample max24Bit = Sample(8388607);
					
					Int32 high = static_cast<Int8>(source[2]);
					
					Int32 aSample = ((high << 16) | ((Int32)source[1] << 8) | (Int32)source[0]);
					
					*destination = Sample(aSample) / max24Bit;
					
					destination++;
					source += stride;
				}
				break;
				
			case 4:
				while ( destination != destinationEnd )
				{
					*destination = sample::convert<Sample>(((Int32)source[3] << 24) | ((Int32)source[2] << 16) |
																((Int32)source[1] << 8) | (Int32)source[0]);
					
					destination++;
					source += stride;
				}
				break;
				
			case 8:
				while ( destination != destinationEnd )
				{
					*destination = sample::convert<Sample>(((Int64)source[7] << 56) | ((Int64)source[6] << 48) |
															((Int64)source[5] << 40) | ((Int64)source[4] << 32) |
															((Int64)source[3] << 24) | ((Int64)source[2] << 16) |
															((Int64)source[1] << 8) | (Int64)source[0]);
					
					destination++;
					source += stride;
				}
				break;
				
			default:
				numOutputSamples = 0;
				break;
		}
	}
	
	// Update the current sample index.
	currentSampleIndex += numOutputSamples;
	
	// Release a mutex which indicates that data is being currently decoded from the WAVE file.
	decodingMutex.release();
	
	return numOutputSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Private File Open/Close Methods
//############		
//##########################################################################################
//##########################################################################################




void WaveDecoder:: openWaveFile()
{
	// Attempt to open the file.
	file = std::fopen( fileName.c_str(), "rb" );
	
	// If the file is NULL, it doesn't exist and we can return here.
	if ( file == NULL )
		return;
	
	//*******************************************************************************
	// Read the header of the wave file.
	
	UByte header[WAVE_HEADER_SIZE];
	
	Size numberRead = std::fread( header, sizeof(UByte), WAVE_HEADER_SIZE, file );
	
	// If the file is shorter than the header size for a WAVE file, close the file.
	if ( numberRead != WAVE_HEADER_SIZE )
	{
		closeWaveFile();
		return;
	}
	
	//*******************************************************************************
	// verify that this is an RIFF file.
	
	Bool isRIFF = header[0] == 'R' && header[1] == 'I' && header[2] == 'F' && header[3] == 'F';
	
	// If the file is not a RIFF file, close it.
	if ( !isRIFF )
	{
		closeWaveFile();
		return;
	}
	
	//*******************************************************************************
	// verify that this is a WAVE file.
	
	Bool isWAVE = header[8] == 'W' && header[9] == 'A' && header[10] == 'V' && header[11] == 'E';
	
	// If the file is not a wave file, close it.
	if ( !isWAVE )
	{
		closeWaveFile();
		return;
	}
	
	
	//*******************************************************************************
	// Make sure that it is in PCM format (we can't read anything else).
	
	// Get the format (1 == PCM).
	Int16 format = ((Int16)header[21] << 8) | (Int16)header[20];
	
	// If the file's format is not PCM, close the wave file.
	if ( format != 1 )
	{
		closeWaveFile();
		return;
	}
	
	
	//*******************************************************************************
	// Get information about the audio format of the file.
	
	
	numChannels = ((Int32)header[23] << 8) | (Int32)header[22];
	
	Int32 intSampleRate = ((Int32)header[27] << 24) | ((Int32)header[26] << 16) +
						((Int32)header[25] << 8) | (Int32)header[24];
	
	sampleRate = Float(intSampleRate);
	
	bytesPerSample = (((Int32)header[35] << 8) | (Int32)header[34]) >> 3;
	
	
	//*******************************************************************************
	// Get the length of the file
	
	Size sizeInBytes = ((Int32)header[43] << 24) | ((Int32)header[42] << 16) +
						((Int32)header[41] << 8) | (Int32)header[40];
	
	lengthInSamples = sizeInBytes / (numChannels * bytesPerSample);
	lengthInSeconds = (Double)lengthInSamples / (Double)sampleRate;
	
	//*******************************************************************************
	// Set the current read position to be the beginning of the file.
	
	currentSampleIndex = 0;
}




void WaveDecoder:: closeWaveFile()
{
	// Reset all internal cached data members that the describe the WAVE file.
	numChannels = Size(0);
	sampleRate = Float(0);
	bytesPerSample = Size(0);
	lengthInSamples = SoundSize(0);
	lengthInSeconds = Float(0);
	currentSampleIndex = SampleIndex(0);
	
	// Close the WAVE file if necessary.
	if ( file != NULL )
	{
		std::fclose( file );
		file = NULL;
	}
}




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
