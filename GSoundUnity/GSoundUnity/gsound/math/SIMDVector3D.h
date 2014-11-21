/*
 * Project:     GSound
 * 
 * File:        gsound/math/SIMDVector3D.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::SIMDVector3D class declaration
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


#ifndef INCLUDE_GSOUND_SIMD_VECTOR_3D_H
#define INCLUDE_GSOUND_SIMD_VECTOR_3D_H


#include "GSoundMathConfig.h"


#include "Vector3D.h"
#include "SIMDScalar.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A template prototype declaration for the SIMDVector3D class.
/**
  * This class is used to store and operate on a set of N 3D vectors
  * in a SIMD fashion. The vectors are stored in a structure-of-arrays format
  * that accelerates SIMD operations.
  */
template < typename T, Size dimension >
class SIMDVector3D;




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A specialization for the SIMDAABB3D class that has a SIMD width of 4.
/**
  * This class is used to store and operate on a set of 4 3D vectors
  * in a SIMD fashion. The vectors are stored in a structure-of-arrays format
  * that accelerates SIMD operations.
  */
template < typename T >
class GSOUND_ALIGN(16) SIMDVector3D<T,4>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a quad 3D SIMD vector with all vector components equal to zero.
			GSOUND_FORCE_INLINE SIMDVector3D()
				:	x(),
					y(),
					z()
			{
			}
			
			
			
			
			/// Create a quad 3D SIMD vector with all of the four vectors equal to the specified vector.
			GSOUND_FORCE_INLINE SIMDVector3D( const Vector3D<T>& vector )
				:	x( vector.x ),
					y( vector.y ),
					z( vector.z )
			{
			}
			
			
			
			
			/// Create a quad 3D SIMD vector with each of the four vectors equal to the specified vector.
			GSOUND_FORCE_INLINE SIMDVector3D( const Vector3D<T>& v1, const Vector3D<T>& v2,
										const Vector3D<T>& v3, const Vector3D<T>& v4 )
#if !GSOUND_USE_SIMD || GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC) || defined(GSOUND_SIMD_SSE) && !GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				:	x( v1.x, v2.x, v3.x, v4.x ),
					y( v1.y, v2.y, v3.y, v4.y ),
					z( v1.z, v2.z, v3.z, v4.z )
#endif
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				
				__m128 xy10 = _mm_setzero_ps();
				__m128 zw10 = _mm_setzero_ps();
				__m128 xy32 = _mm_setzero_ps();
				__m128 zw32 = _mm_setzero_ps();
				
				xy10 = _mm_loadl_pi( xy10, (__m64*)&v1.x );
				zw10 = _mm_loadl_pi( zw10, (__m64*)&v1.z );
				xy10 = _mm_loadh_pi( xy10, (__m64*)&v2.x );
				zw10 = _mm_loadh_pi( zw10, (__m64*)&v2.z );
				
				xy32 = _mm_loadl_pi( xy32, (__m64*)&v3.x );
				zw32 = _mm_loadl_pi( zw32, (__m64*)&v3.z );
				xy32 = _mm_loadh_pi( xy32, (__m64*)&v4.x );
				zw32 = _mm_loadh_pi( zw32, (__m64*)&v4.z );
				
				x.v = _mm_shuffle_ps( xy10, xy32, _MM_SHUFFLE(2,0,2,0) );
				y.v = _mm_shuffle_ps( xy10, xy32, _MM_SHUFFLE(3,1,3,1) );
				z.v = _mm_shuffle_ps( zw10, zw32, _MM_SHUFFLE(2,0,2,0) );
#endif
			}
			
			
			
			
			/// Create a quad 3D SIMD vector with the specified X, Y, and Z SIMDScalars.
			GSOUND_FORCE_INLINE SIMDVector3D( const SIMDScalar<T,4>& newX, const SIMDScalar<T,4>& newY, const SIMDScalar<T,4>& newZ )
				:	x( newX ),
					y( newY ),
					z( newZ )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			
			
			/// Compute and return the component-wise sum of this quad SIMD 3D vector with another.
			GSOUND_FORCE_INLINE SIMDVector3D operator + ( const SIMDVector3D& other ) const
			{
				return SIMDVector3D( x + other.x, y + other.y, z + other.z );
			}
			
			
			
			
			/// Compute and return the component-wise sum of this quad SIMD 3D vector with a quad SIMD scalar.
			GSOUND_FORCE_INLINE SIMDVector3D operator + ( const SIMDScalar<T,4>& quadScalar ) const
			{
				return SIMDVector3D( x + quadScalar, y + quadScalar, z + quadScalar );
			}
			
			
			
			
			/// Compute and return the component-wise difference of this quad SIMD 3D vector with another.
			GSOUND_FORCE_INLINE SIMDVector3D operator - ( const SIMDVector3D& other ) const
			{
				return SIMDVector3D( x - other.x, y - other.y, z - other.z );
			}
			
			
			
			
			/// Compute and return the component-wise difference of this quad SIMD 3D vector with a quad SIMD scalar.
			GSOUND_FORCE_INLINE SIMDVector3D operator - ( const SIMDScalar<T,4>& quadScalar ) const
			{
				return SIMDVector3D( x - quadScalar, y - quadScalar, z - quadScalar );
			}
			
			
			
			
			/// Compute and return the component-wise multiplication of this quad SIMD 3D vector with another.
			GSOUND_FORCE_INLINE SIMDVector3D operator * ( const SIMDVector3D& other ) const
			{
				return SIMDVector3D( x*other.x, y*other.y, z*other.z );
			}
			
			
			
			
			/// Compute and return the component-wise multiplication of this quad SIMD 3D vector with a quad SIMD scalar.
			GSOUND_FORCE_INLINE SIMDVector3D operator * ( const SIMDScalar<T,4>& quadScalar ) const
			{
				return SIMDVector3D( x*quadScalar, y*quadScalar, z*quadScalar );
			}
			
			
			
			
			/// Compute and return the component-wise quotient of this quad SIMD 3D vector divided by a quad SIMD scalar.
			GSOUND_FORCE_INLINE SIMDVector3D operator / ( const SIMDScalar<T,4>& quadScalar ) const
			{
				const SIMDScalar<T,4> inverseQuadScalar = T(1) / quadScalar;
				
				return SIMDVector3D( x*inverseQuadScalar, y*inverseQuadScalar, z*inverseQuadScalar );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			
			
			/// Compute the component-wise sum of this quad SIMD 3D vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDVector3D& operator += ( const SIMDVector3D& other ) const
			{
				x += other.x;
				y += other.y;
				z += other.z;
				
				return *this;
			}
			
			
			
			
			/// Compute the component-wise sum of this quad SIMD 3D vector with a quad SIMD scalar and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDVector3D& operator += ( const SIMDScalar<T,4>& quadScalar ) const
			{
				x += quadScalar;
				y += quadScalar;
				z += quadScalar;
				
				return *this;
			}
			
			
			
			
			/// Compute the component-wise difference of this quad SIMD 3D vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDVector3D& operator -= ( const SIMDVector3D& other ) const
			{
				x -= other.x;
				y -= other.y;
				z -= other.z;
				
				return *this;
			}
			
			
			
			
			/// Compute the component-wise difference of this quad SIMD 3D vector with a quad SIMD scalar and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDVector3D& operator -= ( const SIMDScalar<T,4>& quadScalar ) const
			{
				x -= quadScalar;
				y -= quadScalar;
				z -= quadScalar;
				
				return *this;
			}
			
			
			
			
			/// Compute the component-wise multiplication of this quad SIMD 3D vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDVector3D& operator *= ( const SIMDVector3D& other ) const
			{
				x *= other.x;
				y *= other.y;
				z *= other.z;
				
				return *this;
			}
			
			
			
			
			/// Compute the component-wise multiplication of this quad SIMD 3D vector with a quad SIMD scalar and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDVector3D& operator *= ( const SIMDScalar<T,4>& quadScalar ) const
			{
				x *= quadScalar;
				y *= quadScalar;
				z *= quadScalar;
				
				return *this;
			}
			
			
			
			
			/// Compute the component-wise quotient of this quad SIMD 3D vector divided by a quad SIMD scalar and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDVector3D& operator /= ( const SIMDScalar<T,4>& quadScalar ) const
			{
				const SIMDScalar<T,4> inverseQuadScalar = T(1) / quadScalar;
				
				x *= inverseQuadScalar;
				y *= inverseQuadScalar;
				z *= inverseQuadScalar;
				
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// The X component vector of this SIMDVector3D.
			GSOUND_ALIGN(16) SIMDScalar<T,4> x;
			
			
			
			
			/// The X component vector of this SIMDVector3D.
			GSOUND_ALIGN(16) SIMDScalar<T,4> y;
			
			
			
			
			/// The X component vector of this SIMDVector3D.
			GSOUND_ALIGN(16) SIMDScalar<T,4> z;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Free Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Compute and return the dot product of two SIMD 3D vectors.
/** 
  * This method performs N standard dot product operations for a SIMD width
  * of N values per register and returns the result as an N-wide SIMD scalar.
  * 
  * @param vector1 - The first SIMD 3D vector of the dot product.
  * @param vector2 - The second SIMD 3D vector of the dot product.
  * @return The dot products of the two vector parameters.
  */
template < typename T, Size dimension >
GSOUND_FORCE_INLINE SIMDScalar<T,dimension> dot( const SIMDVector3D<T,dimension>& vector1,
											const SIMDVector3D<T,dimension>& vector2 )
{
	const SIMDVector3D<T,dimension> temp = vector1*vector2;
	
	return temp.x + temp.y + temp.z;
}




/// Compute and return the cross product of two SIMD 3D vectors.
/** 
  * This method performs N standard cross product operations for a SIMD width
  * of N values per register and returns the result as a SIMD 3D vector which
  * contains the results of all cross products.
  * 
  * @param vector1 - The first SIMD 3D vector of the cross product.
  * @param vector2 - The second SIMD 3D vector of the cross product.
  * @return The cross product vectors of the two vector parameters.
  */
template < typename T, Size dimension >
GSOUND_FORCE_INLINE SIMDVector3D<T,dimension> cross( const SIMDVector3D<T,dimension>& vector1,
												const SIMDVector3D<T,dimension>& vector2 )
{
	return SIMDVector3D<T,dimension>( vector1.y*vector2.z - vector1.z*vector2.y,
										vector1.z*vector2.x - vector1.x*vector2.z,
										vector1.x*vector2.y - vector1.y*vector2.x );
}





//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SIMD_VECTOR_3D_H
