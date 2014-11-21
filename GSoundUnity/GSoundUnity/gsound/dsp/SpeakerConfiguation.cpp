/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/SpeakerConfiguration.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::SpeakerConfiguration class implementation
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


#include "SpeakerConfiguration.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Private Static Speaker Configuration Definitions
//############		
//##########################################################################################
//##########################################################################################




SpeakerConfiguration* SpeakerConfiguration:: monoConfiguration = NULL;
SpeakerConfiguration* SpeakerConfiguration:: stereoConfiguration = NULL;
SpeakerConfiguration* SpeakerConfiguration:: headphoneConfiguration = NULL;
SpeakerConfiguration* SpeakerConfiguration:: quadraphonicConfiguration = NULL;
SpeakerConfiguration* SpeakerConfiguration:: fivePointOneConfiguration = NULL;
SpeakerConfiguration* SpeakerConfiguration:: sevenPointOneConfiguration = NULL;




//##########################################################################################
//##########################################################################################
//############		
//############		Speaker Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SpeakerConfiguration:: Speaker
{
	public:
		
		
		/// Create a new Speaker object with the specified direction, channel index, and type.
		GSOUND_INLINE Speaker( const Vector2& newDirection, Index newChannelIndex, SpeakerType newType )
			:	direction( newDirection ),
				channelIndex( newChannelIndex ),
				type( newType )
		{
		}
		
		
		
		/// The unit-length direction in the horizontal plane from the listener to the speaker.
		Vector2 direction;
		
		
		
		/// The audio channel index which this speaker corresponds to.
		Index channelIndex;
		
		
		
		
		/// The semantic usage type associated with this speaker.
		SpeakerType type;
		
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Speaker Pair Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SpeakerConfiguration:: SpeakerPair
{
	public:
		
		
		GSOUND_INLINE SpeakerPair( Index s1, Index s2, Real angle1, Real angle2 )
		{
			speakers[0] = s1;
			speakers[1] = s2;
			angles[0] = angle1;
			angles[1] = angle2;
		}
		
		
		Index speakers[2];
		Real angles[2];
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SpeakerConfiguration:: SpeakerConfiguration()
	:	numChannels( 0 )
{
}




SpeakerConfiguration:: SpeakerConfiguration( const SpeakerConfiguration& other )
	:	speakers( other.speakers ),
		speakerPairs( other.speakerPairs ),
		numChannels( other.numChannels )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SpeakerConfiguration:: ~SpeakerConfiguration()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SpeakerConfiguration& SpeakerConfiguration:: operator = ( const SpeakerConfiguration& other )
{
	if ( this != &other )
	{
		speakers = other.speakers;
		speakerPairs = other.speakerPairs;
		numChannels = other.numChannels;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Speaker Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Index SpeakerConfiguration:: addSpeaker( const Vector2& direction, Index channelIndex, SpeakerType type )
{
	Index speakerIndex = speakers.getSize();
	
	speakers.add( Speaker( direction.normalize(), channelIndex, type ) );
	
	// Update the groups of adjacent speakers in this speaker configuration.
	updateSpeakerGroups();
	
	// Update the number of channels that this speaker configuration needs.
	updateNumberOfChannels();
	
	return speakerIndex;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Speaker Direction Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Vector3 SpeakerConfiguration:: getSpeakerDirection( Index speakerIndex ) const
{
	GSOUND_DEBUG_ASSERT( speakerIndex < speakers.getSize() );
	
	return Vector3( speakers[speakerIndex].direction.x, Real(0), speakers[speakerIndex].direction.y );
}




void SpeakerConfiguration:: setSpeakerDirection( Index speakerIndex, const Vector2& direction )
{
	GSOUND_DEBUG_ASSERT( speakerIndex < speakers.getSize() );
	
	// Set the new speaker direction.
	speakers[speakerIndex].direction = direction.normalize();
	
	// Update the groups of adjacent speakers in this speaker configuration.
	updateSpeakerGroups();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Speaker Channel Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Index SpeakerConfiguration:: getSpeakerChannel( Index speakerIndex ) const
{
	GSOUND_DEBUG_ASSERT( speakerIndex < speakers.getSize() );
	
	return speakers[speakerIndex].channelIndex;
}




void SpeakerConfiguration:: setSpeakerChannel( Index speakerIndex, Index newChannelIndex )
{
	GSOUND_DEBUG_ASSERT( speakerIndex < speakers.getSize() );
	
	// Set the new speaker channel.
	speakers[speakerIndex].channelIndex = newChannelIndex;
	
	// Update the number of channels that this speaker configuration needs.
	updateNumberOfChannels();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Speaker Type Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SpeakerType SpeakerConfiguration:: getSpeakerType( Index speakerIndex ) const
{
	GSOUND_DEBUG_ASSERT( speakerIndex < speakers.getSize() );
	
	return speakers[speakerIndex].type;
}




void SpeakerConfiguration:: setSpeakerType( Index speakerIndex, SpeakerType newType )
{
	GSOUND_DEBUG_ASSERT( speakerIndex < speakers.getSize() );
	
	// Set the new speaker type.
	speakers[speakerIndex].type = newType;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Direction Spatialization Method
//############		
//##########################################################################################
//##########################################################################################




void SpeakerConfiguration:: spatializeDirection( const Vector3& direction, ChannelGainArray& channelGains ) const
{
	if ( numChannels != channelGains.getNumberOfChannels() )
		channelGains.setNumberOfChannels( numChannels );
	else
		channelGains.setGains( Float(0) );
	
	Size numSpeakers = speakers.getSize();
	
	// If there are no speakers in this configuration, return immediately.
	if ( numSpeakers == 0 )
		return;
	
	if ( numSpeakers == 1 )
	{
		// This is a monophonic speaker configuration, the only channel's gain must be 1.
		channelGains.setGain( speakers[0].channelIndex, Float(1) );
	}
	else
	{
		// Extract the horizontal components of the direction vector.
		Vector2 d = direction.getXZ();
		
		Real angle = math::atan2( -d.y, d.x );
		
		if ( angle < Real(0) )
			angle += Real(2)*math::pi<Real>();
		
		Size numSpeakerPairs = speakerPairs.getSize();
		Index lastPairIndex = numSpeakerPairs - 1;
		
		// Find the speaker pair between which this direction vector lies.
		for ( Index i = 0; i < numSpeakerPairs; i++ )
		{
			const SpeakerPair& pair = speakerPairs[i];
			
			// If the angle is between the pair's angle range, it is the right pair.
			if ( angle >= pair.angles[0] && angle <= pair.angles[1] || i == lastPairIndex )
			{
				// Compute the adjusted angle (between 0 and 180 degrees).
				Real adjustedAngle;
				
				if ( i != lastPairIndex )
				{
					adjustedAngle = math::pi<Real>()*((angle - pair.angles[0])/(pair.angles[1] - pair.angles[0]));
				}
				else
				{
					if ( angle < pair.angles[0] )
						angle += Real(2)*math::pi<Real>();
					
					adjustedAngle = math::pi<Real>()*((pair.angles[1] - angle)/math::abs(pair.angles[1] - pair.angles[0]));
				}
				
				// Compute a pan value for the direction vector.
				Real pan = math::cos( adjustedAngle );
				
				// Compute the gain factors for the speaker pair based on the sign of the pan value.
				Vector2 gainFactors = pan > Real(0) ? Vector2( Real(1), Real(1) - pan ) : Vector2( Real(1) + pan, Real(1) );
				gainFactors = gainFactors.normalize();
				
				Index channelIndex1 = speakers[pair.speakers[0]].channelIndex;
				Index channelIndex2 = speakers[pair.speakers[1]].channelIndex;
				
				if ( i != lastPairIndex )
				{
					channelGains.setGain( channelIndex1, gainFactors.x );
					channelGains.setGain( channelIndex2, math::min( channelGains.getGain( channelIndex2 ) + gainFactors.y, Real(1) ) );
				}
				else
				{
					channelGains.setGain( channelIndex2, gainFactors.x );
					channelGains.setGain( channelIndex1, math::min( channelGains.getGain( channelIndex1 ) + gainFactors.y, Real(1) ) );
				}
				
				break;
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Speaker Group Update Method
//############		
//##########################################################################################
//##########################################################################################




struct SpeakerAngle
{
	public:
		
		GSOUND_INLINE SpeakerAngle( Index newSpeakerIndex, Real newAngle )
			:	speakerIndex( newSpeakerIndex ),
				angle( newAngle )
		{
		}
		
		Index speakerIndex;
		Real angle;
		
};




static int compareSpeakerAngles( const void* a, const void* b )
{
	SpeakerAngle* speaker1 = (SpeakerAngle*)a;
	SpeakerAngle* speaker2 = (SpeakerAngle*)b;
	
	if ( speaker1->angle == speaker2->angle )
		return 0;
	else if ( speaker1->angle < speaker2->angle )
		return -1;
	else
		return 1;
}




void SpeakerConfiguration:: updateSpeakerGroups()
{
	speakerPairs.clear();
	
	if ( speakers.getSize() < 2 )
		return;
	else
	{
		ArrayList<SpeakerAngle> angles( speakers.getSize() );
		
		for ( Index i = 0; i < speakers.getSize(); i++ )
		{
			Real angle = math::atan2(-speakers[i].direction.y,speakers[i].direction.x);
			
			if ( angle < Real(0) )
				angle += Real(2)*math::pi<Real>();
			
			angles.add( SpeakerAngle( i, angle ) );
		}
		
		std::qsort( angles.getArrayPointer(), angles.getSize(), sizeof(SpeakerAngle), compareSpeakerAngles );
		
		Index lastAngleIndex = angles.getSize() - 1;
		
		for ( Index i = 0; i < angles.getSize(); i++ )
		{
			if ( i == lastAngleIndex )
			{
				Index i1 = i;
				Index i2 = 0;
				
				SpeakerPair pair( angles[i1].speakerIndex, angles[i2].speakerIndex, angles[i1].angle,
								angles[i2].angle + Real(2)*math::pi<Real>() );
				
				speakerPairs.add( pair );
			}
			else
			{
				Index i1 = i;
				Index i2 = i + 1;
				
				SpeakerPair pair( angles[i1].speakerIndex, angles[i2].speakerIndex, angles[i1].angle, angles[i2].angle );
				
				speakerPairs.add( pair );
			}
		}
	}
}





void SpeakerConfiguration:: updateNumberOfChannels()
{
	if ( speakers.getSize() == 0 )
		numChannels = 0;
	else
	{
		Index maxChannelIndex = 0;
		
		for ( Index i = 0; i < speakers.getSize(); i++ )
		{
			if ( speakers[i].channelIndex > maxChannelIndex )
				maxChannelIndex = speakers[i].channelIndex;
		}
		
		numChannels = maxChannelIndex + 1;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Static Standard Speaker Configuration Accessor Methods
//############		
//##########################################################################################
//##########################################################################################





const SpeakerConfiguration& SpeakerConfiguration:: getMono()
{
	if ( monoConfiguration == NULL )
	{
		monoConfiguration = util::allocate<SpeakerConfiguration>();
		new (monoConfiguration) SpeakerConfiguration();
		
		monoConfiguration->addSpeaker( Vector2( Real(0), Real(0) ), 0, SpeakerType::MONO );
	}
	
	return *monoConfiguration;
}




const SpeakerConfiguration& SpeakerConfiguration:: getStereo()
{
	if ( stereoConfiguration == NULL )
	{
		stereoConfiguration = util::allocate<SpeakerConfiguration>();
		new (stereoConfiguration) SpeakerConfiguration();
		
		// Left Front Speaker
		stereoConfiguration->addSpeaker( Vector2( Real(-1), Real(0) ), 0, SpeakerType::LEFT );
		
		// Right Front Speaker
		stereoConfiguration->addSpeaker( Vector2( Real(1), Real(0) ), 1, SpeakerType::RIGHT );
	}
	
	return *stereoConfiguration;
}




const SpeakerConfiguration& SpeakerConfiguration:: getHeadphones()
{
	if ( headphoneConfiguration == NULL )
	{
		headphoneConfiguration = util::allocate<SpeakerConfiguration>();
		new (headphoneConfiguration) SpeakerConfiguration();
		
		// Left Front Speaker
		headphoneConfiguration->addSpeaker( Vector2( Real(-1), Real(0) ), 0, SpeakerType::HEADPHONE_LEFT );
		
		// Right Front Speaker
		headphoneConfiguration->addSpeaker( Vector2( Real(1), Real(0) ), 1, SpeakerType::HEADPHONE_RIGHT );
	}
	
	return *headphoneConfiguration;
}




const SpeakerConfiguration& SpeakerConfiguration:: getQuadraphonic()
{
	if ( quadraphonicConfiguration == NULL )
	{
		quadraphonicConfiguration = util::allocate<SpeakerConfiguration>();
		new (quadraphonicConfiguration) SpeakerConfiguration();
		
		// Left Front Speaker
		quadraphonicConfiguration->addSpeaker( Vector2( Real(-1), Real(-1) ), 0, SpeakerType::FRONT_LEFT );
		
		// Right Front Speaker
		quadraphonicConfiguration->addSpeaker( Vector2( Real(1), Real(-1) ), 1, SpeakerType::FRONT_RIGHT );
		
		// Left Back Speaker
		quadraphonicConfiguration->addSpeaker( Vector2( Real(-1), Real(1) ), 2, SpeakerType::BACK_LEFT );
		
		// Right Back Speaker
		quadraphonicConfiguration->addSpeaker( Vector2( Real(1), Real(1) ), 3, SpeakerType::BACK_RIGHT );
	}
	
	return *quadraphonicConfiguration;
}




const SpeakerConfiguration& SpeakerConfiguration:: get5Point1()
{
	if ( fivePointOneConfiguration == NULL )
	{
		fivePointOneConfiguration = util::allocate<SpeakerConfiguration>();
		new (fivePointOneConfiguration) SpeakerConfiguration();
		
		// Left Front Speaker (30 degrees left of front center)
		fivePointOneConfiguration->addSpeaker( Vector2( Real(-0.5), Real(-0.8660254) ), 0, SpeakerType::FRONT_LEFT );
		
		// Right Front Speaker (30 degrees right of front center)
		fivePointOneConfiguration->addSpeaker( Vector2( Real(0.5), Real(-0.8660254) ), 1, SpeakerType::FRONT_RIGHT );
		
		// Center Front Speaker (0 degrees from front center)
		fivePointOneConfiguration->addSpeaker( Vector2( Real(0), Real(-1) ), 2, SpeakerType::FRONT_CENTER );
		
		// Left Back Speaker (110 degrees left of front center)
		fivePointOneConfiguration->addSpeaker( Vector2( Real(-0.939693), Real(0.34202) ), 4, SpeakerType::BACK_LEFT );
		
		// Right Back Speaker (110 degrees right of front center)
		fivePointOneConfiguration->addSpeaker( Vector2( Real(0.939693), Real(0.34202) ), 5, SpeakerType::BACK_RIGHT );
	}
	
	return *fivePointOneConfiguration;
}




const SpeakerConfiguration& SpeakerConfiguration:: get7Point1()
{
	if ( sevenPointOneConfiguration == NULL )
	{
		sevenPointOneConfiguration = util::allocate<SpeakerConfiguration>();
		new (sevenPointOneConfiguration) SpeakerConfiguration();
		
		// Left Front Speaker (30 degrees left of front center)
		sevenPointOneConfiguration->addSpeaker( Vector2( Real(-0.5), Real(-0.8660254) ), 0, SpeakerType::FRONT_LEFT );
		
		// Right Front Speaker (30 degrees right of front center)
		sevenPointOneConfiguration->addSpeaker( Vector2( Real(0.5), Real(-0.8660254) ), 1, SpeakerType::FRONT_RIGHT );
		
		// Center Front Speaker (0 degrees from front center)
		sevenPointOneConfiguration->addSpeaker( Vector2( Real(0), Real(-1) ), 2, SpeakerType::FRONT_CENTER );
		
		// Left Back Speaker (135 degrees left of front center)
		sevenPointOneConfiguration->addSpeaker( Vector2( Real(-0.7071068), Real(0.7071068) ), 4, SpeakerType::BACK_LEFT );
		
		// Right Back Speaker (135 degrees right of front center)
		sevenPointOneConfiguration->addSpeaker( Vector2( Real(0.7071068), Real(0.7071068) ), 5, SpeakerType::BACK_RIGHT );
		
		// Left Side Speaker (90 degrees left of front center)
		sevenPointOneConfiguration->addSpeaker( Vector2( Real(-1), Real(0) ), 6, SpeakerType::SIDE_LEFT );
		
		// Right Side Speaker (90 degrees right of front center)
		sevenPointOneConfiguration->addSpeaker( Vector2( Real(1), Real(0) ), 7, SpeakerType::SIDE_RIGHT );
	}
	
	return *sevenPointOneConfiguration;
}




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
