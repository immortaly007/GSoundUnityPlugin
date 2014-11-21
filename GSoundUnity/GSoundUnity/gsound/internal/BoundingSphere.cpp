/*
 * Project:     GSound
 * 
 * File:        gsound/internal/BoundingSphere.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::BoundingSphere class implementation
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


#include "BoundingSphere.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




BoundingSphere:: BoundingSphere( const Vector3& a, const Vector3& b, const Vector3& c )
{
	Real dotABAB = math::dot(b - a, b - a);
	Real dotABAC = math::dot(b - a, c - a);
	Real dotACAC = math::dot(c - a, c - a);
	Real d = Real(2)*(dotABAB*dotACAC - dotABAC*dotABAC);
	Vector3 referencePt = a;
	
	if ( math::abs(d) <= math::epsilon<Real>() )
	{
		// a, b, and c lie on a line. Sphere center is center of AABB of the
		// points, and radius is distance from circle center to AABB corner
		AABB3 boundingBox( a, a );
		boundingBox += b;
		boundingBox += c;
		
		position = boundingBox.getCenter();
		referencePt = boundingBox.min;
	}
	else
	{
		Real inverseD = Real(1) / d;
		Real s = (dotABAB*dotACAC - dotACAC*dotABAC) * inverseD;
		Real t = (dotACAC*dotABAB - dotABAB*dotABAC) * inverseD;
		
		// s controls height over AC, t over AB, (1-s-t) over BC
		if ( s <= Real(0) )
			position = math::midpoint(a,c);
		else if ( t <= Real(0) )
			position = math::midpoint(a,b);
		else if ( s + t >= Real(1) )
		{
			position = math::midpoint(b,c);
			referencePt = b;
		}
		else
			position = a + s*(b - a) + t*(c - a);
	}
	
	radius = math::sqrt( math::dot(position - referencePt, position - referencePt) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Bounding Sphere Union Operator
//############		
//##########################################################################################
//##########################################################################################




BoundingSphere BoundingSphere:: operator + ( const BoundingSphere& sphere ) const
{
	// Compute the squared distance between the sphere centers.
	Vector3 d = sphere.position - position;
	Real distanceSquared = d.getMagnitudeSquared();
	Real radiusDiff = sphere.radius - radius;
	
	if ( radiusDiff*radiusDiff > distanceSquared )
	{
		// The sphere with the larger radius encloses the other.
		// Return the larger of these two spheres.
		if ( sphere.radius > radius )
			return sphere;
		else
			return *this;
	}
	else
	{
		// The spheres are partially overlapping or disjoint.
		Real distance = math::sqrt(distanceSquared);
		Real newRadius = (distance + radius + sphere.radius) * Real(0.5);
		
		return BoundingSphere( distance > math::epsilon<Real>() ? position + ((newRadius - radius) / distance)*d
															: position,
								newRadius );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Approximate Bounding Sphere Computation Method
//############		
//##########################################################################################
//##########################################################################################




void BoundingSphere:: computeApproximateBoundingSphere( const Vector3* points, Size numPoints,
														Vector3& position, Real& radius )
{
	//****************************************************************************
	// First, find the most extreme points along the 3 principle axes.
	
	math::AABB3D<Index> extremePoints;
	
	for ( Index i = 1; i < numPoints; i++ )
	{
		if ( points[i].x < points[extremePoints.min.x].x )
			extremePoints.min.x = i;
		
		if ( points[i].x > points[extremePoints.max.x].x )
			extremePoints.max.x = i;
		
		if ( points[i].y < points[extremePoints.min.y].y )
			extremePoints.min.y = i;
		
		if ( points[i].y > points[extremePoints.max.y].y )
			extremePoints.max.y = i;
		
		if ( points[i].z < points[extremePoints.min.z].z )
			extremePoints.min.z = i;
		
		if ( points[i].z > points[extremePoints.max.z].z )
			extremePoints.max.z = i;
	}
	
	
	//****************************************************************************
	// Next, find the pair of min-max points most distant from each other
	
	Real xDistanceSquared = points[extremePoints.min.x].getDistanceToSquared( points[extremePoints.max.x] );
	Real yDistanceSquared = points[extremePoints.min.y].getDistanceToSquared( points[extremePoints.max.y] );
	Real zDistanceSquared = points[extremePoints.min.z].getDistanceToSquared( points[extremePoints.max.z] );
	
	Index minIndex = extremePoints.min.x;
	Index maxIndex = extremePoints.max.x;
	Real extremePointsDistanceSquared = xDistanceSquared;
	
	if ( yDistanceSquared > xDistanceSquared && yDistanceSquared > zDistanceSquared )
	{
		minIndex = extremePoints.min.y;
		maxIndex = extremePoints.max.y;
		extremePointsDistanceSquared = yDistanceSquared;
	}
	else if ( zDistanceSquared > xDistanceSquared && zDistanceSquared > yDistanceSquared )
	{
		minIndex = extremePoints.min.z;
		maxIndex = extremePoints.max.z;
		extremePointsDistanceSquared = zDistanceSquared;
	}
	
	
	//****************************************************************************
	// Next, make an early approximation of the sphere which just encompases
	// these two extreme points.
	
	position = midpoint( points[minIndex], points[maxIndex] );
	radius = math::sqrt( extremePointsDistanceSquared ) / Real(2);
	
	//****************************************************************************
	// Next, refine this bounding sphere so that it encompases all of the points
	
	for ( Index i = 0; i < numPoints; i++ )
	{
		// Compute squared distance between point and sphere's center
		
		Vector3 centerRealoPoint = points[i] - position;
		
		Real pointDistanceSquared = centerRealoPoint.getMagnitudeSquared();
		
		// if the point is outside of the sphere, then we need to update the 
		// radius and position of the sphere, otherwise go on to next point.
		
		if ( pointDistanceSquared > radius*radius )
		{
			Real pointDistance = math::sqrt( pointDistanceSquared );
			Real newRadius = math::average( pointDistance, radius );
			
			Real positionChange = (newRadius - radius) / pointDistance;
			
			// update the sphere to contain the new point.
			radius = newRadius;
			position += positionChange * centerRealoPoint;
		}
	}
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
