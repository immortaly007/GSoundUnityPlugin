/*
 * Project:     GSound
 * 
 * File:        gsound/internal/QBVHArrayTreeNode.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::QBVHArrayTreeNode class declaration
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


#ifndef INCLUDE_GSOUND_QBVH_ARRAY_TREE_NODE_H
#define INCLUDE_GSOUND_QBVH_ARRAY_TREE_NODE_H


#include "GSoundInternalConfig.h"


#include "InternalSoundTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




class GSOUND_ALIGN(128) QBVHArrayTreeNode
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Type Definitions
			
			
			
			
			typedef InternalSoundTriangle TriangleType;
			
			
			
			
			/// Define the type of data structure to use for SIMD axis-aligned bounding boxes.
			typedef math::SIMDAABB3D<Float,4> SIMDAABB3;
			
			
			
						
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Children Accessor Method
			
			
			
			
			/// Get the left child of this QBVH node with the specified index.
			GSOUND_FORCE_INLINE QBVHArrayTreeNode* getChild( Index child )
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( child < 4, "Cannot get QBVHArrayTreeNode child with invalid index." );
				return this + firstChildOffset[child];
			}
			
			
			
			
			/// Get the left child of this QBVH node with the specified index.
			GSOUND_FORCE_INLINE const QBVHArrayTreeNode* getChild( Index child ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( child < 4, "Cannot get QBVHArrayTreeNode child with invalid index." );
				return this + firstChildOffset[child];
			}
			
			
			
			
			/// Get a SIMD scalar containing the integer offsets from this node of this node's children.
			GSOUND_FORCE_INLINE SIMDInt getChildOffsets() const
			{
				return SIMDInt( (const int*)firstChildOffset );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Node Attribute Accessor Methods
			
			
			
			
			/// Get an integer which specifies the specified split axis of the node.
			/**
			  * Indices range from 0 to 2. The split with index 0 is the split that
			  * was used to partition this node's triangles into two sets. The split
			  * with index 1 is the secondary split for the lesser half of the node,
			  * and the index 2 split is for the greater half of the node.
			  */
			GSOUND_FORCE_INLINE Index getSplitAxis( Index splitIndex ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( splitIndex < 3, "Cannot get QBVHArrayTreeNode split axis with invalid index." );
				
				return splitAxis[splitIndex];
			}
			
			
			
			
			/// Return whether or not the specified child is a leaf node.
			GSOUND_FORCE_INLINE Bool isLeaf( Index child ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( child < 4, "Cannot get QBVHArrayTreeNode leaf status with invalid index." );
				
				return (flags[child] & LEAF_NODE_FLAG);
			}
			
			
			
			
			/// Return a SIMD mask indicating whether or not each node is a leaf.
			GSOUND_FORCE_INLINE SIMDBool getLeafMask() const
			{
				const SIMDInt leafMask( LEAF_NODE_FLAG, LEAF_NODE_FLAG << 8,
										LEAF_NODE_FLAG << 16, LEAF_NODE_FLAG << 24 );
				
				return (SIMDInt(*(const int*)flags) & leafMask) == leafMask;
			}
			
			
			
			
			/// Return whether or not the specified child is an empty node.
			GSOUND_FORCE_INLINE Bool isEmpty( Index child ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( child < 4, "Cannot get QBVHArrayTreeNode empty status with invalid index." );
				
				return flags[child] == LEAF_NODE_FLAG;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Volume Accessor Methods
			
			
			
			
			/// Get the set of SIMD axis-aligned bounding boxes for this quad node.
			GSOUND_FORCE_INLINE const SIMDAABB3& getVolumes() const
			{
				return volumes;
			}
			
			
			
			
			/// Get the axis-aligned bounding box that encompases the whole node.
			/**
			  * The bounding box is computed from scratch each time so this method
			  * should not be used in performance-intensive code.
			  */
			AABB3 getVolume() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Triangle Accessor Methods
			
			
			
			
			/// Get an offset into the triangle array for this tree that indicates the start of a leaf's triangles.
			GSOUND_FORCE_INLINE UInt32 getTriangleStartIndex( Index child ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( child < 4, "Cannot get QBVHArrayTreeNode triangle start with invalid child index." );
				return triangleIndex[child];
			}
			
			
			
			
			/// Get the number of triangles that this node contains (if it is a leaf).
			GSOUND_FORCE_INLINE Size getNumberOfTriangles( Index child ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( child < 4, "Cannot get QBVHArrayTreeNode triangle count with invalid child index." );
				return Size(flags[child] & TRIANGLE_COUNT_MASK);
			}
			
			
			
			
			/// Get a SIMD integer representing the number of triangles that each child node contains (if it is a leaf).
			GSOUND_FORCE_INLINE SIMDInt getNumberOfTriangles() const
			{
				SIMDInt triangleCountMask( TRIANGLE_COUNT_MASK, TRIANGLE_COUNT_MASK << 8,
											TRIANGLE_COUNT_MASK << 16, TRIANGLE_COUNT_MASK << 24 );
				
				SIMDInt counts = SIMDInt(*(const int*)flags) & triangleCountMask;
				
				return (counts | (counts >> 8) | (counts >> 16) | (counts >> 24)) & SIMDInt(TRIANGLE_COUNT_MASK);
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Constructors
			
			
			
			
			QBVHArrayTreeNode( const StaticArray<AABB3,4>& newVolumes, const StaticArray<Index,4>& newIndices,
								const StaticArray<Bool,4>& isALeaf, const StaticArray<Size,4>& numLeafTriangles,
								const StaticArray<Index,3>& splitAxes );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Accessor Methods
			
			
			
			
			/// Set an offset into the triangle array for this tree that indicates the start of a leaf's triangles.
			GSOUND_FORCE_INLINE void setTriangleStartIndex( Index child, UInt32 index )
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( child < 4, "Cannot set QBVHArrayTreeNode triangle start with invalid child index." );
				triangleIndex[child] = index;
			}
			
			
			
			
			/// Set the number of triangles that this node contains (if it is a leaf).
			GSOUND_FORCE_INLINE void setNumberOfTriangles( Index child, Size numTriangles )
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( child < 4, "Cannot set QBVHArrayTreeNode triangle count with invalid child index." );
				flags[child] = (flags[child] & LEAF_NODE_FLAG) | (UInt8)numTriangles;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A set of SIMD axis-aligned bounding boxes for this quad node.
			SIMDAABB3 volumes;
			
			
			
			
			union
			{
				/// The relative index of the first children of each of the 4 sub-nodes.
				UInt32 firstChildOffset[4];
				
				
				
				/// The index within the master triangle list of the start of the leaf-node triangle list.
				UInt32 triangleIndex[4];
				
				
				
				/// The index value to use when it doesn't semantically matter whether the index is a child or a triangle.
				UInt32 indices[4];
			};
			
			
			
			
			/// The axes along which this node was split to produce its children.
			/**
			  * The index of the split axis indicates the axis along which it was split:
			  * (0 = X, 1 = Y, 2 = Z).
			  */
			UInt32 splitAxis[3];
			
			
			
			
			/// A 32 bit integer value containing flags for this node.
			UInt8 flags[4];
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Data Members
			
			
			
			
			static const UInt8 LEAF_NODE_FLAG = 0x80;
			
			
			
			
			static const UInt8 TRIANGLE_COUNT_MASK = 0x7F;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Friend Declaration
			
			
			
			
			/// Declare the QBVHArrayTree class as a friend so that it can access a private constructor.
			friend class QBVHArrayTree;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_QBVH_ARRAY_TREE_NODE_H
