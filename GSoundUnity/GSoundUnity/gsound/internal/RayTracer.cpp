/*
 * Project:     GSound
 * 
 * File:        gsound/internal/RayTracer.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::RayTracer class implementation
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


#include "RayTracer.h"


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




RayTracer:: RayTracer()
	:	objectBVH( NULL ),
		stackSize( TRAVERSAL_STACK_DEPTH ),
		stack( util::allocate<const void*>( TRAVERSAL_STACK_DEPTH ) )
{
}




RayTracer:: RayTracer( const ObjectBVHType* newObjectBVH )
	:	objectBVH( newObjectBVH ),
		stackSize( TRAVERSAL_STACK_DEPTH ),
		stack( util::allocate<const void*>( TRAVERSAL_STACK_DEPTH ) )
{
}




RayTracer:: RayTracer( const RayTracer& other )
	:	objectBVH( other.objectBVH ),
		stackSize( other.stackSize ),
		stack( util::allocate<const void*>( other.stackSize ) )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




RayTracer:: ~RayTracer()
{
	// Destroy the recusion-unwinding stack.
	util::deallocate( stack );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




RayTracer& RayTracer:: operator = ( const RayTracer& other )
{
	if ( this != &other )
	{
		util::deallocate( stack );
		
		stackSize = other.stackSize;
		stack = util::allocate<const void*>( other.stackSize );
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Probe Ray Tracing Methods
//############		
//##########################################################################################
//##########################################################################################




Bool RayTracer:: traceProbeRay( const Ray3& ray, Real& closestIntersection, ObjectSpaceTriangle& objectTriangle )
{
	if ( objectBVH == NULL )
		return false;
	
	// Setup the first element in the stack with the root node of the object hierarchy.
	const void** stackElement = stack + 1;
	*stackElement = objectBVH;
	
	// Keep track of the parameter along the ray's direction where the closest intersection has occurred.
	closestIntersection = math::max<Real>();
	Bool foundIntersection = false;
	
	// A temporary variable used to hold the ray parameter of the ray's intersection with a BVH node.
	Real temporaryIntersectionT;
	
	// Trace ray through the object BVH tree.
	do
	{
		register ObjectBVHType* objectNode = ((ObjectBVHType*)*stackElement);
		stackElement--;
		
		// Does the ray intersect the current node, and if it does, is the intersection closer than
		// the current closest intersection?
		if ( rayIntersectsSphere( ray, objectNode->getVolume(), temporaryIntersectionT ) && 
			temporaryIntersectionT < closestIntersection )
		{
			if ( objectNode->isLeaf() )
			{
				const SoundObject* object = objectNode->getObject();
				
				foundIntersection |= traceObjectSpaceProbeRay( ray, object, (const TriangleNodeType**)stackElement,
																closestIntersection, objectTriangle );
			}
			else
			{
				// Push the right child on the stack first, then the left child.
				*(++stackElement) = objectNode->getRightChild();
				*(++stackElement) = objectNode->getLeftChild();
			}
		}
	}
	while ( stackElement != stack );
	
	return foundIntersection;
}





Bool RayTracer:: traceObjectSpaceProbeRay( const Ray3& worldSpaceRay, const SoundObject* object,
											const TriangleNodeType** stackBase,
											Real& closestIntersection, ObjectSpaceTriangle& closestTriangle )
{
	//***********************************************************************************
	// Transform the ray and the closest intersection into object space.
	
	// Get the transformation from the object.
	const Transformation3& objectTransformation = object->getTransformation();
	
	// Transform the ray and closest T.
	FatSIMDRay3 ray( objectTransformation.transformToObjectSpace( worldSpaceRay ) );
	SIMDFloat objectSpaceClosestT( objectTransformation.transformToObjectSpace( closestIntersection ) );
	
	// Declare a pointer to keep track of the closest triangle.
	const TriangleType* objectSpaceClosestTriangle = nullptr;
	
	//***********************************************************************************
	// Prepare the ray tracing stack for traversing the object's triangle BVH.
	
	// Get the root of the triangle BVH.
	const TriangleTreeType* triangleBVH = object->getMesh()->getBVH();
	
	// Get a pointer to the start of the internal triangle array so that we can index into it.
	const FatSIMDTriangle3* triangles = triangleBVH->getTriangles();
	
	// Push the root of the triangle BVH onto the stack.
	const TriangleNodeType** stackElement = stackBase + 1;
	*stackElement = triangleBVH->getRoot();
	
	// A temporary variable used to hold the ray parameter of the ray's intersection with a BVH node.
	SIMDFloat temporaryIntersectionT;
	
	// Whether or not an intersection has been encountered.
	Bool foundIntersection = false;
	
	do
	{
		const TriangleNodeType* triangleNode = *stackElement;
		stackElement--;
		
		// Test to see if the ray intersects the bounding boxes of this node's children.
		SIMDBool intersectionResults = rayIntersectsBoxSIMD( ray, triangleNode->getVolumes(), temporaryIntersectionT ); 
		
		//**************************************************************************************
		// Sort the child nodes in the order that we will visit them based on the ray's direction
		// and the split axes of the node.
		
		// Avoid looking at nodes that are farther away than intersections that are already detected.
		intersectionResults &= temporaryIntersectionT < objectSpaceClosestT;
		
		SIMDInt offsets = triangleNode->getChildOffsets();
		SIMDInt numLeafTriangles = triangleNode->getNumberOfTriangles();
		SIMDInt innerNodeOffsets = offsets & intersectionResults & (numLeafTriangles == SIMDInt(0));
		
		numLeafTriangles = numLeafTriangles & intersectionResults;
		
		//**************************************************************************************
		// Push the child nodes onto the stack in sorted order if they are not leaves.
		// If a child is a leaf, intersect its triangles with the ray.
		
		for ( Index i = 0; i < 4; i++ )
		{
			if ( innerNodeOffsets[i] )
				*(++stackElement) = triangleNode + innerNodeOffsets[i];
			else if ( numLeafTriangles[i] )
			{
				foundIntersection |= rayIntersectsTriangles( ray, triangles + offsets[i],
															numLeafTriangles[i],
															objectSpaceClosestT, objectSpaceClosestTriangle );
			}
		}
	}
	while ( stackElement != stackBase );
	
	// If there was an intersection found, transform the intersection T and triangle into world space.
	if ( foundIntersection )
	{
		closestTriangle = ObjectSpaceTriangle( objectSpaceClosestTriangle, object );
		closestIntersection = objectTransformation.transformToWorldSpace( objectSpaceClosestT[0] );
	}
	
	return foundIntersection;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Binary Occlusion Ray Tracing Methods
//############		
//##########################################################################################
//##########################################################################################




Bool RayTracer:: traceBinaryOcclusionRay( const Ray3& ray, Real tMax )
{
	Real closestIntersection;
	ObjectSpaceTriangle triangle;
	
	return traceProbeRay( ray, closestIntersection, triangle ) && closestIntersection < tMax;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Transmission Ray Tracing Method
//############		
//##########################################################################################
//##########################################################################################




Bool RayTracer:: traceTransmissionRay( const Ray3& ray, Real maxDistance, ArrayList<RayIntersection>& intersections )
{
	if ( objectBVH == NULL )
		return false;
	
	// Setup the first element in the stack with the root node of the object hierarchy.
	const void** stackElement = stack + 1;
	*stackElement = objectBVH;
	
	// Whether or not this ray has intersected any triangles.
	Bool foundIntersection = false;
	
	// A temporary variable used to hold the ray parameter of the ray's intersection with a BVH node.
	Real temporaryIntersectionT;
	
	// Trace ray through the object BVH tree.
	do
	{
		register ObjectBVHType* objectNode = ((ObjectBVHType*)*stackElement);
		stackElement--;
		
		// Does the ray intersect the current node, and if it does, is the intersection closer than
		// the maximum allowed intersection distance.
		if ( rayIntersectsSphere( ray, objectNode->getVolume(), temporaryIntersectionT ) && 
			temporaryIntersectionT < maxDistance )
		{
			if ( objectNode->isLeaf() )
			{
				const SoundObject* object = objectNode->getObject();
				
				foundIntersection |= traceObjectSpaceTransmissionRay( ray, object, (const TriangleNodeType**)stackElement,
																	maxDistance, intersections );
			}
			else
			{
				// Push the right child on the stack first, then the left child.
				*(++stackElement) = objectNode->getRightChild();
				*(++stackElement) = objectNode->getLeftChild();
			}
		}
	}
	while ( stackElement != stack );
	
	return foundIntersection;
}




Bool RayTracer:: traceObjectSpaceTransmissionRay( const Ray3& worldSpaceRay, const SoundObject* object, 
												const TriangleNodeType** stackBase,
												Real maxDistance, ArrayList<RayIntersection>& intersections )
{
	//***********************************************************************************
	// Transform the ray and the closest intersection into object space.
	
	// Get the transformation from the object.
	const Transformation3& objectTransformation = object->getTransformation();
	
	// Transform the ray and closest T.
	FatSIMDRay3 ray( objectTransformation.transformToObjectSpace( worldSpaceRay ) );
	SIMDFloat objectSpaceMaxDistance( objectTransformation.transformToObjectSpace( maxDistance ) );
	
	//***********************************************************************************
	// Prepare the ray tracing stack for traversing the object's triangle BVH.
	
	// Get the root of the triangle BVH.
	const TriangleTreeType* triangleBVH = object->getMesh()->getBVH();
	
	// Get a pointer to the start of the internal triangle array so that we can index into it.
	const FatSIMDTriangle3* triangles = triangleBVH->getTriangles();
	
	// Push the root of the triangle BVH onto the stack.
	const TriangleNodeType** stackElement = stackBase + 1;
	*stackElement = triangleBVH->getRoot();
	
	// A temporary variable used to hold the ray parameter of the ray's intersection with a BVH node.
	SIMDFloat temporaryIntersectionT;
	
	// Whether or not an intersection has been encountered.
	Bool foundIntersection = false;
	
	do
	{
		const TriangleNodeType* triangleNode = *stackElement;
		stackElement--;
		
		// Test to see if the ray intersects the bounding boxes of this node's children.
		SIMDBool intersectionResults = rayIntersectsBoxSIMD( ray, triangleNode->getVolumes(), temporaryIntersectionT ); 
		
		//**************************************************************************************
		// Sort the child nodes in the order that we will visit them based on the ray's direction
		// and the split axes of the node.
		
		// Avoid looking at nodes that are farther away than the maximum allowed distance.
		intersectionResults &= temporaryIntersectionT < objectSpaceMaxDistance;
		
		SIMDInt offsets = triangleNode->getChildOffsets();
		SIMDInt numLeafTriangles = triangleNode->getNumberOfTriangles();
		SIMDInt innerNodeOffsets = offsets & intersectionResults & (numLeafTriangles == SIMDInt(0));
		
		numLeafTriangles = numLeafTriangles & intersectionResults;
		
		//**************************************************************************************
		// Push the child nodes onto the stack in sorted order if they are not leaves.
		// If a child is a leaf, intersect its triangles with the ray.
		
		for ( Index i = 0; i < 4; i++ )
		{
			// If the child is an inner node, push it onto the stack.
			if ( innerNodeOffsets[i] )
				*(++stackElement) = triangleNode + innerNodeOffsets[i];
			else if ( numLeafTriangles[i] )
			{
				// This is a leaf node, test its triangles for intersections.
				
				const FatSIMDTriangle3* leafTriangles = triangles + offsets[i];
				const FatSIMDTriangle3* const leafTrianglesEnd = leafTriangles + numLeafTriangles[i];
				
				while ( leafTriangles != leafTrianglesEnd )
				{
					// Find the intersection(s) of the ray with 4 triangles.
					SIMDBool intersectionResults = rayIntersectsTriangleSIMD( ray, 
														leafTriangles->v0, leafTriangles->v1, leafTriangles->v2,
														temporaryIntersectionT );
					
					// Cull any intersections that are beyond the maximum allowed distance.
					intersectionResults &= temporaryIntersectionT < objectSpaceMaxDistance;
					
					// If there were any intersections, update the variable which keeps track of that.
					foundIntersection |= intersectionResults;
					
					// For each intersection, add it to the list of output intersections.
					for ( Index i = 0; i < 4; i++ )
					{
						if ( intersectionResults[i] )
						{
							// Compute the world-space ray parameter of this intersection.
							Real worldSpaceT = objectTransformation.transformToWorldSpace( temporaryIntersectionT[i] );
							
							// Add the intersection to the list of of output intersections.
							intersections.add( RayIntersection( worldSpaceT,
													ObjectSpaceTriangle( leafTriangles->getTrianglePointer(i), object ) ) );
						}
					}
					
					leafTriangles++;
				}
			}
		}
	}
	while ( stackElement != stackBase );
	
	return foundIntersection;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Ray-Sphere Intersection Methods
//############		
//##########################################################################################
//##########################################################################################




Bool RayTracer:: rayIntersectsSphere( const Ray3& ray, const BoundingSphere& sphere, Real& distanceAlongRay )
{
	Vector3 d = sphere.position - ray.origin;
	Real dSquared = d.getMagnitudeSquared();
	Real rSquared = sphere.radius*sphere.radius;
	
	if ( dSquared < rSquared )
	{
		// The ray starts inside the sphere and therefore we have an intersection.
		distanceAlongRay = Real(0);
		
		return true;
	}
	else
	{
		// Find the closest point on the ray to the sphere's center.
		Real t1 = math::dot( d, ray.direction );
		
		if ( t1 < Real(0) )
		{
			// The ray points away from the sphere so there is no intersection.
			return false;
		}
		
		// Find the distance from the closest point to the sphere's surface.
		Real t2Squared = rSquared - dSquared + t1*t1;
		
		if ( t2Squared < Real(0) )
			return false;
		
		// Compute the distance along the ray of the intersection.
		distanceAlongRay = t1 - math::sqrt(t2Squared);
		
		return true;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Ray-Box Intersection Methods
//############		
//##########################################################################################
//##########################################################################################




SIMDBool RayTracer:: rayIntersectsBoxSIMD( const FatSIMDRay3& ray, const SIMDAABB3& box,
										SIMDFloat& distanceAlongRay )
{
	SIMDFloat tmin = (box.getMinMax(1 - ray.sign[0]).x - ray.origin.x) * ray.inverseDirection.x;
	SIMDFloat tmax = (box.getMinMax(ray.sign[0]).x - ray.origin.x) * ray.inverseDirection.x;
	SIMDFloat tymin = (box.getMinMax(1 - ray.sign[1]).y - ray.origin.y) * ray.inverseDirection.y;
	SIMDFloat tymax = (box.getMinMax(ray.sign[1]).y - ray.origin.y) * ray.inverseDirection.y;
	
	SIMDBool result = (tmin <= tymax) & (tymin <= tmax);
	
	// Refine the slab.
	tmin = math::max( tmin, tymin );
	tmax = math::min( tmax, tymax );
	
	SIMDFloat tzmin = (box.getMinMax(1 - ray.sign[2]).z - ray.origin.z) * ray.inverseDirection.z;
	SIMDFloat tzmax = (box.getMinMax(ray.sign[2]).z - ray.origin.z) * ray.inverseDirection.z;
	
	result &= (tmin <= tzmax) & (tzmin <= tmax);
	
	// Refine the slab again.
	tmin = math::max( tmin, tzmin );
	tmax = math::min( tmax, tzmax );
	
	distanceAlongRay = tmin;
	
	return result & (tmin <= tmax) & (tmax > Real(0));
}




//##########################################################################################
//##########################################################################################
//############		
//############		Ray-Triangles Intersection Methods
//############		
//##########################################################################################
//##########################################################################################




Bool RayTracer:: rayIntersectsTriangles( const SIMDRay3& ray, const FatSIMDTriangle3* triangles,
										Size numTriangles, SIMDFloat& closestIntersection,
										const TriangleType*& closestTriangle )
{
	// Compute the largest multiple of 4 that is smaller than the number of triangles.
	// This is the number that we can iterate over 4-at-a-time without doing anything extra.
	const FatSIMDTriangle3* const trianglesEnd = triangles + numTriangles;
	
	// A temporary variable used to hold the ray parameters of the ray's intersection with 4 triangles.
	SIMDFloat temporaryIntersectionT;
	Bool foundIntersection = false;
	
	const SIMDInt indices1( 0, 1, 2, 3 );
	const SIMDInt indices2( 2, 3, 0, 1 );
	
	while ( triangles != trianglesEnd )
	{
		SIMDBool result = rayIntersectsTriangleSIMD( ray, triangles->v0, triangles->v1, triangles->v2, temporaryIntersectionT );
		
		// If there was an interesection, find the closest one.
		if ( result )
		{
			// Set all non-intersections to have very large ray distances so that they don't
			// effect the minimum operations to come.
			temporaryIntersectionT = math::select( result, temporaryIntersectionT, SIMDFloat( math::max<float>() ) );
			
			// Shuffle the intersection T's once to find the minimum of 0 & 2, 1 & 3.
			SIMDFloat a = math::shuffle<2,3,0,1>( temporaryIntersectionT );
			
			// Determine the indices of the triangles which are the minimum of 0 & 2, 1 & 3.
			SIMDInt indices3 = math::select( temporaryIntersectionT < a, indices1, indices2 );
			
			// Find the minimum of 0 & 2, 1 & 3.
			a = math::min( temporaryIntersectionT, a );
			
			// Shuffle the intersection T's again to determine the minimum intersection T.
			SIMDFloat b = math::shuffle<1,0,3,2>( a );
			
			// Compute the index of the closest intersection.
			SIMDInt minimumTIndex = math::select( a < b, indices3, math::shuffle<1,0,3,2>( indices3 ) );
			
			// Compute the minimum intersection distance.
			SIMDFloat minimumT = math::min( a, b );
			
			// If there was an intersection, use the minimum intersection distance + index that have been computed.
			if ( minimumT < closestIntersection )
			{
				foundIntersection = true;
				closestIntersection = minimumT;
				closestTriangle = triangles->getTrianglePointer(minimumTIndex[0]);
			}
		}
		
		triangles++;
	}
	
	return foundIntersection;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Ray-Triangle Intersection Method
//############		
//##########################################################################################
//##########################################################################################




SIMDBool RayTracer:: rayIntersectsTriangleSIMD( const SIMDRay3& ray, const SIMDVector3& v0,
												const SIMDVector3& v1, const SIMDVector3& v2,
												SIMDFloat& distanceAlongRay )
{
	// find the edge vectors of the triangle
	SIMDVector3 v1ToV2 = v1 - v0;
	SIMDVector3 v1ToV3 = v2 - v0;
	
	// the vector perpendicular to edge 2 and the ray's direction
	SIMDVector3 pvec = math::cross( ray.direction, v1ToV3 );
	
	SIMDFloat det = math::dot( v1ToV2, pvec );
	
	// Do the first rejection test for the triangles, test to see if the ray is in the same plane as the triangle.
	SIMDBool result = math::abs(det) > math::epsilon<Real>();
	
	//************************************************************************************
	
	SIMDFloat inverseDet = Real(1) / det;
	
	SIMDVector3 v1ToSource = ray.origin - v0;
	
	SIMDFloat u = math::dot( v1ToSource, pvec ) * inverseDet;
	
	// Do the second rejection test for the triangles. See if the UV coordinate is within the valid range.
	result &= u >= Real(0) & u <= Real(1);
	
	//************************************************************************************
	
	SIMDVector3 qvec = math::cross( v1ToSource, v1ToV2 );
	
	SIMDFloat v = math::dot( ray.direction, qvec ) * inverseDet;
	
	// Do the third rejection test for the triangles. See if the UV coordinate is within the valid range.
	result &= v >= Real(0) & u + v <= Real(1);
	
	//************************************************************************************
	
	distanceAlongRay = math::dot( v1ToV3, qvec ) * inverseDet;
	
	// Make sure that the triangles are hit by the forward side of the ray.
	result &= distanceAlongRay >= Real(0);
	
	return result;
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
