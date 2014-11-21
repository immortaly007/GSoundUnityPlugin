/*
 * Project:     GSound
 * 
 * File:        gsound/internal/BoundingSphere.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::BoundingSphere class declaration
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


#ifndef INCLUDE_GSOUND_BOUNDING_SPHERE_H
#define INCLUDE_GSOUND_BOUNDING_SPHERE_H


#include "GSoundInternalConfig.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which represents a sphere that bounds a more complex geometric structure.
/**
  * Typically, this class will be used as part of a bounding volume hierarchy to
  * bound complex point or triangle data.
  */
class BoundingSphere
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a BoundingSphere object that has a radius of 0 and a position centered at the origin.
			GSOUND_INLINE BoundingSphere()
				:	radius( 0 ),
					position()
			{
			}
			
			
			
			
			/// Create a BoundingSphere object with the specified center position and radius.
			/**
			  * @param newPosition - the position of the center of the BoundingSphere in 3D space.
			  * @param newRadius - the radius of the BoundingSphere.
			  */
			GSOUND_INLINE BoundingSphere( const Vector3& newPosition, Real newRadius )
				:	radius( newRadius ),
					position( newPosition )
			{
			}
			
			
			
			
			/// Create a BoundingSphere object which tightly bounds the specified 3 points in space.
			/**
			  * Typically, this constructor is used to easily create a bounding sphere for a triangle
			  * when constructing a bounding volume hierarchy.
			  * 
			  * @param a - a point that should be enclosed by the BoundingSphere.
			  * @param b - a point that should be enclosed by the BoundingSphere.
			  * @param c - a point that should be enclosed by the BoundingSphere.
			  */
			BoundingSphere( const Vector3& a, const Vector3& b, const Vector3& c );
			
			
			
			
			/// Create a BoundingSphere object which encloses all of the points in the specified list.
			/**
			  * This constructor uses an implementation-defined method of generating a bounding sphere
			  * for an arbitrary set of points in 3D space. The resulting sphere is not guaranteed to 
			  * be a minimal bounding sphere but should offer a reasonably-tight fit of the input
			  * point set. If the number of input points is 0, the resulting BoundingSphere will
			  * be a sphere of 0 radius centered at the origin.
			  * 
			  * @param points - the points which this bounding sphere should enclose.
			  */
			GSOUND_INLINE BoundingSphere( const ArrayList<Vector3>& points )
			{
				computeApproximateBoundingSphere( points.getArrayPointer(), points.getSize(), position, radius );
			}
			
			
			
			
			/// Create a BoundingSphere object which encloses all of the points in the specified array.
			/**
			  * This constructor uses an implementation-defined method of generating a bounding sphere
			  * for an arbitrary set of points in 3D space. The resulting sphere is not guaranteed to 
			  * be a minimal bounding sphere but should offer a reasonably-tight fit of the input
			  * point set. The constructor uses numPoints sequential points from the array of input points
			  * to construct the bounding sphere. If the number of input points is 0 or the point array
			  * is NULL, the resulting BoundingSphere will be a sphere of 0 radius centered at the origin.
			  * 
			  * @param points - a pointer to an array of input point data.
			  * @param numPoints - the number of sequential points to use from the point array.
			  */
			GSOUND_INLINE BoundingSphere( const Vector3* points, Size numPoints )
			{
				if ( points != NULL )
					computeApproximateBoundingSphere( points, numPoints, position, radius );
				else
					radius = Real(0);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Intersection Realest Method
			
			
			
			
			/// Return whether or not this BoundingSphere intersects another.
			/**
			  * If the spheres intersect, TRUE is returned. Otherwise, FALSE is 
			  * returned.
			  * 
			  * @param sphere - the sphere to test for intersection with this BoundingSphere.
			  * @return whether or not this BoundingSphere intersects another.
			  */
			GSOUND_INLINE Bool intersects( const BoundingSphere& sphere ) const
			{
				register Real distanceSquared = position.getDistanceToSquared( sphere.position );
				register Real radii = radius + sphere.radius;
				
				return distanceSquared < radii*radii;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Bounding Sphere Union Operator
			
			
			
			
			/// Compute the union of this bounding sphere with another.
			/**
			  * The resulting BoundingSphere object is guaranteed to tightly
			  * bound both the L-value BoundingSphere object as well as the R-value
			  * BoundingSphere.
			  * 
			  * @param sphere - the sphere to union with this BoundingSphere.
			  * @return a BoundingSphere which encloses both spheres.
			  */
			BoundingSphere operator + ( const BoundingSphere& sphere ) const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// The radius of this BoundingSphere.
			Real radius;
			
			
			
			
			/// The position of the center of this BoundingSphere.
			Vector3 position;
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Helper Method
			
			
			
			
			/// Compute the approximate bounding sphere of a set of input points.
			static void computeApproximateBoundingSphere( const Vector3* points, Size numPoints,
														Vector3& position,
														Real& radius );
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_BOUNDING_SPHERE_H
