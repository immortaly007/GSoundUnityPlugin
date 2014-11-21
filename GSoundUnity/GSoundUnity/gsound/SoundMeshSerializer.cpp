/*
 * Project:     GSound
 * 
 * File:        gsound/SoundMeshSerializer.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundMeshSerializer class implementation
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


#include "SoundMeshSerializer.h"


#include <string.h>


#define HEADER_SIZE 16



//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################





//##########################################################################################
//##########################################################################################
//############
//############		Static Version Limit Definitions
//############
//##########################################################################################
//##########################################################################################




const SoundMeshSerializer::SoundMeshVersion SoundMeshSerializer:: minimumSupportedVersion = 1;
const SoundMeshSerializer::SoundMeshVersion SoundMeshSerializer:: maximumSupportedVersion = 1;




//##########################################################################################
//##########################################################################################
//############
//############		Constructor
//############
//##########################################################################################
//##########################################################################################







//##########################################################################################
//##########################################################################################
//############
//############		Serialization Methods
//############
//##########################################################################################
//##########################################################################################




Bool SoundMeshSerializer:: serialize( const String& fileName, const SoundMesh& mesh, Bool overwrite )
{
	return serialize( fileName, mesh, overwrite, maximumSupportedVersion );
}




Bool SoundMeshSerializer:: serialize( const String& fileName, const SoundMesh& mesh, Bool overwrite,
										SoundMeshVersion preferedVersion )
{
	// Make sure that the prefered version is supported.
	if ( preferedVersion < minimumSupportedVersion || preferedVersion > maximumSupportedVersion )
		return false;

	// Test to see if the file already existed.
	std::FILE* file = std::fopen( fileName.c_str(), "r" );

	if ( file != NULL )
	{
		// close the file for reading.
		std::fclose( file );

		// If the file already existed and we can't overwrite it, abort.
		if ( !overwrite )
			return false;
	}

	// Reopen the file for writing.
	file = std::fopen( fileName.c_str(), "w" );

	// If there was a problem when opening the file, return false.
	if ( file == NULL )
		return false;

	Bool result;

	switch ( preferedVersion )
	{
		case 1:
			result = serializeVersion1( file, mesh );
			break;
	}

	// Close the file.
	std::fclose( file );

	return result;
}



//##########################################################################################
//##########################################################################################
//############
//############		Deserialization Methods
//############
//##########################################################################################
//##########################################################################################




SoundMesh* SoundMeshSerializer:: deserialize( const String& fileName )
{
	std::FILE* file = std::fopen( fileName.c_str(), "rb" );

	// If there was a problem when opening the file, return a NULL pointer.
	if ( file == NULL )
		return NULL;

	/*
	* The version-independent 16-byte header for the file is as follows:
	* - Bytes 0 through 8: The ASCII character string 'SOUNDMESH'.
	* - Byte 9: An unsigned 8-bit integer indicating the version number of the mesh format.
	* - Byte 10: A byte that specifies the endianness of the file, 0 if little endian, 1 if big endian.
	* - Byte 11: reserved/padding.
	* - Bytes 12 through 15: A 32-bit unsigned integer indicating a checksum of the rest of the file
	*   after the header. The checksum is computed by summing the value of every byte in the file as
	*   interpreted as 8-bit integers and allowing overflow. This value is provided to detect corruption
	*   of the file.
	*/

	// Read the first 10 bytes of the version-independent header.
	UByte header[10];

	size_t numRead = std::fread( header, sizeof(Byte), 10, file );

	// Make sure that we were able to read the whole header. Also make sure that this is a mesh file.
	// If not, abort the operation.
	if ( numRead != 10 || memcmp( header, "SOUNDMESH", 9 ) != 0 )
	{
		std::fclose( file );
		return NULL;
	}

	// Get the format version.
	UInt32 version = header[9];
	
	SoundMesh* mesh = NULL;
	
	switch ( version )
	{
		case 1:
			mesh = deserializeVersion1( file );
			break;
	}

	// Close the file.
	std::fclose( file );

	return mesh;
}




//##########################################################################################
//##########################################################################################
//############
//############		Version 1 Serialization Method
//############
//##########################################################################################
//##########################################################################################




/**
  * Version 1 of the GSound Sound Mesh binary format.
  *
  * The version-independent 16-byte header for the file is as follows:
  * - Bytes 0 through 8: The ASCII character string 'SOUNDMESH'.
  * - Byte 9: An unsigned 8-bit integer indicating the version number of the mesh format.
  * - Byte 10: A byte that specifies the endianness of the file, 0 if little endian, 1 if big endian.
  * - Byte 11: reserved/padding.
  * - Bytes 12 through 15: A 32-bit unsigned integer indicating a checksum of the rest of the file
  *   after the header. The checksum is computed by summing the value of every byte in the file as
  *   interpreted as 8-bit integers and allowing overflow. This value is provided to detect corruption
  *   of the file.
  *
  * The materials for the mesh:
  * - An unsigned 32-bit integer specifying the number of frequency bands in a material frequency response.
  * - The previously specified number of single-precision IEEE 754 floating point numbers indicating the center
  *   frequencies of the bands in a material frequency response.
  * - An unsigned 32-bit integer specifying the number of materials in the mesh.
  *
  * The previously specified number of instances of the following material block:
  * - The previously specified number of single-precision IEEE 754 floating point numbers indicating
  *   the linear gain factors for each of the frequency band centers when undergoing a reflection.
  * - The previously specified number of single-precision IEEE 754 floating point numbers indicating
  *   the linear gain factors for each of the frequency band centers when undergoing transmission
  *   across a material boundary.
  * - The previously specified number of single-precision IEEE 754 floating point numbers indicating
  *   the linear gain factors for each of the frequency band centers when undergoing transmission
  *   through a material. The gain factors are specified in units of attenuation per world unit.
  *
  * The vertices for the mesh:
  * - An unsigned 32-bit integer specifying the number of vertices in the mesh.
  * - The previously specified number of vertices, each given by a set of three single-precision
  *   IEEE 754 floating point numbers representing the X, Y, and Z coordinate of the vertex in that order.
  *
  * The triangles for the mesh:
  * - An unsigned 32-bit integer specifying the number of triangles in the mesh.
  *
  * The previously specified number of instances of the following triangle block:
  * - Three unsigned 32-bit integers specifying the indices of the triangle's vertices
  *   in the order vertex 0, vertex 1, vertex 2.
  * - Three unsigned 32-bit integers specifying the indices of the triangle's neighbors.
  *   Neighbor 0 shares the edge between vertices 0 and 1, neighbor 1 shares the edge between
  *   vertices 0 and 2, and neighbor 2 shares the edge between vertices 1 and 2. If a triangle
  *   does not have a neighbor at a position, the index will be equal to the triangle's own index.
  * - Three bytes indicating the diffraction type for each edge of the triangle. A value of 0
  *   indicates that an edge is not diffracting, a value of 1 indicates that the edge is diffracting.
  *   The bytes are specified in the following order: edge(0,1), edge(0,2), edge(1,2).
  * - One byte for padding.
  * - An unsigned 32-bit integer indicating the index of the triangle's material.
  *
  * The bounding sphere for the mesh:
  * - Three single-precision IEEE 754 floating point numbers indicating the center of the bounding sphere
  *   of the mesh.
  * - A single-precision IEEE 754 floating point number indicating the radius of the bounding sphere.
  *
  * The BVH for the mesh:
  * - An unsigned 32-bit integer specifying the number of BVH nodes in this mesh.
  *
  * The previously specified number of instances of the following BVH node block:
  * - Six single-precision IEEE 754 floating point numbers indicating the axis-aligned bounding
  *   box for the node. The numbers are specified in the following order: X minimum, X maximum,
  *   Y minimum, Y maximum, Z minimum, Z maximum.
  * - An unsigned 32-bit integer indicating the index of the first child of this node in the
  *   list of nodes. The second child is the next node after the first child. If a node has
  *   no children, this value will be equal to 0. The value of 0 is unused because no node can
  *   have the root node as its child, as the root node always has index 0.
  * - An unsigned 32-bit integer indicating the index of the triangle which the node contains.
  *   This value only has any meaning if the index of the first child is equal to 0, indicating
  *   that the node is a leaf.
  */
Bool SoundMeshSerializer:: serializeVersion1( std::FILE* file, const SoundMesh& mesh )
{
	//***************************************************************************
	// Write the preliminary header for the file.

	UByte header[HEADER_SIZE];

	// Write the identifying ASCII string.
	memcpy( header, "SOUNDMESH", 9 );

	// Write the version number.
	header[9] = 1;

	// If the platform is big-endian, write a byte equal to 1, otherwise write a byte equal to 0.
#if defined(GSOUND_BIG_ENDIAN)
	header[10] = 1;
#else
	header[10] = 0;
#endif

	// Skip the padding byte and checksum UInt32, the checksum value will be updated later.

	// Write the current header to the file.
	if ( std::fwrite( header, sizeof(Byte), HEADER_SIZE, file ) != HEADER_SIZE )
		return false;

	// Declare the checksum variable so that we can compute the checksum as we write the file.
	UInt32 checksum = 0;

	//***************************************************************************
	// Write the material frequency response band information header.

	FrequencyResponse frequencyResponse;

	// Write the number of frequency bands.
	Size numFrequencyBands = frequencyResponse.getNumberOfBands();

	if ( !writeUInt32( file, UInt32(numFrequencyBands) ) )
		return false;

	checksum += sumBytes( &numFrequencyBands, sizeof(UInt32) );

	// For each frequency band, write the center frequency.
	for ( Index i = 0; i < numFrequencyBands; i++ )
	{
		Float bandCenter = frequencyResponse.getBandCenterFrequency(i);

		if ( !writeFloat( file, bandCenter ) )
			return false;

		checksum += sumBytes( &bandCenter, sizeof(Float) );
	}


	//***************************************************************************
	// Write the materials in the mesh.

	// Write the number of materials in the mesh.
	Size numMaterials = mesh.getNumberOfMaterials();

	if ( !writeUInt32( file, UInt32(numMaterials) ) )
		return false;

	checksum += sumBytes( &numMaterials, sizeof(UInt32) );

	// Make sure that the temporary buffer is big enough to hold a material object.
	Size materialSizeInBytes = numFrequencyBands*sizeof(Float)*3;

	UByte* materialBuffer = enlargeBufferTo( materialSizeInBytes );

	// Write each material.
	for ( Index i = 0; i < numMaterials; i++ )
	{
		const SoundMaterial& material = mesh.getMaterial(i);
		UByte* currentPosition = materialBuffer;

		// Write the reflection attenuation for the material.
		const FrequencyResponse& reflectionAttenuation = material.getReflectionAttenuation();

		for ( Index j = 0; j < numFrequencyBands; j++ )
		{
			*((Float*)currentPosition) = reflectionAttenuation[j];
			currentPosition += sizeof(Float);
		}

		// Write the absorption attenuation for the material.
		const FrequencyResponse& absorptionAttenuation = material.getAbsorptionAttenuation();

		for ( Index j = 0; j < numFrequencyBands; j++ )
		{
			*((Float*)currentPosition) = absorptionAttenuation[j];
			currentPosition += sizeof(Float);
		}


		// Write the transmission attenuation for the material.
		const FrequencyResponse& transmissionAttenuation = material.getTransmissionAttenuation();

		for ( Index j = 0; j < numFrequencyBands; j++ )
		{
			*((Float*)currentPosition) = transmissionAttenuation[j];
			currentPosition += sizeof(Float);
		}

		// Write the whole material data buffer in one pass.
		if ( std::fwrite( materialBuffer, sizeof(UByte), materialSizeInBytes, file ) != materialSizeInBytes )
			return false;

		checksum += sumBytes( materialBuffer, materialSizeInBytes );
	}


	//***************************************************************************
	// Write the vertices in the mesh.

	// Write the number of vertices in the mesh.
	Size numVertices = mesh.getNumberOfVertices();

	if ( !writeUInt32( file, UInt32(numVertices) ) )
		return false;

	checksum += sumBytes( &numVertices, sizeof(UInt32) );

	// Make sure that the temporary buffer is big enough to hold a vertex object.
	Size vertexSizeInBytes = sizeof(Float)*3;

	UByte* vertexBuffer = enlargeBufferTo( vertexSizeInBytes );

	// Write each vertex in the mesh.
	for ( Index i = 0; i < numVertices; i++ )
	{
		const SoundVertex& vertex = mesh.getVertex(i);
		UByte* currentPosition = vertexBuffer;

		// Write the X coordinate
		*((Float*)currentPosition) = vertex.x;
		currentPosition += sizeof(Float);

		// Write the Y coordinate
		*((Float*)currentPosition) = vertex.y;
		currentPosition += sizeof(Float);

		// Write the Z coordinate
		*((Float*)currentPosition) = vertex.z;
		currentPosition += sizeof(Float);

		// Write the whole vertex data buffer in one pass.
		if ( std::fwrite( vertexBuffer, sizeof(UByte), vertexSizeInBytes, file ) != vertexSizeInBytes )
			return false;

		checksum += sumBytes( vertexBuffer, vertexSizeInBytes );
	}

	//***************************************************************************
	// Write the triangles in the mesh.

	// Write the number of triangles in the mesh.
	Size numTriangles = mesh.getNumberOfTriangles();

	if ( !writeUInt32( file, UInt32(numTriangles) ) )
		return false;

	checksum += sumBytes( &numTriangles, sizeof(UInt32) );

	// Make sure that the temporary buffer is big enough to hold a triangle object.
	Size triangleSizeInBytes = 7*sizeof(UInt32) + 4*sizeof(UByte);

	UByte* triangleBuffer = enlargeBufferTo( triangleSizeInBytes );

	// Get some pointers that serve as the base pointers for the vertex and material arrays
	// so that we can convert to an indexed format for serialization.
	const SoundMaterial* const materialBasePointer = mesh.materials.getArrayPointer();
	const SoundVertex* const vertexBasePointer = mesh.vertices.getArrayPointer();
	const internal::InternalSoundTriangle* const triangleBasePointer = mesh.triangles.getArrayPointer();

	// Write each triangle in the mesh.
	for ( Index i = 0; i < numTriangles; i++ )
	{
		const internal::InternalSoundTriangle& triangle = triangleBasePointer[i];
		UByte* currentPosition = triangleBuffer;

		// Write the vertex indices of the triangle.
		for ( Index j = 0; j < 3; j++ )
		{
			*((UInt32*)currentPosition) = UInt32(&triangle.getVertex(j) - vertexBasePointer);
			currentPosition += sizeof(UInt32);
		}

		// Write the neighbor indices of the triangle.
		for ( Index j = 0; j < 3; j++ )
		{
			if ( triangle.getNeighbor(j) != NULL )
				*((UInt32*)currentPosition) = UInt32(triangle.getNeighbor(j) - triangleBasePointer);
			else
				*((UInt32*)currentPosition) = UInt32(i);

			currentPosition += sizeof(UInt32);
		}

		// Write the edge diffraction types of the triangle.
		for ( Index j = 0; j < 3; j++ )
		{
			if ( triangle.getEdgeType(j) == internal::InternalSoundTriangle::DIFFRACTING )
				*currentPosition = UByte(1);
			else
				*currentPosition = UByte(0);

			currentPosition += sizeof(UByte);
		}

		// Skip one byte for padding
		currentPosition += sizeof(UByte);

		// Write the material index for the triangle.
		*((UInt32*)currentPosition) = UInt32(&triangle.getMaterial() - materialBasePointer);

		// Write the whole triangle data buffer in one pass.
		if ( std::fwrite( triangleBuffer, sizeof(UByte), triangleSizeInBytes, file ) != triangleSizeInBytes )
			return false;

		checksum += sumBytes( triangleBuffer, triangleSizeInBytes );
	}


	//***************************************************************************
	// Write the bounding sphere for the mesh.

	const internal::BoundingSphere& boundingSphere = mesh.boundingSphere;

	// Write the center of the bounding sphere.
	if ( !writeFloat( file, boundingSphere.position.x ) )
		return false;

	checksum += sumBytes( &boundingSphere.position.x, sizeof(Float) );

	if ( !writeFloat( file, boundingSphere.position.y ) )
		return false;

	checksum += sumBytes( &boundingSphere.position.y, sizeof(Float) );

	if ( !writeFloat( file, boundingSphere.position.z ) )
		return false;

	checksum += sumBytes( &boundingSphere.position.z, sizeof(Float) );

	// Write the radius of the bounding sphere.
	if ( !writeFloat( file, boundingSphere.radius ) )
		return false;

	checksum += sumBytes( &boundingSphere.radius, sizeof(Float) );


	//***************************************************************************
	// Write the checksum for the mesh.

	if ( std::fseek( file, 12, SEEK_SET ) != 0 )
		return false;

	if ( !writeUInt32( file, checksum ) )
		return false;

	return true;
}




//##########################################################################################
//##########################################################################################
//############
//############		Version 1 Deserialization Method
//############
//##########################################################################################
//##########################################################################################




SoundMesh* SoundMeshSerializer:: deserializeVersion1( std::FILE* file )
{
	//***************************************************************************
	// Read the rest of the header for the mesh.

	UByte header[6];

	if ( std::fread( header, sizeof(UByte), 6, file ) != 6 )
		return NULL;

	// Read the endianness of the file.
	Bool isBigEndian = header[0] == 0 ? false : true;

	// Read the checksum for the file.
	UInt32 checksum = readUInt32( header + 2, isBigEndian );


	//***************************************************************************
	// Read the material frequency response band information header.

	// Read the number of frequency bands per material frequency response.
	UInt32 numFrequencyBands;

	if ( std::fread( &numFrequencyBands, sizeof(UInt32), 1, file ) != 1 )
		return NULL;

	numFrequencyBands = swapEndianness( numFrequencyBands, isBigEndian );


	// Read the frequency band centers for a material frequency response.
	ArrayList<Float> frequencyBandCenters( numFrequencyBands );

	// Make sure that the temporary buffer is big enough to hold the frequency band centers.
	Size frequencyBandCenterBufferSize = numFrequencyBands*sizeof(Float);
	UByte* frequencyBandCenterBuffer = enlargeBufferTo( frequencyBandCenterBufferSize );

	if ( std::fread( frequencyBandCenterBuffer, frequencyBandCenterBufferSize, 1, file ) != 1 )
		return NULL;

	for ( Index i = 0; i < numFrequencyBands; i++ )
	{
		frequencyBandCenters.add( readFloat( frequencyBandCenterBuffer, isBigEndian ) );
		frequencyBandCenterBuffer += sizeof(Float);
	}


	//***************************************************************************
	// Create the SoundMesh object which will hold the mesh data.

	SoundMesh* mesh = util::allocate<SoundMesh>();
	new (mesh) SoundMesh();


	//***************************************************************************
	// Read the materials in the mesh.

	// Read the number of materials in the mesh.
	UInt32 numMaterials;

	if ( std::fread( &numMaterials, sizeof(UInt32), 1, file ) != 1 )
	{
		util::destruct( mesh );
		return NULL;
	}

	numMaterials = swapEndianness( numMaterials, isBigEndian );

	// Enlarge the capacity of the mesh's material list to avoid having to resize it multiple times.
	mesh->materials.setCapacity( numMaterials );

	// Make sure that the temporary buffer is big enough to hold a material object.
	Size materialSizeInBytes = numFrequencyBands*sizeof(Float)*3;

	UByte* materialBuffer = enlargeBufferTo( materialSizeInBytes );

	ArrayList<Float> materialFrequencyResponse( numFrequencyBands );

	// Read each material in the mesh.
	for ( Index i = 0; i < numMaterials; i++ )
	{
		UByte* temporaryMaterialBuffer = materialBuffer;

		if ( std::fread( temporaryMaterialBuffer, materialSizeInBytes, 1, file ) != 1 )
		{
			util::destruct( mesh );
			return NULL;
		}

		// Read the reflection attenuation of the material.
		for ( Index j = 0; j < numFrequencyBands; j++ )
		{
			materialFrequencyResponse.add( readFloat( temporaryMaterialBuffer, isBigEndian ) );
			temporaryMaterialBuffer += sizeof(Float);
		}

		FrequencyResponse reflectionAttenuation = approximateFrequencyResponse( frequencyBandCenters,
																				materialFrequencyResponse );

		materialFrequencyResponse.clear();

		// Read the absorption attenuation of the material.
		for ( Index j = 0; j < numFrequencyBands; j++ )
		{
			materialFrequencyResponse.add( readFloat( temporaryMaterialBuffer, isBigEndian ) );
			temporaryMaterialBuffer += sizeof(Float);
		}

		FrequencyResponse absorptionAttenuation = approximateFrequencyResponse( frequencyBandCenters,
																				materialFrequencyResponse );

		materialFrequencyResponse.clear();

		// Read the transmission attenuation of the material.
		for ( Index j = 0; j < numFrequencyBands; j++ )
		{
			materialFrequencyResponse.add( readFloat( temporaryMaterialBuffer, isBigEndian ) );
			temporaryMaterialBuffer += sizeof(Float);
		}

		FrequencyResponse transmissionAttenuation = approximateFrequencyResponse( frequencyBandCenters,
																				materialFrequencyResponse );

		materialFrequencyResponse.clear();

		// Add the material to the mesh.
		mesh->materials.add( SoundMaterial( reflectionAttenuation, absorptionAttenuation, transmissionAttenuation ) );
	}


	//***************************************************************************
	// Read the vertices in the mesh.

	// Read the number of vertices in the mesh.
	UInt32 numVertices;
	
	if ( std::fread( &numVertices, sizeof(UInt32), 1, file ) != 1 )
	{
		util::destruct( mesh );
		return NULL;
	}

	numVertices = swapEndianness( numVertices, isBigEndian );

	// Enlarge the capacity of the mesh's vertex list to avoid having to resize it multiple times.
	mesh->vertices.setCapacity( numVertices );

	// Make sure that the temporary buffer is big enough to hold a vertex object.
	Size vertexSizeInBytes = sizeof(Float)*3;

	UByte* vertexBuffer = enlargeBufferTo( vertexSizeInBytes );

	// Read each vertex in the mesh.
	for ( Index i = 0; i < numVertices; i++ )
	{
		UByte* temporaryVertexBuffer = vertexBuffer;

		if ( std::fread( temporaryVertexBuffer, vertexSizeInBytes, 1, file ) != 1 )
		{
			util::destruct( mesh );
			return NULL;
		}

		// Read the X, Y, and Z coordinates.
		Float x = readFloat( temporaryVertexBuffer, isBigEndian );
		temporaryVertexBuffer += sizeof(Float);

		Float y = readFloat( temporaryVertexBuffer, isBigEndian );
		temporaryVertexBuffer += sizeof(Float);

		Float z = readFloat( temporaryVertexBuffer, isBigEndian );
		temporaryVertexBuffer += sizeof(Float);

		// Add the vertex to the mesh,
		mesh->vertices.add( SoundVertex( x, y, z ) );
	}


	//***************************************************************************
	// Read the triangles in the mesh.

	// Read the number of triangles in the mesh.
	UInt32 numTriangles;

	if ( std::fread( &numTriangles, sizeof(UInt32), 1, file ) != 1 )
	{
		util::destruct( mesh );
		return NULL;
	}

	numTriangles = swapEndianness( numTriangles, isBigEndian );

	// Enlarge the capacity of the mesh's triangle list to avoid having to resize it multiple times.
	mesh->triangles.setCapacity( numTriangles );

	// Make sure that the temporary buffer is big enough to hold a triangle object.
	Size triangleSizeInBytes = 7*sizeof(UInt32) + 4*sizeof(UByte);

	UByte* triangleBuffer = enlargeBufferTo( triangleSizeInBytes );

	// Get pointers to the beginning of the material and vertex arrays so that we can
	// convert from an indexed format to a pointer-based format.
	const SoundVertex* const vertexBasePointer = mesh->vertices.getArrayPointer();
	const internal::InternalSoundTriangle* const triangleBasePointer = mesh->triangles.getArrayPointer();
	const SoundMaterial* const materialBasePointer = mesh->materials.getArrayPointer();

	// Read each triangle in the mesh.
	for ( Index i = 0; i < numTriangles; i++ )
	{
		UByte* temporaryTriangleBuffer = triangleBuffer;

		if ( std::fread( temporaryTriangleBuffer, triangleSizeInBytes, 1, file ) != 1 )
		{
			util::destruct( mesh );
			return NULL;
		}

		const SoundVertex* vertices[3];

		// Read the triangle's vertex indices.
		for ( Index j = 0; j < 3; j++ )
		{
			vertices[j] = vertexBasePointer + readUInt32( temporaryTriangleBuffer, isBigEndian );
			temporaryTriangleBuffer += sizeof(UInt32);
		}

		// Create a preprocessed sound triangle object for the triangle with no material for now.
		internal::InternalSoundTriangle triangle( vertices[0], vertices[1], vertices[2], NULL );

		// Read the triangle's neighbor indices.
		for ( Index j = 0; j < 3; j++ )
		{
			triangle.setNeighbor( j, triangleBasePointer + readUInt32( temporaryTriangleBuffer, isBigEndian ) );
			temporaryTriangleBuffer += sizeof(UInt32);
		}

		// Read the triangle's edge diffraction types.
		for ( Index j = 0; j < 3; j++ )
		{
			UByte diffractionType = *temporaryTriangleBuffer;

			if ( diffractionType == 1 )
				triangle.setEdgeType( j, internal::InternalSoundTriangle::DIFFRACTING );
			else
				triangle.setEdgeType( j, internal::InternalSoundTriangle::NON_DIFFRACTING );

			temporaryTriangleBuffer += sizeof(UByte);
		}

		// Skip the padding byte.
		temporaryTriangleBuffer += sizeof(UByte);

		// Read the triangle's material index.
		triangle.setMaterial( materialBasePointer + readUInt32( temporaryTriangleBuffer, isBigEndian ) );

		// Add the triangle to the mesh.
		mesh->triangles.add( triangle );
	}


	//***************************************************************************
	// Read the bounding sphere for the mesh.

	// Make sure that the temporary buffer is large enough to hold a bounding sphere object.
	Size boundingSphereSize = 4*sizeof(Float);

	UByte* boundingSphereBuffer = enlargeBufferTo( boundingSphereSize );

	if ( std::fread( boundingSphereBuffer, boundingSphereSize, 1, file ) != 1 )
	{
		util::destruct( mesh );
		return NULL;
	}

	// Read the X, Y, and Z coordinates of the bounding sphere's center.
	mesh->boundingSphere.position.x = readFloat( boundingSphereBuffer, isBigEndian );
	boundingSphereBuffer += sizeof(Float);

	mesh->boundingSphere.position.y = readFloat( boundingSphereBuffer, isBigEndian );
	boundingSphereBuffer += sizeof(Float);

	mesh->boundingSphere.position.z = readFloat( boundingSphereBuffer, isBigEndian );
	boundingSphereBuffer += sizeof(Float);

	// Read the radius of the bounding sphere.
	mesh->boundingSphere.radius = readFloat( boundingSphereBuffer, isBigEndian );


	//***************************************************************************
	// Construct the BVH for the mesh.
	
	mesh->bvh = util::construct<internal::QBVHArrayTree>( mesh->triangles, SoundMesh::NUMBER_OF_SPLIT_PLANE_CANDIDATES,
															SoundMesh::MAX_NUMBER_OF_TRIANGLES_PER_LEAF );
	
	return mesh;
}




//##########################################################################################
//##########################################################################################
//############
//############		Private Static File Writing Helper Methods
//############
//##########################################################################################
//##########################################################################################




Bool SoundMeshSerializer:: writeByte( std::FILE* file, Byte byte )
{
	return std::fwrite( &byte, sizeof(Byte), 1, file ) == 1;
}




Bool SoundMeshSerializer:: writeUInt32( std::FILE* file, UInt32 unsignedInteger )
{
	return std::fwrite( &unsignedInteger, sizeof(UInt32), 1, file ) == 1;
}




Bool SoundMeshSerializer:: writeFloat( std::FILE* file, Float floatingPointNumber )
{
	return std::fwrite( &floatingPointNumber, sizeof(Float), 1, file ) == 1;
}




//##########################################################################################
//##########################################################################################
//############
//############		Private Helper Methods
//############
//##########################################################################################
//##########################################################################################




UByte* SoundMeshSerializer:: enlargeBufferTo( Size sizeInBytes )
{
	if ( bufferSize < sizeInBytes )
	{
		if ( buffer != NULL )
			util::deallocate( buffer );

		buffer = util::allocate<UByte>( sizeInBytes );
		bufferSize = sizeInBytes;
	}

	return buffer;
}




UInt32 SoundMeshSerializer:: sumBytes( const void* data, Size number )
{
	UInt32 sum = 0;
	const UByte* bytes = (const UByte*)data;
	const UByte* const bytesEnd = bytes + number;

	while ( bytes != bytesEnd )
	{
		sum += UInt32(*bytes);
		bytes++;
	}

	return sum;
}




UInt32 SoundMeshSerializer:: swapEndianness( UInt32 value )
{
	return (value << 24) | ((value << 8) & 0x00FF0000) |
			((value >> 8) & 0x0000FF00) | (value >> 24);
}




UInt32 SoundMeshSerializer:: swapEndianness( UInt32 value, Bool isBigEndian )
{
	#if defined(GSOUND_BIG_ENDIAN)
		if ( isBigEndian )
			return value;
		else
			return swapEndianness(value);
	#else
		if ( isBigEndian )
			return swapEndianness(value);
		else
			return value;
	#endif
}




UInt32 SoundMeshSerializer:: readUInt32( const UByte* data, Bool isBigEndian )
{
	#if defined(GSOUND_BIG_ENDIAN)
		if ( isBigEndian )
			return *((UInt32*)data);
		else
			return swapEndianness( *((UInt32*)data) );
	#else
		if ( isBigEndian )
			return swapEndianness( *((UInt32*)data) );
		else
			return *((UInt32*)data);
	#endif
}




Float SoundMeshSerializer:: readFloat( const UByte* data, Bool isBigEndian )
{
	#if defined(GSOUND_BIG_ENDIAN)
		if ( isBigEndian )
			return *((Float*)data);
		else
		{
			UInt32 integer = swapEndianness( *((UInt32*)data) );
			return *((Float*)&integer);
		}
	#else
		if ( isBigEndian )
		{
			UInt32 integer = swapEndianness( *((UInt32*)data) );
			return *((Float*)&integer);
		}
		else
			return *((Float*)data);
	#endif
}




FrequencyResponse SoundMeshSerializer:: approximateFrequencyResponse( const ArrayList<Float>& frequencyBandCenters,
																	const ArrayList<Float>& gainCoefficients )
{
	GSOUND_DEBUG_ASSERT( gainCoefficients.getSize() == frequencyBandCenters.getSize() );
	GSOUND_DEBUG_ASSERT( frequencyBandCenters.getSize() != 0 );

	const Size numInputFrequencyBandsMinusOne = frequencyBandCenters.getSize() - 1;

	FrequencyResponse output;
	const Size numOutputFrequencyBands = output.getNumberOfBands();

	for ( Index i = 0; i < numOutputFrequencyBands; i++ )
	{
		Float bandCenter = output.getBandCenterFrequency(i);

		for ( Index j = 0; j < numInputFrequencyBandsMinusOne; j++ )
		{
			if ( j == 0 && bandCenter < frequencyBandCenters[0] )
			{
				output[i] = gainCoefficients[0];
				break;
			}
			else if ( j == numInputFrequencyBandsMinusOne &&
						bandCenter > frequencyBandCenters[numInputFrequencyBandsMinusOne] )
			{
				output[i] = gainCoefficients[numInputFrequencyBandsMinusOne];
				break;
			}
			else if ( bandCenter >= frequencyBandCenters[j] && bandCenter <= frequencyBandCenters[j + 1] )
			{
				Float a = (bandCenter - frequencyBandCenters[j])/
											(frequencyBandCenters[j + 1] - frequencyBandCenters[j]);
				output[i] = a*gainCoefficients[j + 1] + (1.0f - a)*gainCoefficients[j];
				break;
			}
		}
	}

	return output;
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
