/*
 * Project:     GSound
 * 
 * File:        gsound/math/SIMDScalarIntN.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::SIMDScalar class specialization for an N-int SIMD data type.
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


#ifndef INCLUDE_GSOUND_SIMD_SCALAR_INT_N_H
#define INCLUDE_GSOUND_SIMD_SCALAR_INT_N_H


#include "GSoundMathConfig.h"


#include "SIMDScalar.h"
#include "SIMDScalarInt4.h"
#include "SIMDScalarBoolN.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class representing an N-component 32-bit signed-integer SIMD scalar.
/**
  * This specialization of the SIMDScalar class uses one or more 4-component
  * SIMD values to simulate an N-wide SIMD register.
  */
template < Size width >
class GSOUND_ALIGN(16) SIMDScalar<Int32,width>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a new SIMD scalar with all elements left uninitialized.
			GSOUND_FORCE_INLINE SIMDScalar()
			{
			}
			
			
			
			
			/// Create a new SIMD scalar with all elements equal to the specified value.
			GSOUND_FORCE_INLINE SIMDScalar( Int32 value )
			{
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					v[i] = simdValue;
			}
			
			
			
			
			/// Create a new SIMD scalar from the first N values stored at specified pointer's location.
			GSOUND_FORCE_INLINE SIMDScalar( const Int32* array )
			{
				for ( Index i = 0; i < numIterations; i++ )
				{
					v[i] = SIMDBaseType( array );
					array += SIMD_WIDTH;
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Store Method
			
			
			
			
			/// Store this SIMD scalar starting at the specified destination pointer.
			GSOUND_FORCE_INLINE void store( Int32* destination ) const
			{
				for ( Index i = 0; i < numIterations; i++ )
				{
					v[i].store( destination );
					destination += SIMD_WIDTH;
				}
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Accessor Methods
			
			
			
			
			/// Get a reference to the value stored at the specified component index in this scalar.
			GSOUND_FORCE_INLINE Int32& operator [] ( Index i )
			{
				return ((Int32*)v)[i];
			}
			
			
			
			
			/// Get the value stored at the specified component index in this scalar.
			GSOUND_FORCE_INLINE Int32 operator [] ( Index i ) const
			{
				return ((const Int32*)v)[i];
			}
			
			
			
			
			/// Get a pointer to the first element in this scalar.
			/**
			  * The remaining values are in the next 3 locations after the
			  * first element.
			  */
			GSOUND_FORCE_INLINE const Int32* toArray() const
			{
				return (const Float32*)v;
			}
			
			
			
			
			/// Get the width of this scalar (number of components it has).
			GSOUND_FORCE_INLINE static Size getWidth()
			{
				return width;
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
			
			
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with a mask and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator & ( const SIMDScalar<Bool,width>& scalar ) const
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
			
			
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with a mask and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator | ( const SIMDScalar<Bool,width>& scalar ) const
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
			
			
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with a mask and return the result.
			GSOUND_FORCE_INLINE SIMDScalar operator ^ ( const SIMDScalar<Bool,width>& scalar ) const
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
			GSOUND_FORCE_INLINE SIMDScalar& operator &= ( const SIMDScalar& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] &= scalar.v[i];
				
				return *this;
			}
			
			
			
			
			/// Compute the logical OR of this 4D SIMD vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDScalar& operator |= ( const SIMDScalar& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] |= scalar.v[i];
				
				return *this;
			}
			
			
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and assign it to this vector.
			GSOUND_FORCE_INLINE SIMDScalar& operator ^= ( const SIMDScalar& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] ^= scalar.v[i];
				
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
			GSOUND_FORCE_INLINE SIMDScalar<Bool,width> operator == ( const SIMDScalar& scalar ) const
			{
				SIMDScalar<Bool,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] == scalar.v[i];
				
				return result;
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
				SIMDScalar<Bool,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] == simdValue;
				
				return result;
			}
			
			
			
			
			
			/// Compare two 4D SIMD scalars component-wise for inequality
			/**
			  * Return a 4D scalar of booleans indicating the result of the comparison.
			  * If each corresponding pair of components is not equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			GSOUND_FORCE_INLINE SIMDScalar<Bool,4> operator != ( const SIMDScalar& scalar ) const
			{
				SIMDScalar<Bool,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] != scalar.v[i];
				
				return result;
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
				SIMDScalar<Bool,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] != simdValue;
				
				return result;
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
				SIMDScalar<Bool,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] < scalar.v[i];
				
				return result;
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
				SIMDScalar<Bool,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] < simdValue;
				
				return result;
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
				SIMDScalar<Bool,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] > scalar.v[i];
				
				return result;
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
				SIMDScalar<Bool,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] > simdValue;
				
				return result;
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
				SIMDScalar<Bool,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] <= scalar.v[i];
				
				return result;
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
				SIMDScalar<Bool,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] <= simdValue;
				
				return result;
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
				SIMDScalar<Bool,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] >= scalar.v[i];
				
				return result;
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
				SIMDScalar<Bool,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] >= simdValue;
				
				return result;
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
			GSOUND_FORCE_INLINE SIMDScalar operator << ( Int bitShift ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] << bitShift;
				
				return result;
			}
			
			
			
			
			/// Shift each component of the SIMD scalar to the right by the specified amount of bits.
			/**
			  * This method shifts the contents of each component to the right by the specified
			  * amount of bits and sign extends the original values..
			  * 
			  * @param bitShift - the number of bits to shift this SIMD scalar by.
			  * @return the shifted SIMD scalar.
			  */
			GSOUND_FORCE_INLINE SIMDScalar operator >> ( Int bitShift ) const
			{
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] >> bitShift;
				
				return result;
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
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = -v[i];
				
				return result;
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
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] + scalar.v[i];
				
				return result;
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
				SIMDScalar result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] + simdValue;
				
				return result;
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
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] - scalar.v[i];
				
				return result;
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
				SIMDScalar result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] - simdValue;
				
				return result;
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
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] * scalar.v[i];
				
				return result;
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
				SIMDScalar result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] * simdValue;
				
				return result;
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
				SIMDScalar result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] / scalar.v[i];
				
				return result;
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
				SIMDScalar result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] / simdValue;
				
				return result;
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
				for ( Index i = 0; i < numIterations; i++ )
					v[i] += scalar.v[i];
				
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
				for ( Index i = 0; i < numIterations; i++ )
					v[i] -= scalar.v[i];
				
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
				for ( Index i = 0; i < numIterations; i++ )
					v[i] *= scalar.v[i];
				
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
				for ( Index i = 0; i < numIterations; i++ )
					v[i] /= scalar.v[i];
				
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
			static const Size numIterations = SIMD_WIDTH*(width / SIMD_WIDTH) == width ? 
												width / SIMD_WIDTH : width / SIMD_WIDTH + 1;
			
			
			
			
			/// The underlying SIMD type used to implement this class.
			typedef SIMDScalar<Int32,SIMD_WIDTH> SIMDBaseType;
			
			
			
			
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
			
			
			
			
			template < typename T, Size dimension >
			friend class SIMDVector3D;
			
			
			template < Size width2 >
			friend GSOUND_FORCE_INLINE SIMDScalar<Int32,width2> abs( const SIMDScalar<Int32,width2>& scalar );
			
			template < Size width2 >
			friend GSOUND_FORCE_INLINE SIMDScalar<Int32,width2> sqrt( const SIMDScalar<Int32,width2>& scalar );
			
			template < Size width2 >
			friend GSOUND_FORCE_INLINE SIMDScalar<Int32,width2> min( const SIMDScalar<Int32,width2>& scalar1, const SIMDScalar<Int32,width2>& scalar2 );
			
			template < Size width2 >
			friend GSOUND_FORCE_INLINE SIMDScalar<Int32,width2> max( const SIMDScalar<Int32,width2>& scalar1, const SIMDScalar<Int32,width2>& scalar2 );
			
			template < Size width2 >
			friend GSOUND_FORCE_INLINE SIMDScalar<Int32,width2> select( const SIMDScalar<Bool,width2>& selector,
															const SIMDScalar<Int32,width2>& scalar1, const SIMDScalar<Int32,width2>& scalar2 );
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Associative SIMD Scalar Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a scalar value to each component of this scalar and return the resulting scalar.
template < Size width >
GSOUND_FORCE_INLINE SIMDScalar<Int32,width> operator + ( const Int32 value, const SIMDScalar<Int32,width>& scalar )
{
	return SIMDScalar<Int32,width>(value) + scalar;
}




/// Subtract a scalar value from each component of this scalar and return the resulting scalar.
template < Size width >
GSOUND_FORCE_INLINE SIMDScalar<Int32,width> operator - ( const Int32 value, const SIMDScalar<Int32,width>& scalar )
{
	return SIMDScalar<Int32,width>(value) - scalar;
}




/// Multiply a scalar value by each component of this scalar and return the resulting scalar.
template < Size width >
GSOUND_FORCE_INLINE SIMDScalar<Int32,width> operator * ( const Int32 value, const SIMDScalar<Int32,width>& scalar )
{
	return SIMDScalar<Int32,width>(value) * scalar;
}




/// Divide each component of this scalar by a scalar value and return the resulting scalar.
template < Size width >
GSOUND_FORCE_INLINE SIMDScalar<Int32,width> operator / ( const Int32 value, const SIMDScalar<Int32,width>& scalar )
{
	return SIMDScalar<Int32,width>(value) / scalar;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Free Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Compute the absolute value of each component of the specified SIMD scalar and return the result.
template < Size width >
GSOUND_FORCE_INLINE SIMDScalar<Int32,width> abs( const SIMDScalar<Int32,width>& scalar )
{
	SIMDScalar<Int32,width> result;
	
	for ( Index i = 0; i < SIMDScalar<Int32,width>::numIterations; i++ )
		result.v[i] = math::abs( scalar.v[i] );
	
	return result;
}




/// Compute the square root of each component of the specified SIMD scalar and return the result.
template < Size width >
GSOUND_FORCE_INLINE SIMDScalar<Int32,width> sqrt( const SIMDScalar<Int32,width>& scalar )
{
	SIMDScalar<Int32,width> result;
	
	for ( Index i = 0; i < SIMDScalar<Int32,width>::numIterations; i++ )
		result.v[i] = math::sqrt( scalar.v[i] );
	
	return result;
}




/// Compute the minimum of each component of the specified SIMD scalars and return the result.
template < Size width >
GSOUND_FORCE_INLINE SIMDScalar<Int32,width> min( const SIMDScalar<Int32,width>& scalar1, const SIMDScalar<Int32,width>& scalar2 )
{
	SIMDScalar<Int32,width> result;
	
	for ( Index i = 0; i < SIMDScalar<Int32,width>::numIterations; i++ )
		result.v[i] = math::min( scalar1.v[i], scalar2.v[i] );
	
	return result;
}




/// Compute the maximum of each component of the specified SIMD scalars and return the result.
template < Size width >
GSOUND_FORCE_INLINE SIMDScalar<Int32,width> max( const SIMDScalar<Int32,width>& scalar1, const SIMDScalar<Int32,width>& scalar2 )
{
	SIMDScalar<Int32,width> result;
	
	for ( Index i = 0; i < SIMDScalar<Int32,width>::numIterations; i++ )
		result.v[i] = math::max( scalar1.v[i], scalar2.v[i] );
	
	return result;
}




/// Select elements from the first SIMD scalar if the selector is TRUE, otherwise from the second.
template < Size width >
GSOUND_FORCE_INLINE SIMDScalar<Int32,width> select( const SIMDScalar<Bool,width>& selector,
												const SIMDScalar<Int32,width>& scalar1, const SIMDScalar<Int32,width>& scalar2 )
{
	SIMDScalar<Int32,width> result;
	
	for ( Index i = 0; i < SIMDScalar<Int32,width>::numIterations; i++ )
		result.v[i] = math::select( selector.v[i], scalar1.v[i], scalar2.v[i] );
	
	return result;
}




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SIMD_SCALAR_INT_N_H
