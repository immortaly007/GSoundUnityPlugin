/*
 * Project:     GSound
 * 
 * File:        gsound/SoundObject.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundObject class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_OBJECT_H
#define INCLUDE_GSOUND_SOUND_OBJECT_H


#include "GSoundBase.h"


#include "dsp/SoundOutput.h"
#include "internal/BoundingSphere.h"
#include "SoundMesh.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which is used to represent a portion of scene geometry in the sound propagation scene.
/**
  * A sound object has a rigid transformation which is used to position the object's mesh
  * in world space, allowing for dynamic scenes. A sound object can have a mesh, which can
  * be shared among multiple sound objects to allow instancing of geometry.
  */
class SoundObject
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a sound object with the default transformation and no shape.
			GSOUND_INLINE SoundObject()
				:	mesh( NULL ),
					transformation()
			{
				updateWorldSpaceBoundingSphere();
			}
			
			
			
			
			/// Create a sound object with the specified mesh and no transformation.
			GSOUND_INLINE SoundObject( SoundMesh* newMesh )
				:	mesh( newMesh ),
					transformation()
			{
				updateWorldSpaceBoundingSphere();
			}
			
			
			
			
			/// Create a sound object with the specified mesh and transformation.
			GSOUND_INLINE SoundObject( SoundMesh* newMesh,
								const Transformation3& newTransformation )
				:	mesh( newMesh ),
					transformation( newTransformation )
			{
				updateWorldSpaceBoundingSphere();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Transformation Accessor Methods
			
			
			
			
			/// Get the rigid transformation of this object.
			GSOUND_INLINE const Transformation3& getTransformation() const
			{
				return transformation;
			}
			
			
			
			
			/// Set the rigid transformation of this object.
			GSOUND_INLINE void setTransformation( const Transformation3& newTransformation )
			{
				transformation = Transformation3( newTransformation.position,
												newTransformation.orientation.orthonormalize(),
												newTransformation.scale );
				
				updateWorldSpaceBoundingSphere();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Position Accessor Methods
			
			
			
			
			/// Get the position of this object in world space.
			GSOUND_INLINE const Vector3& getPosition() const
			{
				return transformation.position;
			}
			
			
			
			
			/// Set the position of this object in world space.
			GSOUND_INLINE void setPosition( const Vector3& newPosition )
			{
				transformation.position = newPosition;
				
				updateWorldSpaceBoundingSphere();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Orientation Accessor Methods
			
			
			
			
			/// Get the orientation of this sound object in 3D space.
			GSOUND_INLINE const Matrix3& getOrientation() const
			{
				return transformation.orientation;
			}
			
			
			
			
			/// Set the orientation of this sound object in 3D space.
			GSOUND_INLINE void setOrientation( const Matrix3& newOrientation )
			{
				transformation.orientation = newOrientation.orthonormalize();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Scale Accessor Methods
			
			
			
			
			/// Get the scale of this object.
			GSOUND_INLINE Real getScale() const
			{
				return transformation.scale;
			}
			
			
			
			
			/// Set the scale of this object.
			GSOUND_INLINE void setScale( Real newScale )
			{
				transformation.scale = newScale;
				
				updateWorldSpaceBoundingSphere();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Velocity Accessor Methods
			
			
			
			
			/// Get the velocity of this object in world space.
			GSOUND_INLINE const Vector3& getVelocity() const
			{
				return velocity;
			}
			
			
			
			
			/// Set the velocity of this object in world space.
			GSOUND_INLINE void setVelocity( const Vector3& newVelocity )
			{
				velocity = newVelocity;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Bounding Sphere Accessor Method
			
			
			
			
			/// Get the bounding sphere of this sound object.
			/**
			  * This bounding sphere is specified in world-space coordinates.
			  */
			GSOUND_INLINE const internal::BoundingSphere& getBoundingSphere() const
			{
				return worldSpaceBoundingSphere;
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mesh Accessor Method
			
			
			
			
			/// Get the object that describes the mesh of the sound object.
			GSOUND_INLINE SoundMesh* getMesh()
			{
				return mesh;
			}
			
			
			
			
			/// Get the object that describes the mesh of the sound object.
			GSOUND_INLINE const SoundMesh* getMesh() const
			{
				return mesh;
			}
			
			
			
			
			/// Set the object that describes the mesh of the sound object.
			GSOUND_INLINE void setMesh( SoundMesh* newMesh )
			{
				mesh = newMesh;
				
				updateWorldSpaceBoundingSphere();
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Methods
			
			
			
			
			/// Update the world-space bounding sphere for this object.
			GSOUND_INLINE void updateWorldSpaceBoundingSphere()
			{
				if ( mesh != NULL )
				{
					const internal::BoundingSphere& meshBoundingSphere = mesh->getBoundingSphere();
					
					worldSpaceBoundingSphere.position = transformation.transformToWorldSpace( meshBoundingSphere.position );
					worldSpaceBoundingSphere.radius = transformation.transformToWorldSpace( meshBoundingSphere.radius );
				}
				else
					worldSpaceBoundingSphere = internal::BoundingSphere();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The transformation for this sound object.
			Transformation3 transformation;
			
			
			
			
			/// The linear velocity of this sound object.
			Vector3 velocity;
			
			
			
			
			/// The bounding sphere of this sound object in world space.
			internal::BoundingSphere worldSpaceBoundingSphere;
			
			
			
			
			/// The mesh of this sound object.
			SoundMesh* mesh;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_OBJECT_H
