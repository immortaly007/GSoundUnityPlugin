/*
 * Project:     GSound
 * 
 * File:        gsound/SoundMesh.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundMesh class implementation
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


#include "SoundMesh.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




const Size SoundMesh:: MAX_NUMBER_OF_TRIANGLES_PER_LEAF = 4;


const Size SoundMesh:: NUMBER_OF_SPLIT_PLANE_CANDIDATES = 8;




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundMesh:: SoundMesh()
	:	vertices(),
		triangles(),
		materials(),
		bvh( NULL ),
		userData( NULL )
{
}




SoundMesh:: SoundMesh( const ArrayList<SoundVertex>& newVertices,
						const ArrayList<SoundTriangle>& newTriangles,
						const ArrayList<SoundMaterial>& newMaterials )
	:	vertices( newVertices.getSize() ),
		triangles( newTriangles.getSize() ),
		materials( newMaterials ),
		bvh( NULL ),
		userData( NULL )
{
	// Make sure that there is at least one material.
	if ( materials.getSize() == 0 )
		materials.add( SoundMaterial() );
	
	// Build the list of triangles for this mesh shape.
	SoundMeshPreprocessor preprocessor;
	preprocessor.processMesh( newVertices, newTriangles, materials, vertices, triangles );
	
	// Generate a bounding sphere for the mesh.
	boundingSphere = internal::BoundingSphere( vertices );
	
	// Build the bounding volume hierarchy for the mesh.
	buildBVH();
}




SoundMesh:: SoundMesh( const ArrayList<SoundVertex>& newVertices,
						const ArrayList<SoundTriangle>& newTriangles,
						const ArrayList<SoundMaterial>& newMaterials,
						const SoundMeshPreprocessor& preprocessor )
	:	vertices( newVertices.getSize() ),
		triangles( newTriangles.getSize() ),
		materials( newMaterials ),
		bvh( NULL ),
		userData( NULL )
{
	// Make sure that there is at least one material.
	if ( materials.getSize() == 0 )
		materials.add( SoundMaterial() );
	
	// Build the list of triangles for this mesh shape.
	preprocessor.processMesh( newVertices, newTriangles, materials, vertices, triangles );
	
	// Generate a bounding sphere for the mesh.
	boundingSphere = internal::BoundingSphere( vertices );
	
	// Build the bounding volume hierarchy for the mesh.
	buildBVH();
}




SoundMesh:: SoundMesh( const SoundMesh& other )
	:	vertices( other.vertices ),
		materials( other.materials ),
		triangles( other.triangles ),
		bvh( NULL ),
		boundingSphere( other.boundingSphere ),
		userData( other.userData )
{
	const SoundVertex* const verticesStart = other.vertices.getArrayPointer();
	const TriangleType* const trianglesStart = other.triangles.getArrayPointer();
	const SoundMaterial* const materialsStart = other.materials.getArrayPointer();
	
	for ( Index i = 0; i < triangles.getSize(); i++ )
	{
		TriangleType& triangle = triangles[i];
		
		// Compute new vertex pointers.
		for ( Index v = 0; v < 3; v++ )
			triangle.setVertex( v, &vertices[&triangle.getVertex(v) - verticesStart] );
		
		// Compute new triangle neighbor pointers.
		for ( Index n = 0; n < 3; n++ )
		{
			if ( triangle.getNeighbor(n) != NULL )
				triangle.setNeighbor( n, &triangles[triangle.getNeighbor(n) - trianglesStart] );
		}
		
		// Compute a new material pointer.
		triangle.setMaterial( &materials[&triangle.getMaterial() - materialsStart] );
	}
	
	// Rebuild the BVH.
	buildBVH();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundMesh:: ~SoundMesh()
{
	if ( bvh != NULL )
		util::destruct(bvh);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundMesh& SoundMesh:: operator = ( const SoundMesh& other )
{
	if ( this != &other )
	{
		vertices = other.vertices;
		materials = other.materials;
		triangles = other.triangles;
		boundingSphere = other.boundingSphere;
		userData = other.userData;
		
		if ( bvh != NULL )
		{
			util::destruct(bvh);
			bvh = NULL;
		}
		
		const SoundVertex* const verticesStart = other.vertices.getArrayPointer();
		const TriangleType* const trianglesStart = other.triangles.getArrayPointer();
		const SoundMaterial* const materialsStart = other.materials.getArrayPointer();
		
		for ( Index i = 0; i < triangles.getSize(); i++ )
		{
			TriangleType& triangle = triangles[i];
			
			// Compute new vertex pointers.
			for ( Index v = 0; v < 3; v++ )
				triangle.setVertex( v, &vertices[&triangle.getVertex(v) - verticesStart] );
			
			// Compute new triangle neighbor pointers.
			for ( Index n = 0; n < 3; n++ )
			{
				if ( triangle.getNeighbor(n) != NULL )
					triangle.setNeighbor( n, &triangles[triangle.getNeighbor(n) - trianglesStart] );
			}
			
			// Compute a new material pointer.
			triangle.setMaterial( &materials[&triangle.getMaterial() - materialsStart] );
		}
		
		// Rebuild the BVH.
		buildBVH();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		BVH Building Method
//############		
//##########################################################################################
//##########################################################################################




void SoundMesh:: buildBVH()
{
	if ( bvh != NULL )
		util::destruct(bvh);
	
	bvh = util::construct<BVHType>( triangles, NUMBER_OF_SPLIT_PLANE_CANDIDATES, MAX_NUMBER_OF_TRIANGLES_PER_LEAF );
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
