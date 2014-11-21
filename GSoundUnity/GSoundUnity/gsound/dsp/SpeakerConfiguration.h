/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SpeakerConfiguration.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SpeakerConfiguration class declaration
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


#ifndef INCLUDE_GSOUND_SPEAKER_CONFIGURATION_H
#define INCLUDE_GSOUND_SPEAKER_CONFIGURATION_H


#include "GSoundDSPConfig.h"


#include "ChannelGainArray.h"
#include "SpeakerType.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which specifies the configuration of an arbitrary 2D array of speakers.
/**
  * A SpeakerConfiguration contains a list of speakers, where each speaker has a 
  * direction, audio channel index, and logical function. It provides methods to 
  * access these speakers and modify the configuration. The SpeakerConfiguration
  * can then be used to compute gain coefficients for each speaker in order to
  * localize the direction of a sound source in a particular direction using the
  * spatializeDirection() method.
  */
class SpeakerConfiguration
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a SpeakerConfiguraion object which has no speakers specified.
			SpeakerConfiguration();
			
			
			
			
			/// Create a copy of the specified SpeakerConfiguration object.
			SpeakerConfiguration( const SpeakerConfiguration& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy all state used by a SpeakerConfiguration object.
			~SpeakerConfiguration();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the contents of one SpeakerConfiguration to this object.
			SpeakerConfiguration& operator = ( const SpeakerConfiguration& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Speaker Accessor Methods
			
			
			
			
			/// Add a speaker to this SpeakerConfiguration in the specified direction with the given channel index.
			/**
			  * The direction to the speaker should be of unit length and specified using the following
			  * coordinate system: (0,-1) is forwards, (0,-1) is left. This coordinate system is consistent with
			  * a right handed 3D coordinate system. The index of the new speaker within the speaker
			  * configuration is returned.
			  * 
			  * @param type - the logical function of the speaker.
			  * @param direction - a unit-length direction to the new speaker in the listener's median plane.
			  * @param channelIndex - the audio channel index that this speaker corresponds to.
			  * @return the index of the new speaker in this SpeakerConfiguration.
			  */
			Index addSpeaker( const Vector2& direction, Index channelIndex, SpeakerType type = SpeakerType::UNDEFINED );
			
			
			
			
			/// Get the number of speakers in this Speaker Configuration.
			GSOUND_INLINE Size getNumberOfSpeakers() const
			{
				return speakers.getSize();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Speaker Direction Accessor Methods
			
			
			
			
			/// Get the direction to the speaker at the specified index in this SpeakerConfiguration.
			Vector3 getSpeakerDirection( Index speakerIndex ) const;
			
			
			
			
			/// Set the direction to the speaker at the specified index in this SpeakerConfiguration.
			/**
			  * If the specified speaker index is not within the valid range of speaker indices, 
			  * an assertion is raised. The direction to the speaker should be of unit length and
			  * specified using the following coordinate system: (0,-1) is forwards, (-1,0) is left.
			  * This coordinate system is consistent with a right handed 3D coordinate system.
			  * 
			  * @param speakerIndex -  the index of the speaker whose direction should be set.
			  * @param direction - a unit-length direction to the new speaker in the listener's median plane.
			  */
			void setSpeakerDirection( Index speakerIndex, const Vector2& direction );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Speaker Channel Accessor Methods
			
			
			
			
			/// Get the channel of the speaker at the specified index in this SpeakerConfiguration.
			Index getSpeakerChannel( Index speakerIndex ) const;
			
			
			
			
			/// Set the channel of the speaker at the specified index in this SpeakerConfiguration.
			void setSpeakerChannel( Index speakerIndex, Index newChannelIndex );
			
			
			
			
			/// Return the number of channels of audio that this SpeakerConfiguation needs.
			/**
			  * This value is equal to one plus the maximum channel index of the speakers in this speaker
			  * configuration. For this reason, it is a good idea to use contiguous channel indices
			  * for the speakers in a configuration so as to avoid using more audio channels than
			  * necessary.
			  */
			GSOUND_INLINE Size getNumberOfChannels() const
			{
				return numChannels;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Speaker Type Accessor Methods
			
			
			
			
			/// Get the logical function of the speaker at the specified index in this SpeakerConfiguration.
			SpeakerType getSpeakerType( Index speakerIndex ) const;
			
			
			
			
			/// Get the logical function of the speaker at the specified index in this SpeakerConfiguration.
			void setSpeakerType( Index speakerIndex, SpeakerType newSpeakerType );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Direction Spatialization Method
			
			
			
			
			/// Get the channel gain coefficients for a virtual sound source in the specified direction.
			void spatializeDirection( const Vector3& direction, ChannelGainArray& channelGains ) const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Static Standard Speaker Configuration Accessor Methods
			
			
			
			
			/// Get a reference to a predefined SpeakerConfiguration for a 1-channel mono speaker setup.
			static const SpeakerConfiguration& getMono();
			
			
			
			
			/// Get a reference to a predefined SpeakerConfiguration for a 2-channel stereo speaker setup.
			/**
			  * The left and right speakers are placed 90 degrees left and right of front center.
			  */
			static const SpeakerConfiguration& getStereo();
			
			
			
			
			/// Get a reference to a predefined SpeakerConfiguration for a 2-channel headphone speaker setup.
			/**
			  * The left and right speakers are placed on the listener's ears.
			  */
			static const SpeakerConfiguration& getHeadphones();
			
			
			
			
			/// Get a reference to a predefined SpeakerConfiguration for a 4-channel quadraphonic speaker setup.
			/**
			  * The speakers are placed at 45 degrees to the left and right of front and back center.
			  */
			static const SpeakerConfiguration& getQuadraphonic();
			
			
			
			
			/// Get a reference to a predefined SpeakerConfiguration for a 5.1 surround speaker setup.
			/**
			  * Front left and right speakers are placed 30 degrees off of front center. Back left
			  * and right speakers are placed 110 degrees off of front center.
			  */
			static const SpeakerConfiguration& get5Point1();
			
			
			
			
			/// Get a reference to a predefined SpeakerConfiguration for a 7.1 surround speaker setup.
			/**
			  * Front left and right speakers are placed 30 degrees off of front center. Back left
			  * and right speakers are placed 135 degrees off of front center. Side left and right
			  * speakers are placed 90 degrees off of front center.
			  */
			static const SpeakerConfiguration& get7Point1();
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Speaker Class Definition
			
			
			
			
			/// A class which is used to encapsulate data needed for each speaker in a SpeakerConfiguration.
			class Speaker;
			
			
			
			
			/// A pair of speaker indices which are used to identify a pair of speakers.
			class SpeakerPair;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Methods
			
			
			
			
			/// Update the groups of adjacent speakers in this SpeakerConfiguration.
			void updateSpeakerGroups();
			
			
			
			
			/// Update the current number of channels of audio required for this speaker configuration.
			void updateNumberOfChannels();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A list of the speakers within this speaker configuration.
			ArrayList<Speaker> speakers;
			
			
			
			
			/// A list of pairs of adjacent speakers that are used to efficiently spatialize sound sources.
			ArrayList<SpeakerPair> speakerPairs;
			
			
			
			
			/// The number of channels of audio that this SpeakerConfiguration requires.
			Size numChannels;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Data Members
			
			
			
			
			/// A pointer to a SpeakerConfiguration that describes a mono speaker system.
			static SpeakerConfiguration* monoConfiguration;
			
			
			
			
			/// A pointer to a SpeakerConfiguration that describes a stereo speaker system.
			/**
			  * The left and right speakers are placed 90 degrees left and right of front center.
			  */
			static SpeakerConfiguration* stereoConfiguration;
			
			
			
			
			/// A pointer to a SpeakerConfiguration that describes a stereo headphone speaker system.
			/**
			  * The left and right speakers are placed on the listener's ears.
			  */
			static SpeakerConfiguration* headphoneConfiguration;
			
			
			
			
			/// A pointer to a SpeakerConfiguration that describes a quadraphonic surround speaker system.
			/**
			  * The speakers are placed at 45 degrees to the left and right of front and back center.
			  */
			static SpeakerConfiguration* quadraphonicConfiguration;
			
			
			
			
			/// A pointer to a SpeakerConfiguration that describes a 5.1 surround speaker system.
			/**
			  * Front left and right speakers are placed 30 degrees off of front center. Back left
			  * and right speakers are placed 110 degrees off of front center.
			  */
			static SpeakerConfiguration* fivePointOneConfiguration;
			
			
			
			
			/// A pointer to a SpeakerConfiguration that describes a 7.1 surround speaker system.
			/**
			  * Front left and right speakers are placed 30 degrees off of front center. Back left
			  * and right speakers are placed 135 degrees off of front center. Side left and right
			  * speakers are placed 90 degrees off of front center.
			  */
			static SpeakerConfiguration* sevenPointOneConfiguration;
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SPEAKER_CONFIGURATION_H
