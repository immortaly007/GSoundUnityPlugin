/*
 * Project:     GSound
 * 
 * File:        gsound/SoundMeshPreprocessor.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundMeshPreprocessor class implementation
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


#include "SoundMeshPreprocessor.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




void SoundMeshPreprocessor:: processMesh( const ArrayList<SoundVertex>& newVertices,
											const ArrayList<SoundTriangle>& newTriangles,
											const ArrayList<SoundMaterial>& newMaterials,
											ArrayList<SoundVertex>& outputVertices,
											ArrayList<internal::InternalSoundTriangle>& outputTriangles ) const
{
	// Prepare internal data structures for the given lists of vertices and triangles.
	resetHelperDataStructures( newVertices.getSize(), newTriangles.getSize() );
	
	//***************************************************************************
	// Build a list for each vertex of all triangles that share it.
	
	Size numVertices = newVertices.getSize();
	Size numTriangles = newTriangles.getSize();
	
	for ( Index i = 0; i < numTriangles; i++ )
	{
		const SoundTriangle& triangle = newTriangles[i];
		
		if ( triangle.v[0] < numVertices )
			vertexRecords[triangle.v[0]].triangles.add(i);
		
		if ( triangle.v[1] < numVertices )
			vertexRecords[triangle.v[1]].triangles.add(i);
		
		if ( triangle.v[2] < numVertices )
			vertexRecords[triangle.v[2]].triangles.add(i);
	}
	
	
	//***************************************************************************
	// Weld vertices that are close to each other.
	
	// Add all vertices to a hash table to determine which vertices might be
	// close to each other.
	
	if ( isWeldingVertices )
	{
		Real gridSize = 2*weldingTolerance;
		Real weldingToleranceSquared = weldingTolerance*weldingTolerance;
		
		for ( Index i = 0; i < numVertices; i++ )
		{
			// Skip vertices that aren't referenced by any triangles.
			if ( vertexRecords[i].triangles.getSize() == 0 )
				continue;
			
			const SoundVertex& vertex = newVertices[i];
			
			math::AABB3i cellBounds( Int((vertex.x - weldingTolerance)/gridSize),
									Int((vertex.x + weldingTolerance)/gridSize),
									Int((vertex.y - weldingTolerance)/gridSize),
									Int((vertex.y + weldingTolerance)/gridSize),
									Int((vertex.z - weldingTolerance)/gridSize),
									Int((vertex.z + weldingTolerance)/gridSize) );
			
			Bool wasWelded = false;
			Index weldedIndex;
			
			for ( Int x = cellBounds.min.x; x <= cellBounds.max.x; x++ )
			{
				for ( Int y = cellBounds.min.y; y <= cellBounds.max.y; y++ )
				{
					for ( Int z = cellBounds.min.z; z <= cellBounds.max.z; z++ )
					{
						Hash bucketIndex = getGridCellHash( x, y, z ) % gridHashTable.getSize();
						
						ArrayList<Index>& bucket = gridHashTable[bucketIndex];
						
						// Look for vertices within the welding tolerance for this bucket.
						for ( Index j = 0; j < bucket.getSize(); j++ )
						{
							if ( newVertices[bucket[j]].getDistanceToSquared( vertex ) <= weldingToleranceSquared )
							{
								wasWelded = true;
								weldedIndex = bucket[j];
								
								goto foundVertexLabel;
							}
						}
					}
				}
			}
			
			foundVertexLabel:;
			
			if ( wasWelded )
			{
				vertexRecords[i].wasWelded = wasWelded;
				vertexRecords[i].weldedIndex = weldedIndex;
			}
			else
			{
				for ( Int x = cellBounds.min.x; x <= cellBounds.max.x; x++ )
				{
					for ( Int y = cellBounds.min.y; y <= cellBounds.max.y; y++ )
					{
						for ( Int z = cellBounds.min.z; z <= cellBounds.max.z; z++ )
						{
							Hash bucketIndex = getGridCellHash( x, y, z ) % gridHashTable.getSize();
							
							ArrayList<Index>& bucket = gridHashTable[bucketIndex];
							
							// Add the vertex to the bucket if no welding candidates were found.
							bucket.add( i );
						}
					}
				}
			}
		}
	}
	
	//***************************************************************************
	// Build the final list of vertices
	
	// Make sure that the old vertex list is empty.
	outputVertices.clear();
	
	for ( Index i = 0; i < numVertices; i++ )
	{
		// If this vertex was not referenced by any triangles, skip it.
		if ( vertexRecords[i].triangles.getSize() == 0 )
			continue;
		else if ( vertexRecords[i].wasWelded )
		{
			vertexRecords[vertexRecords[i].weldedIndex].triangles.addAll( vertexRecords[i].triangles );
			continue;
		}
		else
		{
			vertexRecords[i].finalIndex = outputVertices.getSize();
			outputVertices.add( newVertices[i] );
		}
	}
	
	
	//***************************************************************************
	// Find all triangle duplicates
	
	for ( Index i = 0, currentIndex = 0; i < newTriangles.getSize(); i++ )
	{
		const SoundTriangle& t = newTriangles[i];
		
		// If the triangle's vertex indices are invalid, skip the triangle.
		if ( t.v[0] >= numVertices || t.v[1] >= numVertices || t.v[2] >= numVertices )
			continue;
		
		SoundTriangle t2( getFinalVertexIndex( t.v[0] ),
							getFinalVertexIndex( t.v[1] ),
							getFinalVertexIndex( t.v[2] ),
							t.materialIndex );
		
		// Keep track of whether or not this triangle is a duplicate after welding.
		if ( t2.v[0] == t2.v[1] || t2.v[0] == t2.v[2] || t2.v[1] == t2.v[2] )
		{
			// This is a degenerate triangle, mark it to be removed.
			triangleRecords[i].isADuplicate = true;
		}
		else if ( triangleSet.add( getTriangleHash( t2.v[0], t2.v[1], t2.v[2] ), t2 ) )
		{
			triangleRecords[i].isADuplicate = false;
			triangleRecords[i].finalIndex = currentIndex;
			currentIndex++;
		}
		else
		{
			triangleRecords[i].isADuplicate = true;
		}
	}
	
	//***************************************************************************
	// Find all triangle edge neighbors
	
	
	for ( Index i = 0; i < newTriangles.getSize(); i++ )
	{
		const SoundTriangle& t = newTriangles[i];
		
		// Skip triangles that are duplicates.
		if ( triangleRecords[i].isADuplicate )
			continue;
		
		for ( Index v = 0; v < 3; v++ )
		{
			const VertexRecord* record = &vertexRecords[t.v[v]];
			
			if ( record->wasWelded )
				record = &vertexRecords[record->weldedIndex];
			
			for ( Index j = 0; j < record->triangles.getSize(); j++ )
			{
				if ( i == record->triangles[j] || triangleRecords[record->triangles[j]].isADuplicate )
					continue;
				
				const SoundTriangle& t2 = newTriangles[record->triangles[j]];
				
				for ( Index l = v + 1; l < 3; l++ )
				{
					Index searchIndex = getFinalVertexIndex(t.v[l]);
					
					for ( Index k = 0; k < 3; k++ )
					{
						if ( getFinalVertexIndex(t2.v[k]) == searchIndex )
						{
							// Calculate the edge index from the vertex indices.
							Index edgeIndex = internal::InternalSoundTriangle::getEdgeIndex( v, l );
							
							triangleRecords[i].foundEdgeNeighbor[edgeIndex] = true;
							triangleRecords[i].neighborIndex[edgeIndex] = triangleRecords[record->triangles[j]].finalIndex;
						}
					}
				}
			}
		}
	}
	
	
	//***************************************************************************
	// Build the final list of triangles.
	
	// Make sure that the old triangle list is empty.
	outputTriangles.clear();
	
	Size numMaterials = newMaterials.getSize();
	
	// For each input triangle, verify its vertex and material indices, and calculate a normal.
	for ( Index i = 0; i < newTriangles.getSize(); i++ )
	{
		const SoundTriangle& t = newTriangles[i];
		
		// If the triangle's vertex indices are invalid, skip the triangle.
		if ( t.v[0] >= numVertices || t.v[1] >= numVertices || t.v[2] >= numVertices )
			continue;
		
		if ( triangleRecords[i].isADuplicate )
			continue;
		
		// If the triangle's material index is invalid, give it the material at index 0.
		Index materialIndex;
		
		if ( t.materialIndex >= numMaterials )
			materialIndex = 0;
		else
			materialIndex = t.materialIndex;
		
		Index v1 = getFinalVertexIndex( t.v[0] );
		Index v2 = getFinalVertexIndex( t.v[1] );
		Index v3 = getFinalVertexIndex( t.v[2] );
		
		// Create a triangle and add it to the list of triangles.
		outputTriangles.add( internal::InternalSoundTriangle(
								&outputVertices[v1], &outputVertices[v2], &outputVertices[v3], 
								&newMaterials[materialIndex] ) );
	}
	
	//***************************************************************************
	// Finalize triangle neighbor pointers and determine edge diffraction classification.
	
	for ( Index i = 0; i < newTriangles.getSize(); i++ )
	{
		if ( triangleRecords[i].isADuplicate )
			continue;
		
		for ( Index j = 0; j < 3; j++ )
		{
			if ( triangleRecords[i].foundEdgeNeighbor[j] )
			{
				internal::InternalSoundTriangle& t = outputTriangles[triangleRecords[i].finalIndex];
				
				t.setNeighbor( j, &outputTriangles[triangleRecords[i].neighborIndex[j]] );
				
				Real nDotN = math::abs( math::dot( t.getNormal(), t.getNeighbor(j)->getNormal() ) );
				
				if ( nDotN > diffractionThreshold )
					t.setEdgeType( j, internal::InternalSoundTriangle::NON_DIFFRACTING );
			}
		}
	}
}






void SoundMeshPreprocessor:: resetHelperDataStructures( Size numVertices, Size numTriangles ) const
{
	//****************************************************************
	// Reset the vertex records data structure.
	
	for ( Index i = 0; i < vertexRecords.getSize(); i++ )
		vertexRecords[i].reset();
	
	if ( numVertices > vertexRecords.getSize() )
	{
		// Make room for the new vertex records.
		vertexRecords.setCapacity( numVertices );
		
		Size numberToAdd = numVertices - vertexRecords.getSize();
		
		for ( Index i = 0; i < numberToAdd; i++ )
			vertexRecords.add( VertexRecord() );
	}
	
	//****************************************************************
	// Reset the vertex welding grid hash table.
	
	for ( Index i = 0; i < gridHashTable.getSize(); i++ )
		gridHashTable[i].clear();
		
	if ( numVertices > gridHashTable.getSize() )
	{
		gridHashTable.setCapacity( numVertices );
		
		Size numberToAdd = numVertices - gridHashTable.getSize();
		
		for ( Index i = 0; i < numberToAdd; i++ )
			gridHashTable.add( ArrayList<Index>() );
	}
	
	//****************************************************************
	// Reset the triangle neighbor structure.
	
	for ( Index i = 0; i < triangleRecords.getSize(); i++ )
		triangleRecords[i] = TriangleRecord();
	
	if ( numTriangles > triangleRecords.getSize() )
	{
		// Make room for the new triangle records.
		triangleRecords.setCapacity( numTriangles );
		
		Size numberToAdd = numTriangles - triangleRecords.getSize();
		
		for ( Index i = 0; i < numberToAdd; i++ )
			triangleRecords.add( TriangleRecord() );
	}
	
	//****************************************************************
	// Reset the triangle set structure.
	
	triangleSet.clear();
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################

