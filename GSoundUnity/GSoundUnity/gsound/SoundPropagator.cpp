/*
 * Project:     GSound
 * 
 * File:        gsound/SoundPropagator.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundPropagator class implementation
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


#include "SoundPropagator.h"


#include "internal/ProbePath.h"
#include "internal/ProbePathCache.h"
#include "internal/ProbedTriangleCache.h"
#include "internal/DiffractionFrequencyResponse.h"
#include "internal/RayDistributionCache.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




using internal::ProbePath;





//##########################################################################################
//##########################################################################################
//############		
//############		Private Internal Probe Intersection Record Class
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagator:: ProbeIntersectionRecord
{
	public:
		
		GSOUND_INLINE ProbeIntersectionRecord( const internal::WorldSpaceTriangle& newTriangle,
											const Vector3& newImagePosition )
			:	triangle( newTriangle ),
				imagePosition( newImagePosition )
		{
		}
		
		
		internal::WorldSpaceTriangle triangle;
		
		Vector3 imagePosition;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




SoundPropagator:: SoundPropagator()
	:	rayTracer( util::construct<internal::RayTracer>() ),
		directSoundIsEnabled( true ),
		transmissionIsEnabled( true ),
		reflectionIsEnabled( true ),
		diffractionIsEnabled( true ),
		reverbIsEnabled( true ),
		timeStamp( 0 ),
		rayEpsilon( Real(0.0001) ),
		maxReverbCacheAge( 10 ),
		scene( NULL ),
		debugDrawingCache( NULL )
{
}




SoundPropagator:: SoundPropagator( const SoundPropagator& other )
	:	rayTracer( util::construct<internal::RayTracer>(*other.rayTracer) ),
		directSoundIsEnabled( other.directSoundIsEnabled ),
		transmissionIsEnabled( other.transmissionIsEnabled ),
		reflectionIsEnabled( other.reflectionIsEnabled ),
		diffractionIsEnabled( other.diffractionIsEnabled ),
		reverbIsEnabled( other.reverbIsEnabled ),
		timeStamp( other.timeStamp ),
		rayEpsilon( other.rayEpsilon ),
		maxReverbCacheAge( other.maxReverbCacheAge ),
		scene( NULL ),
		debugDrawingCache( NULL )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundPropagator:: ~SoundPropagator()
{
	util::destruct( rayTracer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundPropagator& SoundPropagator:: operator = ( const SoundPropagator& other )
{
	if ( this != &other )
	{
		// Copy the ray tracer object.
		util::destruct( rayTracer );
		rayTracer = util::construct<internal::RayTracer>(*other.rayTracer);
		
		// Copy the other internal state of the SoundPropagator object.
		directSoundIsEnabled = other.directSoundIsEnabled;
		transmissionIsEnabled = other.transmissionIsEnabled;
		reflectionIsEnabled = other.reflectionIsEnabled;
		diffractionIsEnabled = other.diffractionIsEnabled;
		reverbIsEnabled = other.reverbIsEnabled;
		timeStamp = other.timeStamp;
		rayEpsilon = other.rayEpsilon;
		maxReverbCacheAge = other.maxReverbCacheAge;
		scene = other.scene;
		debugDrawingCache = other.debugDrawingCache;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Sound Propagation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: propagateSound( const SoundScene& soundScene, const SoundListener& listener,
										Size maxListenerProbeDepth, Size numListenerProbeRays,
										Size maxSourceProbeDepth, Size numSourceProbeRays,
										SoundPropagationPathBuffer& pathBuffer,
										DebugDrawingCache* debugCache )
{
#if GSOUND_FIXED_MAX_PATH_DEPTH
	//***************************************************************************
	// Make sure that the probe depths don't exceed the maximum allowed depths
	
	
	maxListenerProbeDepth = math::min( maxListenerProbeDepth, Size(GSOUND_MAX_PATH_DEPTH) );
	maxSourceProbeDepth = math::min( maxSourceProbeDepth, Size(GSOUND_MAX_PATH_DEPTH) );
	
#endif
	
	//***************************************************************************
	
	
	// Set an internal temporary pointer to the scene where propagation is being performed.
	scene = &soundScene;
	
	
	// Send the object BVH to the ray tracer.
	rayTracer->setObjectBVH( scene->getObjectBVH() );
	
	
	// Set an internal temporary pointer to the object which caches debug drawing information.
	debugDrawingCache = debugCache;
	
	// Draw debug information related to the scene where sound propagation is being performed.
	if ( debugDrawingCache != NULL )
	{
		// Clear the debug drawing cache if necessary.
		debugDrawingCache->clear();
		
		// Draw debug information related to the scene where sound propagation is being performed.
		drawSceneDebug();
		
		// Add the listener to the debug drawing cache.
		if ( debugDrawingCache->getListenersAreEnabled() )
			debugDrawingCache->addListener( listener );
	}
	
	
	//***************************************************************************
	// Prepare data structures for propagation.
	
	
	// Make sure that the contribution list is valid and empty for each sound source.
	preparePropagationPathBuffer( pathBuffer );
	
	
	//***************************************************************************
	// Check previously found cached probe paths for contributions.
	
	
	validateCachedPaths( listener, pathBuffer );
	
	
	//***************************************************************************
	// Find all direct/transmitted contribution paths.
	
	
	addDirectPaths( listener, pathBuffer );
	
	
	// Don't bother propagating sound if there are no objects in the scene.
	if ( scene->getNumberOfObjects() == 0 )
	{
		// Reset temporary pointers to NULL.
		scene = NULL;
		debugDrawingCache = NULL;
		
		return;
	}
	
	
	//***************************************************************************
	// Shoot out probe rays from the listener.
	
	
	if ( maxListenerProbeDepth > 0 && numListenerProbeRays > 0 )
		doListenerPropagation( listener, maxListenerProbeDepth, numListenerProbeRays, pathBuffer );
	
	
	//***************************************************************************
	// Do source-space sound propagation and reverb estimation.
	
	
	if ( maxSourceProbeDepth > 0 && numSourceProbeRays > 0 )
		doSourcePropagation( listener, maxSourceProbeDepth, numSourceProbeRays, pathBuffer );
	
	
	//***************************************************************************
	// Reset temporary pointers to NULL.
	
	scene = NULL;
	debugDrawingCache = NULL;
	
	//***************************************************************************
	// Increment the current timestamp
	
	timeStamp++;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Listener Propagation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: doListenerPropagation( const SoundListener& listener,
												Size maxListenerProbeDepth, Size numListenerProbeRays,
												SoundPropagationPathBuffer& pathBuffer )
{
	internal::ProbePathCache& probePathCache = listener.probePathCache;
	internal::ProbedTriangleCache<SoundListener::ProbeVisibilityRecord>& probedTriangles = listener.probedTriangles;
	internal::RayDistributionCache& rayDistribution = listener.rayDistribution;
	
	ProbePath probePath;
	ArrayList<ProbeIntersectionRecord> path;
	
	Real totalDistance;
	Vector3 directionToSource;
	Vector3 directionFromListener;
	FrequencyResponse attenuation;
	
	// Validate potential reflection paths.
	const Size numSources = scene->getNumberOfSources();
	
	// Get an iterator for the ray distribution.
	internal::RayDistributionCache::Iterator distributionCell = rayDistribution.getIterator();
	
	// Compute the number of rays for a cell with a ray affinity of 1.
	Real raysPerCell = Real(numListenerProbeRays) / rayDistribution.getSum();
	
	// Trace each probe path and store the valid paths.
	while ( distributionCell )
	{
		// Compute the number of rays to trace for this cell. Always trace at least 1 ray per cell.
		Size numCellRays = math::max( Size(distributionCell.getRayAffinity()*raysPerCell), Size(1) );
		
		// Get the ranges for longitude and latitude for this cell.
		AABB1 cellLongitudes = distributionCell.getCellLongitudes();
		AABB1 cellLatitudes = distributionCell.getCellLatitudes();
		
		
		for ( Index i = 0; i < numCellRays; i++ )
		{
			path.clear();
			probePath.clearTriangles();
			
			// Take a random sample of (longitude, latitude) in the valid ranges for this cell.
			Real longitude = probeRandomVariable.sample( cellLongitudes.min, cellLongitudes.max );
			Real latitude = probeRandomVariable.sample( cellLatitudes.min, cellLatitudes.max );
			
			// Create the ray that start at the listener's position and passes
			// through the point specified by (longitude, latitude) in spherical coordinates.
			Ray3 ray( listener.getPosition(), Vector3( math::cos(longitude)*math::sin(latitude),
														math::cos(latitude),
														math::sin(longitude)*math::sin(latitude) ) );
			
			Vector3 firstRayDirection = ray.direction*listener.getOrientation();
			Vector3 currentListenerImagePosition = listener.getPosition();
			
			for ( Index d = 0; d < maxListenerProbeDepth; d++ )
			{
				Real closestIntersection;
				internal::ObjectSpaceTriangle closestTriangle;
				
				if ( rayTracer->traceProbeRay( ray, closestIntersection, closestTriangle ) )
				{
					// We have intersected a triangle. Add this triangle to the probe path array.
					probePath.addTriangle( closestTriangle );
					
					// Test to see if we have already visited this same probe path
					// previously. If not, mark this path as visted for future probe rays.
					Bool pathHasNotBeenVisited = !probePathCache.containsPath( probePath );
					
					// Transform the closest triangle into world space.
					const internal::WorldSpaceTriangle worldSpaceTriangle( closestTriangle );
					const Vector3& normal = worldSpaceTriangle.plane.normal;
					
					Real rayDotNormal = math::dot( ray.direction, normal );
					
					// Add this triangle to a hash map which keeps track of all triangles probed so far.
					probedTriangles.add( closestTriangle,
								SoundListener::ProbeVisibilityRecord( rayDotNormal, firstRayDirection, timeStamp ) );
					
					// Calculate the intersection point of the ray with the triangle in world space.
					Vector3 intersectionPoint = ray.origin + ray.direction*closestIntersection;
					
					// Calculate the direction of the ray reflected off of the closest triangle in world space.
					Vector3 reflectedDirection = ray.direction - Real(2)*normal*rayDotNormal;
					
					// Bias the intersection point by a small amount in order to avoid floating point precision problems.
					Vector3 bias = normal*rayEpsilon;
					intersectionPoint += rayDotNormal < Real(0) ? bias : -bias;
					
					// Draw the probe ray if debug drawing is enabled.
					if ( debugDrawingCache != NULL && debugDrawingCache->getFiniteProbeRaysAreEnabled() && d == 0 )
						debugDrawingCache->addFiniteProbeRay( ray.origin, intersectionPoint );
					
					ray = Ray3( intersectionPoint, reflectedDirection );
					
					Bool foundPaths = false;
					
					// If we have not already visited this path, check the intersected triangle
					// for possible diffraction paths.
					if ( pathHasNotBeenVisited && diffractionIsEnabled )
					{
						foundPaths = addDiffractionPaths( listener, worldSpaceTriangle, path, pathBuffer );
					}
					
					// Reflect the current listener image position over the intersected triangle
					// and store the result for the next depth.
					currentListenerImagePosition = worldSpaceTriangle.plane.getReflection( currentListenerImagePosition );
					
					path.add( ProbeIntersectionRecord( worldSpaceTriangle, currentListenerImagePosition ) );
					
					// If we have not already visited this path, check for any valid
					// reflection paths.
					if ( pathHasNotBeenVisited )
					{
						if ( reflectionIsEnabled )
						{
							for ( Index s = 0; s < numSources; s++ )
							{
								SoundSource& source = *scene->getSource(s);
								
								if ( !source.getIsEnabled() )
									continue;
								
								pathDescription.clearPoints();
								pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::SOURCE, &source ) );
								
								if ( validateReflectionPath( source.getPosition(), listener.getPosition(), 
															source.getRadius(), path, totalDistance,
															directionFromListener, directionToSource, 
															attenuation ) )
								{
									pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::LISTENER, &listener ) );
									
									Real relativeSpeed = getRelativeSpeed( listener, directionFromListener, source, directionToSource );
									attenuation *= getSourceFrequencyResponse( source, directionToSource );
									
									pathBuffer.getSourceBuffer(s).addPropagationPath(
											PropagationPath( directionFromListener*listener.getOrientation(),
																totalDistance, relativeSpeed, 
																scene->getSpeedOfSound(),
																attenuation, pathDescription )  );
									foundPaths = true;
								}
							}
						}
						
						distributionCell.increaseRayAffinity();
						
						probePath.setFoundPaths( foundPaths );
						probePathCache.addPath( probePath );
					}
				}
				else
				{
					distributionCell.decreaseRayAffinity();
					
					if ( debugDrawingCache != NULL && debugDrawingCache->getInfiniteProbeRaysAreEnabled() && d == 0 )
						debugDrawingCache->addInfiniteProbeRay( ray );
					
					// Break if there was no intersection for the probe ray.
					break;
				}
			}
		}
		
		distributionCell++;
	}
	
	//***************************************************************************
	// Update the status of all listener probed triangles, removing those that are
	// older than the maximum age.
	
	{
		internal::ProbedTriangleCache<SoundListener::ProbeVisibilityRecord>::Iterator i = probedTriangles.getIterator();
		
		while ( i )
		{
			if ( timeStamp - i.getData().timeStamp >= maxReverbCacheAge )
				i.remove();
			else
				i++;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Listener Propagation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: doSourcePropagation( const SoundListener& listener,
											Size maxSourceProbeDepth, Size numSourceProbeRays,
											SoundPropagationPathBuffer& pathBuffer )
{
	internal::ProbedTriangleCache<SoundListener::ProbeVisibilityRecord>& listenerProbedTriangles = listener.probedTriangles;
	
	Size numSources = scene->getNumberOfSources();
	
	for ( Index s = 0; s < numSources; s++ )
	{
		const SoundSource& source = *scene->getSource(s);
		
		if ( !source.getIsEnabled() )
			continue;
		
		internal::ProbedTriangleCache<SoundSource::ProbeVisibilityRecord>& probedTriangles = source.probedTriangles;
		
		// The number of probe rays that were shot and hit something.
		Size numValidRays = 0;
		
		// The total number of probe rays that will be cast for this source.
		Size totalNumProbeRays = numSourceProbeRays*maxSourceProbeDepth;
		
		// The total distance traveled by all probe rays in the scene that hit a triangle.
		Real totalFreePath = 0;
		
		for ( Index i = 0; i < numSourceProbeRays; i++ )
		{
			Ray3 ray( source.getPosition(), Vector3( probeRandomVariable.sample( Real(-1), Real(1) ),
													probeRandomVariable.sample( Real(-1), Real(1) ),
													probeRandomVariable.sample( Real(-1), Real(1) ) ).normalize() );
			
			// Bias the ray's starting position by the source's radius.
			ray.origin += source.getRadius();
			
			// The current accumulated attenuation for the reflections of the ray.
			FrequencyResponse rayAttenuation;
			
			
			for ( Index d = 0; d < maxSourceProbeDepth; d++ )
			{
				Real closestIntersection;
				internal::ObjectSpaceTriangle closestTriangle;
				
				if ( rayTracer->traceProbeRay( ray, closestIntersection, closestTriangle ) )
				{
					// Transform the closest triangle into world space.
					const internal::WorldSpaceTriangle worldSpaceTriangle( closestTriangle );
					const Vector3& normal = worldSpaceTriangle.plane.normal;
					
					// Calculate the intersection point of the ray with the triangle in world space.
					Vector3 intersectionPoint = ray.origin + ray.direction*closestIntersection;
					
					// Compute the dot product of the triangle's normal with the incoming ray's direction.
					Real rayDotNormal = math::dot( ray.direction, normal );
					
					// Calculate the direction of the ray reflected off of the closest triangle in world space.
					Vector3 reflectedDirection = ray.direction - Real(2)*normal*rayDotNormal;
					
					// Bias the intersection point by a small amount in order to avoid floating point precision problems.
					if ( rayDotNormal < Real(0) )
						intersectionPoint += normal*rayEpsilon;
					else
						intersectionPoint -= normal*rayEpsilon;
					
					// Draw the probe ray if debug drawing is enabled.
					if ( debugDrawingCache != NULL && debugDrawingCache->getFiniteProbeRaysAreEnabled() )
						debugDrawingCache->addFiniteProbeRay( ray.origin, intersectionPoint );
					
					ray = Ray3( intersectionPoint, reflectedDirection );
					
					numValidRays++;
					
					if ( reverbIsEnabled )
					{
						totalFreePath += closestIntersection;
						
						probedTriangles.add( closestTriangle, SoundSource::ProbeVisibilityRecord( rayDotNormal, timeStamp ) );
					}
				}
				else
				{
					if ( debugDrawingCache != NULL && debugDrawingCache->getInfiniteProbeRaysAreEnabled() )
						debugDrawingCache->addInfiniteProbeRay( ray );
					
					// Break if there was no intersection for the probe ray.
					break;
				}
			}
		}
		
		
		//**************************************************************************************
		// Iterate over the source probed triangles and calculate the overlap area
		// with the listener probed triangles and the approximate scene surface area.
		
		
		// The approximate total surface area of the space that the source is in.
		Real totalSurfaceArea = Real(0);
		
		// The total area of the triangles that have been touched by both the listener's and this source's rays.
		Real listenerOverlapArea = Real(0);
		
		FrequencyResponse attenuationArea( 0 );
		
		
		{
			internal::ProbedTriangleCache<SoundSource::ProbeVisibilityRecord>::Iterator i = probedTriangles.getIterator();
			
			while ( i )
			{
				const internal::ObjectSpaceTriangle& triangle = i.getTriangle();
				const SoundSource::ProbeVisibilityRecord& sourceTriangleRecord = i.getData();
				
				Size sourceTriangleAge = timeStamp - sourceTriangleRecord.timeStamp;
				
				if ( sourceTriangleAge >= maxReverbCacheAge )
				{
					i.remove();
					continue;
				}
				
				// Scale the triangle's area into world space.
				Real triangleArea = math::square(triangle.object->getTransformation().scale)*triangle.triangle->getArea();
				
				// Add this triangle's area to the total surface area of the scene.
				totalSurfaceArea += triangleArea;
				attenuationArea += triangle.triangle->getMaterial().getReflectionAttenuation()*triangleArea;
				
				
				// Test to see if the listener also probed this triangle recently.
				SoundListener::ProbeVisibilityRecord* listenerTriangleRecord;
				
				if ( listenerProbedTriangles.find( triangle, listenerTriangleRecord ) )
				{
					// Make sure that the same side of the triangle was probed by the source and listener.
					if ( (sourceTriangleRecord.rayDotNormal < Real(0) && listenerTriangleRecord->rayDotNormal < Real(0)) || 
						(sourceTriangleRecord.rayDotNormal > Real(0) && listenerTriangleRecord->rayDotNormal > Real(0)) )
					{
						// Compute the contribution of this triangle based on its age.
						// Older triangles are weighted so that they have less effect.
						Real sourceTriangleAgeAttenuation = Real(1) - Real(sourceTriangleAge) / Real(maxReverbCacheAge);
						
						// Compute the contribution of this triangle based on its age.
						// Older triangles are weighted so that they have less effect.
						Size listenerTriangleAge = timeStamp - listenerTriangleRecord->timeStamp;
						Real listenerTriangleAgeAttenuation = Real(1) - Real(listenerTriangleAge) / Real(maxReverbCacheAge);
						
						// Average the attenuation from the source and listener to find the actual attenuation.
						Real triangleAgeAttenuation = Real(0.5)*(sourceTriangleAgeAttenuation + listenerTriangleAgeAttenuation);
						
						// Add the adjusted triangle area to the total area touched by both the source and listener.
						listenerOverlapArea += triangleArea*triangleAgeAttenuation;
					}
				}
				
				i++;
			}
		}
		
		Real voidFraction = Real(numValidRays) / Real(totalNumProbeRays);
		
		Real meanFreePath = (totalFreePath / numValidRays);
		
		Real adjustedTotalArea = totalSurfaceArea/* / voidFraction*/;
		Real totalVolume = adjustedTotalArea*meanFreePath / Real(4);
		
		if ( math::equals( totalSurfaceArea, Real(0) ) )
		{
			Real overlapRatio = Real(0);
			FrequencyResponse averageAttenuation( 0 );
			FrequencyResponse reverbAttenuation( 0 );
			
			pathBuffer.getSourceBuffer(s).setReverbResponse(
				SoundSourceReverbResponse( totalVolume, adjustedTotalArea, averageAttenuation,
											reverbAttenuation*overlapRatio ) );
		}
		else
		{
			Real overlapRatio = listenerOverlapArea / totalSurfaceArea;
			FrequencyResponse averageAttenuation = attenuationArea*(Real(1)/totalSurfaceArea);
			
			Real scalarReverbAttenuation = overlapRatio*source.getDistanceAttenuation( meanFreePath )*voidFraction;
			
			FrequencyResponse reverbAttenuation = averageAttenuation*scalarReverbAttenuation;
			
			SoundSourceReverbResponse reverbResponse( totalVolume, adjustedTotalArea, averageAttenuation,
													reverbAttenuation );
			
			pathBuffer.getSourceBuffer(s).setReverbResponse( reverbResponse );
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cached Path Validation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: validateCachedPaths( const SoundListener& listener, SoundPropagationPathBuffer& pathBuffer )
{
	ArrayList<ProbeIntersectionRecord> path;
	
	Real totalDistance;
	Vector3 directionToSource;
	Vector3 directionFromListener;
	FrequencyResponse attenuation;
	
	
	// Iterate through all previously detected probe paths and add any valid
	// contributions and removing any paths that no longer produce any contributions.
	
	internal::ProbePathCache::Iterator i = listener.probePathCache.getIterator();
	
	while ( i )
	{
		// If no propagation paths were found for the current probe path on the previous frame, remove it.
		if ( !i->getFoundPaths() )
		{
			i.remove();
			continue;
		}
		
		ProbePath& probePath = *i;
		
		// Generate a fake probe path so that we can call the path validation functions.
		path.clear();
		Vector3 listenerImagePosition = listener.getPosition();
		
		for ( Index j = 0; j < probePath.getDepth(); j++ )
		{
			internal::WorldSpaceTriangle worldSpaceTriangle( probePath.getTriangle(j) );;
			listenerImagePosition = worldSpaceTriangle.plane.getReflection( listenerImagePosition );
			path.add( ProbeIntersectionRecord( worldSpaceTriangle, listenerImagePosition ) );
		}
		
		Bool foundPaths = false;
		
		// Validate potential reflection paths.
		if ( reflectionIsEnabled )
		{
			Size numSources = scene->getNumberOfSources();
			
			for ( Index s = 0; s < numSources; s++ )
			{
				SoundSource& source = *scene->getSource(s);
				
				if ( !source.getIsEnabled() )
					continue;
				
				pathDescription.clearPoints();
				pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::SOURCE, &source ) );
				
				if ( validateReflectionPath( source.getPosition(), listener.getPosition(),
											source.getRadius(), path, totalDistance,
											directionFromListener, directionToSource, 
											attenuation ) )
				{
					pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::LISTENER, &listener ) );
					
					Real relativeSpeed = getRelativeSpeed( listener, directionFromListener, source, directionToSource );
					attenuation *= getSourceFrequencyResponse( source, directionToSource );
					
					pathBuffer.getSourceBuffer(s).addPropagationPath(
							PropagationPath( directionFromListener*listener.getOrientation(),
												totalDistance, relativeSpeed, 
												scene->getSpeedOfSound(),
												attenuation, pathDescription )  );
					foundPaths = true;
				}
			}
		}
		
		
		if ( diffractionIsEnabled )
		{
			// Remove the last triangle from the path.
			internal::WorldSpaceTriangle lastTriangle = path.getLast().triangle;
			path.removeLast();
			
			foundPaths |= addDiffractionPaths( listener, lastTriangle, path, pathBuffer );
		}
		
		probePath.setFoundPaths( foundPaths );
		
		i++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Direct Propagation Path Computation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: addDirectPaths( const SoundListener& listener, SoundPropagationPathBuffer& pathBuffer )
{
	const Vector3& listenerPosition = listener.getPosition();
	const Size numSources = scene->getNumberOfSources();
	
	// Shoot out direct transmission rays from the listener to each source.
	for ( Index i = 0; i < numSources; i++ )
	{
		const SoundSource& source = *scene->getSource(i);
		
		if ( !source.getIsEnabled() )
			continue;
		
		Vector3 transmissionVector = source.getPosition() - listenerPosition;
		
		Real distanceToSource = transmissionVector.getMagnitude();
		Real sourceRadius = source.getRadius();
		transmissionVector /= distanceToSource;
		
		// Create a vector to hold the frequency-dependent attenuation.
		FrequencyResponse attenuation;
		
		// Trace an occlusion ray from the listener to the source and determine
		// how much sound was transmitted.
		
		if ( directSoundIsEnabled &&
			!rayTracer->traceBinaryOcclusionRay( Ray3( listenerPosition, transmissionVector),
												distanceToSource - sourceRadius ) )
		{
			pathDescription.clearPoints();
			pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::SOURCE, &source ) );
			pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::LISTENER, &listener ) );
			
			Real relativeSpeed = getRelativeSpeed( listener, transmissionVector, source, transmissionVector );
			attenuation *= getSourceFrequencyResponse( source, transmissionVector );
			
			pathBuffer.getSourceBuffer(i).addPropagationPath(
						PropagationPath( transmissionVector*listener.getOrientation(),
											distanceToSource, relativeSpeed,
											scene->getSpeedOfSound(),
											attenuation, pathDescription ) );
			
			if ( debugDrawingCache != NULL && debugDrawingCache->getDirectPathsAreEnabled() )
				debugDrawingCache->addDirectPath( source.getPosition(), listenerPosition );
		}
		else if ( transmissionIsEnabled )
		{
			Bool foundIntersection = traceTransmissionRay( Ray3( listenerPosition, transmissionVector ), 
															distanceToSource, attenuation );
			
			if ( foundIntersection )
			{
				pathDescription.clearPoints();
				pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::SOURCE, &source ) );
				pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::LISTENER, &listener ) );
				
				Real relativeSpeed = getRelativeSpeed( listener, transmissionVector, source, transmissionVector );
				attenuation *= getSourceFrequencyResponse( source, transmissionVector );
				
				pathBuffer.getSourceBuffer(i).addPropagationPath(
							PropagationPath( transmissionVector*listener.getOrientation(),
												distanceToSource, relativeSpeed,
												scene->getSpeedOfSound(),
												attenuation, pathDescription ) );
				
				if ( debugDrawingCache != NULL && debugDrawingCache->getTransmissionPathsAreEnabled() )
					debugDrawingCache->addTransmissionPath( source.getPosition(), listenerPosition );
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Reflection Path Validation Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPropagator:: validateReflectionPath( const Vector3& sourcePosition, const Vector3& listenerPosition,
												Real sourceRadius,
												const ArrayList<ProbeIntersectionRecord>& path, Real& totalDistance,
												Vector3& directionFromListener, Vector3& directionToSource,
												FrequencyResponse& attenuation )
{
	totalDistance = Real(0);
	attenuation = FrequencyResponse();
	
	Vector3 virtualSourcePosition = sourcePosition;
	Real virtualSourceRadius = sourceRadius;
	
	ArrayList<Vector3> points;
	
	if ( debugDrawingCache != NULL && debugDrawingCache->getReflectionPathsAreEnabled() )
		points.add( virtualSourcePosition );
	
	// Make sure that the vector from the listener image position to the source passes
	// through the given triangle at each depth.
	for ( Index i = path.getSize(); i > 0; i-- )
	{
		const internal::WorldSpaceTriangle& triangle = path[i-1].triangle;
		const Vector3& listenerImagePosition = path[i-1].imagePosition;
		
		// Calculate the vector from the listener image position to the source.
		Vector3 rayDirection = virtualSourcePosition - listenerImagePosition;
		
		Real rayDistance = rayDirection.getMagnitude();
		rayDirection /= rayDistance;
		
		// Make sure that the vector from the listener image position to the source passes
		// through the triangle at each depth.
		Real distanceAlongRay;
		
		// If the virtual source is not valid, return.
		if ( !rayIntersectsTriangle( Ray3( listenerImagePosition, rayDirection ), triangle, distanceAlongRay ) )
			return false;
		
		if ( distanceAlongRay > rayDistance )
			return false;
		
		// Calculate the intersection point of this ray with the triangle.
		virtualSourcePosition = listenerImagePosition + rayDirection*distanceAlongRay;
		
		// Bias the intersection point to avoid precision errors.
		Vector3 bias = triangle.plane.normal*rayEpsilon;
		virtualSourcePosition += math::dot( rayDirection, triangle.plane.normal ) > Real(0) ? bias : -bias;
		
		// Trace a ray from this intersection point to the source to make sure that
		// the source is reachable from this location.
		if ( rayTracer->traceBinaryOcclusionRay( Ray3( virtualSourcePosition, rayDirection ), 
												rayDistance - distanceAlongRay - rayEpsilon - virtualSourceRadius ) )
			return false;
		
		if ( debugDrawingCache != NULL && debugDrawingCache->getReflectionPathsAreEnabled() )
			points.add( virtualSourcePosition );
		
		attenuation *= triangle.objectSpaceTriangle->getMaterial().getReflectionAttenuation();
		totalDistance += rayDistance - distanceAlongRay;
		pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::TRIANGLE_REFLECTION, triangle.objectSpaceTriangle ) );
		
		if ( i == path.getSize() )
		{
			// After the first reflection, set the virtual source's radius to zero because virtual sources have no radius.
			virtualSourceRadius = 0;
			directionToSource = rayDirection;
		}
	}
	
	// Calculate the vector from the listener position to the virtual source.
	directionFromListener = virtualSourcePosition - listenerPosition;
	
	Real rayDistance = directionFromListener.getMagnitude();
	directionFromListener /= rayDistance;
	
	if ( rayTracer->traceBinaryOcclusionRay( Ray3( listenerPosition, directionFromListener ), rayDistance ) )
		return false;
	
	totalDistance += rayDistance;
	
	if ( debugDrawingCache != NULL && debugDrawingCache->getReflectionPathsAreEnabled() )
	{
		points.add( listenerPosition );
		debugDrawingCache->addReflectionPath( points );
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Diffraction Path Generation Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPropagator:: addDiffractionPaths( const SoundListener& listener,
											const internal::WorldSpaceTriangle& probedTriangle,
											const ArrayList<ProbeIntersectionRecord>& path, 
											SoundPropagationPathBuffer& pathBuffer )
{
	const Vector3& listenerPosition = listener.getPosition();
	const internal::InternalSoundTriangle* objectSpaceTriangle = probedTriangle.objectSpaceTriangle;
	const Transformation3& objectTransformation = probedTriangle.object->getTransformation();
	
	Bool foundContributions = false;
	
	// For each edge of the probed triangle, perform diffraction calculations if applicable.
	for ( Index e = 0; e < 3; e++ )
	{
		// Is this edge diffracting?
		if ( objectSpaceTriangle->getEdgeType(e) != internal::InternalSoundTriangle::DIFFRACTING )
			continue;
		
		//***************************************************************************
		// Determine initial information about the probed triangle.
		
		// Determine which vertex is not part of the diffracting edge
		// for the probed triangle.
		const SoundVertex& probedFreeVertex = (e == 0) ? probedTriangle.v3 : 
												(e == 1) ? probedTriangle.v2 : probedTriangle.v1;
		
		// Determine the vertices of the diffracting edge.
		const SoundVertex* sharedV1;
		const SoundVertex* sharedV2;
		
		probedTriangle.getEdgeVertices( e, sharedV1, sharedV2 );
		
		// Determine the listener image position.
		const Vector3& listenerImagePosition = path.getSize() > 0 ?
						path[path.getSize() - 1].imagePosition :
						listenerPosition;
		
		// Correct the normal vector for the probed triangle so that it
		// points towards the listener position.
		Plane3 probedPlane = probedTriangle.plane.getSignedDistanceTo( listenerImagePosition ) < Real(0) ? 
							-probedTriangle.plane : probedTriangle.plane;
		
		
		// The plane of the neighboring triangle (if it exists), or the negatively facing probe plane.
		Plane3 neighborPlane = -probedPlane;
		
		// Calculate the plane that the listener's position forms with the
		// two vertices on the diffracting edge.
		Plane3 shadowRegionBoundary( listenerImagePosition, *sharedV1, *sharedV2 );
		
		// Make sure that the normal of the shadow region boundary points towards
		// The interior of the shadow region.
		if ( shadowRegionBoundary.getSignedDistanceTo( probedFreeVertex ) < Real(0) )
			shadowRegionBoundary = -shadowRegionBoundary;
		
		
		// If this is a diffraction edge shared by two triangles, do some extra culling
		// that we don't have to do with an unshared edge.
		if ( objectSpaceTriangle->getNeighbor(e) != NULL )
		{
			// The diffracting edge is formed with the neighboring triangle.
			const internal::InternalSoundTriangle* neighborTriangle = objectSpaceTriangle->getNeighbor(e);
			SoundVertex neighborFreeVertex;
			
			// Determine which vertex is not part of the diffracting edge
			// for the neighbor triangle.
			if ( objectSpaceTriangle == neighborTriangle->getNeighbor(0) )
				neighborFreeVertex = objectTransformation.transformToWorldSpace( neighborTriangle->getVertex(2) );
			else if ( objectSpaceTriangle == neighborTriangle->getNeighbor(1) )
				neighborFreeVertex = objectTransformation.transformToWorldSpace( neighborTriangle->getVertex(1) );
			else
				neighborFreeVertex = objectTransformation.transformToWorldSpace( neighborTriangle->getVertex(0) );
			
			
			//***************************************************************************
			// Validate whether or not this is a diffracting edge.
			
			// If the free vertex of the neighboring triangle is on the
			// opposite side of the probed triangle as the listener, then 
			// we are vieing a diffraction edge. Otherwise we are viewing
			// the interior of a diffraction edge.
			if ( probedPlane.getSignedDistanceTo( neighborFreeVertex ) > Real(0) )
				continue;
				
			// This is now a confirmed valid diffraction edge.
			// This means that the listener is not viewing the interior of the edge.
			
			// Transform the neighbor triangle's plane into world space.
			neighborPlane = objectTransformation.transformToWorldSpace( neighborTriangle->getPlane() );
			
			// Make sure that the other triangle's normal points away from
			// the probed triangle's free vertex. This will result in both
			// normals pointing outwards when viewed from the diffracting side of the edge.
			if ( neighborPlane.getSignedDistanceTo( probedFreeVertex ) > Real(0) )
				neighborPlane = -neighborPlane;
			
			// The listener position should be in the opposite direction of
			// the neighbor triangle's normal in order for this to be a valid
			// diffraction.
			if ( neighborPlane.getSignedDistanceTo( listenerImagePosition ) > Real(0) )
				continue;
		}
		
		
		// For each source, find and validate the diffraction path if it exists.
		Size numSources = scene->getNumberOfSources();
		
		for ( Index s = 0; s < numSources; s++ )
		{
			const SoundSource& source = *scene->getSource(s);
			
			if ( !source.getIsEnabled() )
				continue;
			
			const Vector3& sourcePosition = source.getPosition();
			
			// Make sure that this source lies within the diffraction wedge. If not,
			// skip this source.
			if ( shadowRegionBoundary.getSignedDistanceTo( sourcePosition ) < Real(0) ||
				neighborPlane.getSignedDistanceTo( sourcePosition ) < Real(0) )
				continue;
			
			// Find the point of closest approach of the edge and the vector between
			// the listener and source.
			Vector3 listenerToSource = (sourcePosition - listenerImagePosition).normalize();
			
			Vector3 edgeVector = *sharedV2 - *sharedV1;
			Real edgeLength = edgeVector.getMagnitude();
			edgeVector /= edgeLength;
			
			Real t1;
			Real t2;
			
			computePointsOfClosestApproach( listenerImagePosition, listenerToSource,
											*sharedV1, edgeVector, t1, t2 );
			
			// Diffraction can only happen if the point of closest approach on the 
			// edge's line is within the boundaries of the edge.
			
			if ( t2 < Real(0) || t2 > edgeLength )
				continue;
			
			// Calculate the point on the edge at which diffraction occurs.
			Vector3 diffractionPoint = *sharedV1 + edgeVector*t2;
			
			// Calculate the vector from the diffraction point to the sound source.
			Vector3 diffractionPointToSource = sourcePosition - diffractionPoint;
			Real rayDistance = diffractionPointToSource.getMagnitude();
			diffractionPointToSource /= rayDistance;
			
			// Verify that the diffraction point is visible to the sound source.
			if ( rayTracer->traceBinaryOcclusionRay( Ray3( diffractionPoint + diffractionPointToSource*rayEpsilon,
															diffractionPointToSource ), 
													rayDistance - source.getRadius() ) )
				continue;
			
			
			Vector3 diffractionPointToListener = (listenerImagePosition - diffractionPoint).normalize();
			
			pathDescription.clearPoints();
			pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::SOURCE, &source ) );
			pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::EDGE_DIFFRACTION, &objectSpaceTriangle->getNeighbor(e) ) );
			
			// Validate the backwards reflection path for the diffracion point.
			Real totalDistance;
			Vector3 directionFromListener;
			Vector3 fakeDirectionToSource;
			FrequencyResponse attenuation;
			
			if ( validateReflectionPath( diffractionPoint + diffractionPointToListener*rayEpsilon,
										listenerPosition, Real(0), path, totalDistance,
										directionFromListener, fakeDirectionToSource, attenuation ) )
			{
				attenuation *= internal::DiffractionFrequencyResponse( sourcePosition,
																		listenerPosition,
																		diffractionPoint,
																		neighborPlane.normal,
																		probedPlane.normal,
																		edgeVector,
																		scene->getSpeedOfSound() );
				
				// Draw the last part of the diffraction path if necessary.
				if ( debugDrawingCache != NULL && debugDrawingCache->getReflectionPathsAreEnabled() )
				{
					ArrayList<Vector3> points( 2 );
					
					points.add( diffractionPoint );
					points.add( sourcePosition );
					
					debugDrawingCache->addDiffractionPath( points );
				}
				
				totalDistance += rayDistance;
				
				Real sourceSpeed = math::dot( source.getVelocity(), diffractionPointToSource );
				Real listenerSpeed = math::dot( listener.getVelocity(), directionFromListener );
				
				attenuation *= getSourceFrequencyResponse( source, diffractionPointToSource );
				
				pathDescription.addPoint( PropagationPathPoint( PropagationPathPoint::LISTENER, &listener ) );
				
				pathBuffer.getSourceBuffer(s).addPropagationPath(
							PropagationPath( directionFromListener*listener.getOrientation(),
												totalDistance, sourceSpeed - listenerSpeed,
												scene->getSpeedOfSound(),
												attenuation, pathDescription ) );
				foundContributions = true;
			}
		}
	}
	
	return foundContributions;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Ray-Triangle Intersection Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPropagator:: traceTransmissionRay( const Ray3& ray, Real maxDistance, FrequencyResponse& transmissionAttenuation )
{
	ArrayList<internal::RayTracer::RayIntersection> intersections;
	
	if ( rayTracer->traceTransmissionRay( ray, maxDistance, intersections ) )
	{
		if ( intersections.getSize() == 1 )
		{
			const SoundMaterial& material = intersections[0].triangle.triangle->getMaterial();
			const FrequencyResponse& materialReflectionAttenuation = material.getReflectionAttenuation();
			const FrequencyResponse& materialAbsorptionAttenuation = material.getAbsorptionAttenuation();
			
			for ( Index i = 0; i < transmissionAttenuation.getNumberOfBands(); i++ )
			{
				// The amount of sound that gets transmitted at each medium boundary.
				Real absorption = (Real(1) - materialReflectionAttenuation[i])*materialAbsorptionAttenuation[i];
				
				transmissionAttenuation[i] = absorption*absorption;
			}
		}
		else
		{
			// Sort the array of intersections in order of distance along the ray.
			std::qsort( intersections.getArrayPointer(), intersections.getSize(),
						sizeof(internal::RayTracer::RayIntersection), compareIntersectionsByDistance );
			
			// Set the transmission attenuation to be initially 1 for all frequency bands.
			transmissionAttenuation = FrequencyResponse();
			Index i;
			
			// Find the total transmission attenuation based on all intersections.
			for ( i = 0; i < intersections.getSize() - 1; i += 2 )
			{
				Real distance = intersections[i+1].distanceAlongRay - intersections[i].distanceAlongRay;
				
				const SoundMaterial& material1 = intersections[i].triangle.triangle->getMaterial();
				const SoundMaterial& material2 = intersections[i+1].triangle.triangle->getMaterial();
				
				for ( Index i = 0; i < transmissionAttenuation.getNumberOfBands(); i++ )
				{
					// The amount of sound that gets transmitted at each medium boundary.
					Real absorption1 = (Real(1) - material1.getReflectionAttenuation()[i])*
																material1.getAbsorptionAttenuation()[i];
					Real absorption2 = (Real(1) - material2.getReflectionAttenuation()[i])*
																material2.getAbsorptionAttenuation()[i];
					
					Real attenuation = (material1.getTransmissionAttenuation()[i] + 
										material2.getTransmissionAttenuation()[i])*Real(0.5);
					
					transmissionAttenuation[i] *= math::pow( attenuation, distance )*absorption1*absorption2;
				}
			}
		}
		
		return true;
	}
	
	return false;
}




int SoundPropagator:: compareIntersectionsByDistance( const void* a, const void* b )
{
	const internal::RayTracer::RayIntersection* i1 = (const internal::RayTracer::RayIntersection*)a;
	const internal::RayTracer::RayIntersection* i2 = (const internal::RayTracer::RayIntersection*)b;
	
	if ( i1->distanceAlongRay > i2->distanceAlongRay )
		return 1;
	else if ( i1->distanceAlongRay < i2->distanceAlongRay )
		return -1;
	else
		return 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Ray-Triangle Intersection Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPropagator:: rayIntersectsTriangle( const Ray3& ray, const internal::WorldSpaceTriangle& triangle,
											Real& distanceAlongRay )
{
	// find the edge vectors of the triangle
	Vector3 v1ToV2 = triangle.v2 - triangle.v1;
	Vector3 v1ToV3 = triangle.v3 - triangle.v1;
	
	// the vector perpendicular to edge 2 and the ray's direction
	Vector3 pvec = math::cross( ray.direction, v1ToV3 );
	
	Real det = math::dot( v1ToV2, pvec );
	
	if ( math::abs(det) < math::epsilon<Real>() )
		return false;
	
	Real inverseDet = Real(1) / det;
	
	Vector3 v1ToSource = ray.origin - triangle.v1;
	
	Real u = math::dot( v1ToSource, pvec ) * inverseDet;
	
	if ( u < Real(0) || u > Real(1) )
		return false;
	
	Vector3 qvec = math::cross( v1ToSource, v1ToV2 );
	
	Real v = math::dot( ray.direction, qvec ) * inverseDet;
	
	if ( v < Real(0) || u + v > Real(1) )
		return false;
	
	distanceAlongRay = math::dot( v1ToV3, qvec ) * inverseDet;
	
	return distanceAlongRay > Real(0);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Propagation Path Buffer Preparation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: preparePropagationPathBuffer( SoundPropagationPathBuffer& pathBuffer )
{
	// Set the number of sources that the buffer should hold.
	pathBuffer.setNumberOfSources( scene->getNumberOfSources() );
	
	// Reset the state of all source buffers.
	pathBuffer.clearPropagationPaths();
	
	// Make sure that each source buffer has the correct source pointer.
	for ( Index i = 0; i < pathBuffer.getNumberOfSources(); i++ )
		pathBuffer.getSourceBuffer(i).setSource( scene->getSource( i ) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Compute Points of Closest Approach of Two Lines Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: computePointsOfClosestApproach( const Vector3& p1, const Vector3& v1,
														const Vector3& p2, const Vector3& v2,
														Real& v1t, Real& v2t )
{
	Real v1DotV2 = math::dot( v1, v2 );
	Vector3 p2ToP1 = p1 - p2;
	
	// calculate the parameters of the axis lines of the points of closest approach.
	v2t = (math::dot( p2ToP1, v2 ) - math::dot( p2ToP1, v1 )*v1DotV2) / ( 1 - v1DotV2*v1DotV2 );
	v1t = (v2t*v1DotV2 - math::dot( p2ToP1, v1 ));
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Source Frequency Response Calculation Method
//############		
//##########################################################################################
//##########################################################################################




FrequencyResponse SoundPropagator:: getSourceFrequencyResponse( const SoundSource& source,
																const Vector3& viewDirection )
{
	if ( source.getIsDirectional() )
	{
		Real amountOnAxis = (Real(1) - math::dot( source.getDirection(), viewDirection ))*Real(0.5);
		
		return source.getOnAxisFrequencyResponse()*amountOnAxis + 
				source.getOffAxisFrequencyResponse()*(Real(1) - amountOnAxis);
	}
	else
		return source.getOnAxisFrequencyResponse();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Distance Delay Calculation Method
//############		
//##########################################################################################
//##########################################################################################




Real SoundPropagator:: getDelayForDistance( Real distance )
{
	GSOUND_DEBUG_ASSERT( scene != NULL );
	
	Real speedOfSound = scene->getSpeedOfSound();
	
	if ( speedOfSound == Real(0) )
		return math::infinity<Real>();
	else
		return distance / speedOfSound;
}




Real SoundPropagator:: getRelativeSpeed( const SoundListener& listener, const Vector3& directionFromListener, 
										const SoundSource& source, const Vector3& directionToSource )
{
	Real listenerSpeed = math::dot( listener.getVelocity(), directionFromListener );
	Real sourceSpeed = math::dot( source.getVelocity(), directionToSource );
	
	return sourceSpeed - listenerSpeed;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Scene Debug Drawing Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: drawSceneDebug() const
{
	// Make sure that we have a debug drawing cache and scene.
	if ( debugDrawingCache == NULL || scene == NULL )
		return;
	
	//*************************************************************************************
	
	if ( debugDrawingCache->getSourcesAreEnabled() )
	{
		Size numSources = scene->getNumberOfSources();
		
		for ( Index i = 0; i < numSources; i++ )
			debugDrawingCache->addSource( *scene->getSource(i) );
	}
	
	//*************************************************************************************
	
	ArrayList<SoundVertex> vertices;
	ArrayList<SoundTriangle> triangles;
	
	Size numObjects = scene->getNumberOfObjects();
	
	for ( Index i = 0; i < numObjects; i++ )
	{
		const SoundMesh& mesh = *scene->getObject(i)->getMesh();
		const Transformation3& transformation = scene->getObject(i)->getTransformation();
		
		for ( Index j = 0; j < mesh.getNumberOfVertices(); j++ )
			vertices.add( transformation.transformToWorldSpace( mesh.getVertex(j) ) );
		
		for ( Index j = 0; j < mesh.getNumberOfTriangles(); j++ )
		{
			SoundTriangle t = mesh.getTriangle(j);
			
			if ( debugDrawingCache->getObjectMeshesAreEnabled() )
				triangles.add( t );
			
			for ( Index e = 0; e < 3; e++ )
			{
				Index neighborIndex;
				
				if ( mesh.getTriangleNeighbor( j, e, neighborIndex ) )
				{
					if ( debugDrawingCache->getDiffractionEdgesAreEnabled() )
					{
						if ( neighborIndex < j && mesh.edgeIsDiffracting( j, e ) )
						{
							Index v1 = 0;
							Index v2 = 0;
							internal::InternalSoundTriangle::getEdgeVertexIndices( e, v1, v2 );
							debugDrawingCache->addDiffractionEdge( vertices[t.v[v1]], vertices[t.v[v2]] );
						}
					}
				}
				else if ( mesh.edgeIsDiffracting( j, e ) )
				{
					if ( debugDrawingCache->getDisconnectedDiffractionEdgesAreEnabled() )
					{
						Index v1 = 0;
						Index v2 = 0;
						internal::InternalSoundTriangle::getEdgeVertexIndices( e, v1, v2 );
						debugDrawingCache->addDisconnectedDiffractionEdge( vertices[t.v[v1]], vertices[t.v[v2]] );
					}
				}
			}
		}
		
		if ( debugDrawingCache->getObjectMeshesAreEnabled() )
		{
			debugDrawingCache->addObjectMesh( vertices, triangles );
			triangles.clear();
		}
		
		vertices.clear();
	}
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
