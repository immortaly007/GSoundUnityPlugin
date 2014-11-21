/*
 * Project:     GSound
 * 
 * File:        gsound/math/Transformation3D.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::Transformation3D class declaration
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


#ifndef INCLUDE_GSOUND_TRANSFORMATION_3D_H
#define INCLUDE_GSOUND_TRANSFORMATION_3D_H


#include "GSoundMathConfig.h"


#include "ScalarMath.h"
#include "Vector3D.h"
#include "Matrix3D.h"
#include "Ray3D.h"
#include "Plane3D.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which represents a 3-dimensional rigid transformation.
/**
  * The transformation is composed of translation, rotation, and uniform scaling.
  * The components are assumed to be in the following order: translation, rotation,
  * and scaling. Thus, when transforming a point from world to object space by the
  * transformation, translation is first applied, followed by scaling, and finally
  * rotation. The reverse holds true for transformations from object to world space.
  */
template < typename T >
class Transformation3D
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create an identity transformation that doesn't modify transformed points.
			GSOUND_FORCE_INLINE Transformation3D()
				:	position( Vector3D<T>() ),
					orientation( Matrix3D<T>::IDENTITY ),
					scale( 1 )
			{
			}
			
			
			
			
			/// Create a transformation with the specified translation and no rotation or scaling.
			GSOUND_FORCE_INLINE Transformation3D( const Vector3D<T>& newPosition )
				:	position( newPosition ),
					orientation( Matrix3D<T>::IDENTITY ),
					scale( 1 )
			{
			}
			
			
			
			
			/// Create a transformation with the specified translation, rotation, and no scaling.
			GSOUND_FORCE_INLINE Transformation3D( const Vector3D<T>& newPosition, 
									const Matrix3D<T>& newOrientation )
				:	position( newPosition ),
					orientation( newOrientation ),
					scale( 1 )
			{
			}
			
			
			
			
			/// Create a transformation with the specified translation, rotation, and uniform scaling.
			GSOUND_FORCE_INLINE Transformation3D(	const Vector3D<T>& newPosition, 
										const Matrix3D<T>& newOrientation, 
										T newScale )
				:	position( newPosition ),
					orientation( newOrientation ),
					scale( newScale )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Object Space Transformations
			
			
			
			
			/// Transform the specified scalar value to object space.
			/**
			  * This will perform any scaling necessary to satisfy the transformation.
			  */
			GSOUND_FORCE_INLINE T transformToObjectSpace( T original ) const
			{
				return original/scale;
			}
			
			
			
			
			/// Transform the specified position vector to object space.
			GSOUND_FORCE_INLINE Vector3D<T> transformToObjectSpace( const Vector3D<T>& original ) const
			{
				return ((original - position)*orientation)/scale;
			}
			
			
			
			
			/// Transform the specified matrix to object space.
			/**
			  * This returns what the specified matrix would be in this transformation's
			  * coordinate frame. This method does not perform any scaling on the input
			  * matrix. The input matrix is assumed to be an orthonormal rotation matrix.
			  */
			GSOUND_FORCE_INLINE Matrix3D<T> transformToObjectSpace( const Matrix3D<T>& original ) const
			{
				return original*orientation;
			}
			
			
			
			
			/// Transform the specified ray into object space.
			/**
			  * This method performs a standard vector transformation for the ray origin
			  * and only rotates the ray direction, preserving the length of the ray's direction
			  * vector.
			  */
			GSOUND_FORCE_INLINE Ray3D<T> transformToObjectSpace( const Ray3D<T>& ray ) const
			{
				return Ray3D<T>( ((ray.origin - position)*orientation)/scale, ray.direction*orientation );
			}
			
			
			
			
			/// Rotate the specified vector to object space.
			/**
			  * This method does not perform any translation or scaling on the
			  * input point. This function is ideal for transforming directional
			  * quantities like surface normal vectors.
			  */
			GSOUND_FORCE_INLINE Vector3D<T> rotateToObjectSpace( const Vector3D<T>& original ) const
			{
				return original*orientation;
			}
			
			
			
			
			/// Scale a vector to object space.
			GSOUND_FORCE_INLINE Vector3D<T> scaleToObjectSpace( const Vector3D<T>& original ) const
			{
				return original/scale;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	World Space Transformations
			
			
			
			
			/// Transform the specified scalar value to world space.
			/**
			  * This will perform any scaling necessary to satisfy the transformation.
			  */
			GSOUND_FORCE_INLINE T transformToWorldSpace( T original ) const
			{
				return original*scale;
			}
			
			
			
			
			/// Transform the specified position vector to world space.
			GSOUND_FORCE_INLINE Vector3D<T> transformToWorldSpace( const Vector3D<T>& original ) const
			{
				return position + (orientation*original)*scale;
			}
			
			
			
			
			/// Transform the specified matrix to world space.
			/**
			  * This returns what the specified matrix would be in this transformation's
			  * coordinate frame. This method does not perform any scaling on the input
			  * matrix. The input matrix is assumed to be an orthonormal rotation matrix.
			  */
			GSOUND_FORCE_INLINE Matrix3D<T> transformToWorldSpace( const Matrix3D<T>& original ) const
			{
				return orientation*original;
			}
			
			
			
			
			/// Transform the specified ray into world space.
			/**
			  * This method performs a standard vector transformation for the ray origin
			  * and only rotates the ray direction, preserving the length of the ray's direction
			  * vector.
			  */
			GSOUND_FORCE_INLINE Ray3D<T> transformToWorldSpace( const Ray3D<T>& ray ) const
			{
				return Ray3D<T>( position + (orientation*ray.origin)*scale, orientation*ray.direction );
			}
			
			
			
			
			/// Transform the specified plane into world space.
			/**
			  * This method rotates the normal of the plane into the new coordinate frame
			  * and calculates a new offset for the plane based on the projection of the origin
			  * onto the plane in object space transformed to world space.
			  */
			GSOUND_FORCE_INLINE Plane3D<T> transformToWorldSpace( const Plane3D<T>& plane ) const
			{
				return Plane3D<T>( orientation*plane.normal, this->transformToWorldSpace(-plane.offset*plane.normal) );
			}
			
			
			
			
			/// Rotate the specified vector to world space.
			/**
			  * This method does not perform any translation or scaling on the
			  * input point. This function is ideal for transforming directional
			  * quantities like surface normal vectors.
			  */
			GSOUND_FORCE_INLINE Vector3D<T> rotateToWorldSpace( const Vector3D<T>& original ) const
			{
				return orientation*original;
			}
			
			
			
			
			/// Scale a vector to world space.
			GSOUND_FORCE_INLINE Vector3D<T> scaleToWorldSpace( const Vector3D<T>& original ) const
			{
				return original*scale;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// The translation component of the rigid transformation.
			Vector3D<T> position;
			
			
			
			
			/// The rotation component of the rigid transformation.
			Matrix3D<T> orientation;
			
			
			
			
			/// The scaling component of the rigid transformation.
			T scale;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		3D Transformation Type Definitions
//############		
//##########################################################################################
//##########################################################################################




typedef Transformation3D<int>		Transformation3i;
typedef Transformation3D<float>		Transformation3f;
typedef Transformation3D<double>	Transformation3d;




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_GSOUND_TRANSFORMATION_3D_H
