/*
 * Project:     GSound
 * 
 * File:        gsound/math/SIMDScalarInt4.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::SIMDScalar class specialization for a 4-int SIMD data type.
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


#ifndef INCLUDE_GSOUND_SIMD_SCALAR_INT_4_H
#define INCLUDE_GSOUND_SIMD_SCALAR_INT_4_H


#include "GSoundMathConfig.h"


#include "SIMDScalar.h"
#include "SIMDScalarBool4.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class representing a 4-component 32-bit signed-integer SIMD scalar.
/**
  * This specialization of the SIMDScalar class uses a 128-bit value to encode
  * 4 32-bit signed-integer values. All basic arithmetic operations are supported,
  * plus a subset of standard scalar operations: abs(), min(), max(), sqrt().
  */
template <>
class GSOUND_ALIGN(16) SIMDScalar<Int32,4>
{
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Type Declarations
			
			
			
			
			/// Define the type for a 4x float scalar structure.
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
			typedef GSOUND_ALTIVEC_VECTOR int SIMDScalar4;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE)
	#if GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
			typedef __m128 SIMDScalar4Float;
	#endif
	
	#if GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
			typedef __m128i SIMDScalar4;
	#endif
#endif
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Constructor
			
			
			
			
#if GSOUND_USE_SIMD && (defined(GSOUND_SIMD_ALTIVEC) || defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0))
			/// Create a new 4D vector with the specified 4D SIMD vector value.
			GSOUND_FORCE_INLINE SIMDScalar( SIMDScalar4 simdScalar )
				:	v( simdScalar )
			{
			}
#endif

#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
			/// Create a new 4D vector with the specified 4D SIMD vector value.
			GSOUND_FORCE_INLINE SIMDScalar( SIMDScalar4Float simdScalar )
				:	vFloat( simdScalar )
			{
			}
#endif
			
			
			
			
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a new 4D SIMD scalar with all elements left uninitialized.
			GSOUND_FORCE_INLINE SIMDScalar()
			{
			}
			
			
			
			
			/// Create a new 4D SIMD scalar with all elements equal to the specified value.
			GSOUND_FORCE_INLINE SIMDScalar( Int32 value )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = (SIMDScalar4)(value);
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = _mm_set1_epi32( value );
#else
				a = b = c = d = value;
#endif
			}
			
			
			
			
			/// Create a new 4D SIMD scalar with the elements equal to the specified 4 values.
			GSOUND_FORCE_INLINE SIMDScalar( Int32 newA, Int32 newB, Int32 newC, Int32 newD )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = (SIMDScalar4)( newA, newB, newC, newD );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				// The parameters are reversed to keep things consistent with loading from an address.
				v = _mm_set_epi32( newD, newC, newB, newA );
#else
				a = newA;
				b = newB;
				c = newC;
				d = newD;
#endif
			}
			
			
			
			
			/// Create a new 4D SIMD scalar from the first 4 values stored at specified pointer's location.
			GSOUND_FORCE_INLINE SIMDScalar( const Int32* array )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				a = array[0];
				b = array[1];
				c = array[2];
				d = array[3];
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = _mm_load_si128( (const SIMDScalar4*)array );
#else
				a = array[0];
				b = array[1];
				c = array[2];
				d = array[3];
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Copy Constructor
			
			
			
			
			/// Create a new SIMD scalar with the same contents as another.
			/**
			  * This shouldn't have to be overloaded, but for some reason the compiler (GCC)
			  * optimizes SIMD code better with it overloaded. Before, the compiler would
			  * store the result of a SIMD operation on the stack before transfering it to
			  * the destination, resulting in an extra 8+ load/stores per computation.
			  */
			GSOUND_FORCE_INLINE SIMDScalar( const SIMDScalar& other )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = other.v;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = other.v;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				vFloat = other.vFloat;
#else
				a = other.a;
				b = other.b;
				c = other.c;
				d = other.d;
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the contents of one SIMDScalar object to another.
			/**
			  * This shouldn't have to be overloaded, but for some reason the compiler (GCC)
			  * optimizes SIMD code better with it overloaded. Before, the compiler would
			  * store the result of a SIMD operation on the stack before transfering it to
			  * the destination, resulting in an extra 8+ load/stores per computation.
			  */
			GSOUND_FORCE_INLINE SIMDScalar& operator = ( const SIMDScalar& other )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = other.v;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = other.v;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				vFloat = other.vFloat;
#else
				a = other.a;
				b = other.b;
				c = other.c;
				d = other.d;
#endif
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Store Method
			
			
			
			
			GSOUND_FORCE_INLINE void store( Int32* destination ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				destination[0] = a;
				destination[1] = b;
				destination[2] = c;
				destination[3] = d;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				_mm_store_si128( (SIMDScalar4*)destination, v );
#else
				destination[0] = a;
				destination[1] = b;
				destination[2] = c;
				destination[3] = d;
#endif
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Accessor Methods
			
			
			
			
			/// Get a reference to the value stored at the specified component index in this scalar.
			GSOUND_FORCE_INLINE Int32& operator [] ( Index i )
			{
				return x[i];
			}
			
			
			
			
			/// Get the value stored at the specified component index in this scalar.
			GSOUND_FORCE_INLINE Int32 operator [] ( Index i ) const
			{
				return x[i];
			}
			
			
			
			
			/// Get a pointer to the first element in this scalar.
			/**
			  * The remaining values are in the next 3 locations after the
			  * first element.
			  */
			GSOUND_FORCE_INLINE const Int32* toArray() const
			{
				return x;
			}
			
			
			
			
			/// Get the width of this scalar (number of components it has).
			GSOUND_FORCE_INLINE static Size getWidth()
			{
				return 4;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Logical Operators
			
			
			
			
			/// Return the bitwise NOT of this 4D SIMD vector.
			GSOUND_FORCE_INLINE SIMDScalar operator ~ () const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_nor( v, v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( v, _mm_set1_epi32( 0xFFFFFFFF ) ) );
#else
				return SIMDScalar( ~a, ~b, ~c, ~d );
#endif
			}
			
			
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with another and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator & ( const SIMDScalar& vector ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_and( v, vector.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_and_si128( v, vector.v ) );
#else
				return SIMDScalar( a & vector.a, b & vector.b, c & vector.c, d & vector.d );
#endif
			}
			
			
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with a mask and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator & ( const SIMDScalar<Bool,4>& vector ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_and( v, vector.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_and_si128( v, vector.v ) );
#else
				return SIMDScalar( a & vector.a, b & vector.b, c & vector.c, d & vector.d );
#endif
			}
			
			
			
			
			/// Compute the bitwise OR of this 4D SIMD vector with another and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator | ( const SIMDScalar& vector ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_or( v, vector.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_or_si128( v, vector.v ) );
#else
				return SIMDScalar( a | vector.a, b | vector.b, c | vector.c, d | vector.d );
#endif
			}
			
			
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with a mask and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator | ( const SIMDScalar<Bool,4>& vector ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_or( v, vector.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_or_si128( v, vector.v ) );
#else
				return SIMDScalar( a | vector.a, b | vector.b, c | vector.c, d | vector.d );
#endif
			}
			
			
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator ^ ( const SIMDScalar& vector ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_xor( v, vector.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( v, vector.v ) );
#else
				return SIMDScalar( a ^ vector.a, b ^ vector.b, c ^ vector.c, d ^ vector.d );
#endif
			}
			
			
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with a mask and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator ^ ( const SIMDScalar<Bool,4>& vector ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_xor( v, vector.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( v, vector.v ) );
#else
				return SIMDScalar( a ^ vector.a, b ^ vector.b, c ^ vector.c, d ^ vector.d );
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Logical Assignment Operators
			
			
			
			
			/// Compute the logical AND of this 4D SIMD vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDScalar& operator &= ( const SIMDScalar& vector )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = vec_and( v, vector.v );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = _mm_and_si128( v, vector.v );
#else
				a &= vector.a;
				b &= vector.b;
				c &= vector.c;
				d &= vector.d;
#endif
				return *this;
			}
			
			
			
			
			/// Compute the logical OR of this 4D SIMD vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDScalar& operator |= ( const SIMDScalar& vector )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = vec_or( v, vector.v );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = _mm_or_si128( v, vector.v );
#else
				a |= vector.a;
				b |= vector.b;
				c |= vector.c;
				d |= vector.d;
#endif
				return *this;
			}
			
			
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDScalar& operator ^= ( const SIMDScalar& vector )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = vec_xor( v, vector.v );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = _mm_xor_si128( v, vector.v );
#else
				a ^= vector.a;
				b ^= vector.b;
				c ^= vector.c;
				d ^= vector.d;
#endif
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Comparison Operators
			
			
			
			
			/// Compare two 4D SIMD scalars component-wise for equality.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * If each corresponding pair of components is equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator == ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpeq( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Bool,4>( _mm_cmpeq_epi32( v, scalar.v ) );
#else
				return SIMDScalar<Bool,4>( a == scalar.a, b == scalar.b, c == scalar.c, d == scalar.d );
#endif
			}
			
			
			
			
			/// Compare this scalar to a single floating point value for equality.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components is equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator == ( const Int32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpeq( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Bool,4>( _mm_cmpeq_epi32( v, _mm_set1_epi32(value) ) );
#else
				return SIMDScalar<Bool,4>( a == value, b == value, c == value, d == value );
#endif
			}
			
			
			
			
			
			/// Compare two 4D SIMD scalars component-wise for inequality
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * If each corresponding pair of components is not equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator != ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				const SIMDScalar4 temp = vec_cmpeq( v, scalar.v );
				return SIMDScalar<Bool,4>( vec_nor( temp, temp ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Bool,4>( _mm_xor_si128( _mm_cmpeq_epi32( v, scalar.v ), _mm_set1_epi32(0xFFFFFFFF) ) );
#else
				return SIMDScalar<Bool,4>( a != scalar.a, b != scalar.b, c != scalar.c, d != scalar.d );
#endif
			}
			
			
			
			
			/// Compare this scalar to a single floating point value for inequality.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components is not equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator != ( const Int32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				const SIMDScalar4 temp = vec_cmpeq( v, (SIMDScalar4)(value) );
				return SIMDScalar<Bool,4>( vec_nor( temp, temp ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Bool,4>( _mm_xor_si128( _mm_cmpeq_epi32( v, _mm_set1_epi32(value) ), _mm_set1_epi32(0xFFFFFFFF) ) );
#else
				return SIMDScalar<Bool,4>( a != value, b != value, c != value, d != value );
#endif
			}
			
			
			
			
			/// Perform a component-wise less-than comparison between this an another 4D SIMD scalar.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component less than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator < ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmplt( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Bool,4>( _mm_cmplt_epi32( v, scalar.v ) );
#else
				return SIMDScalar<Bool,4>( a < scalar.a, b < scalar.b, c < scalar.c, d < scalar.d );
#endif
			}
			
			
			
			
			/// Perform a component-wise less-than comparison between this 4D SIMD scalar and an expanded scalar.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component less than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator < ( const Int32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmplt( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Bool,4>( _mm_cmplt_epi32( v, _mm_set1_epi32(value) ) );
#else
				return SIMDScalar<Bool,4>( a < value, b < value, c < value, d < value );
#endif
			}
			
			
			
			
			/// Perform a component-wise greater-than comparison between this an another 4D SIMD scalar.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator > ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpgt( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Bool,4>( _mm_cmpgt_epi32( v, scalar.v ) );
#else
				return SIMDScalar<Bool,4>( a > scalar.a, b > scalar.b, c > scalar.c, d > scalar.d );
#endif
			}
			
			
			
			
			/// Perform a component-wise greater-than comparison between this 4D SIMD scalar and an expanded scalar.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator > ( const Int32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpgt( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Bool,4>( _mm_cmpgt_epi32( v, _mm_set1_epi32(value) ) );
#else
				return SIMDScalar<Bool,4>( a > value, b > value, c > value, d > value );
#endif
			}
			
			
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this an another 4D SIMD scalar.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component less than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator <= ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmple( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Bool,4>( _mm_or_si128( _mm_cmplt_epi32( v, scalar.v ),
														_mm_cmpeq_epi32( v, scalar.v ) ) );
#else
				return SIMDScalar<Bool,4>( a <= scalar.a, b <= scalar.b, c <= scalar.c, d <= scalar.d );
#endif
			}
			
			
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component less than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator <= ( const Int32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmple( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				SIMDScalar4 scalar = _mm_set1_epi32(value);
				return SIMDScalar<Bool,4>( _mm_or_si128( _mm_cmplt_epi32( v, scalar ),
														_mm_cmpeq_epi32( v, scalar ) ) );
#else
				return SIMDScalar<Bool,4>( a <= value, b <= value, c <= value, d <= value );
#endif
			}
			
			
			
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this an another 4D SIMD scalar.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator >= ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpge( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Bool,4>( _mm_or_si128( _mm_cmpgt_epi32( v, scalar.v ),
														_mm_cmpeq_epi32( v, scalar.v ) ) );
#else
				return SIMDScalar<Bool,4>( a >= scalar.a, b >= scalar.b, c >= scalar.c, d >= scalar.d );
#endif
			}
			
			
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator >= ( const Int32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpge( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				SIMDScalar4 scalar = _mm_set1_epi32(value);
				return SIMDScalar<Bool,4>( _mm_or_si128( _mm_cmpgt_epi32( v, scalar ),
														_mm_cmpeq_epi32( v, scalar ) ) );
#else
				return SIMDScalar<Bool,4>( a >= value, b >= value, c >= value, d >= value );
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Shifting Operators
			
			
			
			
			/// Shift each component of the SIMD scalar to the left by the specified amount of bits.
			/**
			  * This method shifts the contents of each component to the left by the specified
			  * amount of bits and inserts zeros.
			  * 
			  * @param bitShift - the number of bits to shift this SIMD scalar by.
			  * @return the shifted SIMD scalar.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator << ( Int32 bitShift ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_sl( v, (SIMDScalar4)(bitShift) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_slli_epi32( v, bitShift ) );
#else
				return SIMDScalar( a << bitShift, b << bitShift, c << bitShift, d << bitShift );
#endif
			}
			
			
			
			
			/// Shift each component of the SIMD scalar to the right by the specified amount of bits.
			/**
			  * This method shifts the contents of each component to the right by the specified
			  * amount of bits and sign extends the original values..
			  * 
			  * @param bitShift - the number of bits to shift this SIMD scalar by.
			  * @return the shifted SIMD scalar.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator >> ( Int32 bitShift ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_sra( v, (SIMDScalar4)(bitShift) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_srai_epi32( v, bitShift ) );
#else
				return SIMDScalar( a >> bitShift, b >> bitShift, c >> bitShift, d >> bitShift );
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			
			
			/// Negate a scalar.
			/** 
			  * This method negates every component of this 4D SIMD scalar
			  * and returns the result, leaving this scalar unmodified.
			  * 
			  * @return the negation of the original scalar.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator - () const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( (SIMDScalar4)(0), v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi32( _mm_set1_epi32(0), v ) );
#else
				return SIMDScalar( -a, -b, -c, -d );
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			
			
			
			/// Add this scalar to another and return the result.
			/** 
			  * This method adds another scalar to this one, component-wise,
			  * and returns this addition. It does not modify either of the original
			  * scalars.
			  * 
			  * @param scalar - The scalar to add to this one.
			  * @return The addition of this scalar and the parameter.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator + ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_add( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_add_epi32( v, scalar.v ) );
#else
				return SIMDScalar( a + scalar.a, b + scalar.b, c + scalar.c, d + scalar.d );
#endif
			}
			
			
			
			
			/// Add a value to every component of this scalar.
			/** 
			  * This method adds the value parameter to every component
			  * of the scalar, and returns a scalar representing this result.
			  * It does not modifiy the original scalar.
			  * 
			  * @param value - The value to add to all components of this scalar.
			  * @return The resulting scalar of this addition.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator + ( const Int32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_add( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_add_epi32( v, _mm_set1_epi32(value) ) );
#else
				return SIMDScalar( a + value, b + value, c + value, d + value );
#endif
			}
			
			
			
			
			/// Subtract a scalar from this scalar component-wise and return the result.
			/** 
			  * This method subtracts another scalar from this one, component-wise,
			  * and returns this subtraction. It does not modify either of the original
			  * scalars.
			  * 
			  * @param scalar - The scalar to subtract from this one.
			  * @return The subtraction of the the parameter from this scalar.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator - ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi32( v, scalar.v ) );
#else
				return SIMDScalar( a - scalar.a, b - scalar.b, c - scalar.c, d - scalar.d );
#endif
			}
			
			
			
			
			/// Subtract a value from every component of this scalar.
			/** 
			  * This method subtracts the value parameter from every component
			  * of the scalar, and returns a scalar representing this result.
			  * It does not modifiy the original scalar.
			  * 
			  * @param value - The value to subtract from all components of this scalar.
			  * @return The resulting scalar of this subtraction.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator - ( const Int32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi32( v, _mm_set1_epi32(value) ) );
#else
				return SIMDScalar( a - value, b - value, c - value, d - value );
#endif
			}
			
			
			
			
			/// Multiply component-wise this scalar and another scalar.
			/** 
			  * This operator multiplies each component of this scalar
			  * by the corresponding component of the other scalar and
			  * returns a scalar representing this result. It does not modify
			  * either original scalar.
			  * 
			  * @param scalar - The scalar to multiply this scalar by.
			  * @return The result of the multiplication.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator * ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( a*scalar.a, b*scalar.b, c*scalar.c, d*scalar.d );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE)
				return SIMDScalar( a*scalar.a, b*scalar.b, c*scalar.c, d*scalar.d );
#else
				return SIMDScalar( a*scalar.a, b*scalar.b, c*scalar.c, d*scalar.d );
#endif
			}
			
			
			
			
			/// Multiply every component of this scalar by a value and return the result.
			/** 
			  * This method multiplies the value parameter with every component
			  * of the scalar, and returns a scalar representing this result.
			  * It does not modifiy the original scalar.
			  * 
			  * @param value - The value to multiplly with all components of this scalar.
			  * @return The resulting scalar of this multiplication.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator * ( const Int32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( a*value, b*value, c*value, d*value );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE)
				return SIMDScalar( a*value, b*value, c*value, d*value );
#else
				return SIMDScalar( a*value, b*value, c*value, d*value );
#endif
			}
			
			
			
			
			/// Divide this scalar by another scalar component-wise.
			/** 
			  * This operator divides each component of this scalar
			  * by the corresponding component of the other scalar and
			  * returns a scalar representing this result. It does not modify
			  * either original scalar.
			  * 
			  * @param scalar - The scalar to multiply this scalar by.
			  * @return The result of the division.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator / ( const SIMDScalar& scalar ) const
			{
#if GGSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( a/scalar.a, b/scalar.b, c/scalar.c, d/scalar.d );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE)
				return SIMDScalar( a/scalar.a, b/scalar.b, c/scalar.c, d/scalar.d );
#else
				return SIMDScalar( a/scalar.a, b/scalar.b, c/scalar.c, d/scalar.d );
#endif
			}
			
			
			
			
			/// Divide every component of this scalar by a value and return the result.
			/** 
			  * This method Divides every component of the scalar by the value parameter, 
			  * and returns a scalar representing this result.
			  * It does not modifiy the original scalar.
			  * 
			  * @param value - The value to divide all components of this scalar by.
			  * @return The resulting scalar of this division.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator / ( const Int32 value ) const
			{
#if GGSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( a/value, b/value, c/value, d/value );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE)
				return SIMDScalar( a/value, b/value, c/value, d/value );
#else
				return SIMDScalar( a/value, b/value, c/value, d/value );
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			
			
			/// Add a scalar to this scalar, modifying this original scalar.
			/**
			  * This method adds another scalar to this scalar, component-wise,
			  * and sets this scalar to have the result of this addition.
			  * 
			  * @param scalar - The scalar to add to this scalar.
			  * @return A reference to this modified scalar.
			  */
			GSOUND_FORCE_INLINE SIMDScalar& operator += ( const SIMDScalar& scalar )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = vec_add( v, scalar.v );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = _mm_add_epi32( v, scalar.v );
#else
				a += scalar.a;
				b += scalar.b;
				c += scalar.c;
				d += scalar.d;
#endif
				return *this;
			}
			
			
			
			/// Subtract a scalar from this scalar, modifying this original scalar.
			/**
			  * This method subtracts another scalar from this scalar, component-wise,
			  * and sets this scalar to have the result of this subtraction.
			  * 
			  * @param scalar - The scalar to subtract from this scalar.
			  * @return A reference to this modified scalar.
			  */
			GSOUND_FORCE_INLINE SIMDScalar& operator -= ( const SIMDScalar& scalar )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = vec_sub( v, scalar.v );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = _mm_sub_epi32( v, scalar.v );
#else
				a -= scalar.a;
				b -= scalar.b;
				c -= scalar.c;
				d -= scalar.d;
#endif
				return *this;
			}
			
			
			
			
			/// Multiply component-wise this scalar and another scalar and modify this scalar.
			/** 
			  * This operator multiplies each component of this scalar
			  * by the corresponding component of the other scalar and
			  * modifies this scalar to contain the result.
			  * 
			  * @param scalar - The scalar to multiply this scalar by.
			  * @return A reference to this modified scalar.
			  */
			GSOUND_FORCE_INLINE SIMDScalar& operator *= ( const SIMDScalar& scalar )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				a *= scalar.a;
				b *= scalar.b;
				c *= scalar.c;
				d *= scalar.d;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE)
				a *= scalar.a;
				b *= scalar.b;
				c *= scalar.c;
				d *= scalar.d;
#else
				a *= scalar.a;
				b *= scalar.b;
				c *= scalar.c;
				d *= scalar.d;
#endif
				return *this;
			}
			
			
			
			
			/// Divide this scalar by another scalar component-wise and modify this scalar.
			/** 
			  * This operator divides each component of this scalar
			  * by the corresponding component of the other scalar and
			  * modifies this scalar to contain the result.
			  * 
			  * @param scalar - The scalar to divide this scalar by.
			  * @return A reference to this modified scalar.
			  */
			GSOUND_FORCE_INLINE SIMDScalar& operator /= ( const SIMDScalar& scalar )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				a /= scalar.a;
				b /= scalar.b;
				c /= scalar.c;
				d /= scalar.d;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE)
				a /= scalar.a;
				b /= scalar.b;
				c /= scalar.c;
				d /= scalar.d;
#else
				a /= scalar.a;
				b /= scalar.b;
				c /= scalar.c;
				d /= scalar.d;
#endif
				return *this;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			GSOUND_ALIGN(16) union
			{
#if GSOUND_USE_SIMD && (defined(GSOUND_SIMD_ALTIVEC) || defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0))
				/// The 4D SIMD vector used internally.
				SIMDScalar4 v;
#endif

#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				/// A floating-point alias of the integer SIMD scalar.
				SIMDScalar4Float vFloat;
#endif
				
				struct
				{
					/// The A component of a 4D SIMD scalar.
					Int32 a;
					
					
					
					/// The B component of a 4D SIMD scalar.
					Int32 b;
					
					
					
					/// The C component of a 4D SIMD scalar.
					Int32 c;
					
					
					
					/// The D component of a 4D SIMD scalar.
					Int32 d;
				};
				
				
				/// The components of a 4D SIMD scalar in array format.
				Int32 x[4];
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Friend Declarations
			
			
			
			
			template < typename T, Size dimension >
			friend class SIMDVector3D;
			
			
			
			friend GSOUND_FORCE_INLINE SIMDScalar abs( const SIMDScalar& scalar );
			friend GSOUND_FORCE_INLINE SIMDScalar sqrt( const SIMDScalar& scalar );
			friend GSOUND_FORCE_INLINE SIMDScalar min( const SIMDScalar& scalar1, const SIMDScalar& scalar2 );
			friend GSOUND_FORCE_INLINE SIMDScalar max( const SIMDScalar& scalar1, const SIMDScalar& scalar2 );
			
			template < UInt i1, UInt i2, UInt i3, UInt i4 >
			friend GSOUND_FORCE_INLINE SIMDScalar shuffle( const SIMDScalar& scalar1 );
			
			template < UInt i1, UInt i2, UInt i3, UInt i4 >
			friend GSOUND_FORCE_INLINE SIMDScalar shuffle( const SIMDScalar& scalar1, const SIMDScalar& scalar2 );
			
			
			friend GSOUND_FORCE_INLINE SIMDScalar select( const SIMDScalar<Bool,4>& selector,
										const SIMDScalar& scalar1, const SIMDScalar& scalar2 );
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Associative SIMD Scalar Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a scalar value to each component of this scalar and return the resulting scalar.
GSOUND_FORCE_INLINE SIMDScalar<Int32,4> operator + ( const Int32 value, const SIMDScalar<Int32,4>& scalar )
{
	return SIMDScalar<Int32,4>(value) + scalar;
}




/// Subtract a scalar value from each component of this scalar and return the resulting scalar.
GSOUND_FORCE_INLINE SIMDScalar<Int32,4> operator - ( const Int32 value, const SIMDScalar<Int32,4>& scalar )
{
	return SIMDScalar<Int32,4>(value) - scalar;
}




/// Multiply a scalar value by each component of this scalar and return the resulting scalar.
GSOUND_FORCE_INLINE SIMDScalar<Int32,4> operator * ( const Int32 value, const SIMDScalar<Int32,4>& scalar )
{
	return SIMDScalar<Int32,4>(value) * scalar;
}




/// Divide each component of this scalar by a scalar value and return the resulting scalar.
GSOUND_FORCE_INLINE SIMDScalar<Int32,4> operator / ( const Int32 value, const SIMDScalar<Int32,4>& scalar )
{
	return SIMDScalar<Int32,4>(value) / scalar;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Free Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Compute the absolute value of each component of the specified SIMD scalar and return the result.
GSOUND_FORCE_INLINE SIMDScalar<Int32,4> abs( const SIMDScalar<Int32,4>& scalar )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( vec_abs( scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE)
	return SIMDScalar<Int32,4>( math::abs(scalar.a), math::abs(scalar.b), math::abs(scalar.c), math::abs(scalar.d) );
#else
	return SIMDScalar<Int32,4>( math::abs(scalar.a), math::abs(scalar.b), math::abs(scalar.c), math::abs(scalar.d) );
#endif
}




/// Compute the square root of each component of the specified SIMD scalar and return the result.
GSOUND_FORCE_INLINE SIMDScalar<Int32,4> sqrt( const SIMDScalar<Int32,4>& scalar )
{
	return SIMDScalar<Int32,4>( math::sqrt(scalar.a), math::sqrt(scalar.b), math::sqrt(scalar.c), math::sqrt(scalar.d) );
}




/// Compute the minimum of each component of the specified SIMD scalars and return the result.
GSOUND_FORCE_INLINE SIMDScalar<Int32,4> min( const SIMDScalar<Int32,4>& scalar1, const SIMDScalar<Int32,4>& scalar2 )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( vec_min( scalar1.v, scalar2.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE)
	return SIMDScalar<Int32,4>( math::min(scalar1.a, scalar2.a),
								math::min(scalar1.b, scalar2.b),
								math::min(scalar1.c, scalar2.c),
								math::min(scalar1.d, scalar2.d) );
#else
	return SIMDScalar<Int32,4>( math::min(scalar1.a, scalar2.a),
								math::min(scalar1.b, scalar2.b),
								math::min(scalar1.c, scalar2.c),
								math::min(scalar1.d, scalar2.d) );
#endif
}




/// Compute the maximum of each component of the specified SIMD scalars and return the result.
GSOUND_FORCE_INLINE SIMDScalar<Int32,4> max( const SIMDScalar<Int32,4>& scalar1, const SIMDScalar<Int32,4>& scalar2 )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( vec_max( scalar1.v, scalar2.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE)
	return SIMDScalar<Int32,4>( math::max(scalar1.a, scalar2.a),
								math::max(scalar1.b, scalar2.b),
								math::max(scalar1.c, scalar2.c),
								math::max(scalar1.d, scalar2.d) );
#else
	return SIMDScalar<Int32,4>( math::max(scalar1.a, scalar2.a),
								math::max(scalar1.b, scalar2.b),
								math::max(scalar1.c, scalar2.c),
								math::max(scalar1.d, scalar2.d) );
#endif
}




/// Pick 4 elements from the specified SIMD scalar and return the result.
template < UInt i1, UInt i2, UInt i3, UInt i4 >
GSOUND_FORCE_INLINE SIMDScalar<Int32,4> shuffle( const SIMDScalar<Int32,4>& scalar )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( scalar[i1], scalar[i2], scalar[i3], scalar[i4] );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Int32,4>( _mm_shuffle_ps( scalar.vFloat, scalar.vFloat, _MM_SHUFFLE(i4, i3, i2, i1) ) );
#else
	return SIMDScalar<Int32,4>( scalar[i1], scalar[i2], scalar[i3], scalar[i4] );
#endif
}




/// Pick two elements from each SIMD scalar and return the result.
template < UInt i1, UInt i2, UInt i3, UInt i4 >
GSOUND_FORCE_INLINE SIMDScalar<Int32,4> shuffle( const SIMDScalar<Int32,4>& scalar1, const SIMDScalar<Int32,4>& scalar2 )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( scalar1[i1], scalar1[i2], scalar2[i3], scalar2[i4] );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Int32,4>( _mm_shuffle_ps( scalar1.vFloat, scalar2.vFloat, _MM_SHUFFLE(i4, i3, i2, i1) ) );
#else
	return SIMDScalar<Int32,4>( scalar1[i1], scalar1[i2], scalar2[i3], scalar2[i4] );
#endif
}




/// Select elements from the first SIMD scalar if the selector is TRUE, otherwise from the second.
GSOUND_FORCE_INLINE SIMDScalar<Int32,4> select( const SIMDScalar<Bool,4>& selector,
												const SIMDScalar<Int32,4>& scalar1, const SIMDScalar<Int32,4>& scalar2 )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( vec_sel( scalar2.v, scalar1.v, selector.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	// (((b^a) & selector) ^ a)
	return SIMDScalar<Int32,4>( _mm_xor_ps( scalar2.vFloat,
										_mm_and_ps( selector.vFloat,
													_mm_xor_ps( scalar1.vFloat, scalar2.vFloat ) ) ) );
#else
	return SIMDScalar<Int32,4>( selector.a ? scalar1.a : scalar2.a,
								selector.b ? scalar1.b : scalar2.b,
								selector.c ? scalar1.c : scalar2.c,
								selector.d ? scalar1.d : scalar2.d );
#endif
}




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SIMD_SCALAR_INT_4_H
