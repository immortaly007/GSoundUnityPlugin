/*
 * Project:     GSound
 * 
 * File:        gsound/internal/RayTracer.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::RayTracer class declaration
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


#ifndef INCLUDE_GSOUND_RAY_TRACER_H
#define INCLUDE_GSOUND_RAY_TRACER_H


#include "GSoundInternalConfig.h"


#include "../SoundScene.h"
#include "InternalSoundTriangle.h"
#include "ObjectSpaceTriangle.h"
#include "SphereTree.h"
#include "QBVHArrayTree.h"
#include "FatSIMDRay3D.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




class RayTracer
{
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Type Definitions
			
			
			
			
			/// Define the type of an internal triangle.
			typedef InternalSoundTriangle TriangleType;
			
			
			
			
			/// Define the type of object BVH to use.
			typedef SphereTree ObjectBVHType;
			
			
			
			
			/// Define the type of triangle BVH to use.
			typedef QBVHArrayTree TriangleTreeType;
			
			
			
			
			/// Define the type of triangle BVH node to use.
			typedef QBVHArrayTreeNode TriangleNodeType;
			
			
			
			
			/// Define the type of ray to use internally.
			typedef FatSIMDRay3D<float,4> FatSIMDRay3;
			
			
			
			
			/// The type of triangle to use internally.
			typedef FatSIMDTriangle3D<Real,4> FatSIMDTriangle3;
			
			
			
			
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Ray Intersection Class Definition
			
			
			
			
			/// A class which is used to encapsulate a ray intersection point and the intersected triangle.
			class RayIntersection
			{
				public:
					
					/// Create a ray intersection record with the specified distance along the ray and triangle.
					GSOUND_INLINE RayIntersection( Real newDistanceAlongRay, const ObjectSpaceTriangle& newTriangle )
						:	distanceAlongRay( newDistanceAlongRay ),
							triangle( newTriangle )
					{
					}
					
					
					
					
					/// The distance along the ray at where the intersection occurred.
					Real distanceAlongRay;
					
					
					
					/// The triangle which was intersected by the ray.
					ObjectSpaceTriangle triangle;
					
					
					
					
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a ray tracer with no object bounding volume hierarchy to ray trace.
			RayTracer();
			
			
			
			
			/// Create a ray tracer which traces rays through the specified bounding volume hierarchy.
			RayTracer( const ObjectBVHType* newObjectBVH );
			
			
			
			
			/// Create a copy of another ray tracer object, copying all internal state.
			RayTracer( const RayTracer& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy this ray tracer and all internal state.
			~RayTracer();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the state of another ray tracer object to this object.
			RayTracer& operator = ( const RayTracer& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Object BVH Accessor Methods
			
			
			
			
			/// Get a pointer to the root node of the object bounding volume heirarchy which is being ray traced.
			GSOUND_INLINE const ObjectBVHType* getObjectBVH() const
			{
				return objectBVH;
			}
			
			
			
			
			/// Set a pointer to the root node of the object bounding volume heirarchy which should be ray traced.
			GSOUND_INLINE void setObjectBVH( const ObjectBVHType* newObjectBVH )
			{
				objectBVH = newObjectBVH;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Ray Tracing Methods
			
			
			
			
			/// Trace a single ray through the current scene and return the closest triangle that it intersected.
			/**
			  * The ray is traced through the scene until it strikes a triangle. If no triangle
			  * is hit by the ray, FALSE is returned and the output parameters are unmodified.
			  * If the ray hits a triangle, TRUE is returned and the closest of these triangles
			  * is placed in the triangle output parameter and the distance along the ray of the
			  * intersection with the triangle is placed in the closestIntersection output parameter.
			  * 
			  * @param ray - The ray to be traced through the scene.
			  * @param closestIntersection - An output parameter returning the distance along the ray of the first intersection.
			  * @param triangle - The triangle which was hit by the ray.
			  */
			Bool traceProbeRay( const Ray3& ray, Real& closestIntersection, ObjectSpaceTriangle& triangle );
			
			
			
			
			/// Trace a single ray through the current scene and return whether or not any triangles were intersected.
			/**
			  * The ray is traced through the scene until it strikes a triangle or until
			  * the maximum distance along the ray is exceeded. If a triangle is intersected,
			  * TRUE is returned. Otherwise, FALSE is returned.
			  * 
			  * @param ray - The ray to be traced through the scene.
			  * @param maxDistance - The maximum distance along the ray that an intersection can be found.
			  * @return whether or not any triangles were intersected by the ray.
			  */
			Bool traceBinaryOcclusionRay( const Ray3& ray, Real maxDistance );
			
			
			
			
			/// Trace a single ray through the current scene and return all intersections with the scene.
			/**
			  * The ray is traced through the scene and every triangle which it intersects is
			  * added to the parameter list of intersections. If any triangles are intersected,
			  * TRUE is returned. Otherwise, FALSE is returned.
			  * 
			  * @param ray - the ray to be traced through the scene.
			  * @param maxDistance - the maximum distance along the ray that an intersection can be found.
			  * @param intersections - an output list of the detected ray-triangles intersections.
			  * @return whether or not any triangles were intersected by the ray.
			  */
			Bool traceTransmissionRay( const Ray3& ray, Real maxDistance, ArrayList<RayIntersection>& intersections );
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Ray Tracing Methods
			
			
			
			
			/// Trace a ray through an object's triangle BVH and return the closest intersection.
			static Bool traceObjectSpaceProbeRay( const Ray3& worldSpaceRay, const SoundObject* object, 
												const TriangleNodeType** stackBase,
												Real& closestIntersection, ObjectSpaceTriangle& closestTriangle );
			
			
			
			
			/// Trace a ray through an object's triangle BVH and return the closest intersection.
			Bool traceObjectSpaceTransmissionRay( const Ray3& worldSpaceRay, const SoundObject* object, 
												const TriangleNodeType** stackBase,
												Real maxDistance, ArrayList<RayIntersection>& intersections );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Intersection Methods
			
			
			
			
			/// Return whether or not the specified ray and sphere intersect.
			/**
			  * This method computes the distance along the ray of the intersection point.
			  */
			GSOUND_FORCE_INLINE static Bool rayIntersectsSphere( const Ray3& ray, const BoundingSphere& sphere,
																Real& distanceAlongRay );
			
			
			
			
			/// Return whether or not the specified ray and box intersect.
			/**
			  * This method computes the distance along the ray of the intersection point.
			  */
			GSOUND_FORCE_INLINE static SIMDBool rayIntersectsBoxSIMD( const FatSIMDRay3& ray, const SIMDAABB3& box,
																	SIMDFloat& distanceAlongRay );
			
			
			
			
			/// Return whether or not the specified ray intersects any of the specified triangles.
			GSOUND_NO_INLINE static Bool rayIntersectsTriangles( const SIMDRay3& ray,
													const FatSIMDTriangle3* triangles, Size numTriangles, 
													SIMDFloat& closestIntersection, const TriangleType*& closestTriangle );
			
			
			
			
			/// Return whether or not the specified ray and triangle intersect.
			/**
			  * This method computes the distance along the ray of the intersection point.
			  */
			GSOUND_FORCE_INLINE static SIMDBool rayIntersectsTriangleSIMD( const SIMDRay3& ray, const SIMDVector3& v0,
																		const SIMDVector3& v1, const SIMDVector3& v2,
																		SIMDFloat& distanceAlongRay );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A pointer to the root node of the bounding volume hierarchy of objects to be ray traced.
			const ObjectBVHType* objectBVH;
			
			
			
			
			/// A stack which is used to implement iterative ray tracing.
			const void** stack;
			
			
			
			
			/// The size of the ray tracing traversal stack.
			Size stackSize;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Data Members
			
			
			
			
			/// Define the size of the ray tracing stack.
			static const Size TRAVERSAL_STACK_DEPTH = 100;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_RAY_TRACER_H
