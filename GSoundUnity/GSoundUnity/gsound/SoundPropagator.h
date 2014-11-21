/*
 * Project:     GSound
 * 
 * File:        gsound/SoundPropagator.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundPropagator class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_PROPAGATOR_H
#define INCLUDE_GSOUND_SOUND_PROPAGATOR_H


#include "GSoundBase.h"
#ifdef GSOUND_EXPORTS
#define GSOUND_API __declspec(dllexport) 
#else 
#define GSOUND_API __declspec(dllimport) 
#endif 

#include "internal/RayTracer.h"
#include "internal/WorldSpaceTriangle.h"
#include "SoundScene.h"
#include "SoundPropagationPathBuffer.h"
#include "DebugDrawingCache.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




namespace internal
{
	class ProbePathCache;
};




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which performs sound propagation.
class GSOUND_API SoundPropagator
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a sound propagator object with the default initial state.
			SoundPropagator();
			
			
			
			
			/// Create a copy of a sound propagator object, copying all internal state of that object.
			SoundPropagator( const SoundPropagator& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy a sound propagator object, releasing all internal state and memory.
			~SoundPropagator();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the state of another sound propagator object to this object.
			SoundPropagator& operator = ( const SoundPropagator& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Propagation Method
			
			
			
			
			/// Perform sound propagation in the specified scene with the given listener.
			/**
			  * This is the primary method by which sound propagation is performed. When
			  * this method is called, the specified scene is used to describe where sound propagation
			  * is performed: the objects in the scene and the sources for which propagation should
			  * be performed.
			  *
			  * The listener specifies the reciever of the sound. The primary propagation step
			  * traces probe rays from this listener's position through the scene to find
			  * propagation paths. These probe rays are reflected in the scene up to the
			  * specified maximum probe depth.
			  *
			  * As of version 1.0.0, sound propagation is not performed from each SoundSource's
			  * perspective (as is done from the listener). Source probe rays are used only for
			  * the reverb estimation step of the propagation pipeline. They are propagated up
			  * to the specified maximum source probe depth and this propagation is used to 
			  * estimate the scene's volume, surface area, and average attenuation for use
			  * in reverb time estimation. In future versions, source-probed sound propagation
			  * may be enabled.
			  *
			  * The output of the sound propagation is placed in the specified propagation path buffer.
			  * The path buffer's current contents are removed and replaced with contents that indicate
			  * the results of the propagation.
			  * 
			  * @param scene - the scene where sound propagation should be performed.
			  * @param listener - the listener from whose perspective sound propagation is to be performed.
			  * @param maxListenerProbeDepth - the maximum depth to which listener probe rays should be propagated.
			  * @param numListenerProbeRays - the number of initial listener probe rays to trace.
			  * @param maxSourceProbeDepth - the maximum depth to which source probe rays should be propagated.
			  * @param numSourceProbeRays - the number of initial source probe rays to trace per source.
			  * @param propagationPathBuffer - a buffer where output of the propagation should be placed.
			  */
			GSOUND_INLINE void propagateSound( const SoundScene& scene, const SoundListener& listener,
								Size maxListenerProbeDepth, Size numListenerProbeRays,
								Size maxSourceProbeDepth, Size numSourceProbeRays,
								SoundPropagationPathBuffer& propagationPathBuffer )
			{
				this->propagateSound( scene, listener, maxListenerProbeDepth, numListenerProbeRays,
									maxSourceProbeDepth, numSourceProbeRays, propagationPathBuffer,
									NULL );
			}
			
			
			
			
			/// Perform sound propagation in the specified scene with the given listener, caching debug drawing information.
			/**
			  * Debug information is placed into the specified DebugDrawingCache which is cleared
			  * before the sound propagation begins. Each type of debug information that is enabled
			  * in the DebugDrawingCache is placed in the cache and those types that are disabled
			  * are ignored.
			  * 
			  * @param scene - the scene where sound propagation should be performed.
			  * @param listener - the listener from whose perspective sound propagation is to be performed.
			  * @param maxListenerProbeDepth - the maximum depth to which listener probe rays should be propagated.
			  * @param numListenerProbeRays - the number of initial listener probe rays to trace.
			  * @param maxSourceProbeDepth - the maximum depth to which source probe rays should be propagated.
			  * @param numSourceProbeRays - the number of initial source probe rays to trace per source.
			  * @param propagationPathBuffer - a buffer where output of the propagation should be placed.
			  * @param debugCache - a DebugDrawingCache in which to place debug drawing information.
			  */
			GSOUND_INLINE void propagateSound( const SoundScene& scene, const SoundListener& listener,
												Size maxListenerProbeDepth, Size numListenerProbeRays,
												Size maxSourceProbeDepth, Size numSourceProbeRays,
												SoundPropagationPathBuffer& propagationPathBuffer,
												DebugDrawingCache& debugCache )
			{
				this->propagateSound( scene, listener, maxListenerProbeDepth, numListenerProbeRays,
									maxSourceProbeDepth, numSourceProbeRays, propagationPathBuffer,
									&debugCache );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Direct Sound Enable Accessor Methods
			
			
			
			
			/// Get whether or not direct sound contributions are enabled.
			GSOUND_INLINE Bool getDirectSoundIsEnabled() const
			{
				return directSoundIsEnabled;
			}
			
			
			
			
			/// Set whether or not direct sound contributions are enabled.
			GSOUND_INLINE void setDirectSoundIsEnabled( Bool newDirectSoundIsEnabled )
			{
				directSoundIsEnabled = newDirectSoundIsEnabled;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Transmitted Sound Enable Accessor Methods
			
			
			
			
			/// Get whether or not transmitted sound contributions are enabled.
			GSOUND_INLINE Bool getTransmissionIsEnabled() const
			{
				return transmissionIsEnabled;
			}
			
			
			
			
			/// Set whether or not transmitted sound contributions are enabled.
			GSOUND_INLINE void setTransmissionIsEnabled( Bool newTransmissionIsEnabled )
			{
				transmissionIsEnabled = newTransmissionIsEnabled;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Reflected Sound Enable Accessor Methods
			
			
			
			
			/// Get whether or not reflected sound contributions are enabled.
			GSOUND_INLINE Bool getReflectionIsEnabled() const
			{
				return reflectionIsEnabled;
			}
			
			
			
			
			/// Set whether or not reflected sound contributions are enabled.
			GSOUND_INLINE void setReflectionIsEnabled( Bool newReflectionIsEnabled )
			{
				reflectionIsEnabled = newReflectionIsEnabled;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Diffracted Sound Enable Accessor Methods
			
			
			
			
			/// Get whether or not diffracted sound contributions are enabled.
			GSOUND_INLINE Bool getDiffractionIsEnabled() const
			{
				return diffractionIsEnabled;
			}
			
			
			
			
			/// Set whether or not diffracted sound contributions are enabled.
			GSOUND_INLINE void setDiffractionIsEnabled( Bool newDiffractionIsEnabled )
			{
				diffractionIsEnabled = newDiffractionIsEnabled;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Reverb Enable Accessor Methods
			
			
			
			
			/// Get whether or not reverb sound contributions are enabled.
			GSOUND_INLINE Bool getReverbIsEnabled() const
			{
				return reverbIsEnabled;
			}
			
			
			
			
			/// Set whether or not reverb sound contributions are enabled.
			GSOUND_INLINE void setReverbIsEnabled( Bool newReverbIsEnabled )
			{
				reverbIsEnabled = newReverbIsEnabled;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Ray Intersection Point Bias Accessor Methods
			
			
			
			
			/// Get the value used to bias ray-triangle intersection points away from the triangle.
			/**
			  * This is done to reduce the prevalence of precision problems in ray tracing. A good
			  * rule of thumb is to set this value to be an order of magnitude smaller than the smallest
			  * features in a scene.
			  */
			GSOUND_INLINE Real getRayEpsilon() const
			{
				return rayEpsilon;
			}
			
			
			
			
			/// Set the value used to bias ray-triangle intersection points away from the triangle.
			/**
			  * This is done to reduce the prevalence of precision problems in ray tracing. A good
			  * rule of thumb is to set this value to be an order of magnitude smaller than the smallest
			  * features in a scene. The supplied value must be greater than or equal to zero.
			  */
			GSOUND_INLINE void setRayEpsilon( Real newRayEpsilon )
			{
				rayEpsilon = math::max( Real(0), newRayEpsilon );
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Probe Intersection Record Class Declaration
			
			
			
			
			/// A class which keeps track of the intersected triangle and listener image position at each probe depth.
			class ProbeIntersectionRecord;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Sound Propagation Methods
			
			
			
			
			/// Perform sound propagation in the specified scene with the given listener.
			void propagateSound( const SoundScene& scene, const SoundListener& listener,
								Size maxListenerProbeDepth, Size numListenerProbeRays,
								Size maxSourceProbeDepth, Size numSourceProbeRays,
								SoundPropagationPathBuffer& propagationPathBuffer,
								DebugDrawingCache* debugCache );
			
			
			
			
			/// Do sound propagation from the listener's perspective, adding all paths to the propagation path buffer.
			void doListenerPropagation( const SoundListener& listener,
										Size maxListenerProbeDepth, Size numListenerProbeRays,
										SoundPropagationPathBuffer& pathBuffer );
			
			
			
			
			/// Do sound propagation from each sources's perspective, adding all paths to the propagation path buffer.
			void doSourcePropagation( const SoundListener& listener,
										Size maxSourceProbeDepth, Size numSourceProbeRays,
										SoundPropagationPathBuffer& pathBuffer );
			
			
			
			
			/// Check all previously cached probe paths for validity. If they are valid, add them to the path buffer.
			void validateCachedPaths( const SoundListener& listener, SoundPropagationPathBuffer& pathBuffer );
			
			
			
			
			/// Add all direct/transmitted propagation paths to the propagation path buffer.
			void addDirectPaths( const SoundListener& listener, SoundPropagationPathBuffer& pathBuffer );
			
			
			
			
			Bool validateReflectionPath( const Vector3& sourcePosition, const Vector3& listenerPosition,
										Real sourceRadius,
										const ArrayList<ProbeIntersectionRecord>& path, Real& totalDistance,
										Vector3& directionFromListener, Vector3& directionToSource,
										FrequencyResponse& attenuation );
			
			
			
			
			/// Add any valid diffraction propagation paths for the specified triangle to the propagation path buffer.
			Bool addDiffractionPaths( const SoundListener& listener,
									const internal::WorldSpaceTriangle& probedTriangle,
									const ArrayList<ProbeIntersectionRecord>& path,
									SoundPropagationPathBuffer& pathBuffer );
			
			
		
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Intersection Helper Methods
			
			
			
			
			/// Return whether or not the specified ray and world-space triangle intersect.
			/**
			  * This method computes the distance along the ray of the intersection point.
			  */
			GSOUND_INLINE static Bool rayIntersectsTriangle( const Ray3& ray, const internal::WorldSpaceTriangle& triangle,
															Real& distanceAlongRay );
			
			
		
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Transmission Ray Helper Methods
			
			
			
			
			/// Trace a transmission ray through the scene in order to find the transmission attenuation.
			Bool traceTransmissionRay( const Ray3& ray, Real maxDistance, FrequencyResponse& transmissionAttenuation );
			
			
			
			
			static int compareIntersectionsByDistance( const void* a, const void* b );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Helper Methods
			
			
			
			
			/// Send static scene information to the debug drawing cache if there is one.
			/**
			  * This includes all scene geometry, sound sources, and listeners.
			  */
			void drawSceneDebug() const;
			
			
			
			
			/// Prepare the specified propagation path buffer to accept propagation paths.
			GSOUND_INLINE void preparePropagationPathBuffer( SoundPropagationPathBuffer& propagationPathBuffer );
			
			
			
			
			/// Find the points of closest approach on two lines.
			GSOUND_INLINE static void computePointsOfClosestApproach( const Vector3& p1, const Vector3& v1,
																	const Vector3& p2, const Vector3& v2,
																	Real& v1t, Real& v2t );
			
			
			
			
			/// Get the frequency response of the specified sound source when viewed from the given direction.
			GSOUND_INLINE static FrequencyResponse getSourceFrequencyResponse( const SoundSource& source,
																				const Vector3& viewDirection );
			
			
			
			
			/// Get the delay time in seconds that it takes sound to travel the specified distance in the current scene.
			GSOUND_INLINE Real getDelayForDistance( Real distance );
			
			
			
			
			/// Get the relative speed of the specified listener and source along the specified path.
			GSOUND_INLINE Real getRelativeSpeed( const SoundListener& listener, const Vector3& directionFromListener, 
												const SoundSource& source, const Vector3& directionToSource );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A temporary pointer to the current scene where sound propagation is being performed.
			const SoundScene* scene;
			
			
			
			
			/// A temporary pointer to a cache which buffers debug drawing information produced during propagation.
			DebugDrawingCache* debugDrawingCache;
			
			
			
			
			/// A pointer to the ray tracer which this sound propagator uses to trace rays through a scene.
			internal::RayTracer* rayTracer;
			
			
			
			
			/// An object which is used to accumulate all points along a give propagation path.
			PropagationPathDescription pathDescription;
			
			
			
			
			/// A random variable which generates the initial directions for probe rays.
			math::RandomVariable<Real> probeRandomVariable;
			
			
			
			
			Index timeStamp;
			
			
			
			
			/// A small value used to bias ray-triangle intersection points away from the triangle.
			/**
			  * This is done to reduce the prevalence of precision problems in ray tracing. A good
			  * rule of thumb is to set this value to be an order of magnitude smaller than the smallest
			  * features in a scene.
			  */
			Real rayEpsilon;
			
			
			
			
			/// Whether or not direct sound contributions are found.
			Bool directSoundIsEnabled;
			
			
			
			
			/// Whether or not transmitted sound contributions are found.
			Bool transmissionIsEnabled;
			
			
			
			
			/// Whether or not reflected contributions are found.
			Bool reflectionIsEnabled;
			
			
			
			
			/// Whether or not diffracted contributions are found.
			Bool diffractionIsEnabled;
			
			
			
			
			/// Whether or not late reverb characteristics are determined.
			Bool reverbIsEnabled;
			
			
			
			
			Size maxReverbCacheAge;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_PROPAGATOR_H
