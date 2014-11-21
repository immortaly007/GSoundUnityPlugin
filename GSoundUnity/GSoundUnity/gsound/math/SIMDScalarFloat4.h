/*
 * Project:     GSound
 * 
 * File:        gsound/math/SIMDScalarFloat4.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::SIMDScalar class specialization for a 4-float SIMD data type.
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


#ifndef INCLUDE_GSOUND_SIMD_SCALAR_FLOAT_4_H
#define INCLUDE_GSOUND_SIMD_SCALAR_FLOAT_4_H


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
/// A class representing a 4-component 32-bit floating-point SIMD scalar.
/**
  * This specialization of the SIMDScalar class uses a 128-bit value to encode
  * 4 32-bit floating-point values. All basic arithmetic operations are supported,
  * plus a subset of standard scalar operations: abs(), min(), max(), sqrt().
  */
template <>
class GSOUND_ALIGN(16) SIMDScalar<Float32,4>
{
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Type Declarations
			
			
			
			
			/// Define the type for a 4x float scalar structure.
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
			typedef GSOUND_ALTIVEC_VECTOR float SIMDScalar4;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
			typedef __m128 SIMDScalar4;
#endif
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Constructor
			
			
			
			
#if GSOUND_USE_SIMD && (defined(GSOUND_SIMD_ALTIVEC) || defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0))
			/// Create a new 4D scalar with the specified 4D SIMD scalar value.
			GSOUND_FORCE_INLINE SIMDScalar( SIMDScalar4 simdScalar )
				:	v( simdScalar )
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
			GSOUND_FORCE_INLINE SIMDScalar( Float32 value )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = (SIMDScalar4)(value);
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				v = _mm_set1_ps( value );
#else
				a = b = c = d = value;
#endif
			}
			
			
			
			
			/// Create a new 4D SIMD scalar with the elements equal to the specified 4 values.
			GSOUND_FORCE_INLINE SIMDScalar( Float32 newA, Float32 newB, Float32 newC, Float32 newD )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = (SIMDScalar4)( newA, newB, newC, newD );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				// The parameters are reversed to keep things consistent with loading from an address.
				v = _mm_set_ps( newD, newC, newB, newA );
#else
				a = newA;
				b = newB;
				c = newC;
				d = newD;
#endif
			}
			
			
			
			
			/// Create a new 4D SIMD scalar with the first 3 elements equal to the specified vector's components.
			/**
			  * The last element of the SIMD scalar is initialized to 0.
			  */
			GSOUND_FORCE_INLINE SIMDScalar( const Vector3D<Float32>& vector )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				v = (SIMDScalar4)( vector.x, vector.y, vector.z, Float32(0) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				// The parameters are reversed to keep things consistent with loading from an address.
				v = _mm_set_ps( Float32(0), vector.z, vector.y, vector.x );
#else
				a = vector.x;
				b = vector.y;
				c = vector.z;
				d = 0;
#endif
			}
			
			
			
			
			/// Create a new 4D SIMD scalar from the first 4 values stored at specified pointer's location.
			GSOUND_FORCE_INLINE SIMDScalar( const Float32* array )
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				a = array[0];
				b = array[1];
				c = array[2];
				d = array[3];
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				v = _mm_load_ps( array );
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
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				v = other.v;
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
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				v = other.v;
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
		//******	Accessor Methods
			
			
			
			
			/// Get a reference to the value stored at the specified component index in this scalar.
			GSOUND_FORCE_INLINE Float32& operator [] ( Index i )
			{
				return x[i];
			}
			
			
			
			
			/// Get the value stored at the specified component index in this scalar.
			GSOUND_FORCE_INLINE Float32 operator [] ( Index i ) const
			{
				return x[i];
			}
			
			
			
			
			/// Get a pointer to the first element in this scalar.
			/**
			  * The remaining values are in the next 3 locations after the
			  * first element.
			  */
			GSOUND_FORCE_INLINE const Float32* toArray() const
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
		//******	Comparison Operators
			
			
			
			
			/// Compare two 4D SIMD scalars component-wise for equality.
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components is equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator == ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpeq( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmpeq_ps( v, scalar.v ) );
#else
				return SIMDScalar<Bool,4>( a == scalar.a, b == scalar.b, c == scalar.c, d == scalar.d );
#endif
			}
			
			
			
			
			/// Compare this scalar to a single floating point value for equality.
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components is equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator == ( const Float32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpeq( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmpeq_ps( v, _mm_set1_ps( value ) ) );
#else
				return SIMDScalar<Bool,4>( a == value, b == value, c == value, d == value );
#endif
			}
			
			
			
			
			
			/// Compare two 4D SIMD scalars component-wise for inequality
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components is not equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator != ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				const GSOUND_ALTIVEC_VECTOR bool temp = vec_cmpeq( v, scalar.v );
				return SIMDScalar<Bool,4>( vec_nor( temp, temp ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmpneq_ps( v, scalar.v ) );
#else
				return SIMDScalar<Bool,4>( a != scalar.a, b != scalar.b, c != scalar.c, d != scalar.d );
#endif
			}
			
			
			
			
			/// Compare this scalar to a single floating point value for inequality.
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components is not equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator != ( const Float32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				const GSOUND_ALTIVEC_VECTOR bool temp = vec_cmpeq( v, (SIMDScalar4)(value) );
				return SIMDScalar<Bool,4>( vec_nor( temp, temp ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmpneq_ps( v, _mm_set1_ps( value ) ) );
#else
				return SIMDScalar<Bool,4>( a != value, b != value, c != value, d != value );
#endif
			}
			
			
			
			
			/// Perform a component-wise less-than comparison between this an another 4D SIMD scalar.
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component less than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator < ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmplt( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmplt_ps( v, scalar.v ) );
#else
				return SIMDScalar<Bool,4>( a < scalar.a, b < scalar.b, c < scalar.c, d < scalar.d );
#endif
			}
			
			
			
			
			/// Perform a component-wise less-than comparison between this 4D SIMD scalar and an expanded scalar.
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component less than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator < ( const Float32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmplt( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmplt_ps( v, _mm_set1_ps( value ) ) );
#else
				return SIMDScalar<Bool,4>( a < value, b < value, c < value, d < value );
#endif
			}
			
			
			
			
			/// Perform a component-wise greater-than comparison between this an another 4D SIMD scalar.
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator > ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpgt( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmpgt_ps( v, scalar.v ) );
#else
				return SIMDScalar<Bool,4>( a > scalar.a, b > scalar.b, c > scalar.c, d > scalar.d );
#endif
			}
			
			
			
			
			/// Perform a component-wise greater-than comparison between this 4D SIMD scalar and an expanded scalar.
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator > ( const Float32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpgt( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmpgt_ps( v, _mm_set1_ps( value ) ) );
#else
				return SIMDScalar<Bool,4>( a > value, b > value, c > value, d > value );
#endif
			}
			
			
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this an another 4D SIMD scalar.
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component less than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator <= ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmple( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmple_ps( v, scalar.v ) );
#else
				return SIMDScalar<Bool,4>( a <= scalar.a, b <= scalar.b, c <= scalar.c, d <= scalar.d );
#endif
			}
			
			
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component less than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator <= ( const Float32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmple( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmple_ps( v, _mm_set1_ps( value ) ) );
#else
				return SIMDScalar<Bool,4>( a <= value, b <= value, c <= value, d <= value );
#endif
			}
			
			
			
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this an another 4D SIMD scalar.
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator >= ( const SIMDScalar& scalar ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpge( v, scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmpge_ps( v, scalar.v ) );
#else
				return SIMDScalar<Bool,4>( a >= scalar.a, b >= scalar.b, c >= scalar.c, d >= scalar.d );
#endif
			}
			
			
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			/**
			  * Return a 4D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator >= ( const Float32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar<Bool,4>( vec_cmpge( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Bool,4>( _mm_cmpge_ps( v, _mm_set1_ps( value ) ) );
#else
				return SIMDScalar<Bool,4>( a >= value, b >= value, c >= value, d >= value );
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sum Method
			
			
			
			
			GSOUND_FORCE_INLINE Float32 sum() const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return a + b + c + d;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(3,0)
				__m128 temp = _mm_hadd_ps( v, v );
				return SIMDScalar( _mm_hadd_ps( temp, temp ) ).a;
#else
				return a + b + c + d;
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Store Method
			
			
			
			
			GSOUND_FORCE_INLINE void store( Float32* destination ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				destination[0] = a;
				destination[1] = b;
				destination[2] = c;
				destination[3] = d;
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				_mm_store_ps( destination, v );
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
				return SIMDScalar( vec_sub( (SIMDScalar4)(Float32(0)), v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_sub_ps( _mm_set1_ps(Float32(0)), v ) );
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
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_add_ps( v, scalar.v ) );
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
			GSOUND_FORCE_INLINE SIMDScalar operator + ( const Float32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_add( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_add_ps( v, _mm_set1_ps(value) ) );
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
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_sub_ps( v, scalar.v ) );
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
			GSOUND_FORCE_INLINE SIMDScalar operator - ( const Float32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( v, (SIMDScalar4)(value) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_sub_ps( v, _mm_set1_ps(value) ) );
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
				return SIMDScalar( vec_madd( v, scalar.v, (SIMDScalar4)(float(0)) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_mul_ps( v, scalar.v ) );
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
			GSOUND_FORCE_INLINE SIMDScalar operator * ( const Float32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_madd( v, (SIMDScalar4)(value),
												(SIMDScalar4)(Float32(0)) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_mul_ps( v, _mm_set1_ps(value) ) );
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
				//Get the reciprocal estimate
				SIMDScalar4 reciprocalEstimate = vec_re( scalar.v );
				
				//One round of Newton-Raphson refinement
				SIMDScalar4 reciprocal = vec_madd( vec_nmsub( reciprocalEstimate, v, 
														(SIMDScalar4)(1) ), reciprocalEstimate, reciprocalEstimate );
				
				return SIMDScalar( vec_madd( v, reciprocal, ((SIMDScalar4)(0)) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_div_ps( v, scalar.v ) );
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
			GSOUND_FORCE_INLINE SIMDScalar operator / ( const Float32 value ) const
			{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
				return SIMDScalar( vec_madd( v, ((SIMDScalar4)(Float32(1) / value)), ((SIMDScalar4)(0)) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_mul_ps( v, _mm_set1_ps(Float32(1) / value) ) );
#else
				Float32 inverse = Float32(1) / value;
				return SIMDScalar( a*inverse, b*inverse, c*inverse, d*inverse );
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
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				v = _mm_add_ps( v, scalar.v );
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
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				v = _mm_sub_ps( v, scalar.v );
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
				v = vec_madd( v, scalar.v, (SIMDScalar4)(Float32(0)) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				v = _mm_mul_ps( v, scalar.v );
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
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
				v = _mm_div_ps( v, scalar.v );
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
#if GSOUND_USE_SIMD && (defined(GSOUND_SIMD_ALTIVEC) || defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0))
				/// The 4D SIMD vector used internally.
				SIMDScalar4 v;
#endif
				
				struct
				{
					/// The A component of a 4D SIMD scalar.
					Float32 a;
					
					
					
					/// The B component of a 4D SIMD scalar.
					Float32 b;
					
					
					
					/// The C component of a 4D SIMD scalar.
					Float32 c;
					
					
					
					/// The D component of a 4D SIMD scalar.
					Float32 d;
				};
				
				
				/// The components of a 4D SIMD scalar in array format.
				Float32 x[4];
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
GSOUND_FORCE_INLINE SIMDScalar<Float32,4> operator + ( const Float32 value, const SIMDScalar<Float32,4>& scalar )
{
	return SIMDScalar<Float32,4>(value) + scalar;
}




/// Subtract a scalar value from each component of this scalar and return the resulting scalar.
GSOUND_FORCE_INLINE SIMDScalar<Float32,4> operator - ( const Float32 value, const SIMDScalar<Float32,4>& scalar )
{
	return SIMDScalar<Float32,4>(value) - scalar;
}




/// Multiply a scalar value by each component of this scalar and return the resulting scalar.
GSOUND_FORCE_INLINE SIMDScalar<Float32,4> operator * ( const Float32 value, const SIMDScalar<Float32,4>& scalar )
{
	return SIMDScalar<Float32,4>(value) * scalar;
}




/// Divide each component of this scalar by a scalar value and return the resulting scalar.
GSOUND_FORCE_INLINE SIMDScalar<Float32,4> operator / ( const Float32 value, const SIMDScalar<Float32,4>& scalar )
{
	return SIMDScalar<Float32,4>(value) / scalar;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Free Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Compute the absolute value of each component of the specified SIMD scalar and return the result.
GSOUND_FORCE_INLINE SIMDScalar<Float32,4> abs( const SIMDScalar<Float32,4>& scalar )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_abs( scalar.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	GSOUND_ALIGN(16) const UInt32 absMask[4] = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF };
	return SIMDScalar<Float32,4>( _mm_and_ps( scalar.v, _mm_load_ps( reinterpret_cast<const Float32*>(absMask) ) ) );
#else
	return SIMDScalar<Float32,4>( math::abs(scalar.a), math::abs(scalar.b), math::abs(scalar.c), math::abs(scalar.d) );
#endif
}




/// Compute the square root of each component of the specified SIMD scalar and return the result.
GSOUND_FORCE_INLINE SIMDScalar<Float32,4> sqrt( const SIMDScalar<Float32,4>& scalar )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	// Get the square root reciprocal estimate
	GSOUND_ALTIVEC_VECTOR float zero = (GSOUND_ALTIVEC_VECTOR float)(0);
	GSOUND_ALTIVEC_VECTOR float oneHalf = (GSOUND_ALTIVEC_VECTOR float)(0.5);
	GSOUND_ALTIVEC_VECTOR float one = (GSOUND_ALTIVEC_VECTOR float)(1.0);
	GSOUND_ALTIVEC_VECTOR float estimate = vec_rsqrte( scalar.v );
	
	// One round of Newton-Raphson refinement
	GSOUND_ALTIVEC_VECTOR float estimateSquared = vec_madd( estimate, estimate, zero );
	GSOUND_ALTIVEC_VECTOR float halfEstimate = vec_madd( estimate, oneHalf, zero );
	
	GSOUND_ALTIVEC_VECTOR float reciprocalSquareRoot = vec_madd( vec_nmsub( scalar.v, estimateSquared, one ), halfEstimate, estimate );
	
	return SIMDScalar<Float32,4>( vec_madd( scalar.v, reciprocalSquareRoot, (GSOUND_ALTIVEC_VECTOR float)(0) ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_sqrt_ps( scalar.v ) );
#else
	return SIMDScalar<Float32,4>( math::sqrt(scalar.a), math::sqrt(scalar.b), math::sqrt(scalar.c), math::sqrt(scalar.d) );
#endif
}




/// Compute the minimum of each component of the specified SIMD scalars and return the result.
GSOUND_FORCE_INLINE SIMDScalar<Float32,4> min( const SIMDScalar<Float32,4>& scalar1, const SIMDScalar<Float32,4>& scalar2 )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_min( scalar1.v, scalar2.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_min_ps( scalar1.v, scalar2.v ) );
#else
	return SIMDScalar<Float32,4>( math::min(scalar1.a, scalar2.a),
								math::min(scalar1.b, scalar2.b),
								math::min(scalar1.c, scalar2.c),
								math::min(scalar1.d, scalar2.d) );
#endif
}




/// Compute the maximum of each component of the specified SIMD scalars and return the result.
GSOUND_FORCE_INLINE SIMDScalar<Float32,4> max( const SIMDScalar<Float32,4>& scalar1, const SIMDScalar<Float32,4>& scalar2 )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_max( scalar1.v, scalar2.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_max_ps( scalar1.v, scalar2.v ) );
#else
	return SIMDScalar<Float32,4>( math::max(scalar1.a, scalar2.a),
								math::max(scalar1.b, scalar2.b),
								math::max(scalar1.c, scalar2.c),
								math::max(scalar1.d, scalar2.d) );
#endif
}




/// Pick 4 elements from the specified SIMD scalar and return the result.
template < UInt i1, UInt i2, UInt i3, UInt i4 >
GSOUND_FORCE_INLINE SIMDScalar<Float32,4> shuffle( const SIMDScalar<Float32,4>& scalar )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( scalar[i1], scalar[i2], scalar[i3], scalar[i4] );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_shuffle_ps( scalar.v, scalar.v, _MM_SHUFFLE(i4, i3, i2, i1) ) );
#else
	return SIMDScalar<Float32,4>( scalar[i1], scalar[i2], scalar[i3], scalar[i4] );
#endif
}




/// Pick two elements from each SIMD scalar and return the result.
template < UInt i1, UInt i2, UInt i3, UInt i4 >
GSOUND_FORCE_INLINE SIMDScalar<Float32,4> shuffle( const SIMDScalar<Float32,4>& scalar1, const SIMDScalar<Float32,4>& scalar2 )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( scalar1[i1], scalar1[i2], scalar2[i3], scalar2[i4] );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_shuffle_ps( scalar1.v, scalar2.v, _MM_SHUFFLE(i4, i3, i2, i1) ) );
#else
	return SIMDScalar<Float32,4>( scalar1[i1], scalar1[i2], scalar2[i3], scalar2[i4] );
#endif
}




/// Select elements from the first SIMD scalar if the selector is TRUE, otherwise from the second.
GSOUND_FORCE_INLINE SIMDScalar<Float32,4> select( const SIMDScalar<Bool,4>& selector,
												const SIMDScalar<Float32,4>& scalar1, const SIMDScalar<Float32,4>& scalar2 )
{
#if GSOUND_USE_SIMD && defined(GSOUND_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_sel( scalar2.v, scalar1.v, selector.v ) );
#elif GSOUND_USE_SIMD && defined(GSOUND_SIMD_SSE) && GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
	// (((b^a) & selector) ^ a)
	return SIMDScalar<Float32,4>( _mm_xor_ps( scalar2.v, _mm_and_ps( selector.vFloat, _mm_xor_ps( scalar1.v, scalar2.v ) ) ) );
#else
	return SIMDScalar<Float32,4>( selector.a ? scalar1.a : scalar2.a,
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


#endif // INCLUDE_GSOUND_SIMD_SCALAR_FLOAT_4_H
