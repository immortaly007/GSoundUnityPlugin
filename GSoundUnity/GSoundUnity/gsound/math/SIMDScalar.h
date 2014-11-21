/*
 * Project:     GSound
 * 
 * File:        gsound/math/SIMDScalar.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::SIMDScalar class declaration and related functions
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


#ifndef INCLUDE_GSOUND_SIMD_SCALAR_H
#define INCLUDE_GSOUND_SIMD_SCALAR_H


#include "GSoundMathConfig.h"


#include "ScalarMath.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// The prototype for the SIMDScalar class.
/**
  * This template prototype is a scalar implementation of basic SIMD functionality.
  * Supported operations include all basic arithmetic operations
  * (+, -, *, /, +=, -=, *=, /=) and functions that operate on SIMDScalar instances
  * to find the per-component result for abs(), min(), max(), and sqrt() operations.
  *
  * This implementation is provided as a fallback for when SIMD instructions are not
  * available and for completeness. On any given platform, specializations for this 
  * class/functions should be implemented which use real SIMD operations.
  */
template < typename T, Size width >
class SIMDScalar
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			/// Create a scalar with it's elements equal to zero
			GSOUND_FORCE_INLINE SIMDScalar()
			{
				for ( Index i = 0; i < width; i++ )
					x[i] = T(0);
			}
			
			
			
			/// Create a scalar with it's elements equal to zero
			GSOUND_FORCE_INLINE SIMDScalar( T value )
			{
				for ( Index i = 0; i < width; i++ )
					x[i] = value;
			}
			
			
			
			/// Create a scalar with elements from the specified array.
			/**
			  * The array must be of length greater than or equal to the
			  * width of the scalar.
			  */
			GSOUND_FORCE_INLINE SIMDScalar( const T* array )
			{
				for ( Index i = 0; i < width; i++ )
					x[i] = array[i];
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Accessor Methods
			
			
			
			
			/// Return an array representation of this scalar.
			GSOUND_FORCE_INLINE const T* toArray() const
			{
				return x;
			}
			
			
			
			
			/// Get the element at the specified index in the scalar.
			GSOUND_FORCE_INLINE T& get( Index i )
			{
				return x[i];
			}
			
			
			
			
			/// Get the element at the specified index in the scalar.
			GSOUND_FORCE_INLINE const T& get( Index i ) const
			{
				return x[i];
			}
			
			
			
			
			/// Get the element at the specified index in the scalar.
			GSOUND_FORCE_INLINE T& operator () ( Index i )
			{
				return x[i];
			}
			
			
			
			
			/// Get the element at the specified index in the scalar.
			GSOUND_FORCE_INLINE const T& operator () ( Index i ) const
			{
				return x[i];
			}
			
			
			
			
			/// Get the element at the specified index in the scalar.
			GSOUND_FORCE_INLINE T& operator [] ( Index i )
			{
				return x[i];
			}
			
			
			
			
			/// Get the element at the specified index in the scalar.
			GSOUND_FORCE_INLINE const T& operator [] ( Index i ) const
			{
				return x[i];
			}
			
			
			
			
			/// Get the width of this scalar (number of components it has).
			GSOUND_FORCE_INLINE static Size getWidth()
			{
				return width;
			}
			
			
			
			
			/// Set the element at the specified index in the scalar.
			GSOUND_FORCE_INLINE void set( Index i, T newX )
			{
				x[i] = newX;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sum Method
			
			
			
			
			/// Return the horizontal sum of all components of this SIMD scalar.
			GSOUND_FORCE_INLINE T sum() const
			{
				T total = T(0);
				
				for ( Index i = 0; i < width; i++ )
					total += x[i];
				
				return total;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			
			
			/// Negate every component of this scalar and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator - () const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < width; i++ )
					result.x[i] = -x[i];
				
				return result;
			}
			
			
			
			
			/// 'Positivate' every component of this scalar, effectively returning a copy.
			GSOUND_FORCE_INLINE SIMDScalar operator + () const
			{
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			
			
			/// Add a scalar to this scalar and return the resulting scalar.
			GSOUND_FORCE_INLINE SIMDScalar operator + ( const SIMDScalar& scalar ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < width; i++ )
					result.x[i] = x[i] + scalar.x[i];
				
				return result;
			}
			
			
			
			
			/// Add a scalar value to each component of this scalar and return the resulting scalar.
			GSOUND_FORCE_INLINE SIMDScalar operator + ( const T value ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < width; i++ )
					result.x[i] = x[i] + value;
				
				return result;
			}
			
			
			
			
			/// Subtract a scalar from this scalar and return the resulting scalar.
			GSOUND_FORCE_INLINE SIMDScalar operator - ( const SIMDScalar& scalar ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < width; i++ )
					result.x[i] = x[i] - scalar.x[i];
				
				return result;
			}
			
			
			
			
			/// Subtract a scalar value from each component of this scalar and return the resulting scalar.
			GSOUND_FORCE_INLINE SIMDScalar operator - ( const T value ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < width; i++ )
					result.x[i] = x[i] - value;
				
				return result;
			}
			
			
			
			
			/// Return the result of a component-wise scalar multiplication with this scalar.
			GSOUND_FORCE_INLINE SIMDScalar operator * ( const SIMDScalar& scalar ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < width; i++ )
					result.x[i] = x[i]*scalar.x[i];
				
				return result;
			}
			
			
			
			
			/// Multiply a scalar value by each component of this scalar and return the resulting scalar.
			GSOUND_FORCE_INLINE SIMDScalar operator * ( const T value ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < width; i++ )
					result.x[i] = x[i]*value;
				
				return result;
			}
			
			
			
			
			/// Divide each component of this scalar by a scalar value and return the resulting scalar.
			GSOUND_FORCE_INLINE SIMDScalar operator / ( const T value ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < width; i++ )
					result.x[i] = x[i] / value;
				
				return result;
			}
			
			
			
			
			/// Divide each component of this scalar by a scalar component and return the resulting scalar.
			GSOUND_FORCE_INLINE SIMDScalar operator / ( const SIMDScalar& scalar ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < width; i++ )
					result.x[i] = x[i] / scalar.x[i];
				
				return result;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			
			
			GSOUND_FORCE_INLINE SIMDScalar& operator += ( const SIMDScalar& v2 )
			{
				for ( Index i = 0; i < width; i++ )
					x[i] += v2.x[i];
				
				return *this;
			}
			
			
			
			
			GSOUND_FORCE_INLINE SIMDScalar& operator += ( const T value )
			{
				for ( Index i = 0; i < width; i++ )
					x[i] += value;
				
				return *this;
			}
			
			
			
			
			GSOUND_FORCE_INLINE SIMDScalar& operator -= ( const SIMDScalar& v2 )
			{
				for ( Index i = 0; i < width; i++ )
					x[i] -= v2.x[i];
				
				return *this;
			}
			
			
			
			
			GSOUND_FORCE_INLINE SIMDScalar& operator -= ( const T value )
			{
				for ( Index i = 0; i < width; i++ )
					x[i] -= value;
				
				return *this;
			}
			
			
			
			
			GSOUND_FORCE_INLINE SIMDScalar& operator *= ( const SIMDScalar& scalar )
			{
				for ( Index i = 0; i < width; i++ )
					x[i] *= scalar.x[i];
				
				return *this;
			}
			
			
			
			
			GSOUND_FORCE_INLINE SIMDScalar& operator *= ( const T value )
			{
				for ( Index i = 0; i < width; i++ )
					x[i] *= value;
				
				return *this;
			}
			
			
			
			
			GSOUND_FORCE_INLINE SIMDScalar& operator /= ( const SIMDScalar& scalar )
			{
				for ( Index i = 0; i < width; i++ )
					x[i] /= scalar.x[i];
				
				return *this;
			}
			
			
			
			
			GSOUND_FORCE_INLINE SIMDScalar& operator /= ( const T value )
			{
				for ( Index i = 0; i < width; i++ )
					x[i] /= value;
				
				return *this;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The components of this SIMD scalar.
			T x[width];
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Friend Function Declarations
			
			
			
			
			template < typename U, Size dim >
			friend SIMDScalar<U,dim> operator + ( const U value, const SIMDScalar<T,dim>& scalar );
			
			template < typename U, Size dim >
			friend SIMDScalar<U,dim> operator - ( const U value, const SIMDScalar<T,dim>& scalar );
			
			template < typename U, Size dim >
			friend SIMDScalar<U,dim> operator * ( const U value, const SIMDScalar<T,dim>& scalar );
			
			template < typename U, Size dim >
			friend SIMDScalar<U,dim> operator / ( const U value, const SIMDScalar<T,dim>& scalar );
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Associative SIMD Scalar Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a scalar value to each component of this scalar and return the resulting scalar.
template < typename T, Size width >
GSOUND_FORCE_INLINE SIMDScalar<T,width> operator + ( const T value, const SIMDScalar<T,width>& scalar )
{
	SIMDScalar<T,width> result;
	
	for ( Index i = 0; i < width; i++ )
		result.x[i] = value + scalar.x[i];
	
	return result;
}




/// Subtract a scalar value from each component of this scalar and return the resulting scalar.
template < typename T, Size width >
GSOUND_FORCE_INLINE SIMDScalar<T,width> operator - ( const T value, const SIMDScalar<T,width>& scalar )
{
	SIMDScalar<T,width> result;
	
	for ( Index i = 0; i < width; i++ )
		result.x[i] = value - scalar.x[i];
	
	return result;
}




/// Multiply a scalar value by each component of this scalar and return the resulting scalar.
template < typename T, Size width >
GSOUND_FORCE_INLINE SIMDScalar<T,width> operator * ( const T value, const SIMDScalar<T,width>& scalar )
{
	SIMDScalar<T,width> result;
	
	for ( Index i = 0; i < width; i++ )
		result.x[i] = value*scalar.x[i];
	
	return result;
}




/// Divide each component of this scalar by a scalar value and return the resulting scalar.
template < typename T, Size width >
GSOUND_FORCE_INLINE SIMDScalar<T,width> operator / ( const T value, const SIMDScalar<T,width>& scalar )
{
	SIMDScalar<T,width> result;
	
	for ( Index i = 0; i < width; i++ )
		result.x[i] = value / scalar.x[i];
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Free Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Compute the absolute value of each component of the specified SIMD scalar and return the result.
template < typename T, Size width >
GSOUND_FORCE_INLINE SIMDScalar<T,width> abs( const SIMDScalar<T,width>& scalar )
{
	SIMDScalar<T,width> result;
	
	for ( Index i = 0; i < width; i++ )
		result.x[i] = math::abs(scalar.x[i]);
	
	return result;
}




/// Compute the square root of each component of the specified SIMD scalar and return the result.
template < typename T, Size width >
GSOUND_FORCE_INLINE SIMDScalar<T,width> sqrt( const SIMDScalar<T,width>& scalar )
{
	SIMDScalar<T,width> result;
	
	for ( Index i = 0; i < width; i++ )
		result.x[i] = math::sqrt(scalar.x[i]);
	
	return result;
}




/// Compute the minimum of each component of the specified SIMD scalars and return the result.
template < typename T, Size width >
GSOUND_FORCE_INLINE SIMDScalar<T,width> min( const SIMDScalar<T,width>& scalar1,
												const SIMDScalar<T,width>& scalar2 )
{
	SIMDScalar<T,width> result;
	
	for ( Index i = 0; i < width; i++ )
		result.x[i] = math::min( scalar1.x[i], scalar2.x[i] );
	
	return result;
}




/// Compute the maximum of each component of the specified SIMD scalars and return the result.
template < typename T, Size width >
GSOUND_FORCE_INLINE SIMDScalar<T,width> max( const SIMDScalar<T,width>& scalar1,
												const SIMDScalar<T,width>& scalar2 )
{
	SIMDScalar<T,width> result;
	
	for ( Index i = 0; i < width; i++ )
		result.x[i] = math::max( scalar1.x[i], scalar2.x[i] );
	
	return result;
}



//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SIMD_SCALAR_H
