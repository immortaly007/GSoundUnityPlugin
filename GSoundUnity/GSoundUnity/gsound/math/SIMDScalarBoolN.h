/*
 * Project:     GSound
 * 
 * File:        gsound/math/SIMDScalarBoolN.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::SIMDScalar class specialization for a N-bool SIMD data type.
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


#ifndef INCLUDE_GSOUND_SIMD_SCALAR_BOOL_N_H
#define INCLUDE_GSOUND_SIMD_SCALAR_BOOL_N_H


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
/// A class representing an N-component pseudo-boolean SIMD vector.
/**
  * This specialization of the SIMDScalar class uses 32-bit integer values
  * which each semantically represent a single boolean value where all 1's is
  * TRUE and all 0's is FALSE.
  */
template < Size width >
class GSOUND_ALIGN(16) SIMDScalar<Bool,width>
{
	private:
		
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
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					v[i] = simdValue;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Cast Operator
			
			
			
			
			/// Return a boolean value which indicates whether or not any of the components are true.
			GSOUND_FORCE_INLINE operator Bool () const
			{
				Bool result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result |= v[i].getMask() != 0;
				
				return result;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Accessor Methods
			
			
			
			
			/// Get the element at the specified index in the vector.
			GSOUND_FORCE_INLINE Bool operator [] ( Index i ) const
			{
				return (Bool)*((const UInt32*)&v);
			}
			
			
			
			
			/// Get the width of this scalar (number of components it has).
			GSOUND_FORCE_INLINE static Size getWidth()
			{
				return width;
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
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] == scalar.v[i];
				
				return result;
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
				SIMDScalar result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] == simdValue;
				
				return result;
			}
			
			
			
			
			
			/// Compare two 4D SIMD booleans component-wise for inequality
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * If each corresponding pair of components is not equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator != ( const SIMDScalar& scalar ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] != scalar.v[i];
				
				return result;
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
				SIMDScalar result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] != simdValue;
				
				return result;	
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Logical Operators
			
			
			
			
			/// Return the bitwise NOT of this 4D SIMD vector.
			GSOUND_FORCE_INLINE SIMDScalar operator ~ () const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = ~v[i];
				
				return result;
			}
			
			
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with another and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator & ( const SIMDScalar& scalar ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] & scalar.v[i];
				
				return result;
			}
			
			
			
			
			/// Compute the bitwise OR of this 4D SIMD vector with another and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator | ( const SIMDScalar& scalar ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] | scalar.v[i];
				
				return result;
			}
			
			
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator ^ ( const SIMDScalar& scalar ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] ^ scalar.v[i];
				
				return result;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Logical Assignment Operators
			
			
			
			
			/// Compute the logical AND of this 4D SIMD vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDScalar operator &= ( const SIMDScalar& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] &= scalar.v[i];
				
				return *this;
			}
			
			
			
			
			/// Compute the logical OR of this 4D SIMD vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDScalar operator |= ( const SIMDScalar& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] |= scalar.v[i];
				
				return *this;
			}
			
			
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDScalar operator ^= ( const SIMDScalar& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] ^= scalar.v[i];
				
				return *this;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Data Members
			
			
			
			
			/// The width of the underlying SIMD type used.
			static const Size SIMD_WIDTH = 4;
			
			
			
			
			/// The number of SIMD processing iterations that must occur.
			static const Size numIterations = SIMD_WIDTH*(width / SIMD_WIDTH) == width ? width / SIMD_WIDTH : width / SIMD_WIDTH + 1;
			
			
			
			
			/// The underlying SIMD type used to implement this class.
			typedef SIMDScalar<Bool,SIMD_WIDTH> SIMDBaseType;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// An array of SIMD values that simulate an N-wide SIMD register.
			SIMDBaseType v[numIterations];
			
			
			
			
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
			friend class SIMDScalar<float,width>;
			
			
			
			
			/// Declare the integer point version of this class as a friend.
			/**
			  * This is done because the integer version needs to access the
			  * private constructor of this class for speed when storing the results
			  * of SIMD comparisons.
			  */
			friend class SIMDScalar<int,width>;
			
			
			
			template < Size width2 >
			friend GSOUND_FORCE_INLINE SIMDScalar<float,width2> select( const SIMDScalar<Bool,width2>& selector,
											const SIMDScalar<float,width2>& scalar1, const SIMDScalar<float,width2>& scalar2 );
			
			
			template < Size width2 >
			friend GSOUND_FORCE_INLINE SIMDScalar<int,4> select( const SIMDScalar<Bool,width2>& selector,
											const SIMDScalar<int,width2>& scalar1, const SIMDScalar<int,width2>& scalar2 );
			
			
			template < Size width2 >
			friend GSOUND_FORCE_INLINE SIMDScalar<Bool,width2> select( const SIMDScalar<Bool,width2>& selector,
											const SIMDScalar<Bool,width2>& scalar1, const SIMDScalar<Bool,width2>& scalar2 );
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Free Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Select elements from the first SIMD scalar if the selector is TRUE, otherwise from the second.
template < Size width >
GSOUND_FORCE_INLINE SIMDScalar<Bool,width> select( const SIMDScalar<Bool,width>& selector,
												const SIMDScalar<Bool,width>& scalar1, const SIMDScalar<Bool,width>& scalar2 )
{
	SIMDScalar<Bool,width> result;
	
	for ( Index i = 0; i < SIMDScalar<Bool,width>::numIterations; i++ )
		result.v[i] = math::select( selector.v[i], scalar1.v[i], scalar2.v[i] );
	
	return result;
}




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SIMD_SCALAR_BOOL_N_H
