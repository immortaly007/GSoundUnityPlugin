/*
 * Project:     GSound
 * 
 * File:        gsound/math/Plane3D.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::Plane3D class declaration
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


#ifndef INCLUDE_GSOUND_PLANE_3D_H
#define INCLUDE_GSOUND_PLANE_3D_H


#include "GSoundMathConfig.h"


#include "ScalarMath.h"
#include "Vector3D.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which represents a plane in 3D space.
/**
  * It uses the normal and offset plane representation as it is the most universally
  * useful in computational mathematics, especially relating to graphics and geometry.
  */
template < typename T >
class Plane3D
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a plane in 3D space with the normal pointing along the positive Z axis with offset = 0.
			GSOUND_FORCE_INLINE Plane3D()
				:	normal( 0, 0, 1 ),
					offset( 0 )
			{
			}
			
			
			
			
			/// Create a plane in 3D space with the specified normal and offset from the origin.
			GSOUND_FORCE_INLINE Plane3D( const Vector3D<T>& planeNormal, T planeOffset )
				:	normal( planeNormal ),
					offset( planeOffset )
			{
			}
			
			
			
			
			/// Create a plane in 3D space from the specified normal and point on the plane.
			GSOUND_FORCE_INLINE Plane3D( const Vector3D<T>& planeNormal, const Vector3D<T>& pointOnPlane )
				:	normal( planeNormal ),
					offset( -math::dot( pointOnPlane, normal ) )
			{
			}
			
			
			
			
			/// Create a plane in 3D space from three points in that plane.
			GSOUND_FORCE_INLINE Plane3D( const Vector3D<T>& p1, const Vector3D<T>& p2, const Vector3D<T>& p3 )
				:	normal( math::cross( p2 - p1, p3 - p1 ).normalize() )
			{
				offset = -math::dot( p1, normal );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Point Distance Methods
			
			
			
			
			/// Get the perpendicular distance from the specified point to the plane.
			GSOUND_FORCE_INLINE T getDistanceTo( const Vector3D<T>& point ) const
			{
				return math::abs( math::dot( normal, point ) + offset );
			}
			
			
			
			
			/// Get the perpendicular distance from the specified point to the plane.
			GSOUND_FORCE_INLINE T getSignedDistanceTo( const Vector3D<T>& point ) const
			{
				return math::dot( normal, point ) + offset;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Point Projection Methods
			
			
			
			
			/// Return the projection of the given point onto the plane.
			GSOUND_FORCE_INLINE Vector3D<T> getProjection( const Vector3D<T>& point ) const
			{
				T t = getSignedDistanceTo(point) / math::dot( normal, normal );
				return point - t*normal;
			}
			
			
			
			
			/// Return the projection of the given point onto the plane.
			/**
			  * The plane is assumed to have a normal vector of unit length. This
			  * results in a significantly faster function, however the results are
			  * meaningless if the precondition is not met.
			  */
			GSOUND_FORCE_INLINE Vector3D<T> getProjectionNormalized( const Vector3D<T>& point ) const
			{
				return point - getSignedDistanceTo(point)*normal;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Point Reflection Methods
			
			
			
			
			/// Get the reflection of a point over the plane.
			GSOUND_FORCE_INLINE Vector3D<T> getReflection( const Vector3D<T>& point ) const
			{
				T t = getSignedDistanceTo(point) / math::dot( normal, normal );
				return point - T(2)*t*normal;
			}
			
			
			
			
			/// Get the reflection of a point over the plane.
			GSOUND_FORCE_INLINE Vector3D<T> getReflectionNormalized( const Vector3D<T>& point ) const
			{
				return point - T(2)*getSignedDistanceTo(point)*normal;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Plane Normalization Method
			
			
			
			
			/// Normalize the plane's normal vector and correct the offset to match.
			GSOUND_FORCE_INLINE Plane3D normalize() const
			{
				T inverseMagnitude = T(1)/normal.getMagnitude();
				
				return Plane3D( normal*inverseMagnitude, offset*inverseMagnitude );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Plane Inversion Operator
			
			
			
			
			/// Return the plane with the opposite normal vector and offset.
			/** 
			  * This plane is mathematically the same as the original plane.
			  */
			GSOUND_FORCE_INLINE Plane3D operator - () const
			{
				return Plane3D( -normal, -offset );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// A vector perpendicular to the plane.
			Vector3D<T> normal;
			
			
			
			
			/// The distance that the plane is offset from the origin.
			T offset;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		3D Plane Type Definitions
//############		
//##########################################################################################
//##########################################################################################




typedef Plane3D<int> Plane3i;
typedef Plane3D<float> Plane3f;
typedef Plane3D<double> Plane3d;




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PLANE_3D_H
