/*
 * Project:     GSound
 * 
 * File:        gsound/SoundListener.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundListener class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_LISTENER_H
#define INCLUDE_GSOUND_SOUND_LISTENER_H


#include "GSoundBase.h"


#include "internal/ProbePathCache.h"
#include "internal/ProbedTriangleCache.h"
#include "internal/RayDistributionCache.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which represents a sound reciever in a 3D sound propagation system.
class SoundListener
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a default listener centered at the origin.
			SoundListener();
			
			
			
			
			/// Create a listener with the specified position and orientation.
			SoundListener( const Vector3& newPosition, const Matrix3& newOrientation );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Position Accessor Methods
			
			
			
			
			/// Get the position of the listener.
			GSOUND_FORCE_INLINE const Vector3& getPosition() const
			{
				return position;
			}
			
			
			
			
			/// Set the position of the listener.
			GSOUND_FORCE_INLINE void setPosition( const Vector3& newPosition )
			{
				position = newPosition;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Orientation Accessor Methods
			
			
			
			
			/// Get the orientation of the listener.
			/**
			  * The orientation is represented by a 3x3 orthonormal rotation
			  * matrix using a right-handed coordinate system.
			  */
			GSOUND_FORCE_INLINE const Matrix3& getOrientation() const
			{
				return orientation;
			}
			
			
			
			
			/// Set the orientation of the listener.
			/**
			  * The orientation is represented by a 3x3 orthonormal rotation
			  * matrix using a right-handed coordinate system. The new
			  * orientation is automatically orthonormalized using Grahm-Schmit
			  * orthonormalization. Use the setOrientationRaw() method to set the
			  * matrix directly and avoid the time spent in this operation if you
			  * are sure that your matrix will be orthonormal.
			  */
			GSOUND_NO_INLINE void setOrientation( const Matrix3& newOrientation )
			{
				orientation = newOrientation.orthonormalize();
			}
			
			
			
			
			/// Set the raw orientation of the listener.
			/**
			  * The orientation is represented by a 3x3 orthonormal rotation
			  * matrix using a right-handed coordinate system. This method avoids
			  * the cost of the setOrientation() method by directly setting the matrix,
			  * but should be used only if you are sure that the new orientation matrix
			  * is orthonormal.
			  */
			GSOUND_INLINE void setOrientationRaw( const Matrix3& newOrientation )
			{
				orientation = newOrientation;
			}
			
			
			
			
			/// Get the view direction of the listener.
			GSOUND_INLINE Vector3 getViewDirection() const
			{
				return -orientation.z;
			}
			
			
			
			
			/// Get the up direction of the listener.
			GSOUND_FORCE_INLINE const Vector3& getUpDirection() const
			{
				return orientation.y;
			}
			
			
			
			
			/// Get the left direction of the listener.
			GSOUND_INLINE Vector3 getLeftDirection() const
			{
				return -orientation.x;
			}
			
			
			
			
			/// Get the right direction of the listener.
			GSOUND_FORCE_INLINE const Vector3& getRightDirection() const
			{
				return orientation.x;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Velocity Accessor Methods
			
			
			
			
			/// Get the velocity of the listener.
			GSOUND_FORCE_INLINE const Vector3& getVelocity() const
			{
				return velocity;
			}
			
			
			
			
			/// Set the velocity of the listener.
			GSOUND_FORCE_INLINE void setVelocity( const Vector3& newVelocity )
			{
				velocity = newVelocity;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	User Data Accessor Methods
			
			
			
			
			/// Get an opaque pointer to user-defined data for this SoundListener.
			GSOUND_FORCE_INLINE void* getUserData() const
			{
				return userData;
			}
			
			
			
			
			/// Get an opaque pointer to user-defined data for this SoundListener.
			GSOUND_FORCE_INLINE void setUserData( void* newUserData )
			{
				userData = newUserData;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Friend Class Declarations
			
			
			
			
			/// Allow the SoundPropagator class to access the private cache data for this listener.
			friend class SoundPropagator;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Listener 
			
			
			
			
			/// A class which holds information about a recently probed triangle.
			class ProbeVisibilityRecord
			{
				public:
					
					GSOUND_INLINE ProbeVisibilityRecord( Real newRayDotNormal, const Vector3& newRayDirection,
														Index newTimeStamp )
						:	rayDotNormal( newRayDotNormal ),
							rayDirection( rayDirection ),
							timeStamp( newTimeStamp )
					{
					}
					
					
					
					/// The dot product of the probe ray's direction and the triangle's normal vector.
					Real rayDotNormal;
					
					
					/// The direction of the original probe ray in the listener's coordinate frame.
					Vector3 rayDirection;
					
					
					/// The time stamp indicating the last frame that this visibility record was updated.
					Index timeStamp;
					
					
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The position of the SoundListener in 3D spacce.
			Vector3 position;
			
			
			
			
			/// The velocity of the SoundListener in 3D spacce.
			Vector3 velocity;
			
			
			
			
			/// The orientation of the SoundListener in 3D spacce.
			Matrix3 orientation;
			
			
			
			
			/// An opaque pointer to user-defined data for this SoundListener.
			void* userData;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Sound Propagation Cache Data Members
			
			
			
			
			/// A cache of the previously-valid probed triangle sequences.
			mutable internal::ProbePathCache probePathCache;
			
			
			
			
			/// A cache of triangles that have been recently probed from this listener's position.
			mutable internal::ProbedTriangleCache<ProbeVisibilityRecord> probedTriangles;
			
			
			
			
			/// A class which keeps track of which ray directions produce the most paths.
			mutable internal::RayDistributionCache rayDistribution;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_LISTENER_H
