/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/ChannelIOMap.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::ChannelIOMap class declaration
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


#ifndef INCLUDE_GSOUND_CHANNEL_IO_MAP_H
#define INCLUDE_GSOUND_CHANNEL_IO_MAP_H


#include "GSoundDSPConfig.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which maps a set of input audio channels to a set of output channels.
/**
  * Typically, a ChannelIOMap will be used to specify which channels of an input or 
  * output stream of audio get sent to/from an audio device. This allows device output
  * channels to be easily configured depending on the format of the output audio stream.
  */
class ChannelIOMap
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			/// Create a channel I/O map with no channel mappings.
			GSOUND_INLINE ChannelIOMap()
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Channel Mapping Accessor Methods
			
						
			
			
			/// Set a mapping from the specified input channel to the specified output channel.
			/**
			  * This mapping replaces any previous mappings associated with each of these channels.
			  * 
			  * @param inputChannel - the input channel which is to be sent to the corresponding output channel.
			  * @param outputChannel - the output channel which is provided audio by the corresponding input channel.
			  */
			GSOUND_INLINE void setChannelMapping( Index inputChannel, Index outputChannel )
			{
				inputToOutput.set( Hash(inputChannel), inputChannel, outputChannel );
				outputToInput.set( Hash(outputChannel), outputChannel, inputChannel );
			}
			
			
			
			
			/// Get the current output channel mapped to the specified input channel.
			/**
			  * If there is already a mapping for that input channel in this channel IO map,
			  * the mapped output channel is returned. If a mapping does not exist, assume
			  * a 1-to-1 mapping and return the same index as the input channel.
			  * 
			  * @param inputChannel - the channel for which the mapping will be queried.
			  * @return the output channel mapped to the specified input channel.
			  */
			GSOUND_INLINE Index getInputChannelMapping( Index inputChannel ) const
			{
				const Index* outputChannel;
				
				if ( inputToOutput.find( Hash(inputChannel), inputChannel, outputChannel ) )
					return *outputChannel;
				else
					return inputChannel;
			}
			
			
			
			
			/// Get the current input channel mapped to the specified output channel.
			/**
			  * If there is already a mapping for that output channel in this channel IO map,
			  * the mapped input channel is returned. If a mapping does not exist, assume
			  * a 1-to-1 mapping and return the same index as the output channel.
			  * 
			  * @param outputChannel - the channel for which the mapping will be queried.
			  * @return the input channel mapped to the specified output channel.
			  */
			GSOUND_INLINE Index getOutputChannelMapping( Index outputChannel ) const
			{
				const Index* inputChannel;
				
				if ( outputToInput.find( Hash(outputChannel), outputChannel, inputChannel ) )
					return *inputChannel;
				else
					return outputChannel;
			}
			
			
			
			
			/// Return whether or not there is an explicit I/O mapping associated with the specified input channel.
			/**
			  * If there is no explicit channel mapping for this channel, FALSE is 
			  * returned. Otherwise, TRUE is returned.
			  * 
			  * @param inputChannel - the input channel to query to see if there is an associated output channel.
			  * @return whether or not the specified input channel has an associated output channel.
			  */
			GSOUND_INLINE Bool inputChannelHasMapping( Index inputChannel ) const
			{
				return inputToOutput.contains( Hash(inputChannel), inputChannel );
			}
			
			
			
			
			/// Return whether or not there is an explicit I/O mapping associated with the specified output channel.
			/**
			  * If there is no explicit channel mapping for this channel, FALSE is 
			  * returned. Otherwise, TRUE is returned.
			  * 
			  * @param outputChannel - the output channel to query to see if there is an associated input channel.
			  * @return whether or not the specified output channel has an associated input channel.
			  */
			GSOUND_INLINE Bool outputChannelHasMapping( Index outputChannel ) const
			{
				return outputToInput.contains( Hash(outputChannel), outputChannel );
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A mapping from input channels to the output channels which they correspond to.
			HashMap<Index,Index> inputToOutput;
			
			
			
			
			/// A mapping from output channels to the input channels which provide their input audio.
			HashMap<Index,Index> outputToInput;
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_CHANNEL_IO_MAP_H
