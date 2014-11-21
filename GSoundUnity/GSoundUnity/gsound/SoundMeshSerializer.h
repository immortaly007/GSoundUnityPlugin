/*
 * Project:     GSound
 * 
 * File:        gsound/SoundMeshSerializer.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundMeshSerializer class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_MESH_SERIALIZER_H
#define INCLUDE_GSOUND_SOUND_MESH_SERIALIZER_H


#include "GSoundBase.h"


#include "SoundMesh.h"
#include <cstdio>


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class that loads and saves preprocessed SoundMesh objects to/from mass storage.
/**
  * Using the SoundMeshSerializer class, the user can reduce loading times at runtime by 
  * preprocessing SoundMesh objects and then saving them to mass storage. Thus, when
  * loading a SoundMesh object, minimal preprocessing is necessary before it is ready
  * to use, dramatically reducing load times for large meshes.
  *
  * The SoundMeshSerializer uses a versioned proprietary format for the serialized SoundMesh
  * data. Typically, a serialized SoundMesh contains geometry, connectivity information,
  * diffraction edge type information, materials, and a bounding volume hierarchy for the mesh.
  */
class SoundMeshSerializer
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Type Definitions
			
			
			
			
			/// Define the type to use to represent the version number for a sound mesh file format.
			typedef Index SoundMeshVersion;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			/// Create a new SoundMeshSerializer object.
			GSOUND_INLINE SoundMeshSerializer()
				:	buffer( NULL ),
					bufferSize( 0 )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Mesh Serialization/Deserialization Methods
			
			
			
			
			/// Serialize the specified mesh and write the data to the file with the specified name.
			/**
			  * The method returns whether or not the serialization operation was successful.
			  * The operation can fail for any of the following reasons:
			  * - The file name specifies an invalid path.
			  * - The serializer has insufficient permissions to write the file at the specified path.
			  * - The serializer doesn't have permission to overwrite a preexisting file.
			  *
			  * @param fileName - the path where the serialized mesh file should be written.
			  * @param mesh - the SoundMesh to serialize.
			  * @param overwrite - whether or not the serializer should overwrite a preexisting file.
			  * @return whether or not the serialization operation was successful.
			  */
			Bool serialize( const String& fileName, const SoundMesh& mesh, Bool overwrite );
			
			
			
			
			/// Serialize the specified mesh and write the data to the file with the specified name.
			/**
			  * This variant of the method allows the user to specify a perfered mesh format version
			  * number for the serialized mesh. The method returns whether or not the serialization
			  * operation was successful.
			  * The operation can fail for any of the following reasons:
			  * - The file name specifies an invalid path.
			  * - The serializer has insufficient permissions to write the file at the specified path.
			  * - The serializer doesn't have permission to overwrite a preexisting file.
			  * - The serializer doesn't support the specified format version.
			  * - The file has been corrupted
			  *
			  * @param fileName - the path where the serialized mesh file should be written.
			  * @param mesh - the SoundMesh to serialize.
			  * @param overwrite - whether or not the serializer should overwrite a preexisting file.
			  * @param preferedVersion - the mesh format version to use when serializing the mesh.
			  * @return whether or not the serialization operation was successful.
			  */
			Bool serialize( const String& fileName, const SoundMesh& mesh, Bool overwrite,
							SoundMeshVersion preferedVersion );
			
			
			
			
			/// Deserialize the mesh with the specified file name and return a pointer to the mesh.
			/**
			  * If the deserialization operation fails, NULL is returned.
			  * This can happen for any of the following reasons:
			  * - The file name specifies an invalid path or file that does not exist.
			  * - The specified file is not a serialized sound mesh or has an unsupported version number.
			  *
			  * @param fileName - the path to the serialized mesh file which is to be deserialized.
			  * @return a pointer to the successfully deserialized SoundMesh or NULL if the operation failed.
			  */
			SoundMesh* deserialize( const String& fileName );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mesh File Property Determination Methods
			
			
			
			
			/// Return whether or not the specified file exists.
			/**
			  * If the file exists, TRUE is returned. Otherwise, FALSE is returned.
			  * 
			  * @param fileName - the path to the file to test for existence.
			  * @return whether or not the specified file exists.
			  */
			Bool fileExists( const String& fileName );
			
			
			
			
			/// Return whether or not the specified file is a supported mesh format.
			/**
			  * If the file exists and is a supported mesh format, TRUE is returned.
			  * Otherwise FALSE is returned.
			  * 
			  * @param fileName - the path to the file to test for existence.
			  * @return whether or not the specified file is a supported mesh format.
			  */
			Bool fileIsSupportedMeshFormat( const String& fileName );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mesh File Format Version Accessor Methods
			
			
			
			
			/// Get the earliest format version supported by this SoundMeshSerializer.
			GSOUND_INLINE SoundMeshVersion getMinimumSupportedVersion() const
			{
				return minimumSupportedVersion;
			}
			
			
			
			
			/// Get the latest format version supported by this SoundMeshSerializer.
			GSOUND_INLINE SoundMeshVersion getMaximumSupportedVersion() const
			{
				return maximumSupportedVersion;
			}
			
			
			
			
			/// Return whether or not the specified sound mesh format version is supported by this serializer.
			GSOUND_INLINE Bool versionIsSupported( SoundMeshVersion version ) const
			{
				return version >= minimumSupportedVersion && version <= maximumSupportedVersion;
			}
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Mesh Serialization Methods
			
			
			
			
			/// Serialize a mesh to the specified file handle pointer in file format version 1.
			Bool serializeVersion1( std::FILE* file, const SoundMesh& mesh );
			
			
			
			
			/// Read a mesh from the specified file handle pointer that is in file format version 1.
			SoundMesh* deserializeVersion1( std::FILE* file );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static File Writing Helper Methods
			
			
			
			
			/// Write a byte to the specified file handle pointer.
			GSOUND_INLINE static Bool writeByte( std::FILE* file, Byte byte );
			
			
			
			
			/// Write a 32-bit integer to the specified file handle pointer in the native platform endianness.
			GSOUND_INLINE static Bool writeUInt32( std::FILE* file, UInt32 unsignedInteger );
			
			
			
			
			/// Write a 32-bit floating-point number to the specified file handle pointer in the native platform endianness.
			GSOUND_INLINE static Bool writeFloat( std::FILE* file, Float floatingPointNumber );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Helper Methods
			
			
			
			
			/// Ensure that the internal read input buffer for the serializer is at least as certain size.
			GSOUND_INLINE UByte* enlargeBufferTo( Size sizeInBytes );
			
			
			
			
			/// Compute the sum of the specified number of bytes at the specified location.
			GSOUND_INLINE static UInt32 sumBytes( const void* data, Size number );
			
			
			
			
			/// Swap the endianness of the specified 32-bit integer value and return the result.
			GSOUND_INLINE static UInt32 swapEndianness( UInt32 value );
			
			
			
			
			/// Swap the endianness of the specified 32-bit integer value to the native platform endianness.
			/**
			  * @param isBigEndian - whether or not the original value was in big-endian format.
			  */
			GSOUND_INLINE static UInt32 swapEndianness( UInt32 value, Bool isBigEndian );
			
			
			
			
			/// Read a 32-bit integer value from a buffer of bytes and convert it to the native platform endianness.
			/**
			  * @param isBigEndian - whether or not the original value was stored in big-endian format.
			  */
			GSOUND_INLINE static UInt32 readUInt32( const UByte* data, Bool isBigEndian );
			
			
			
			
			/// Read a 32-bit floating-point value from a buffer of bytes and convert it to the native platform endianness.
			/**
			  * @param isBigEndian - whether or not the original value was stored in big-endian format.
			  */
			GSOUND_INLINE static Float readFloat( const UByte* data, Bool isBigEndian );
			
			
			
			
			static FrequencyResponse approximateFrequencyResponse( const ArrayList<Float>& frequencyBandCenters,
																	const ArrayList<Float>& gainCoefficients );
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Data Members
			
			
			
			
			/// The earliest format version supported by this SoundMeshSerializer.
			static const SoundMeshVersion minimumSupportedVersion;
			
			
			
			
			/// The latest format version supported by this SoundMeshSerializer.
			static const SoundMeshVersion maximumSupportedVersion;
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The size of the internal read buffer.
			Size bufferSize;
			
			
			
			
			/// A pointer to the internal read buffer.
			UByte* buffer;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_MESH_SERIALIZER_H
