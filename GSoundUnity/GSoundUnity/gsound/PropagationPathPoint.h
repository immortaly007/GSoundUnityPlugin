/*
 * Project:     GSound
 * 
 * File:        gsound/PropagationPathPoint.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::PropagationPathPoint class declaration
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


#ifndef INCLUDE_GSOUND_PROPAGATION_PATH_POINT_H
#define INCLUDE_GSOUND_PROPAGATION_PATH_POINT_H


#include "GSoundBase.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class used to uniquely identify a point on a sound propagation path.
/**
  * Every possible type of sound-scene interaction can be represented by this class.
  */
class PropagationPathPoint
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Propagation Path Point Type Enum
			
			
			
			
			/// An enum specifying the type of a point on a propagation path.
			typedef enum Type
			{
				/// The path point represents a sound source.
				SOURCE = 0,
				
				/// The path point represents a sound listener.
				LISTENER = 1,
				
				/// The path point represents a specular triangle reflection.
				TRIANGLE_REFLECTION = 2,
				
				/// The path point represents a diffuse triangle reflection.
				DIFFUSE_REFLECTION = 3,
				
				/// The path point represents diffraction over a triangle's edge.
				EDGE_DIFFRACTION = 4
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			/// Create a new propagation path point with the specified type and opaque object pointer.
			GSOUND_INLINE PropagationPathPoint( const Type& newType, const void* newObject )
				:	type( newType ),
					object( newObject )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Data Accessor Methods
			
			
			
			
			/// Get the type of this propagation path point.
			GSOUND_INLINE const Type& getType() const
			{
				return type;
			}
			
			
			
			
			/// Get an opaque pointer to an object associated with this path point.
			/**
			  * This pointer, along with the type enum, is used as a unique identifier to the 
			  * scene interaction that has occurred. If the path point's type is SOURCE, the
			  * pointer should point to a SoundSource object. A listener path point should point
			  * to a SoundListener object, and so on. However, one should not assume that 
			  * this is always true. The only assumption that can be made is that the pointer
			  * is a unique identifier to the object being interacted with in the scene.
			  */
			GSOUND_INLINE const void* getObject() const
			{
				return object;
			}
			
			
			
			
			/// Get an integral hash code for this propagation path point.
			Hash getHashCode() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Equality Comparison Operators
			
			
			
			
			/// Return whether or not this propagation path is equal to another.
			GSOUND_INLINE Bool operator == ( const PropagationPathPoint& other ) const
			{
				return type == other.type && object == other.object;
			}
			
			
			
			
			/// Return whether or not this propagation path is not equal to another.
			GSOUND_INLINE Bool operator != ( const PropagationPathPoint& other ) const
			{
				return type != other.type || object != other.object;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Hashing Methods
			
			
			
			GSOUND_INLINE static Hash getPointerHash( const void* pointer )
			{
				return Hash(UInt64(pointer) >> 2)*27;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// An opaque pointer to an object associated with this path point.
			/**
			  * This pointer, along with the type enum, is used as a unique identifier to the 
			  * scene interaction that has occurred. If the path point's type is SOURCE, the
			  * pointer should point to a SoundSource object. A listener path point should point
			  * to a SoundListener object, and so on. However, one should not assume that 
			  * this is always true. The only assumption that can be made is that the pointer
			  * is a unique identifier to the object being interacted with in the scene.
			  */
			const void* object;
			
			
			
			
			/// The type of this propagation path point.
			Type type;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PROPAGATION_PATH_POINT_H
