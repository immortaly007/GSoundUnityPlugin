/*
 * Project:     GSound
 * 
 * File:        gsound/internal/InternalSoundTriangle.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::InternalSoundTriangle class declaration
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


#ifndef INCLUDE_GSOUND_INTERNAL_SOUND_TRIANGLE_H
#define INCLUDE_GSOUND_INTERNAL_SOUND_TRIANGLE_H


#include "GSoundInternalConfig.h"


#include "../SoundMaterial.h"
#include "../SoundVertex.h"
#include "../SoundTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which is used internally to represent a triangle that is part of a mesh.
/**
  * Each triangle has pointers to the vertices and material that it shares, pointers
  * to any neighboring triangles, and edge diffraction type indicators. This data
  * is needed internally in order perform fast sound propagation.
  */
class InternalSoundTriangle
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Edge Diffraction Type Enum Definition
			
			
			
			
			/// An enum which represents the diffraction type of an edge.
			typedef enum EdgeDiffractionType
			{
				NON_DIFFRACTING = 0,
				DIFFRACTING = 1
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			/// Create an internal sound triangle with the specified vertex and material pointers.
			GSOUND_INLINE InternalSoundTriangle( const SoundVertex* newV1, const SoundVertex* newV2, const SoundVertex* newV3,
											const SoundMaterial* newMaterial )
				:	plane( *newV1, *newV2, *newV3 ),
					material( newMaterial )
			{
				vertex[0] = newV1;
				vertex[1] = newV2;
				vertex[2] = newV3;
				edgeType[0] = DIFFRACTING;
				edgeType[1] = DIFFRACTING;
				edgeType[2] = DIFFRACTING;
				neighbor[0] = NULL;
				neighbor[1] = NULL;
				neighbor[2] = NULL;
				area = Real(0.5)*cross( (Vector3)(*newV3 - *newV1), (Vector3)(*newV3 - *newV2) ).getMagnitude();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Vertex Accessor Method
			
			
			
			
			/// Return a reference to the vertex at the specified index in this triangle.
			GSOUND_FORCE_INLINE const SoundVertex& getVertex( Index index ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( index < 3, "Cannot access vertex at invalid index in triangle." );
				
				return *vertex[index];
			}
			
			
			
			
			/// Set the triangle's vertex at the specified index.
			GSOUND_FORCE_INLINE void setVertex( Index index, const SoundVertex* newVertex )
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( index < 3, "Cannot set vertex at invalid index in triangle." );
				
				vertex[index] = newVertex;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Material Accessor Method
			
			
			
			
			/// Return a reference to the SoundMaterial associated with this triangle.
			GSOUND_FORCE_INLINE const SoundMaterial& getMaterial() const
			{
				return *material;
			}
			
			
			
			
			/// Set the SoundMaterial associated with this triangle.
			GSOUND_FORCE_INLINE void setMaterial( const SoundMaterial* newMaterial )
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( newMaterial != NULL, "Cannot use NULL material for triangle." );
				
				material = newMaterial;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Plane Accessor Methods
			
			
			
			
			/// Return a reference to an object representing the plane of this triangle.
			GSOUND_FORCE_INLINE const Plane3& getPlane() const
			{
				return plane;
			}
			
			
			
			
			/// Return a reference to the normal of this triangle.
			GSOUND_FORCE_INLINE const Vector3& getNormal() const
			{
				return plane.normal;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Area Accessor Methods
			
			
			
			
			/// Return the area of the triangle in square units.
			GSOUND_FORCE_INLINE Real getArea() const
			{
				return area;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Neighbor Accessor Methods
			
			
			
			
			/// Return a pointer to the triangle's neighbor along the specified edge index.
			/**
			  * If there is no neighbor along that edge, NULL is returned.
			  *
			  * Edge indices are denoted as follows:
			  * - 0: edge between vertices 0 and 1
			  * - 1: edge between vertices 0 and 2
			  * - 2: edge between vertices 1 and 2
			  */
			GSOUND_FORCE_INLINE const InternalSoundTriangle* const & getNeighbor( Index edgeIndex ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( edgeIndex < 3, "Cannot access edge neighbor at invalid index in triangle." );
				
				return neighbor[edgeIndex];
			}
			
			
			
			
			/// Set the triangle's neighbor along the specified edge index.
			/**
			  * A NULL pointer indicates that there is no neighbor along that edge.
			  *
			  * Edge indices are denoted as follows:
			  * - 0: edge between vertices 0 and 1
			  * - 1: edge between vertices 0 and 2
			  * - 2: edge between vertices 1 and 2
			  */
			GSOUND_FORCE_INLINE void setNeighbor( Index edgeIndex, const InternalSoundTriangle* newNeighbor )
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( edgeIndex < 3, "Cannot set edge neighbor at invalid index in triangle." );
				
				neighbor[edgeIndex] = newNeighbor;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Edge Type Accessor Method
			
			
			
			
			/// Return the diffraction type for the edge at the specified index.
			/**
			  * If there is no neighbor along that edge, NULL is returned.
			  *
			  * Edge indices are denoted as follows:
			  * - 0: edge between vertices 0 and 1
			  * - 1: edge between vertices 0 and 2
			  * - 2: edge between vertices 1 and 2
			  */
			GSOUND_FORCE_INLINE EdgeDiffractionType getEdgeType( Index edgeIndex ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( edgeIndex < 3, "Cannot access edge type at invalid index in triangle." );
				
				return EdgeDiffractionType(edgeType[edgeIndex]);
			}
			
			
			
			
			/// Set the diffraction type for the edge at the specified index.
			/**
			  * Edge indices are denoted as follows:
			  * - 0: edge between vertices 0 and 1
			  * - 1: edge between vertices 0 and 2
			  * - 2: edge between vertices 1 and 2
			  */
			GSOUND_FORCE_INLINE void setEdgeType( Index edgeIndex, EdgeDiffractionType newType )
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( edgeIndex < 3, "Cannot access edge type at invalid index in triangle." );
				
				edgeType[edgeIndex] = newType;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Edge Vertex Accessor Method
			
			
			
			
			/// Get the pointers to the vertices which share the edge at the specified index.
			GSOUND_INLINE void getEdgeVertices( Index edgeIndex, const SoundVertex*& v1, const SoundVertex*& v2 ) const
			{
				if ( edgeIndex == 0 )
				{
					v1 = vertex[0];
					v2 = vertex[1];
				}
				else if ( edgeIndex == 1 )
				{
					v1 = vertex[0];
					v2 = vertex[2];
				}
				else if ( edgeIndex == 2 )
				{
					v1 = vertex[1];
					v2 = vertex[2];
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Hash Code Accessor Method
			
			
			
			
			/// Get a hash code for this triangle.
			GSOUND_FORCE_INLINE Hash getHashCode() const
			{
				return Hash(UInt64(this) >> 2)*27;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Static Helper Methods
			
			
			
			
			/// Get the index of the edge shared by the vertices with the specified indices within the triangle.
			GSOUND_FORCE_INLINE static Index getEdgeIndex( Index vIndex1, Index vIndex2 )
			{
				return vIndex1 + vIndex2 - 1;
			}
			
			
			
			
			/// Get the indices within the triangle of the vertices that that share the specified edge.
			GSOUND_INLINE static void getEdgeVertexIndices( Index edgeIndex, Index& vIndex1, Index& vIndex2 )
			{
				if ( edgeIndex == 0 )
				{
					vIndex1 = 0;
					vIndex2 = 1;
				}
				else if ( edgeIndex == 1 )
				{
					vIndex1 = 0;
					vIndex2 = 2;
				}
				else if ( edgeIndex == 2 )
				{
					vIndex1 = 1;
					vIndex2 = 2;
				}
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// Pointers to the vertices of this triangle.
			const SoundVertex* vertex[3];
			
			
			
			
			/// A pointer to the triangle's material.
			const SoundMaterial* material;
			
			
			
			
			/// A pointer to the neighbors for this triangle at each edge.
			/**
			  * Edge indices are denoted as follows:
			  * 0 = edge between vertices 0 and 1
			  * 1 = edge between vertices 0 and 2
			  * 2 = edge between vertices 1 and 2
			  */
			const InternalSoundTriangle* neighbor[3];
			
			
			
			
			/// The plane in which the triangle lies.
			Plane3 plane;
			
			
			
			
			/// The area of the triangle in square units.
			Real area;
			
			
			
			
			/// The edge type for each edge on the triangle.
			/**
			  * Edge indices are denoted as follows:
			  * 0 = edge between vertices 0 and 1
			  * 1 = edge between vertices 0 and 2
			  * 2 = edge between vertices 1 and 2
			  * 3 = padding
			  */
			UInt8 edgeType[4];
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Friend Class Declaration
			
			
			
			
			/// Declare the SoundMesh class as a friend so that it can access internal data.
			friend class SoundMesh;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_INTERNAL_SOUND_TRIANGLE_H
