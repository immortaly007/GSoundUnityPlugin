/*
 * Project:     GSound
 * 
 * File:        gsound/math/SIMDScalarBool4.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::SIMDScalar class specialization for a 4-bool SIMD data type.
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


#ifndef INCLUDE_GSOUND_SIMD_SCALAR_BOOL_4_H
#define INCLUDE_GSOUND_SIMD_SCALAR_BOOL_4_H


#include "GSoundMathConfig.h"


#include "SIMDScalar.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class representing a 4-component pseudo-boolean SIMD vector.
/**
  * This specialization of the SIMDScalar class uses a 128-bit value to encode
  * 4 32-bit integer values which each semantically represent a single boolean
  * value where all 1's is TRUE and all 0's is FALSE.
  */
template <>
class GSOUND_ALIGN(16) SIMDScalar<Bool,4>
{
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Type Declarations
			
			
			
			
			/// Define the type for a 4x bool vector structure.
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
			
			typedef GSOUND_ALTIVEC_VECTOR bool int SIMDScalar4;
			
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
			
			
			
			
			/// Create a new 4D SIMD vector with the elements equal to the specified 4 values.
			GSOUND_FORCE_INLINE SIMDScalar( UInt32 newA, UInt32 newB, UInt32 newC, UInt32 newD )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = (SIMDScalar4)( newA, newB, newC, newD );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = _mm_set_epi32( newD, newC, newB, newA );
#else
				a = newA;
				b = newB;
				c = newC;
				d = newD;
#endif
			}
			
			
			
			
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a new 4D SIMD scalar with all elements left uninitialized.
			GSOUND_FORCE_INLINE SIMDScalar()
			{
			}
			
			
			
			
			/// Create a new 4D SIMD vector with all elements equal to the specified value.
			GSOUND_FORCE_INLINE SIMDScalar( Bool value )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = (SIMDScalar4)( value );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = _mm_cmpeq_epi32( _mm_set1_epi32( value ), _mm_set1_epi32(1) );
#else
				a = b = c = d = value;
#endif
			}
			
			
			
			
			/// Create a new 4D SIMD vector with the low and high words set to the specified values.
			GSOUND_FORCE_INLINE SIMDScalar( Bool newLow, Bool newHigh )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = (SIMDScalar4)( newLow, newLow, newHigh, newHigh );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = _mm_cmpeq_epi32( _mm_unpackhi_epi64( _mm_set1_epi32( newLow ), _mm_set1_epi32( newHigh ) ),
									_mm_set1_epi32(1) );
#else
				a = newLow ? 0xFFFFFFFF : 0;
				b = newLow ? 0xFFFFFFFF : 0;
				c = newHigh ? 0xFFFFFFFF : 0;
				d = newHigh ? 0xFFFFFFFF : 0;
#endif
			}
			
			
			
			
			/// Create a new 4D SIMD vector with the elements equal to the specified 4 values.
			GSOUND_FORCE_INLINE SIMDScalar( Bool newA, Bool newB, Bool newC, Bool newD )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = (SIMDScalar4)( newA, newB, newC, newD );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				v = _mm_cmpeq_epi32( _mm_set_epi32( newD, newC, newB, newA ), _mm_set1_epi32(1) );
#else
				a = newA ? 0xFFFFFFFF : 0;
				b = newB ? 0xFFFFFFFF : 0;
				c = newC ? 0xFFFFFFFF : 0;
				d = newD ? 0xFFFFFFFF : 0;
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
		//******	Cast Operator
			
			
			
			
			/// Return a boolean value which indicates whether or not any of the components are true.
			GSOUND_FORCE_INLINE operator Int () const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return a | b | c | d;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return _mm_movemask_ps( vFloat );
#else
				return a | b | c | d;
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mask Operator
			
			
			
			
			/// Return a mask which indicates if each of the components are true.
			GSOUND_FORCE_INLINE Int getMask() const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return ((a & 0x80000000) << 3) | ((b & 0x80000000) << 2) | ((c & 0x80000000) << 1) | ((d & 0x80000000));
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return _mm_movemask_ps( vFloat );
#else
				return ((a & 0x80000000) << 3) | ((b & 0x80000000) << 2) | ((c & 0x80000000) << 1) | ((d & 0x80000000));
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Accessor Methods
			
			
			
			
			/// Get the element at the specified index in the vector.
			GSOUND_FORCE_INLINE Bool operator [] ( Index i ) const
			{
				return *(Bool*)(x + i);
			}
			
			
			
			
			/// Get the width of this scalar (number of components it has).
			GSOUND_FORCE_INLINE static Size getWidth()
			{
				return 4;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Comparison Operators
			
			
			
			
			/// Compare two 4D SIMD booleans component-wise for inequality
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
			
			
			
			
			/// Compare this 4D SIMD boolean to a single boolean value for equality.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components is equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator == ( const Bool value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpeq( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpeq_epi32( v, SIMDScalar(value).v ) );
#else
				UInt32 valueInt = value ? UInt32(0xFFFFFFFF) : UInt32(0);
				return SIMDScalar( a == valueInt, b == valueInt, c == valueInt, d == valueInt );
#endif
			}
			
			
			
			
			
			/// Compare two 4D SIMD booleans component-wise for inequality
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * If each corresponding pair of components is not equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator != ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				const SIMDScalar4 temp = vec_cmpeq( v, scalar.v );
				return SIMDScalar( vec_nor( temp, temp ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( _mm_cmpeq_epi32( v, scalar.v ), _mm_set1_epi32(0xFFFFFFFF) ) );
#else
				return SIMDScalar( a != scalar.a, b != scalar.b, c != scalar.c, d != scalar.d );
#endif
			}
			
			
			
			
			/// Compare this 4D SIMD boolean to a single boolean value for inequality.
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components is not equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator != ( const Bool value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				const SIMDScalar4 temp = vec_cmpeq( v, (SIMDScalar4)(value) );
				return SIMDScalar( vec_nor( temp, temp ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( _mm_cmpeq_epi32( v, SIMDScalar(value).v ), _mm_set1_epi32(0xFFFFFFFF) ) );
#else
				UInt32 valueInt = value ? UInt32(0xFFFFFFFF) : UInt32(0);
				return SIMDScalar( a != valueInt, b != valueInt, c != valueInt, d != valueInt );
#endif
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
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Logical Assignment Operators
			
			
			
			
			/// Compute the logical AND of this 4D SIMD vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDScalar operator &= ( const SIMDScalar& vector )
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
			GSOUND_FORCE_INLINE SIMDScalar operator |= ( const SIMDScalar& vector )
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
			GSOUND_FORCE_INLINE SIMDScalar operator ^= ( const SIMDScalar& vector )
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
					/// The A component of a 4D SIMD vector.
					GSOUND_ALIGN(4) UInt32 a;
					
					
					
					/// The B component of a 4D SIMD vector.
					GSOUND_ALIGN(4) UInt32 b;
					
					
					
					/// The C component of a 4D SIMD vector.
					GSOUND_ALIGN(4) UInt32 c;
					
					
					
					/// The D component of a 4D SIMD vector.
					GSOUND_ALIGN(4) UInt32 d;
				};
				
				
				/// The components of a 4D SIMD vector in array format.
				UInt32 x[4];
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Friend Declarations
			
			
			
			
			/// Declare the floating point version of this class as a friend.
			/**
			  * This is done because the floating-point version needs to access the
			  * private constructor of this class for speed when storing the results
			  * of SIMD comparisons.
			  */
			friend class SIMDScalar<float,4>;
			
			
			
			
			/// Declare the integer point version of this class as a friend.
			/**
			  * This is done because the integer version needs to access the
			  * private constructor of this class for speed when storing the results
			  * of SIMD comparisons.
			  */
			friend class SIMDScalar<int,4>;
			
			
			
			
			template < int i1, int i2, int i3, int i4 >
			friend GSOUND_FORCE_INLINE SIMDScalar shuffle( const SIMDScalar& scalar1 );
			
			template < int i1, int i2, int i3, int i4 >
			friend GSOUND_FORCE_INLINE SIMDScalar shuffle( const SIMDScalar& scalar1, const SIMDScalar& scalar2 );
			
			
			friend GSOUND_FORCE_INLINE SIMDScalar<float,4> select( const SIMDScalar& selector,
											const SIMDScalar<float,4>& scalar1, const SIMDScalar<float,4>& scalar2 );
			
			
			friend GSOUND_FORCE_INLINE SIMDScalar<int,4> select( const SIMDScalar& selector,
											const SIMDScalar<int,4>& scalar1, const SIMDScalar<int,4>& scalar2 );
			
			
			friend GSOUND_FORCE_INLINE SIMDScalar select( const SIMDScalar& selector, const SIMDScalar& scalar1, const SIMDScalar& scalar2 );
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Free Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Pick 4 elements from the specified SIMD scalar and return the result.
template < int i1, int i2, int i3, int i4 >
GSOUND_FORCE_INLINE SIMDScalar<Bool,4> shuffle( const SIMDScalar<Bool,4>& scalar )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Bool,4>( scalar[i1], scalar[i2], scalar[i3], scalar[i4] );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Bool,4>( _mm_shuffle_ps( scalar.vFloat, scalar.vFloat, _MM_SHUFFLE(i4, i3, i2, i1) ) );
#else
	return SIMDScalar<Bool,4>( scalar[i1], scalar[i2], scalar[i3], scalar[i4] );
#endif
}




/// Pick two elements from each SIMD scalar and return the result.
template < int i1, int i2, int i3, int i4 >
GSOUND_FORCE_INLINE SIMDScalar<Bool,4> shuffle( const SIMDScalar<Bool,4>& scalar1, const SIMDScalar<Bool,4>& scalar2 )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Bool,4>( scalar1[i1], scalar1[i2], scalar2[i3], scalar2[i4] );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Bool,4>( _mm_shuffle_ps( scalar1.vFloat, scalar2.vFloat, _MM_SHUFFLE(i4, i3, i2, i1) ) );
#else
	return SIMDScalar<Bool,4>( scalar1[i1], scalar1[i2], scalar2[i3], scalar2[i4] );
#endif
}




/// Select elements from the first SIMD scalar if the selector is TRUE, otherwise from the second.
GSOUND_FORCE_INLINE SIMDScalar<Bool,4> select( const SIMDScalar<Bool,4>& selector,
												const SIMDScalar<Bool,4>& scalar1, const SIMDScalar<Bool,4>& scalar2 )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Bool,4>( vec_sel( scalar2.v, scalar1.v, selector.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	// (((b^a) & selector) ^ a)
	return SIMDScalar<Bool,4>( _mm_xor_ps( scalar2.vFloat,
										_mm_and_ps( selector.vFloat,
													_mm_xor_ps( scalar1.vFloat, scalar2.vFloat ) ) ) );
#else
	return SIMDScalar<Bool,4>( selector.a ? scalar1.a : scalar2.a,
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


#endif // INCLUDE_GSOUND_SIMD_SCALAR_BOOL_4_H
