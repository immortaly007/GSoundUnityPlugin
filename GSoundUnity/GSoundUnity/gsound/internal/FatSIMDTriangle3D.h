/*
 * Project:     GSound
 * 
 * File:        gsound/internal/FatSIMDTriangle3D.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::FatSIMDTriangle3D class declaration
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


#ifndef INCLUDE_GSOUND_FAT_SIMD_TRIANGLE_3D_H
#define INCLUDE_GSOUND_FAT_SIMD_TRIANGLE_3D_H


#include "GSoundInternalConfig.h"


#include "InternalSoundTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// A prototype for the FatSIMDTriangle3D class which allows different SIMD implementations via template class specialization.
template < typename T, Size dimension >
class FatSIMDTriangle3D;




/// A class which is used as an extension to the SIMDTriangle3D class to encode extra information.
/**
  * The additional information about the triangle(s) that is stored is the pointer to the
  * InternalSoundTriangle for each of the triangles.
  */
template < typename T >
class GSOUND_ALIGN(16) FatSIMDTriangle3D<T,4> : public SIMDTriangle3D<T,4>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Type Definitions
			
			
			
			
			/// The type of triangle to use for the triangles that this class references.
			typedef InternalSoundTriangle TriangleType;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a SIMD triangle with N copies of the specified triangle for a SIMD width of N.
			GSOUND_INLINE FatSIMDTriangle3D( const TriangleType* triangle )
				:	SIMDTriangle3D<T,4>( triangle->getVertex(0), triangle->getVertex(1), triangle->getVertex(2) )
			{
				trianglePointers[0] = trianglePointers[1] = trianglePointers[2] = trianglePointers[3] = triangle;
			}
			
			
			
			
			/// Create a SIMD ray with the 4 rays it contains equal to the specified rays.
			GSOUND_INLINE FatSIMDTriangle3D( const TriangleType* t0, const TriangleType* t1,
											const TriangleType* t2, const TriangleType* t3 )
				:	SIMDTriangle3D<T,4>( SIMDVector3D<T,4>( t0->getVertex(0), t1->getVertex(0), t2->getVertex(0), t3->getVertex(0) ),
										SIMDVector3D<T,4>( t0->getVertex(1), t1->getVertex(1), t2->getVertex(1), t3->getVertex(1) ),
										SIMDVector3D<T,4>( t0->getVertex(2), t1->getVertex(2), t2->getVertex(2), t3->getVertex(2) ) )
			{
				trianglePointers[0] = t0;
				trianglePointers[1] = t1;
				trianglePointers[2] = t2;
				trianglePointers[3] = t3;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// Get the pointer to the triangle that is stored here at the specified SIMD index.
			GSOUND_INLINE const TriangleType* getTrianglePointer( Index simdIndex ) const
			{
				return trianglePointers[simdIndex];
			}
			
			
			
			
	private:
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The pointers to the triangles that this SIMD triangle contains.
			const TriangleType* trianglePointers[4];
			
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_FAT_SIMD_TRIANGLE_3D_H
