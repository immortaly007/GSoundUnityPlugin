/*
 * Project:     GSound
 * 
 * File:        gsound/SoundSource.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundSource class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_SOURCE_H
#define INCLUDE_GSOUND_SOUND_SOURCE_H


#include "GSoundBase.h"


#include "dsp/SoundOutput.h"
#include "FrequencyResponse.h"
#include "SoundDistanceAttenuation.h"
#include "internal/ProbedTriangleCache.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which corresponds to a sound source in a 3D environment.
class SoundSource
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a default sound source positioned at the origin.
			SoundSource();
			
			
			
			
			/// Create a sound source at the specified position.
			SoundSource( const Vector3& newPosition );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Position Accessor Methods
			
			
			
			
			/// Get the position in world coordinates of the sound source.
			GSOUND_FORCE_INLINE const Vector3& getPosition() const
			{
				return position;
			}
			
			
			
			
			/// Get the position in world coordinates of the sound source.
			GSOUND_FORCE_INLINE void setPosition( const Vector3& newPosition )
			{
				position = newPosition;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Velocity Accessor Methods
			
			
			
			
			/// Get the velocity in world coordinates of the sound source.
			GSOUND_FORCE_INLINE const Vector3& getVelocity() const
			{
				return velocity;
			}
			
			
			
			
			/// Set the velocity in world coordinates of the sound source.
			GSOUND_FORCE_INLINE void setVelocity( const Vector3& newVelocity )
			{
				velocity = newVelocity;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Intensity Accessor Methods
			
			
			
			
			/// Get the intensity (loudness) of the sound source.
			GSOUND_FORCE_INLINE Real getIntensity() const
			{
				return intensity;
			}
			
			
			
			
			/// Set the intensity (loudness) of the sound source.
			GSOUND_FORCE_INLINE void setIntensity( Real newIntensity )
			{
				intensity = newIntensity;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Radius Accessor Methods
			
			
			
			
			/// Get the radius of the sound source.
			/**
			  * If the sound source's position is obstructed from the listener by
			  * world geometry but the source would still be visible as a sphere with
			  * this radius, the sound will still be heard. This value does not effect
			  * the quality of simulation or how the source attenuates with distance.
			  * The default radius is 0.
			  */
			GSOUND_FORCE_INLINE Real getRadius() const
			{
				return radius;
			}
			
			
			
			
			/// Set the radius of the sound source.
			/**
			  * If the sound source's position is obstructed from the listener by
			  * world geometry but the source would still be visible as a sphere with
			  * this radius, the sound will still be heard. This value does not effect
			  * the quality of simulation or how the source attenuates with distance.
			  * The default radius is 0 and all values are clamped to [0,+infinity).
			  */
			GSOUND_INLINE void setRadius( Real newRadius )
			{
				radius = math::max( newRadius, Real(0) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Distance Attenuation Accessor Methods
			
			
			
			
			/// Get the linear gain attenuation factor for the sound source at the specified distance.
			GSOUND_FORCE_INLINE Real getDistanceAttenuation( Real distance ) const
			{
				return distanceAttenuation.getAttenuation( distance );
			}
			
			
			
			
			/// Get a reference to the object which describes how sound from this source is attenuated with distance.
			GSOUND_FORCE_INLINE SoundDistanceAttenuation& getDistanceAttenuation()
			{
				return distanceAttenuation;
			}
			
			
			
			
			/// Get a const reference to the object which describes how sound from this source is attenuated with distance.
			GSOUND_FORCE_INLINE const SoundDistanceAttenuation& getDistanceAttenuation() const
			{
				return distanceAttenuation;
			}
			
			
			
			
			/// Set the object which describes how sound from this source is attenuated with distance.
			GSOUND_FORCE_INLINE void setDistanceAttenuation( const SoundDistanceAttenuation& newDistanceAttenuation )
			{
				distanceAttenuation = newDistanceAttenuation;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Reverb Distance Attenuation Accessor Methods
			
			
			
			
			/// Get the linear gain attenuation factor for the sound source's reverb at the specified distance.
			GSOUND_FORCE_INLINE Real getReverbDistanceAttenuation( Real distance ) const
			{
				return reverbDistanceAttenuation.getAttenuation( distance );
			}
			
			
			
			
			/// Get a reference to the object which describes how reverb from this source is attenuated with distance.
			GSOUND_FORCE_INLINE SoundDistanceAttenuation& getReverbDistanceAttenuation()
			{
				return reverbDistanceAttenuation;
			}
			
			
			
			
			/// Get a const reference to the object which describes how reverb from this source is attenuated with distance.
			GSOUND_FORCE_INLINE const SoundDistanceAttenuation& getReverbDistanceAttenuation() const
			{
				return reverbDistanceAttenuation;
			}
			
			
			
			
			/// Set the object which describes how reverb from this source is attenuated with distance.
			GSOUND_FORCE_INLINE void setReverbDistanceAttenuation( const SoundDistanceAttenuation& newAttenuation )
			{
				reverbDistanceAttenuation = newAttenuation;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Direction Accessor Methods
			
			
			
			
			/// Get the direction in which the sound source is facing.
			GSOUND_FORCE_INLINE const Vector3& getDirection() const
			{
				return direction;
			}
			
			
			
			
			/// Set the direction in which the sound source is facing.
			GSOUND_FORCE_INLINE void setDirection( const Vector3& newDirection )
			{
				direction = newDirection;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Directionality Accessor Methods
			
			
			
			
			/// Get whether or not the sound source should be treated as directional.
			GSOUND_FORCE_INLINE Bool getIsDirectional() const
			{
				return isDirectional;
			}
			
			
			
			
			/// Set whether or not the sound source should be treated as directional.
			GSOUND_FORCE_INLINE void setIsDirectional( Bool newIsDirectional )
			{
				isDirectional = newIsDirectional;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	On-Axis Frequency Response Accessor Methods
			
			
			
			
			/// Get the on-axis frequency response for this sound source.
			GSOUND_FORCE_INLINE FrequencyResponse& getOnAxisFrequencyResponse()
			{
				return onAxisFrequencyResponse;
			}
			
			
			
			
			/// Get the on-axis frequency response for this sound source.
			GSOUND_FORCE_INLINE const FrequencyResponse& getOnAxisFrequencyResponse() const
			{
				return onAxisFrequencyResponse;
			}
			
			
			
			
			/// Set the on-axis frequency response for this sound source.
			GSOUND_INLINE void setOnAxisFrequencyResponse( const FrequencyResponse& newOnAxisResponse )
			{
				onAxisFrequencyResponse = newOnAxisResponse;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Off-Axis Frequency Response Accessor Methods
			
			
			
			
			/// Get the off-axis frequency response for this sound source.
			GSOUND_FORCE_INLINE FrequencyResponse& getOffAxisFrequencyResponse()
			{
				return offAxisFrequencyResponse;
			}
			
			
			
			
			/// Get the off-axis frequency response for this sound source.
			GSOUND_FORCE_INLINE const FrequencyResponse& getOffAxisFrequencyResponse() const
			{
				return offAxisFrequencyResponse;
			}
			
			
			
			
			/// Set the off-axis frequency response for this sound source.
			GSOUND_INLINE void setOffAxisFrequencyResponse( const FrequencyResponse& newOffAxisResponse )
			{
				offAxisFrequencyResponse = newOffAxisResponse;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Output Accessor Methods
			
			
			
			
			/// Get the object that generates the sound for the sound source.
			GSOUND_FORCE_INLINE dsp::SoundOutput* getSoundInput()
			{
				return input;
			}
			
			
			
			
			/// Get the object that generates the sound for the sound source.
			GSOUND_FORCE_INLINE const dsp::SoundOutput* getSoundInput() const
			{
				return input;
			}
			
			
			
			
			/// Set the object that generates the sound for the sound source.
			/**
			  * This SoundOutput object should have 1 output channel. If there is
			  * more than one output channel, the channels will be mixed to mono
			  * before being rendered.
			  * 
			  * @param newSoundInput - the object which generates the input audio for this SoundSource.
			  */
			GSOUND_FORCE_INLINE void setSoundInput( dsp::SoundOutput* newSoundInput )
			{
				input = newSoundInput;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	User Data Accessor Methods
			
			
			
			
			/// Get an opaque pointer to user-defined data for this SoundSource.
			GSOUND_FORCE_INLINE void* getUserData() const
			{
				return userData;
			}
			
			
			
			
			/// Get an opaque pointer to user-defined data for this SoundSource.
			GSOUND_FORCE_INLINE void setUserData( void* newUserData )
			{
				userData = newUserData;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Is Enabled Accessor Methods
			
			
			
			
			/// Return whether or not this sound source is enabled for sound propagation and rendering.
			/**
			  * Sound sources are enabled by default but can be disabled if no audio is being
			  * played for a source. This can increase the performance in scenes with large
			  * numbers of sources that might not all be active at any given time.
			  */
			GSOUND_FORCE_INLINE Bool getIsEnabled() const
			{
				return isEnabled;
			}
			
			
			
			
			/// Set whether or not this sound source should be enabled for sound propagation and rendering.
			/**
			  * Sound sources are enabled by default but can be disabled if no audio is being
			  * played for a source. This can increase the performance in scenes with large
			  * numbers of sources that might not all be active at any given time.
			  */
			GSOUND_FORCE_INLINE void setIsEnabled( Bool newIsEnabled )
			{
				isEnabled = newIsEnabled;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Hash Code Accessor Method
			
			
			
			
			/// Return an integral identifier for the sound source.
			/**
			  * This identifier is not guaranteed to be unique among sound sources
			  * but will be unique in most cases.
			  */
			GSOUND_FORCE_INLINE Hash getHashCode() const
			{
				return hashCode;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Friend Class Declarations
			
			
			
			
			/// Allow the SoundPropagator class to access the private cache data for this SoundSource.
			friend class SoundPropagator;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Listener 
			
			
			
			
			/// A class which holds information about a recently probed triangle.
			class ProbeVisibilityRecord
			{
				public:
					
					GSOUND_INLINE ProbeVisibilityRecord( Real newRayDotNormal, Index newTimeStamp )
						:	rayDotNormal( newRayDotNormal ),
							timeStamp( newTimeStamp )
					{
					}
					
					
					
					/// The dot product of the probe ray's direction and the triangle's normal vector.
					Real rayDotNormal;
					
					
					/// The time stamp indicating the last frame that this visibility record was updated.
					Index timeStamp;
					
					
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The frequency response of the sound source when viewed directly on-axis.
			FrequencyResponse onAxisFrequencyResponse;
			
			
			
			
			/// The frequency response of the sound source when viewed directly off-axis.
			FrequencyResponse offAxisFrequencyResponse;
			
			
			
			
			/// The position of the sound source in world space coordinates.
			Vector3 position;
			
			
			
			
			/// The velocity of the sound source in world space coordinates.
			Vector3 velocity;
			
			
			
			
			/// The direction that the sound source is facing
			Vector3 direction;
			
			
			
			
			/// A linear scaling factor applied to all sound produced by the sound source.
			/**
			  * This floating-point value can be used to modify the playback level of 
			  * sound from the source in order to compensate for attenuation. For instance,
			  * a gun shot could have an intensity of 100, while a whisper could have an
			  * intensity of 0.1.
			  */
			Real intensity;
			
			
			
			
			/// A value which specifies how large a sound source is.
			/**
			  * If the sound source's position is obstructed from the listener by
			  * world geometry but the source would still be visible as a sphere with
			  * this radius, the sound will still be heard. This value does not effect
			  * the quality of simulation or how the source attenuates with distance.
			  * The default radius is 0.
			  */
			Real radius;
			
			
			
			
			/// The distance attenuation applied when the listener recieves a contribution from this source.
			SoundDistanceAttenuation distanceAttenuation;
			
			
			
			
			/// The distance attenuation applied when calculating the attenuation of reverb.
			SoundDistanceAttenuation reverbDistanceAttenuation;
			
			
			
			
			/// A pseudo-unique identifier for the sound source generated randomly at contruction.
			Hash hashCode;
			
			
			
			
			/// The audio output unit which produces the input audio for this SoundSource.
			dsp::SoundOutput* input;
			
			
			
			
			/// An opaque pointer to user-defined data for this SoundSource.
			void* userData;
			
			
			
			
			/// A boolean value indicating whether or not the SoundSource is directional.
			Bool isDirectional;
			
			
			
			
			/// Whether or not this sound source is enabled for sound propagation and rendering.
			Bool isEnabled;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Sound Propagation Cache Data Members
			
			
			
			
			/// A cache of triangles that have been recently probed from this source's position.
			mutable internal::ProbedTriangleCache<ProbeVisibilityRecord> probedTriangles;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_SOURCE_H
