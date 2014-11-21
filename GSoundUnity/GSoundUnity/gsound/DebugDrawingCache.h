/*
 * Project:     GSound
 * 
 * File:        gsound/DebugDrawingCache.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::DebugDrawingCache class declaration
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

#ifndef INCLUDE_GSOUND_DEBUG_DRAWING_CACHE_H
#define INCLUDE_GSOUND_DEBUG_DRAWING_CACHE_H


#include "GSoundBase.h"


#include "SoundVertex.h"
#include "SoundTriangle.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class used to cache debug information so that it can be drawn by an external renderer.
/**
  * This class consists of a list of elements to be drawn for each type of sound
  * propagation debug information. In practice, one passes this class to a 
  * SoundPropagator object when performing sound propagation. The SoundPropagator
  * object then adds debugging information to the DebugDrawingCache which can
  * then be drawn later by an external renderer.
  *
  * The types of debugging information that a DebugDrawingCache contains are:
  * - Finite Probe Rays: These are probe rays that hit a triangle in the scene.
  * - Infinite Probe Rays: These are probe rays that didn't hit anything in the scene.
  * - Direct Paths: These are unoccluded propagation paths from sound sources to the listener.
  * - Transmission Paths: These are direct paths that passed through an object in the scene.
  * - Reflection Paths: These are paths that consist of one or more specular reflections through the scene.
  * - Diffraction Paths: These are propagation paths where sound has diffracted over an edge.
  * - Object Meshes: These are world-space indexes mesh representations of all scene objects.
  * - Diffraction Edges: These are edges shared by two triangles that have been marked as diffracting.
  * - Disconnected Diffraction Edges: These are unshared edges in the scene that are diffracting.
  * - Sound Sources: These are the sound sources for which sound propagation has been performed.
  * - Sound Listeners: These are the sound listeners with which sound proapgation has been performed.
  *
  * Each of these types of debugging information can be enabled or disabled
  * if only certain types are needed. This can also provide a performance boost by
  * reducing the work that the SoundPropagator must do in order to produce the debugging
  * information.
  */
class DebugDrawingCache
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Class Declarations
			
			
			
			
			/// A class which contains the start and endpoint of a probe ray which hit a triangle.
			class FiniteProbeRay
			{
				public:
					
					GSOUND_INLINE FiniteProbeRay( const Vector3& newStart, const Vector3& newEnd )
						:	start( newStart ),
							end( newEnd )
					{
					}
					
					
					
					Vector3 start;
					Vector3 end;
					
			};
			
			
			
			
			/// A class which contains the position of the listener and source of a direct propagation path.
			class DirectPath
			{
				public:
					
					GSOUND_INLINE DirectPath( const Vector3& newListenerPosition, const Vector3& newSourcePosition )
						:	listenerPosition( newListenerPosition ),
							sourcePosition( newSourcePosition )
					{
					}
					
					
					
					Vector3 listenerPosition;
					Vector3 sourcePosition;
					
			};
			
			
			
			
			/// A class containing a list of points which represent interactions along an indirect propagation path.
			class IndirectPath
			{
				public:
					
					GSOUND_INLINE IndirectPath( const ArrayList<Vector3>& newPoints )
						:	points( newPoints )
					{
					}
					
					
					ArrayList<Vector3> points;
					
					
			};
			
			
			
			
			/// A class which contains the two endpoints of a diffraction edge.
			class DiffractionEdge
			{
				public:
					
					GSOUND_INLINE DiffractionEdge( const Vector3& newV1, const Vector3& newV2 )
						:	v1( newV1 ),
							v2( newV2 )
					{
					}
					
					
					Vector3 v1;
					Vector3 v2;
					
					
			};
			
			
			
			
			/// A class containing a list of the vertices and indexed triangles of a single object in a scene.
			class ObjectMesh
			{
				public:
					
					GSOUND_INLINE ObjectMesh( const ArrayList<SoundVertex>& newVertices,
												const ArrayList<SoundTriangle>& newTriangles )
						:	vertices( newVertices ),
							triangles( newTriangles )
					{
					}
					
					
					ArrayList<SoundVertex> vertices;
					ArrayList<SoundTriangle> triangles;
					
					
					
					
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			/// Create a debug drawing cache with the default data buffers enabled.
			GSOUND_INLINE DebugDrawingCache()
				:	finiteProbeRaysEnabled( false ),
					infiniteProbeRaysEnabled( false ),
					directPathsEnabled( true ),
					transmissionPathsEnabled( true ),
					reflectionPathsEnabled( true ),
					diffractionPathsEnabled( true ),
					objectMeshesEnabled( true ),
					diffractionEdgesEnabled( true ),
					disconnectedDiffractionEdgesEnabled( true ),
					sourcesAreEnabled( true ),
					listenersAreEnabled( true )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Finite Probe Ray Accessor Methods
			
			
			
			
			/// Get whether or not caching of finite probe rays (rays that hit a triangle) is enabled.
			GSOUND_INLINE Bool getFiniteProbeRaysAreEnabled() const
			{
				return finiteProbeRaysEnabled;
			}
			
			
			
			
			/// Set whether or not caching of finite probe rays (rays that hit a triangle) is enabled.
			GSOUND_INLINE void setFiniteProbeRaysAreEnabled( Bool newFiniteProbeRaysEnabled )
			{
				finiteProbeRaysEnabled = newFiniteProbeRaysEnabled;
			}
			
			
			
			
			/// Add a finite probe ray with the specified start and end points to this debug drawing cache.
			GSOUND_INLINE void addFiniteProbeRay( const Vector3& start, const Vector3& end )
			{
				if ( finiteProbeRaysEnabled )
					finiteProbeRays.add( FiniteProbeRay( start, end ) );
			}
			
			
			
			/// Get a const reference to the internal list of finite probe rays.
			GSOUND_INLINE const ArrayList<FiniteProbeRay>& getFiniteProbeRays() const
			{
				return finiteProbeRays;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Infinite Probe Ray Accessor Methods
			
			
			
			
			/// Get whether or not caching of infinite probe rays (rays that don't hit anything) is enabled.
			GSOUND_INLINE Bool getInfiniteProbeRaysAreEnabled() const
			{
				return infiniteProbeRaysEnabled;
			}
			
			
			
			
			/// Set whether or not caching of infinite probe rays (rays that don't hit anything) is enabled.
			GSOUND_INLINE void setInfiniteProbeRaysAreEnabled( Bool newInfiniteProbeRaysEnabled )
			{
				infiniteProbeRaysEnabled = newInfiniteProbeRaysEnabled;
			}
			
			
			
			
			/// Add an infinite probe ray to this debug drawing cache.
			GSOUND_INLINE void addInfiniteProbeRay( const Ray3& probeRay )
			{
				if ( infiniteProbeRaysEnabled )
					infiniteProbeRays.add( probeRay );
			}
			
			
			
			
			/// Get a const reference to the internal list of infinite probe rays.
			GSOUND_INLINE const ArrayList<Ray3>& getInfiniteProbeRays() const
			{
				return infiniteProbeRays;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Direct Propagation Path Accessor Methods
			
			
			
			
			/// Get whether or not caching of direct propagation paths is enabled.
			GSOUND_INLINE Bool getDirectPathsAreEnabled() const
			{
				return directPathsEnabled;
			}
			
			
			
			
			/// Set whether or not caching of direct propagation paths is enabled.
			GSOUND_INLINE void setDirectPathsAreEnabled( Bool newDirectPathsEnabled )
			{
				directPathsEnabled = newDirectPathsEnabled;
			}
			
			
			
			
			/// Add a direct propagation path to this cache with the specified listener and source positions.
			GSOUND_INLINE void addDirectPath( const Vector3& listenerPosition, const Vector3& sourcePosition )
			{
				if ( directPathsEnabled )
					directPaths.add( DirectPath( listenerPosition, sourcePosition ) );
			}
			
			
			
			
			/// Get a const reference to the internal list of direct propagation paths.
			GSOUND_INLINE const ArrayList<DirectPath>& getDirectPaths() const
			{
				return directPaths;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Transmission Propagation Path Accessor Methods
			
			
			
			
			/// Get whether or not caching of transmission propagation paths is enabled.
			GSOUND_INLINE Bool getTransmissionPathsAreEnabled() const
			{
				return transmissionPathsEnabled;
			}
			
			
			
			
			/// Set whether or not caching of transmission propagation paths is enabled.
			GSOUND_INLINE void setTransmissionPathsAreEnabled( Bool newTransmissionPathsEnabled )
			{
				transmissionPathsEnabled = newTransmissionPathsEnabled;
			}
			
			
			
			
			/// Add a transmission propagation path to this cache with the specified listener and source positions.
			GSOUND_INLINE void addTransmissionPath( const Vector3& listenerPosition, const Vector3& sourcePosition )
			{
				if ( transmissionPathsEnabled )
					transmissionPaths.add( DirectPath( listenerPosition, sourcePosition ) );
			}
			
			
			
			
			/// Get a const reference to the internal list of transmission propagation paths.
			GSOUND_INLINE const ArrayList<DirectPath>& getTransmissionPaths() const
			{
				return transmissionPaths;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Reflection Propagation Path Accessor Methods
			
			
			
			
			/// Get whether or not caching of reflection propagation paths is enabled.
			GSOUND_INLINE Bool getReflectionPathsAreEnabled() const
			{
				return reflectionPathsEnabled;
			}
			
			
			
			
			/// Set whether or not caching of reflection propagation paths is enabled.
			GSOUND_INLINE void setReflectionPathsAreEnabled( Bool newReflectionPathsEnabled )
			{
				reflectionPathsEnabled = newReflectionPathsEnabled;
			}
			
			
			
			
			/// Add a reflection propagation path with the specified path points to this debug drawing cache.
			GSOUND_INLINE void addReflectionPath( const ArrayList<Vector3>& points )
			{
				if ( reflectionPathsEnabled )
					reflectionPaths.add( IndirectPath( points ) );
			}
			
			
			
			
			/// Get a const reference to the internal list of reflection propagation paths.
			GSOUND_INLINE const ArrayList<IndirectPath>& getReflectionPaths() const
			{
				return reflectionPaths;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Diffraction Propagation Path Accessor Methods
			
			
			
			
			/// Get whether or not caching of diffraction propagation paths is enabled.
			GSOUND_INLINE Bool getDiffractionPathsAreEnabled() const
			{
				return diffractionPathsEnabled;
			}
			
			
			
			
			/// Set whether or not caching of diffraction propagation paths is enabled.
			GSOUND_INLINE void setDiffractionPathsAreEnabled( Bool newDiffractionPathsEnabled )
			{
				diffractionPathsEnabled = newDiffractionPathsEnabled;
			}
			
			
			
			
			/// Add a diffraction propagation path with the specified path points to this debug drawing cache.
			GSOUND_INLINE void addDiffractionPath( const ArrayList<Vector3>& points )
			{
				if ( diffractionPathsEnabled )
					diffractionPaths.add( IndirectPath( points ) );
			}
			
			
			
			
			/// Get a const reference to the internal list of diffraction propagation paths.
			GSOUND_INLINE const ArrayList<IndirectPath>& getDiffractionPaths() const
			{
				return diffractionPaths;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Object Mesh Accessor Methods
			
			
			
			
			/// Get whether or not caching of object meshes is enabled.
			GSOUND_INLINE Bool getObjectMeshesAreEnabled() const
			{
				return objectMeshesEnabled;
			}
			
			
			
			
			/// Set whether or not caching of object meshes is enabled.
			GSOUND_INLINE void setObjectMeshesAreEnabled( Bool newObjectMeshesEnabled )
			{
				objectMeshesEnabled = newObjectMeshesEnabled;
			}
			
			
			
			
			/// Add an object mesh with the specified vertices and triangles to this debug drawing cache.
			GSOUND_INLINE void addObjectMesh( const ArrayList<SoundVertex>& vertices,
												const ArrayList<SoundTriangle>& triangles )
			{
				if ( objectMeshesEnabled )
					objectMeshes.add( ObjectMesh( vertices, triangles ) );
			}
			
			
			
			
			/// Get a const reference to the internal list of object meshes.
			GSOUND_INLINE const ArrayList<ObjectMesh>& getObjectMeshes() const
			{
				return objectMeshes;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Diffraction Edges Accessor Methods
			
			
			
			
			/// Get whether or not caching of diffraction edges is enabled.
			GSOUND_INLINE Bool getDiffractionEdgesAreEnabled() const
			{
				return diffractionEdgesEnabled;
			}
			
			
			
			
			/// Set whether or not caching of diffraction edges is enabled.
			GSOUND_INLINE void setDiffractionEdgesAreEnabled( Bool newDiffractionEdgesEnabled )
			{
				diffractionEdgesEnabled = newDiffractionEdgesEnabled;
			}
			
			
			
			
			/// Add a diffraction edge with the specified edge endpoints to this debug drawing cache.
			GSOUND_INLINE void addDiffractionEdge( const Vector3& v1, const Vector3& v2 )
			{
				if ( diffractionEdgesEnabled )
					diffractionEdges.add( DiffractionEdge( v1, v2 ) );
			}
			
			
			
			
			/// Get a const reference to the internal list of diffraction edges.
			GSOUND_INLINE const ArrayList<DiffractionEdge>& getDiffractionEdges() const
			{
				return diffractionEdges;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Disconnected Diffraction Edges Accessor Methods
			
			
			
			
			/// Get whether or not caching of disconnected diffraction edges is enabled.
			GSOUND_INLINE Bool getDisconnectedDiffractionEdgesAreEnabled() const
			{
				return disconnectedDiffractionEdgesEnabled;
			}
			
			
			
			
			/// Set whether or not caching of disconnected diffraction edges is enabled.
			GSOUND_INLINE void setDisconnectedDiffractionEdgesAreEnabled( Bool newDisconnectedDiffractionEdgesEnabled )
			{
				disconnectedDiffractionEdgesEnabled = newDisconnectedDiffractionEdgesEnabled;
			}
			
			
			
			
			/// Add a disconnected diffraction edge with the specified edge endpoints to this debug drawing cache.
			GSOUND_INLINE void addDisconnectedDiffractionEdge( const Vector3& v1, const Vector3& v2 )
			{
				if ( disconnectedDiffractionEdgesEnabled )
					disconnectedDiffractionEdges.add( DiffractionEdge( v1, v2 ) );
			}
			
			
			
			
			/// Get a const reference to the internal list of diffraction edges.
			GSOUND_INLINE const ArrayList<DiffractionEdge>& getDisconnectedDiffractionEdges() const
			{
				return disconnectedDiffractionEdges;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Source Accessor Methods
			
			
			
			
			/// Get whether or not caching of sound sources is enabled.
			GSOUND_INLINE Bool getSourcesAreEnabled() const
			{
				return sourcesAreEnabled;
			}
			
			
			
			
			/// Set whether or not caching of sound sources is enabled.
			GSOUND_INLINE void setSourcesAreEnabled( Bool newSourcesAreEnabled )
			{
				sourcesAreEnabled = newSourcesAreEnabled;
			}
			
			
			
			
			/// Add a sound source to this debug drawing cache.
			GSOUND_INLINE void addSource( const SoundSource& source )
			{
				if ( sourcesAreEnabled )
					sources.add( &source );
			}
			
			
			
			
			/// Get a const reference to the internal list of sound sources.
			GSOUND_INLINE const ArrayList<const SoundSource*>& getSources() const
			{
				return sources;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Listener Accessor Methods
			
			
			
			
			/// Get whether or not caching of sound listeners is enabled.
			GSOUND_INLINE Bool getListenersAreEnabled() const
			{
				return listenersAreEnabled;
			}
			
			
			
			
			/// Set whether or not caching of sound listeners is enabled.
			GSOUND_INLINE void setListenersAreEnabled( Bool newListenersAreEnabled )
			{
				listenersAreEnabled = newListenersAreEnabled;
			}
			
			
			
			
			/// Add a sound listener to this debug drawing cache.
			GSOUND_INLINE void addListener( const SoundListener& listener )
			{
				if ( listenersAreEnabled )
					listeners.add( &listener );
			}
			
			
			
			
			/// Get a const reference to the internal list of sound sources.
			GSOUND_INLINE const ArrayList<const SoundListener*>& getListeners() const
			{
				return listeners;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Cache Clear Method
			
			
			
			
			/// Clear all of the internal arrays of debug drawing information.
			GSOUND_INLINE void clear()
			{
				finiteProbeRays.clear();
				infiniteProbeRays.clear();
				directPaths.clear();
				transmissionPaths.clear();
				reflectionPaths.clear();
				diffractionPaths.clear();
				objectMeshes.clear();
				diffractionEdges.clear();
				disconnectedDiffractionEdges.clear();
				sources.clear();
				listeners.clear();
			};
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			ArrayList<FiniteProbeRay> finiteProbeRays;
			
			
			
			
			Bool finiteProbeRaysEnabled;
			
			
			
			
			ArrayList<Ray3> infiniteProbeRays;
			
			
			
			
			Bool infiniteProbeRaysEnabled;
			
			
			
			
			ArrayList<DirectPath> directPaths;
			
			
			
			
			Bool directPathsEnabled;
			
			
			
			
			ArrayList<DirectPath> transmissionPaths;
			
			
			
			
			Bool transmissionPathsEnabled;
			
			
			
			
			ArrayList<IndirectPath> reflectionPaths;
			
			
			
			
			Bool reflectionPathsEnabled;
			
			
			
			
			ArrayList<IndirectPath> diffractionPaths;
			
			
			
			
			Bool diffractionPathsEnabled;
			
			
			
			
			ArrayList<ObjectMesh> objectMeshes;
			
			
			
			
			Bool objectMeshesEnabled;
			
			
			
			
			ArrayList<DiffractionEdge> diffractionEdges;
			
			
			
			
			Bool diffractionEdgesEnabled;
			
			
			
			
			ArrayList<DiffractionEdge> disconnectedDiffractionEdges;
			
			
			
			
			Bool disconnectedDiffractionEdgesEnabled;
			
			
			
			
			ArrayList<const SoundSource*> sources;
			
			
			
			
			Bool sourcesAreEnabled;
			
			
			
			
			ArrayList<const SoundListener*> listeners;
			
			
			
			
			Bool listenersAreEnabled;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_DEBUG_DRAWING_CACHE_H
