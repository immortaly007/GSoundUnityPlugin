/*
 * Project:     GSound
 * 
 * File:        gsound/math/AABB3D.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::AABB3D class declaration
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


#ifndef INCLUDE_GSOUND_AABB_3D_H
#define INCLUDE_GSOUND_AABB_3D_H


#include "GSoundMathConfig.h"


#include "Vector3D.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which represents a range of values in 3D space.
/**
  * This class contains two data members: min and max. These indicate the minimum
  * and maximum coordinates that this axis-aligned bounding box represents. The class
  * invariant is that min is less than max (on at least one dimension), though this
  * is not enforced. The class supports union, containment, and intersection operations.
  */
template < typename T >
class AABB3D
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a 3D axis-aligned bounding box with no extent centered about the origin.
			GSOUND_FORCE_INLINE AABB3D()
				:	min(),
					max()
			{
			}
			
			
			
			
			/// Create a 3D axis-aligned bounding box with the specified minimum and maximum coodinates.
			GSOUND_FORCE_INLINE AABB3D(	T newXMin, T newXMax, 
							T newYMin, T newYMax, 
							T newZMin, T newZMax )
				:	min( newXMin, newYMin, newZMin ),
					max( newXMax, newYMax, newZMax )
			{
			}
			
			
			
			
			/// Create a 3D axis-aligned bounding box with the specified minimum and maximum coodinates.
			GSOUND_FORCE_INLINE AABB3D(	const Vector3D<T>& newMin, const Vector3D<T>& newMax )
				:	min( newMin ),
					max( newMax )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	AABB Cast Operator
			
			
			
			
			/// Cast this bounding box to a bounding box with a different underlying primitive type.
			template < typename U >
			GSOUND_FORCE_INLINE operator AABB3D<U> () const
			{
				return AABB3D<U>( (U)min.x, (U)max.x, (U)min.y, (U)max.y, (U)min.z, (U)max.z );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	AABB Comparison Methods
			
			
			
			
			/// Return whether or not this bounding box completely contains another.
			GSOUND_FORCE_INLINE Bool contains( const AABB3D& bounds )
			{
				return	min.x <= bounds.min.x && max.x >= bounds.max.x && 
						min.y <= bounds.min.y && max.y >= bounds.max.y && 
						min.z <= bounds.min.z && max.z >= bounds.max.z;
			}
			
			
			
			
			/// Return whether or not this bounding box contains the specified coordinate.
			GSOUND_FORCE_INLINE Bool contains( const Vector3D<T>& coordinate )
			{
				return coordinate.x >= min.x && coordinate.x <= max.x &&
						coordinate.y >= min.y && coordinate.y <= max.y &&
						coordinate.z >= min.z && coordinate.z <= max.z;
			}
			
			
			
			
			/// Return whether or not this bounding box intersects another.
			GSOUND_FORCE_INLINE Bool intersects( const AABB3D& bounds )
			{
				return	(min.x < bounds.max.x) && (max.x > bounds.min.x) && 
						(min.y < bounds.max.y) && (max.y > bounds.min.y) && 
						(min.z < bounds.max.z) && (max.z > bounds.min.z);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Accessor Methods
			
			
			
			
			/// Set the minimum and maximum coordinates of the axis-aligned bounding box.
			GSOUND_FORCE_INLINE void set( T newXMin, T newXMax, T newYMin, T newYMax, T newZMin, T newZMax )
			{
				min.set( newXMin, newYMin, newZMin );
				max.set( newXMax, newYMax, newZMax );
			}
			
			
			
			
			/// Get the different between the maximum and minimum X coordinates.
			GSOUND_FORCE_INLINE T getWidth() const
			{
				return max.x - min.x;
			}
			
			
			
			
			/// Get the different between the maximum and minimum Y coordinates.
			GSOUND_FORCE_INLINE T getHeight() const
			{
				return max.y - min.y;
			}
			
			
			
			
			/// Get the different between the maximum and minimum Z coordinates.
			GSOUND_FORCE_INLINE T getDepth() const
			{
				return max.z - min.z;
			}
			
			
			
			
			/// Get the vector from the minimum coordinate to the maximum.
			GSOUND_FORCE_INLINE Vector3D<T> getDiagonal() const
			{
				return max - min;
			}
			
			
			
			
			/// Get the center of the bounding box.
			GSOUND_FORCE_INLINE Vector3D<T> getCenter() const
			{
				return math::midpoint( min, max );
			}
			
			
			
			
			/// Get the volume in cubic units enclosed by this 3D range.
			GSOUND_FORCE_INLINE T getVolume() const
			{
				return this->getWidth()*this->getHeight()*this->getDepth();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Enlargement Methods
			
			
			
			/// Modify the current bounding box such that it encloses the specified point.
			GSOUND_FORCE_INLINE void enlargeFor( const Vector3D<T>& point )
			{
				if ( point.x < min.x ) min.x = point.x;
				else if ( point.x > max.x ) max.x = point.x;
				
				if ( point.y < min.y ) min.y = point.y;
				else if ( point.y > max.y ) max.y = point.y;
				
				if ( point.z < min.z ) min.z = point.z;
				else if ( point.z > max.z ) max.z = point.z;
			}
			
			
			
			
			/// Modify the current bounding box such that it encloses the specified point.
			GSOUND_FORCE_INLINE AABB3D<T>& operator += ( const Vector3D<T>& point )
			{
				if ( point.x < min.x ) min.x = point.x;
				else if ( point.x > max.x ) max.x = point.x;
				
				if ( point.y < min.y ) min.y = point.y;
				else if ( point.y > max.y ) max.y = point.y;
				
				if ( point.z < min.z ) min.z = point.z;
				else if ( point.z > max.z ) max.z = point.z;
				
				return *this;
			}
			
			
			
			
			/// Return the bounding box necessary to enclose a point and the current bounding box.
			GSOUND_FORCE_INLINE AABB3D<T> operator + ( const Vector3D<T>& point ) const
			{
				return AABB3D<T>(	point.x < min.x ? point.x : min.x,
									point.x > max.x ? point.x : max.x,
									point.y < min.y ? point.y : min.y,
									point.y > max.y ? point.y : max.y,
									point.z < min.z ? point.z : min.z,
									point.z > max.z ? point.z : max.z );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Union Methods
			
			
			
			
			/// Return the union of this bounding box and another.
			GSOUND_FORCE_INLINE AABB3D<T> getUnion( const AABB3D<T>& bounds ) const
			{
				return AABB3D<T>(	bounds.min.x < min.x ? bounds.min.x : min.x,
									bounds.max.x > max.x ? bounds.max.x : max.x,
									bounds.min.y < min.y ? bounds.min.y : min.y,
									bounds.max.y > max.y ? bounds.max.y : max.y,
									bounds.min.z < min.z ? bounds.min.z : min.z,
									bounds.max.z > max.z ? bounds.max.z : max.z );
			}
			
			
			
			
			/// Modify this bounding box such that it contains the specified bounding box.
			GSOUND_FORCE_INLINE AABB3D<T>& operator += ( const AABB3D<T>& bounds )
			{
				if ( bounds.min.x < min.x )
					min.x = bounds.min.x;
				
				if ( bounds.max.x > max.x )
					max.x = bounds.max.x;
				
				if ( bounds.min.y < min.y )
					min.y = bounds.min.y;
				
				if ( bounds.max.y > max.y )
					max.y = bounds.max.y;
				
				if ( bounds.min.z < min.z )
					min.z = bounds.min.z;
				
				if ( bounds.max.z > max.z )
					max.z = bounds.max.z;
				
				return *this;
			}
			
			
			
			
			/// Return the union of this bounding box and another.
			GSOUND_FORCE_INLINE AABB3D<T> operator + ( const AABB3D<T>& bounds ) const
			{
				return AABB3D<T>(	bounds.min.x < min.x ? bounds.min.x : min.x,
									bounds.max.x > max.x ? bounds.max.x : max.x,
									bounds.min.y < min.y ? bounds.min.y : min.y,
									bounds.max.y > max.y ? bounds.max.y : max.y,
									bounds.min.z < min.z ? bounds.min.z : min.z,
									bounds.max.z > max.z ? bounds.max.z : max.z );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Data Members
			
			
			
			
			/// The minumum coordinate of the bounding box.
			Vector3D<T> min;
			
			
			
			
			/// The maximum coordinate of the bounding box.
			Vector3D<T> max;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		3D Bounding Box Type Definitions
//############		
//##########################################################################################
//##########################################################################################




typedef AABB3D<int>			AABB3i;
typedef AABB3D<float>		AABB3f;
typedef AABB3D<double>		AABB3d;




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_AABB_3D_H
