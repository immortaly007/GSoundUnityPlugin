/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/ChannelGainArray.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::ChannelGainArray class declaration
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


#ifndef INCLUDE_GSOUND_CHANNEL_GAIN_ARRAY_H
#define INCLUDE_GSOUND_CHANNEL_GAIN_ARRAY_H


#include "GSoundDSPConfig.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which encapsulates the gain coefficients for an arbitrary number of channels of audio.
/**
  * A ChannelGainArray contains a dynamically allocated array of floating-point channel
  * gain coefficients. The number of channels that a ChannelGainArray holds gains for can
  * be changed to accomodate changing channel counts. This class is typically used to 
  * hold the gain coefficients that are the result of an audio panning operation.
  */
class ChannelGainArray
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a channel gain array with the 0 channels.
			GSOUND_INLINE ChannelGainArray()
				:	numChannels( 0 ),
					gains( NULL )
			{
			}
			
			
			
			
			/// Create a channel gain array with the specified number of channels with all gains = 0.
			GSOUND_INLINE ChannelGainArray( Size newNumChannels )
				:	numChannels( newNumChannels )
			{
				if ( newNumChannels != Size(0) )
					gains = util::constructArray<Float>( newNumChannels, Float(0) );
				else
					gains = NULL;
			}
			
			
			
			
			/// Create a copy of the specified channel gain array object, copying all internal state.
			GSOUND_INLINE ChannelGainArray( const ChannelGainArray& other )
				:	numChannels( other.numChannels )
			{
				if ( other.gains != NULL )
					gains = util::copyArray( other.gains, numChannels );
				else
					gains = NULL;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy a channel gain array object, releasing all internal state and memory.
			GSOUND_INLINE ~ChannelGainArray()
			{
				if ( gains != NULL )
					util::destructArray( gains, numChannels );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the state of another channel gain array object to this object.
			GSOUND_INLINE ChannelGainArray& operator = ( const ChannelGainArray& other )
			{
				if ( this != &other )
				{
					if ( gains != NULL )
						util::destructArray( gains, numChannels );
					
					numChannels = other.numChannels;
					
					if ( other.gains != NULL )
						gains = util::copyArray( other.gains, numChannels );
					else
						gains = NULL;
				}
				
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Channel Count Accessor Methods
			
			
			
			
			/// Get the number of channels in this channel gain array.
			GSOUND_INLINE Size getNumberOfChannels() const
			{
				return numChannels;
			}
			
			
			
			
			/// Set the number of input channels in this channel gain array.
			/**
			  * As a consequence of this operation, all internal gain coefficients will be
			  * reset to zero.
			  * 
			  * @param newNumberOfInputChannels - the desired number of input channels for this channel gain array.
			  */
			GSOUND_INLINE void setNumberOfChannels( Size newNumberOfChannels )
			{
				// Destroy the old array of gain coefficients.
				if ( gains != NULL )
					util::destructArray( gains, numChannels );
				
				numChannels = newNumberOfChannels;
				
				// Create a new array of gain coefficient initialized to 0.
				if ( newNumberOfChannels != Size(0) )
					gains = util::constructArray<Float>( newNumberOfChannels, Float(0) );
				else
					gains = NULL;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Gain Accessor Methods
			
			
			
			
			/// Get the gain coefficient for the specified channel index.
			/**
			  * This method returns the gain coeffient of the specified channel.
			  * If the channel index is not within the valid range of [0, numChannels - 1],
			  * an assertion is raised.
			  * 
			  * @param channelIndex - the index of the channel gain to query.
			  * @return the gain coefficient (amount) of the specified input channel in the output channel.
			  */
			GSOUND_INLINE Float getGain( Index channelIndex ) const
			{
				GSOUND_DEBUG_ASSERT( channelIndex < numChannels );
				
				return gains[channelIndex];
			}
			
			
			
			
			/// Set the gain coefficient for the specified channel index.
			/**
			  * This method sets the gain coeffient of the specified channel.
			  * If the channel index is not within the valid range of [0, numChannels - 1],
			  * an assertion is raised.
			  * 
			  * @param channelIndex - the index of the input channel to set.
			  * @param newGain - the new gain coefficient for the specified channel.
			  */
			GSOUND_INLINE void setGain( Index channelIndex, Float newGain )
			{
				GSOUND_DEBUG_ASSERT( channelIndex < numChannels );
				
				gains[channelIndex] = newGain;
			}
			
			
			
			
			/// Set every gain coefficient in this channel gain array to the specified value.
			GSOUND_INLINE void setGains( Float newGain )
			{
				if ( gains != NULL )
				{
					Float* gain = gains;
					const Float* const gainsEnd = gains + numChannels;
					
					while ( gain != gainsEnd )
					{
						*gain = newGain;
						gain++;
					}
				}
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The number of channels in this channel gain array.
			Size numChannels;
			
			
			
			
			/// An array of gain coefficients that specify how much of each input channel is in each output channel.
			Float* gains;
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_CHANNEL_GAIN_ARRAY_H
