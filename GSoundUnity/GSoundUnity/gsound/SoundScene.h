/*
 * Project:     GSound
 * 
 * File:        gsound/SoundScene.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundScene class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_SCENE_H
#define INCLUDE_GSOUND_SOUND_SCENE_H


#include "GSoundBase.h"


#include "internal/SphereTree.h"
#include "SoundSource.h"
#include "SoundListener.h"
#include "SoundObject.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which represents a scene where sound propagation is performed.
/**
  * A SoundScene object contains a list of pointers to SoundSource objects.
  * These objects describe points in space that produce sound. A SoundScene also
  * contains a list of pointers to SoundObject objects. These objects represent
  * geometry in the scene which can obstruct the propagation of sound.
  *
  * In addition, a SoundScene allows one to specify the speed of sound within
  * the scene in world units per second.
  */
class SoundScene
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Type Definitions
			
			
			
			
			/// The type of bounding volume hierarchy to use for the objects in the scene.
			typedef internal::SphereTree ObjectBVHType;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create an empty sound scene with no objects, sources, or listeners.
			SoundScene();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Source Accessor Methods
			
			
			
			
			/// Add a new source to this sound scene.
			/**
			  * If the new source is NULL, the method has no effect.
			  * 
			  * @param newSource - a source to be added to the sound scene.
			  */
			void addSource( SoundSource* newSource );
			
			
			
			
			/// Remove the specified source from this sound scene.
			/**
			  * This method performs a linear search for the specified source
			  * in the scene and removes it if it is found. A value of TRUE is
			  * returned if the source was found and removed. Otherwise, FALSE
			  * is returned.
			  * 
			  * @param source - a source to remove from the sound scene.
			  * @return whether or not the source was successfully removed.
			  */
			Bool removeSource( SoundSource* source );
			
			
			
			
			/// Remove all sources from this sound scene.
			/**
			  * This method removes all sources in this scene in constant time.
			  */
			void removeAllSources();
			
			
			
			
			/// Get a pointer to the source with the specified index.
			/**
			  * If the given index is greater than or equal to the number of
			  * sources in the scene, a NULL pointer is returned. Otherwise,
			  * a pointer to the source at the specified index in an internal
			  * array is returned.
			  * 
			  * @param sourceIndex - the index of the source to be queried.
			  * @return a pointer to the source at the given index.
			  */
			GSOUND_INLINE SoundSource* getSource( Index sourceIndex ) const
			{
				return sourceIndex < sources.getSize() ? sources[sourceIndex] : NULL;
			}
			
			
			
			
			/// Get the number of sources in this sound scene.
			GSOUND_FORCE_INLINE Size getNumberOfSources() const
			{
				return sources.getSize();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Object Accessor Methods
			
			
			
			
			/// Add a new object to this sound scene.
			/**
			  * If the new object is NULL, the method has no effect. The
			  * object bounding volume heirarchy for this scene is then rebuilt
			  * with the new object, incurring the cost of this operation.
			  * 
			  * @param newObject - an object to be added to the sound scene.
			  */
			void addObject( SoundObject* newObject );
			
			
			
			
			
			/// Remove the specified object from this sound scene.
			/**
			  * This method performs a linear search for the specified object
			  * in the scene and removes it if it is found. A value of TRUE is
			  * returned if the object was found and removed. Otherwise, FALSE
			  * is returned. If the object was removed successfully, the object
			  * bounding volume hierarchy is rebuilt, incurring the cost of this
			  * operation.
			  * 
			  * @param object - an object to remove from the sound scene.
			  * @return whether or not the object was successfully removed.
			  */
			Bool removeObject( SoundObject* object );
			
			
			
			
			/// Remove all objects from this sound scene.
			/**
			  * This method removes all objects in this scene in constant time
			  * and then deallocates any bounding volume hierarchy for the objects
			  * that previously existed.
			  */
			void removeAllObjects();
			
			
			
			
			/// Get a pointer to the object with the specified index.
			/**
			  * If the given index is greater than or equal to the number of
			  * objects in the scene, a NULL pointer is returned. Otherwise,
			  * a pointer to the object at the specified index in an internal
			  * array is returned.
			  * 
			  * @param objectIndex - the index of the object to be queried.
			  * @return a pointer to the object at the given index.
			  */
			GSOUND_INLINE SoundObject* getObject( Index objectIndex ) const
			{
				return objectIndex < objects.getSize() ? objects[objectIndex] : NULL;
			}
			
			
			
			
			/// Get the number of objects in this sound scene.
			GSOUND_FORCE_INLINE Size getNumberOfObjects() const
			{
				return objects.getSize();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Object BVH Accessor Methods
			
			
			
			
			/// Get a const pointer to the root node of the object bounding volume hierarchy for the scene.
			/**
			  * If there are no objects in the scene, a NULL pointer is returned.
			  * 
			  * @return a const pointer to the root node of the object bounding volume heirarchy or NULL.
			  */
			GSOUND_INLINE const ObjectBVHType* getObjectBVH() const
			{
				rebuildObjectBVH();
				
				return objectBVH;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Speed of Sound Accessor Methods
			
			
			
			
			/// Get the speed of sound for this scene in world units per second.
			/**
			  * @return the current speed of sound in this scene in world units per second.
			  */
			GSOUND_FORCE_INLINE Real getSpeedOfSound() const
			{
				return speedOfSound;
			}
			
			
			
			
			/// Set the speed of sound for this scene in world units per second.
			/**
			  * The new speed of sound is clamped to the range of [0,+infinity].
			  * 
			  * @param newSpeedOfSound - the new speed of sound to use in this scene.
			  */
			GSOUND_INLINE void setSpeedOfSound( Real newSpeedOfSound )
			{
				speedOfSound = math::max( newSpeedOfSound, Real(0) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	User Data Accessor Methods
			
			
			
			
			/// Get an opaque pointer to user-defined data for this SoundScene.
			GSOUND_FORCE_INLINE void* getUserData() const
			{
				return userData;
			}
			
			
			
			
			/// Get an opaque pointer to user-defined data for this SoundScene.
			GSOUND_FORCE_INLINE void setUserData( void* newUserData )
			{
				userData = newUserData;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Methods
			
			
			
			
			/// Rebuild the bounding volume hierarchy of objects in the scene.
			void rebuildObjectBVH() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// An array list of pointers to all sources in this scene.
			ArrayList<SoundSource*> sources;
			
			
			
			
			/// An array list of pointers to all objects in this scene.
			ArrayList<SoundObject*> objects;
			
			
			
			
			/// A pointer to the root node of the bounding volume hierarchy for the objects in this scene.
			mutable ObjectBVHType* objectBVH;
			
			
			
			
			/// The speed of sound in this scene, specified in world units per second.
			Real speedOfSound;
			
			
			
			
			/// An opaque pointer to user-defined data for this SoundScene.
			void* userData;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_SCENE_H
