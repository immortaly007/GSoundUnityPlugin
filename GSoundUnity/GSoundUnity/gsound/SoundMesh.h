/*
 * Project:     GSound
 * 
 * File:        gsound/SoundMesh.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundMesh class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_MESH_H
#define INCLUDE_GSOUND_SOUND_MESH_H


#include "GSoundBase.h"


#include "internal/InternalSoundTriangle.h"
#include "internal/BoundingSphere.h"
#include "internal/QBVHArrayTree.h"
#include "SoundVertex.h"
#include "SoundTriangle.h"
#include "SoundMaterial.h"
#include "SoundMeshPreprocessor.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class representing a triangle mesh for use in sound propagation.
/**
  * Once created, the mesh is immutable. However, material properties of the mesh
  * can be changed at runtime. The mesh cannot be modified once it is created because
  * doing so would require rebuilding a bounding volume hierarchy for the mesh and
  * re-evaluting diffraction edge criteria. When the mesh is created, a triangle
  * adjacency graph is computed for fast diffraction and a bounding volume heirarchy
  * is constructed. These operations are time consuming for large meshes and should
  * be done at load time, rather than at runtime.
  */
class SoundMesh
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Type Definitions
			
			
			
			
			typedef internal::InternalSoundTriangle TriangleType;
			typedef internal::QBVHArrayTree BVHType;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a new mesh shape with the specified vertices, triangles, and materials.
			/**
			  * This constructor uses a default mesh preprocessor object to potentially simplify
			  * the given mesh. Thus, there is no guarantee that all triangles/vertices that
			  * are specified here will be part of the final mesh shape. This consructor is 
			  * non-trivial and may take several seconds to execute for very large meshes.
			  */
			SoundMesh( const ArrayList<SoundVertex>& newVertices,
							const ArrayList<SoundTriangle>& newTriangles,
							const ArrayList<SoundMaterial>& newMaterials );
			
			
			
			
			/// Create a new mesh shape with the specified vertices, triangles, and materials.
			/**
			  * This constructor uses the specified mesh preprocessor object to potentially simplify
			  * the given mesh. Thus, if the mesh preprocessor is set to simplify the mesh,
			  * there is no guarantee that all triangles/vertices that are specified here will
			  * be part of the final mesh shape. This consructor is non-trivial and may take
			  * several seconds to execute for very large meshes.
			  */
			SoundMesh( const ArrayList<SoundVertex>& newVertices,
							const ArrayList<SoundTriangle>& newTriangles,
							const ArrayList<SoundMaterial>& newMaterials,
							const SoundMeshPreprocessor& preprocessor );
			
			
			
			
			/// Create a copy of another mesh shape.
			SoundMesh( const SoundMesh& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy this mesh shape and all data that it contains.
			~SoundMesh();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the contents of one SoundMesh to another.
			/**
			  * This method replaces the current state of this SoundMesh object
			  * with the entire state of another SoundMesh object.
			  * 
			  * @param other - the SoundMesh object whose state will be copied.
			  * @return a reference to this SoundMesh object in order to allow assignment chaining.
			  */
			SoundMesh& operator = ( const SoundMesh& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Triangle Accessor Methods
			
			
			
			
			/// Get the triangle in this SoundMesh at the specified index.
			/**
			  * If the specified triangle index is outside the valid range of triangle
			  * indices, an assertion is raised. Otherwise, the triangle at the specified
			  * index is returned.
			  * 
			  * @param triangleIndex - the index of the triangle to get a reference to.
			  * @return the triangle at the specified index in this SoundMesh.
			  */
			GSOUND_INLINE SoundTriangle getTriangle( Index triangleIndex ) const
			{
				GSOUND_DEBUG_ASSERT( triangleIndex < triangles.getSize() );
				
				const SoundVertex* verticesStart = vertices.getArrayPointer();
				const SoundMaterial* materialsStart = materials.getArrayPointer();
				const TriangleType& triangle = triangles[triangleIndex];
				
				return SoundTriangle( &triangle.getVertex(0) - verticesStart, &triangle.getVertex(1) - verticesStart,
										&triangle.getVertex(2) - verticesStart, &triangle.getMaterial() - materialsStart );
			}
			
			
			
			
			/// Get the number of triangles in this SoundMesh.
			/**
			  * @return the number of triangles in this SoundMesh.
			  */
			GSOUND_FORCE_INLINE Size getNumberOfTriangles() const
			{
				return triangles.getSize();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Vertex Accessor Methods
			
			
			
			
			/// Get the vertex at the specified index in this SoundMesh.
			/**
			  * If the specified vertex index is outside the valid range of vertex
			  * indices, an assertion is raised. Otherwise, a reference to the
			  * vertex at the specified index is returned.
			  * 
			  * @param vertexIndex - the index of the vertex to get a reference to.
			  * @return a reference to the vertex at the specified index in this SoundMesh.
			  */
			GSOUND_INLINE const SoundVertex& getVertex( Index vertexIndex ) const
			{
				GSOUND_DEBUG_ASSERT( vertexIndex < vertices.getSize() );
				
				return vertices[vertexIndex];
			}
			
			
			
			
			/// Get the number of vertices in this SoundMesh.
			/**
			  * @return the number of vertices in this SoundMesh.
			  */
			GSOUND_FORCE_INLINE Size getNumberOfVertices() const
			{
				return vertices.getSize();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Diffraction Candidacy Accessor Methods
			
			
			
			
			/// Query whether or not the specified triangle edge has been marked as diffracting.
			/**
			  * If the edge with the specified index of the triangle with the specified index has
			  * been marked as diffracting as part of a preprocessing step, TRUE is returned.
			  * Otherwise, FALSE is returned.
			  * 
			  * @param triangleIndex - the index of the triangle in this SoundMesh to query.
			  * @param edgeIndex - the index of the edge of the triangle (0, 1, or 2) to query.
			  * @return whether or not the specified triangle edge has been marked as diffracting.
			  */
			GSOUND_INLINE Bool edgeIsDiffracting( Index triangleIndex, Index edgeIndex ) const
			{
				if ( triangleIndex >= triangles.getSize() || edgeIndex > 2 )
					return false;
				else
					return triangles[triangleIndex].getEdgeType(edgeIndex) == TriangleType::DIFFRACTING;
			}
			
			
			
			
			/// Return whether or not the triangle with the specified index has a neighbor on the specified edge.
			/**
			  * If the triangle with the specified index has a neighboring triangle at the specified edge
			  * index (0, 1, or 2), TRUE is returned. Otherwise, FALSE is returned.
			  * 
			  * @param triangleIndex - the index of the triangle in this SoundMesh to query.
			  * @param edgeIndex - the index of the edge of the triangle (0, 1, or 2) to query.
			  * @return whether or not the specified triangle edge has a neighboring triangle.
			  */
			GSOUND_INLINE Bool triangleHasNeighbor( Index triangleIndex, Index edgeIndex ) const
			{
				if ( triangleIndex >= triangles.getSize() || edgeIndex > 2 )
					return false;
				else
					return triangles[triangleIndex].getNeighbor(edgeIndex) != NULL;
			}
			
			
			
			
			/// Return whether or not the triangle with the specified index has a neighbor on the specified edge.
			/**
			  * The function places the index of the neighboring triangle into the value
			  * referenced by the third parameter to the function.
			  * 
			  * @param triangleIndex - the index of the triangle in this SoundMesh to query.
			  * @param edgeIndex - the index of the edge of the triangle (0, 1, or 2) to query.
			  * @param neighborIndex - a reference to a variable where the neighbor triangle index should be placed.
			  * @return whether or not there is a neighboring triangle for the specified triangle edge.
			  */
			GSOUND_INLINE Bool getTriangleNeighbor( Index triangleIndex, Index edgeIndex, Index& neighborIndex ) const
			{
				if ( triangleIndex >= triangles.getSize() || edgeIndex > 2 )
					return false;
				else if ( triangles[triangleIndex].getNeighbor(edgeIndex) != NULL )
				{
					neighborIndex = triangles[triangleIndex].getNeighbor(edgeIndex) - triangles.getArrayPointer();
					return true;
				}
				else
					return false;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Material Accessor Methods
			
			
			
			
			/// Get the material at the specified index in this SoundMesh.
			GSOUND_FORCE_INLINE SoundMaterial& getMaterial( Index materialIndex )
			{
				GSOUND_DEBUG_ASSERT( materialIndex < materials.getSize() );
				
				return materials[materialIndex];
			}
			
			
			
			
			/// Get the material at the specified index in this SoundMesh.
			GSOUND_FORCE_INLINE const SoundMaterial& getMaterial( Index materialIndex ) const
			{
				GSOUND_DEBUG_ASSERT( materialIndex < materials.getSize() );
				
				return materials[materialIndex];
			}
			
			
			
			
			/// Set the material at the specified index in this SoundMesh.
			GSOUND_INLINE void setMaterial( Index materialIndex, const SoundMaterial& newMaterial )
			{
				GSOUND_DEBUG_ASSERT( materialIndex < materials.getSize() );
				
				materials[materialIndex] = newMaterial;
			}
			
			
			
			
			/// Get the number of materials in this SoundMesh.
			GSOUND_FORCE_INLINE Size getNumberOfMaterials() const
			{
				return materials.getSize();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Bounding Volume Accessor Methods
			
			
			
			
			/// Get the bounding sphere for this SoundMesh.
			GSOUND_FORCE_INLINE const internal::BoundingSphere& getBoundingSphere() const
			{
				return boundingSphere;
			}
			
			
			
			
			/// Return a pointer to the root node of this SoundMesh's bounding volume hierarchy.
			GSOUND_FORCE_INLINE const BVHType* getBVH() const
			{
				return bvh;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	User Data Accessor Methods
			
			
			
			
			/// Get an opaque pointer to user-defined data for this SoundMesh.
			GSOUND_FORCE_INLINE void* getUserData() const
			{
				return userData;
			}
			
			
			
			
			/// Get an opaque pointer to user-defined data for this SoundMesh.
			GSOUND_FORCE_INLINE void setUserData( void* newUserData )
			{
				userData = newUserData;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Constructor
			
			
			
			
			/// Create an empty SoundMesh, used when deserializing saved meshes.
			SoundMesh();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Methods
			
			
			
			
			/// Build the bounding volume hierarchy for this SoundMesh.
			void buildBVH();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A list of all of the triangles in this mesh.
			ArrayList<TriangleType> triangles;
			
			
			
			
			/// A list of all of the vertices in this mesh.
			ArrayList<SoundVertex> vertices;
			
			
			
			
			/// A list of all of the materials in this mesh.w
			ArrayList<SoundMaterial> materials;
			
			
			
			
			/// A pointer to the root node of the triangle BVH of this mesh.
			BVHType* bvh;
			
			
			
			
			/// A bounding sphere for the triangle mesh.
			internal::BoundingSphere boundingSphere;
			
			
			
			
			/// An opaque pointer to user-defined data for this SoundMesh.
			void* userData;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Static Data Members
			
			
			
			
			/// Define the maximum number of triangles that can be in a leaf node of the BVH.
			static const Size MAX_NUMBER_OF_TRIANGLES_PER_LEAF;
			
			
			
			
			/// Define the number of potential split planes to use when constructing the BVH.
			static const Size NUMBER_OF_SPLIT_PLANE_CANDIDATES;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Friend Class Declarations
			
			
			
			
			/// Mark the SoundMeshSerializer class as a friend so that it can access internal data.
			friend class SoundMeshSerializer;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_MESH_H
