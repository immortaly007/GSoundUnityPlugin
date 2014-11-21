/*
 * Project:     GSound
 * 
 * File:        gsound/internal/QBVHArrayTree.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::QBVHArrayTree class declaration
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


#ifndef INCLUDE_GSOUND_QBVH_ARRAY_TREE_H
#define INCLUDE_GSOUND_QBVH_ARRAY_TREE_H


#include "GSoundInternalConfig.h"


#include "QBVHArrayTreeNode.h"
#include "FatSIMDTriangle3D.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




class QBVHArrayTree
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Type Definitions
			
			
			
			
			/// Define the type of triangle that this QBVHArrayTree uses.
			typedef InternalSoundTriangle TriangleType;
			
			
			
			
			/// The type of triangle stored internally.
			typedef FatSIMDTriangle3D<Real,4> FatSIMDTriangle3;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a QBVHArrayTree for the specified list of triangles.
			QBVHArrayTree( const ArrayList<TriangleType>& triangles,
							Size numSplitCandidates, Size maxNumTrianglesPerLeaf );
			
			
			
			
			/// Create a QBVHArrayTree for the triangles at the specified pointer.
			QBVHArrayTree( const TriangleType* triangles, Size numTriangles,
							Size numSplitCandidates, Size maxNumTrianglesPerLeaf );
			
			
			
			
			/// Create a QBVHArrayTree that is an exact copy of another tree.
			QBVHArrayTree( const QBVHArrayTree& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy a QBVHArrayTree object.
			~QBVHArrayTree();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the state of a QBVHArrayTree to this tree.
			QBVHArrayTree& operator = ( const QBVHArrayTree& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Root Node Accessor Method
			
			
			
			
			/// Get a pointer to the root node of this QBVHArrayTree.
			GSOUND_FORCE_INLINE const QBVHArrayTreeNode* getRoot() const
			{
				return nodes;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Tree Size Accesssor Method
			
			
			
			
			/// Get the number of nodes that this QBVHArrayTree has.
			GSOUND_FORCE_INLINE Size getNumberOfNodes() const
			{
				return numNodes;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Triangle Accesssor Methods
			
			
			
			
			/// Get a pointer to the internal list of triangles that this QBVHArrayTree has.
			/**
			  * This list is used to determine which triangles are in each leaf node of the
			  * tree. Each leaf has an index into this array which indicates the start of
			  * the leaf node's triangles. The triangles for the leaf are accessed in order
			  * starting from this index.
			  */
			GSOUND_FORCE_INLINE const FatSIMDTriangle3* getTriangles() const
			{
				return triangles;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Class Declarations
			
			
			
			
			/// A class used to hold intermediate data about a triangle during tree construction.
			class TriangleAABB;
			
			
			
			
			/// A class used to keep track of surface-area-heuristic paritioning data.
			class SplitBin;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Tree Management Methods
			
			
			
			
			/// Construct a QBVHArrayTree for the specified list of triangles.
			void buildTree( const TriangleType* triangles, Size numTriangles,
							Size numSplitCandidates, Size maxNumTrianglesPerLeaf );
			
			
			
			
			/// Build a QBVH array tree starting at the specified node using the specified triangles.
			/**
			  * This method returns the number of nodes in the tree created.
			  */
			static Size buildTreeRecursive( QBVHArrayTreeNode* node,
											TriangleAABB* triangleAABBs, Index start, Size numTriangles,
											SplitBin* splitBins, Size numSplitCandidates,
											Size maxNumTrianglesPerLeaf );
			
			
			
			
			/// Destroy and deallocate the tree currently contained in this QBVHArrayTree object.
			void destroyTree();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Partitioning Methods
			
			
			
			
			/// Partition the specified list of triangles into two sets based on the given split plane.
			/**
			  * The triangles are sorted so that the first N triangles in the list are deemed "less" than
			  * the split plane along the split axis, and the next M triangles are the remainder.
			  * The number of "lesser" triangles is placed in the output variable.
			  */
			static void partitionTrianglesSAH( TriangleAABB* triangleAABBs, Size numTriangles,
												SplitBin* splitBins, Size numSplitCandidates,
												Index& axis, Size& numLesserTriangles,
												AABB3& lesserVolume, AABB3& greaterVolume );
			
			
			
			
			/// Partition the specified list of triangles into two sets based on their median along the given axis.
			static void partitionTrianglesMedian( TriangleAABB* triangleAABBs, Size numTriangles,
												Index splitAxis, Size& numLesserTriangles,
												AABB3& lesserVolume, AABB3& greaterVolume );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Geometric Methods
			
			
			
			
			/// Compute the axis-aligned bounding box for the specified list of triangles.
			static AABB3 computeTriangleAABB( const TriangleAABB* triangleAABBs, Size numTriangles );
			
			
			
			
			/// Compute the axis-aligned bounding box for the specified list of triangles' centroids.
			static AABB3 computeCentroidAABB( const TriangleAABB* triangleAABBs, Size numTriangles );
			
			
			
			
			/// Get the surface area of a 3D axis-aligned bounding box specified by 2 SIMD min-max vectors.
			GSOUND_FORCE_INLINE static Real getAABBSurfaceArea( const SIMDScalar<float,4>& min,
																const SIMDScalar<float,4>& max );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Triangle List Building Methods
			
			
			
			
			/// Get the SIMD-padded size required for the triangle indirection array.
			static Size getTriangleArraySize( const QBVHArrayTreeNode* node );
			
			
			
			
			/// Fill the specified array of SIMD-padded triangles with pointers, based on the tree structure.
			static Size fillTriangleArray( FatSIMDTriangle3* triangles, const TriangleAABB* aabbs,
											QBVHArrayTreeNode* node, Size numFilled );
			
			
			
			
			/// Fill the specified array of SIMD-padded triangles with pointers, based on the tree structure.
			static Size fillTriangleArray( const TriangleType** triangles, const TriangleAABB* aabbs,
											QBVHArrayTreeNode* node, Size numFilled );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A pointer to the array of packed nodes in this QBVHArrayTree.
			QBVHArrayTreeNode* nodes;
			
			
			
			
			/// The number of nodes in this QBVHArrayTree.
			Size numNodes;
			
			
			
			
			/// A list of pointers to the triangles in the QBVHArrayTree.
			FatSIMDTriangle3* triangles;
			
			
			
			
			/// The number of SIMD triangles in this QBVHArrayTree's triangle indirection array.
			Size numTriangles;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Data Members
			
			
			
			
			/// Define the maximum number of triangles per leaf.
			/**
			  * This value is determined to be 4*(2^d - 1), where d is the number
			  * of bits available to represent the triangle count: 7 in this case.
			  */
			static const Size MAX_NUM_TRIANGLES_PER_LEAF = 508;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Friend Class Declaration
			
			
			
			
			/// Mark the SoundMeshSerializer class as a friend so that it can access internal data when serializing.
			friend class SoundMeshSerializer;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_QBVH_ARRAY_TREE_H
