/*
 * Project:     GSound
 * 
 * File:        gsound/SoundMeshPreprocessor.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundMeshPreprocessor class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_MESH_PREPROCESSOR_H
#define INCLUDE_GSOUND_SOUND_MESH_PREPROCESSOR_H


#include "GSoundBase.h"


#include "SoundVertex.h"
#include "SoundTriangle.h"
#include "internal/InternalSoundTriangle.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which performs preprocessing on a triangle mesh before it is used for sound propagation.
/**
  * This can include mesh simplication and diffraction edge analysis. The class
  * outputs a potentially simplified mesh with adjaceny and other information used
  * in sound propagation.
  */
class SoundMeshPreprocessor
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a mesh preprocessor with default parameters.
			/**
			  * This create a mesh preprocessor which welds vertices which are within
			  * a machine epsilon's distance from other. It also uses a diffraction
			  * threshold of 0.99 to determine diffraction edges.
			  */
			GSOUND_INLINE SoundMeshPreprocessor()
				:	weldingTolerance( math::epsilon<Real>() ),
					isWeldingVertices( false ),
					diffractionThreshold( Real(0.99) )
			{
			}
			
			
			
			
			/// Create a mesh preprocessor with the specified welding tolerance and diffraction threshold.
			GSOUND_INLINE SoundMeshPreprocessor( Real newWeldingTolerance, Real newDiffractionThreshold )
				:	weldingTolerance( math::max( newWeldingTolerance, Real(0) ) ),
					isWeldingVertices( true ),
					diffractionThreshold( math::clamp( newDiffractionThreshold, Real(0), Real(1) ) )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mesh Processing Method
			
			
			
			
			/// Process the input lists of vertices, triangles, and materials.
			/**
			  * The output simplified mesh is placed into the lists specified by the
			  * last two parameters, which are cleared before any vertices or triangles
			  * are added to them.
			  */
			void processMesh( const ArrayList<SoundVertex>& newVertices,
								const ArrayList<SoundTriangle>& newTriangles,
								const ArrayList<SoundMaterial>& newMaterials,
								ArrayList<SoundVertex>& outputVertices,
								ArrayList<internal::InternalSoundTriangle>& outputTriangles ) const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Edge Diffraction Threshold Accessor Methods
			
			
			
			
			/// Get the diffraction threshold used by this mesh preprocessor to determine diffracting edges.
			GSOUND_INLINE Real getDiffractionThreshold() const
			{
				return diffractionThreshold;
			}
			
			
			
			
			/// Set the diffraction threshold used by this mesh preprocessor to determine diffracting edges.
			/**
			  * The value passed into the function is clamped to the range of [0,1].
			  */
			GSOUND_INLINE void setDiffractionThreshold( Real newDiffractionThreshold )
			{
				diffractionThreshold = math::clamp( newDiffractionThreshold, Real(0), Real(1) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Vertex Welding Type Accessor Methods
			
			
			
			
			/// Get the vertex welding tolerance used by the mesh preprocessor to weld vertices.
			GSOUND_INLINE Real getVertexWeldingTolerance() const
			{
				return weldingTolerance;
			}
			
			
			
			
			/// Set the vertex welding tolerance used by the mesh preprocessor to weld vertices.
			/**
			  * This value is clamped to the range [0,+infinity].
			  */
			GSOUND_INLINE void setVertexWeldingTolerance( Real newVertexWeldingTolerance )
			{
				weldingTolerance = math::max( newVertexWeldingTolerance, Real(0) );
			}
			
			
			
			
			/// Get whether or not the mesh preprocessor is welding vertices.
			GSOUND_INLINE Bool getIsWeldingVertices() const
			{
				return isWeldingVertices;
			}
			
			
			
			
			/// Set whether or not the mesh preprocessor is welding vertices.
			GSOUND_INLINE void setIsWeldingVertices( Bool newIsWeldingVertices )
			{
				isWeldingVertices = newIsWeldingVertices;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Helper Classes
			
			
			
			
			/// A class used to hold intermediate data for each vertex used during mesh simplification.
			class VertexRecord
			{
				public:
					
					GSOUND_INLINE VertexRecord()
						:	triangles(),
							wasWelded( false ),
							weldedIndex( 0 ),
							finalIndex( 0 )
					{
					}
					
					
					GSOUND_INLINE void reset()
					{
						triangles.clear();
						wasWelded = false;
						weldedIndex = 0;
						finalIndex = 0;
					}
					
					ArrayList<Index> triangles;
					
					Bool wasWelded;
					
					Index weldedIndex;
					
					Index finalIndex;
					
			};
			
			
			
			
			/// A class used to hold intermediate data for each triangle used during mesh simplification.
			class TriangleRecord
			{
				public:
					
					GSOUND_INLINE TriangleRecord()
						:	isADuplicate( false ),
							finalIndex( 0 )
					{
						foundEdgeNeighbor[0] = false;
						foundEdgeNeighbor[1] = false;
						foundEdgeNeighbor[2] = false;
					}
					
					Bool foundEdgeNeighbor[3];
					Index neighborIndex[3];
					
					Bool isADuplicate;
					Index finalIndex;
					
			};
			
			
			
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Methods
			
			
			
			
			/// Get a hash code for a grid cell in a 3D infinite spatial hash grid.
			GSOUND_INLINE static Hash getGridCellHash( Int x, Int y, Int z )
			{
				return	(*(Hash*)&x)*Hash(0x8DA6B343) ^ 
						(*(Hash*)&y)*Hash(0xD8163841) ^ 
						(*(Hash*)&z)*Hash(0xCB1AB31F);
			}
			
			
			
			
			/// Get a hash code for the specified triangle vertex indices.
			GSOUND_INLINE static Hash getTriangleHash( Index v1, Index v2, Index v3 )
			{
				return	(*(Hash*)&v1) * 
						(*(Hash*)&v2) * 
						(*(Hash*)&v3);
			}
			
			
			
			
			/// Get the final index of the vertex with the specified original index.
			GSOUND_INLINE Index getFinalVertexIndex( Index i ) const
			{
				if ( vertexRecords[i].wasWelded )
					return vertexRecords[vertexRecords[i].weldedIndex].finalIndex;
				else
					return vertexRecords[i].finalIndex;
			}
			
			
			
			
			/// Reset the internal data structures used for mesh simplification.
			void resetHelperDataStructures( Size numVertices, Size numTriangles ) const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The welding tolerance that this mesh preprocessor uses to weld vertices.
			/**
			  * If any two vertices in the mesh are closer to each other than this distance,
			  * they will be merged into one vertex and all triangles that share them
			  * will now reference the welded vertex.
			  */
			Real weldingTolerance;
			
			
			
			
			/// Whether or not the mesh preprocessor is welding vertices.
			Bool isWeldingVertices;
			
			
			
			
			/// The diffraction threshold used by the mesh preprocessor to find diffraction edges.
			/**
			  * If the absolute value of the dot product of the normals of two neighboring triangles
			  * is less than this value, then the edge that they share is considered diffracting.
			  * Thus, a lower diffraction threshold will result in less diffracting edges while
			  * higher thresold will result in more edges.
			  *		Alternatively, if the afformentioned normal dot product is greater than this
			  * threshold, the two triangles are considered coplanar and thus cannot diffract the
			  * potentiall diffracting edge.
			  */
			Real diffractionThreshold;
			
			
			
			
			/// An array of data for each vertex which holds intermediate data used in mesh simplification.
			mutable ArrayList<VertexRecord> vertexRecords;
			
			
			
			
			/// An array of data for each triangle which holds intermediate data used in mesh simplification.
			mutable ArrayList<TriangleRecord> triangleRecords;
			
			
			
			
			/// A hash table used to determine which sets of vertices could potentially be welded.
			mutable ArrayList< ArrayList<Index> > gridHashTable;
			
			
			
			
			/// A set of triangles used to determine and remove any duplicate triangles in a mesh.
			mutable HashSet<SoundTriangle> triangleSet;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_MESH_PREPROCESSOR_H
