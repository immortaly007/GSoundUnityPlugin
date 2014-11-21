/*
 * Project:     GSound
 * 
 * File:        gsound/internal/QBVHArrayTree.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::QBVHArrayTree class implementation
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


#include "QBVHArrayTree.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		TriangleAABB Class Definition
//############		
//##########################################################################################
//##########################################################################################




class GSOUND_ALIGN(16) QBVHArrayTree:: TriangleAABB
{
	public:
		
		GSOUND_FORCE_INLINE TriangleAABB( const TriangleType* newTriangle )
			:	triangle( newTriangle ),
				min( newTriangle->getVertex(0) ),
				max( newTriangle->getVertex(0) )
		{
			SIMDScalar<float,4> v1( newTriangle->getVertex(1) );
			SIMDScalar<float,4> v2( newTriangle->getVertex(2) );
			
			min = math::min( min, v1 );
			min = math::min( min, v2 );
			max = math::max( max, v1 );
			max = math::max( max, v2 );
			
			centroid = (min + max)*Float(0.5);
		}
		
		
		
		
		/// The minimum coordinate of the triangle's axis-aligned bounding box.
		GSOUND_ALIGN(16) SIMDScalar<float,4> min;
		
		
		
		
		/// The maximum coordinate of the triangle's axis-aligned bounding box.
		GSOUND_ALIGN(16) SIMDScalar<float,4> max;
		
		
		
		
		/// The centroid of the triangle's axis-aligned bounding box.
		GSOUND_ALIGN(16) SIMDScalar<float,4> centroid;
		
		
		
		
		/// A pointer to the triangle from which this triangle AABB was computed.
		const TriangleType* triangle;
		
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		SplitBin Class Definition
//############		
//##########################################################################################
//##########################################################################################




class GSOUND_ALIGN(16) QBVHArrayTree:: SplitBin
{
	public:
		
		GSOUND_FORCE_INLINE SplitBin()
			:	min( math::max<float>() ),
				max( math::min<float>() ),
				numTriangles( 0 )
		{
		}
		
		GSOUND_ALIGN(16) SIMDScalar<float,4> min;
		GSOUND_ALIGN(16) SIMDScalar<float,4> max;
		
		
		Size numTriangles;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################





QBVHArrayTree:: QBVHArrayTree( const ArrayList<TriangleType>& newTriangles,
								Size numSplitCandidates, Size maxNumTrianglesPerLeaf )
	:	nodes( NULL ),
		numNodes( 0 ),
		triangles( NULL ),
		numTriangles( 0 )
{
	if ( newTriangles.getSize() != 0 )
		buildTree( newTriangles.getArrayPointer(), newTriangles.getSize(), numSplitCandidates, maxNumTrianglesPerLeaf );
}




QBVHArrayTree:: QBVHArrayTree( const TriangleType* newTriangles, Size newNumTriangles,
								Size numSplitCandidates, Size maxNumTrianglesPerLeaf )
	:	nodes( NULL ),
		numNodes( 0 ),
		triangles( NULL ),
		numTriangles( 0 )
{
	if ( newTriangles != NULL && newNumTriangles != 0 )
		buildTree( newTriangles, newNumTriangles, numSplitCandidates, maxNumTrianglesPerLeaf );
}




QBVHArrayTree:: QBVHArrayTree( const QBVHArrayTree& other )
	:	nodes( util::copyArrayAligned( other.nodes, other.numNodes, sizeof(QBVHArrayTreeNode) ) ),
		numNodes( other.numNodes ),
		triangles( util::copyArrayAligned( other.triangles, other.numTriangles, 16 ) ),
		numTriangles( other.numTriangles )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




QBVHArrayTree:: ~QBVHArrayTree()
{
	// Deallocate the tree.
	destroyTree();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




QBVHArrayTree& QBVHArrayTree:: operator = ( const QBVHArrayTree& other )
{
	if ( this != &other )
	{
		destroyTree();
		
		nodes = util::copyArrayAligned( other.nodes, other.numNodes, sizeof(QBVHArrayTreeNode) );
		numNodes = other.numNodes;
		triangles = util::copyArrayAligned( other.triangles, other.numTriangles, 16 );
		numTriangles = other.numTriangles;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Tree Destruction Method
//############		
//##########################################################################################
//##########################################################################################




void QBVHArrayTree:: destroyTree()
{
	if ( nodes != NULL )
		util::deallocateAligned( nodes );
	
	if ( triangles != NULL )
		util::deallocateAligned( triangles );
	
	nodes = NULL;
	triangles = NULL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Tree Construction Start Method
//############		
//##########################################################################################
//##########################################################################################




void QBVHArrayTree:: buildTree( const TriangleType* newTriangles, Size newNumTriangles,
								Size numSplitCandidates, Size maxNumTrianglesPerLeaf )
{
	// Make sure that there won't be more triangles per leaf than is allowed.
	maxNumTrianglesPerLeaf = math::min( maxNumTrianglesPerLeaf, MAX_NUM_TRIANGLES_PER_LEAF );
	
	// Destroy the previous tree if there was one.
	destroyTree();

	//**************************************************************************************
	
	// Allocate an array to hold the list of TriangleAABB objects.
	TriangleAABB* triangleAABBs = util::allocateAligned<TriangleAABB>( newNumTriangles, 16 );
	
	// Initialize all TriangleAABB objects with the triangles for this tree.
	{
		const TriangleType* triangle = newTriangles;
		TriangleAABB* triangleAABB = triangleAABBs;
		TriangleAABB* const triangleAABBsEnd = triangleAABBs + newNumTriangles;
		
		for ( ; triangleAABB != triangleAABBsEnd; triangleAABB++, triangle++ )
			new (triangleAABB) TriangleAABB( triangle );
	}
	
	//**************************************************************************************
	
	Size numSplitBins = numSplitCandidates + 1;
	
	// Allocate a temporary array to hold the split bins.
	SplitBin* splitBins = util::allocateAligned<SplitBin>( numSplitBins, 16 );
	
	//**************************************************************************************
	
	// Compute the number of nodes needed for this tree.
	numNodes = newNumTriangles*Size(2) - 1;
	
	// Allocate space for the nodes in this tree.
	nodes = util::allocateAligned<QBVHArrayTreeNode>( numNodes, sizeof(QBVHArrayTreeNode) );
	
	// Build the tree, starting with the root node.
	buildTreeRecursive( nodes, triangleAABBs, 0, newNumTriangles,
						splitBins, numSplitBins, maxNumTrianglesPerLeaf );
	
	//**************************************************************************************
	
	// The size needed for the triangle indirection array. This is 1/4 the number of
	// Triangles actually stored because each 'triangle' in this case is actually 4.
	numTriangles = getTriangleArraySize( nodes );
	
	// Allocate an array to hold the triangle indirection list.
	triangles = util::allocateAligned<FatSIMDTriangle3>( numTriangles, 16 );
	
	// Copy the current order of the TriangleAABB list into the tree's list of triangle pointers.
	fillTriangleArray( triangles, triangleAABBs, nodes, 0 );
	
	//**************************************************************************************
	// Clean up the temporary arrays of TriangleAABB objects and split bins.
	
	util::deallocateAligned( triangleAABBs );
	util::deallocateAligned( splitBins );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Recursive Tree Construction Method
//############		
//##########################################################################################
//##########################################################################################




Size QBVHArrayTree:: buildTreeRecursive( QBVHArrayTreeNode* node, TriangleAABB* triangleAABBs,
										Index start, Size numTriangles,
										SplitBin* splitBins, Size numSplitBins, 
										Size maxNumTrianglesPerLeaf )
{
	// The split axis used for each split (0 = X, 1 = Y, 2 = Z).
	StaticArray<Index,3> splitAxis;
	
	// The number of triangles in a child node (leaf or not).
	StaticArray<Index,4> numChildTriangles;
	
	// The 4 volumes of the child nodes.
	StaticArray<AABB3,4> volumes;
	
	//***************************************************************************
	// Partition the set of triangles into two sets.
	
	TriangleAABB* const triangleAABBStart = triangleAABBs + start;
	Size numLesserTriangles;
	
	partitionTrianglesSAH( triangleAABBStart, numTriangles,
						splitBins, numSplitBins,
						splitAxis[0], numLesserTriangles, volumes[0], volumes[2] );
	
	// Compute the number of triangles greater than the split plane along the split axis.
	Size numGreaterTriangles = numTriangles - numLesserTriangles;
	
	//***************************************************************************
	// Partition the triangle subsets into four sets based on the next two splitting planes.
	
	// If the number of triangles on this side of the first partition is less than the max number of
	// triangles per leaf, put all the triangles in the first child.
	if ( numLesserTriangles <= maxNumTrianglesPerLeaf )
	{
		numChildTriangles[0] = numLesserTriangles;
		numChildTriangles[1] = 0;
		volumes[0] = computeTriangleAABB( triangleAABBStart, numLesserTriangles );
	}
	else
	{
		partitionTrianglesSAH( triangleAABBStart, numLesserTriangles,
							splitBins, numSplitBins,
							splitAxis[1], numChildTriangles[0], volumes[0], volumes[1] );
	}
	
	// If the number of triangles on this side of the first partition is less than the max number of
	// triangles per leaf, put all the triangles in the first child.
	if ( numGreaterTriangles <= maxNumTrianglesPerLeaf )
	{
		numChildTriangles[2] = numGreaterTriangles;
		numChildTriangles[3] = 0;
		volumes[2] = computeTriangleAABB( triangleAABBStart + numLesserTriangles, numGreaterTriangles );
	}
	else
	{
		partitionTrianglesSAH( triangleAABBStart + numLesserTriangles, numGreaterTriangles,
							splitBins, numSplitBins,
							splitAxis[2], numChildTriangles[2], volumes[2], volumes[3] );
	}
	
	// Compute the number of triangles greater than the split plane along the split axis.
	numChildTriangles[1] = numLesserTriangles - numChildTriangles[0];
	numChildTriangles[3] = numGreaterTriangles - numChildTriangles[2];
	
	//***************************************************************************
	// Determine for each child whether to create a leaf node or an inner node.
	
	// The 4 indices of either the location in the triangle list of a leaf's triangles,
	// or the relative offset of the child node from the parent.
	StaticArray<Index,4> indices;
	
	// Whether or not the specified child is a leaf node.
	StaticArray<Bool,4> isALeaf( false );
	
	//***************************************************************************
	// Determine the type and attributes for each node.
	
	// Keep track of the total number of nodes in the subtree.
	Size numTreeNodes = 1;
	Size triangleStartIndex = start;
	
	for ( Index i = 0; i < 4; i++ )
	{
		if ( numChildTriangles[i] <= maxNumTrianglesPerLeaf )
		{
			// This child is a leaf node.
			isALeaf[i] = true;
			indices[i] = triangleStartIndex;
		}
		else
		{
			// This child is an inner node, construct it recursively.
			Size numChildNodes = buildTreeRecursive( node + numTreeNodes, triangleAABBs,
													triangleStartIndex, numChildTriangles[i],
													splitBins, numSplitBins, maxNumTrianglesPerLeaf );
			
			// The relative index of this child from the parent node.
			indices[i] = numTreeNodes;
			
			numTreeNodes += numChildNodes;
		}
		
		triangleStartIndex += numChildTriangles[i];
	}
	
	//***************************************************************************
	// Create the node.
	
	new (node) QBVHArrayTreeNode( volumes, indices, isALeaf, numChildTriangles, splitAxis );
	
	// Return the number of nodes in this subtree.
	return numTreeNodes;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Triangle Partition Method
//############		
//##########################################################################################
//##########################################################################################




void QBVHArrayTree:: partitionTrianglesSAH( TriangleAABB* triangleAABBs, Size numTriangles,
											SplitBin* splitBins, Size numSplitBins,
											Index& splitAxis, Size& numLesserTriangles,
											AABB3& lesserVolume, AABB3& greaterVolume )
{
	// If there are no triangles to partition, return immediately.
	if ( numTriangles < 2 )
	{
		splitAxis = 0;
		numLesserTriangles = numTriangles;
		lesserVolume = computeTriangleAABB( triangleAABBs, numTriangles );
		return;
	}
	
	//**************************************************************************************
	// Compute the AABB of the triangle centroids.
	
	// We use the centroids as the 'keys' in splitting triangles.
	const AABB3 centroidAABB = computeCentroidAABB( triangleAABBs, numTriangles );
	const Vector3 aabbDimension = centroidAABB.max - centroidAABB.min;
	
	//**************************************************************************************
	// Initialize the split bins.
	
	const Size numSplitCandidates = numSplitBins - 1;
	
	const Real binningConstant1 = Real(numSplitBins)*(Real(1) - Real(0.00001));
	Real minSplitCost = math::max<Real>();
	Real minSplitPlane = 0;
	SIMDScalar<float,4> lesserMin;
	SIMDScalar<float,4> lesserMax;
	SIMDScalar<float,4> greaterMin;
	SIMDScalar<float,4> greaterMax;
	numLesserTriangles = 0;
	
	splitAxis = 0;
	
	for ( Index axis = 0; axis < 3; axis++ )
	{
		// Compute some constants that are valid for all bins/triangles.
		const Real binningConstant = binningConstant1 / aabbDimension[axis];
		const Real binWidth = aabbDimension[axis] / Real(numSplitBins);
		const Real binsStart = centroidAABB.min[axis];
		
		// Initialize the split bins to their starting values.
		for ( Index i = 0; i < numSplitBins; i++ )
			new (splitBins + i) SplitBin();
		
		//**************************************************************************************
		// For each triangle, determine which bin it overlaps and increase that bin's counter.
		
		for ( Index i = 0; i < numTriangles; i++ )
		{
			const TriangleAABB& t = triangleAABBs[i];
			
			Index binIndex = (Index)(binningConstant*(t.centroid[axis] - binsStart));
			SplitBin& bin = splitBins[binIndex];
			
			// Update the number of triangles that this bin contains, as well as the AABB for those triangles.
			bin.numTriangles++;
			bin.min = math::min( bin.min, t.min );
			bin.max = math::max( bin.max, t.max );
		}
		
		//**************************************************************************************
		// Find the split plane with the smallest SAH cost.
		
		Size numLeftTriangles = 0;
		SIMDScalar<float,4> leftMin( math::max<float>() );
		SIMDScalar<float,4> leftMax( math::min<float>() );
		
		for ( Index i = 0; i < numSplitCandidates; i++ )
		{
			// Since the left candidate is only growing, we can incrementally construct the AABB for this side.
			// Incrementally enlarge the bounding box for this side, and compute the number of triangles
			// on this side of the split.
			{
				SplitBin& bin = splitBins[i];
				numLeftTriangles += bin.numTriangles;
				leftMin = math::min( leftMin, bin.min );
				leftMax = math::max( leftMax, bin.max );
			}
			
			Size numRightTriangles = 0;
			SIMDScalar<float,4> rightMin( math::max<float>() );
			SIMDScalar<float,4> rightMax( math::min<float>() );
			
			// Compute the bounding box for this side, and compute the number of triangles
			// on this side of the split.
			for ( Index j = i + 1; j < numSplitBins; j++ )
			{
				SplitBin& bin = splitBins[j];
				numRightTriangles += bin.numTriangles;
				rightMin = math::min( rightMin, bin.min );
				rightMax = math::max( rightMax, bin.max );
			}
			
			// Compute the cost for this split candidate.
			Real splitCost = Real(numLeftTriangles)*getAABBSurfaceArea( leftMin, leftMax ) + 
							Real(numRightTriangles)*getAABBSurfaceArea( rightMin, rightMax );
			
			// If the split cost is the lowest so far, use it as the new minimum split.
			if ( splitCost <= minSplitCost )
			{
				minSplitCost = splitCost;
				minSplitPlane = binsStart + binWidth*Float(i + 1);
				
				// Save the bounding boxes for this split candidate.
				lesserMin = leftMin;
				lesserMax = leftMax;
				greaterMin = rightMin;
				greaterMax = rightMax;
				
				// Save the number of triangles to the left of the split.
				numLesserTriangles = numLeftTriangles;
				
				// Save the axis of the minimum cost split candidate.
				splitAxis = axis;
			}
		}
	}
	
	//**************************************************************************************
	
	// If the split was unsuccessful, try a median split which is guaranteed to split the triangles.
	if ( numLesserTriangles == 0 || numLesserTriangles == numTriangles )
	{
		// Choose to split along the axis with the largest extent.
		Size splitAxis = aabbDimension[0] > aabbDimension[1] ? 
						aabbDimension[0] > aabbDimension[2] ? 0 : 2 :
						aabbDimension[1] > aabbDimension[2] ? 1 : 2;
		
		// Use a median-based partition to split the triangles.
		partitionTrianglesMedian( triangleAABBs, numTriangles, splitAxis, numLesserTriangles, lesserVolume, greaterVolume );
		
		return;
	}
	
	//**************************************************************************************
	// Partition the triangles into two sets based on the minimal cost split plane.
	
	Index left = 0;
	Index right = numTriangles - 1;
	
	while ( left < right )
	{
		// Move right while triangle < split plane.
		while ( triangleAABBs[left].centroid[splitAxis] <= minSplitPlane && left < right )
			left++;
		
		// Move left while triangle > split plane.
		while ( triangleAABBs[right].centroid[splitAxis] > minSplitPlane && left < right )
			right--;
		
		if ( left < right )
		{
			// Swap the triangles because they are out of order.
			const TriangleAABB temp = triangleAABBs[left];
			triangleAABBs[left] = triangleAABBs[right];
			triangleAABBs[right] = temp;
		}
	}
	
	// Set the number of triangles that are to the left of the split plane.
	lesserVolume = AABB3( lesserMin[0], lesserMax[0], lesserMin[1], lesserMax[1], lesserMin[2], lesserMax[2] );
	greaterVolume = AABB3( greaterMin[0], greaterMax[0], greaterMin[1], greaterMax[1], greaterMin[2], greaterMax[2] );
}




void QBVHArrayTree:: partitionTrianglesMedian( TriangleAABB* triangleAABBs, Size numTriangles,
												Index splitAxis, Size& numLesserTriangles,
												AABB3& lesserVolume, AABB3& greaterVolume )
{
	if ( numTriangles == 2 )
	{
		numLesserTriangles = 1;
		lesserVolume = computeTriangleAABB( triangleAABBs, 1 );
		greaterVolume = computeTriangleAABB( triangleAABBs + 1, 1 );
		return;
	}
	
	Index first = 0;
	Index last = numTriangles - 1;
	Index middle = (first + last)/2;
	
	while ( 1 )
	{
		Index mid = first;
		const SIMDScalar<float,4>& key = triangleAABBs[mid].centroid;
		
		for ( Index j = first + 1; j <= last; j ++)
		{
			if ( triangleAABBs[j].centroid[splitAxis] > key[splitAxis] )
			{
				mid++;
				
				// interchange values.
				const TriangleAABB temp = triangleAABBs[mid];
				triangleAABBs[mid] = triangleAABBs[j];
				triangleAABBs[j] = temp;
			}
		}
		
		// interchange the first and mid value.
		const TriangleAABB temp = triangleAABBs[mid];
		triangleAABBs[mid] = triangleAABBs[first];
		triangleAABBs[first] = temp;
		
		if ( mid + 1 == middle )
			break;
		
		if ( mid + 1 > middle )
			last = mid - 1;
		else
			first = mid + 1;
	}
	
	numLesserTriangles = numTriangles / 2;
	
	lesserVolume = computeTriangleAABB( triangleAABBs, numLesserTriangles );
	greaterVolume = computeTriangleAABB( triangleAABBs + numLesserTriangles, numTriangles - numLesserTriangles );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Axis-Aligned Bound Box Calculation Methods
//############		
//##########################################################################################
//##########################################################################################




AABB3 QBVHArrayTree:: computeTriangleAABB( const TriangleAABB* triangleAABBs, Size numTriangles )
{
	/// Create a bounding box with the minimum at the max float value and visce versa.
	SIMDScalar<float,4> min( math::max<float>() );
	SIMDScalar<float,4> max( math::min<float>() );
	
	const TriangleAABB* const triangleAABBsEnd = triangleAABBs + numTriangles;
	
	while ( triangleAABBs != triangleAABBsEnd )
	{
		min = math::min( min, triangleAABBs->min );
		max = math::max( max, triangleAABBs->max );
		
		triangleAABBs++;
	}
	
	return AABB3( min[0], max[0], min[1], max[1], min[2], max[2] );
}




AABB3 QBVHArrayTree:: computeCentroidAABB( const TriangleAABB* triangleAABBs, Size numTriangles )
{
	/// Create a bounding box with the minimum at the max float value and visce versa.
	SIMDScalar<float,4> min( math::max<float>() );
	SIMDScalar<float,4> max( math::min<float>() );
	
	const TriangleAABB* const triangleAABBsEnd = triangleAABBs + numTriangles;
	
	while ( triangleAABBs != triangleAABBsEnd )
	{
		min = math::min( min, triangleAABBs->centroid );
		max = math::max( max, triangleAABBs->centroid );
		
		triangleAABBs++;
	}
	
	return AABB3( min[0], max[0], min[1], max[1], min[2], max[2] );
}




Real QBVHArrayTree:: getAABBSurfaceArea( const SIMDScalar<float,4>& min, const SIMDScalar<float,4>& max )
{
	const SIMDScalar<float,4> aabbDimension = max - min;
	
	return Real(2)*(aabbDimension[0]*aabbDimension[1] +
					aabbDimension[0]*aabbDimension[2] +
					aabbDimension[1]*aabbDimension[2]);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Triangle List Building Methods
//############		
//##########################################################################################
//##########################################################################################




Size QBVHArrayTree:: getTriangleArraySize( const QBVHArrayTreeNode* node )
{
	Size result = 0;
	
	for ( Index i = 0; i < 4; i++ )
	{
		if ( node->isLeaf(i) )
			result += math::nextMultiple( node->getNumberOfTriangles(i), Size(4) ) >> 2;
		else
			result += getTriangleArraySize( node->getChild(i) );
	}
	
	return result;
}




Size QBVHArrayTree:: fillTriangleArray( const TriangleType** triangles, const TriangleAABB* aabbs, QBVHArrayTreeNode* node, Size numFilled )
{
	for ( Index i = 0; i < 4; i++ )
	{
		if ( node->isLeaf(i) )
		{
			Size numLeafTriangles = node->getNumberOfTriangles(i);
			Size numTruncatedTriangles = ((numLeafTriangles >> 2) << 2);
			Size numPaddedTriangles = numTruncatedTriangles == numLeafTriangles ? 
										numTruncatedTriangles : numTruncatedTriangles + 4;
			
			Index j = node->getTriangleStartIndex(i);
			Index k = 0;
			node->setTriangleStartIndex( i, numFilled );
			node->setNumberOfTriangles( i, numPaddedTriangles );
			
			for ( ; k < numLeafTriangles; k++, j++, numFilled++ )
				triangles[numFilled] = aabbs[j].triangle;
			
			j--;
			
			for ( ; k < numPaddedTriangles; k++, numFilled++ )
				triangles[numFilled] = aabbs[j].triangle;
		}
		else
			numFilled = fillTriangleArray( triangles, aabbs, node->getChild(i), numFilled );
	}
	
	return numFilled;
}




Size QBVHArrayTree:: fillTriangleArray( FatSIMDTriangle3* triangles, const TriangleAABB* aabbs, QBVHArrayTreeNode* node, Size numFilled )
{
	Size currentOutputIndex = numFilled;
	
	for ( Index i = 0; i < 4; i++ )
	{
		if ( node->isLeaf(i) )
		{
			if ( node->isEmpty(i) )
				continue;
			
			Size numLeafTriangles = node->getNumberOfTriangles(i);
			Size numTruncatedTriangles = ((numLeafTriangles >> 2) << 2);
			Size numPaddedTriangles = numTruncatedTriangles == numLeafTriangles ? 
										numTruncatedTriangles : numTruncatedTriangles + 4;
			
			Index j = node->getTriangleStartIndex(i);
			node->setTriangleStartIndex( i, currentOutputIndex );
			node->setNumberOfTriangles( i, numPaddedTriangles >> 2 );
			
			Size numIterations = numTruncatedTriangles >> 2;
			
			for ( Index k = 0; k < numIterations; k++ )
			{
				new (triangles + currentOutputIndex) FatSIMDTriangle3( aabbs[j].triangle, aabbs[j+1].triangle,
																		aabbs[j+2].triangle, aabbs[j+3].triangle );
				j += 4;
				
				currentOutputIndex++;
			}
			
			Size numRemainingTriangles = numLeafTriangles - numTruncatedTriangles;
			
			if ( numRemainingTriangles > 0 )
			{
				// For the last 4 triangles, copy the pointers of the valid triangles.
				// If there are extras, put additional copies of the last triangle.
				const TriangleType* lastTriangles[4];
				
				Index k = 0;
				
				for ( ; k < numRemainingTriangles; k++, j++ )
					lastTriangles[k] = aabbs[j].triangle;
				
				j--;
				
				for ( ; k < 4; k++ )
					lastTriangles[k] = aabbs[j].triangle;
				
				// Construct the last triangle
				new (triangles + currentOutputIndex) FatSIMDTriangle3( lastTriangles[0], lastTriangles[1], lastTriangles[2], lastTriangles[3] );
				
				currentOutputIndex++;
			}
		}
		else
			currentOutputIndex += fillTriangleArray( triangles, aabbs, node->getChild(i), currentOutputIndex );
	}
	
	return currentOutputIndex - numFilled;
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
