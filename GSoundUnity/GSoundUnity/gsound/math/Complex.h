/*
 * Project:     GSound
 * 
 * File:        gsound/math/Complex.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::Complex class declaration
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


#ifndef INCLUDE_GSOUND_COMPLEX_H
#define INCLUDE_GSOUND_COMPLEX_H


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
/// A class which represents a complex number.
/**
  * Overloaded arithmetic operators are provided for all basic arithmetic operations
  * (+, -, *, /, +=, -=, *=, /=). The class allows any underlying type to be used
  * as the coefficients of the real and imaginary components of the complex number
  * via a template parameter.
  */
template < typename T >
class Complex
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a complex number with both real and imaginary parts equal to zero.
			GSOUND_FORCE_INLINE Complex()
				:	real( 0 ),
					imaginary( 0 )
			{
			}
			
			
			
			/// Create a complex number with the specified real component and zero imaginary component.
			GSOUND_FORCE_INLINE Complex( T newReal )
				:	real( newReal ),
					imaginary( 0 )
			{
			}
			
			
			
			/// Create a complex number with the specified real and imaginary components.
			GSOUND_FORCE_INLINE Complex( T newReal, T newImaginary )
				:	real( newReal ),
					imaginary( newImaginary )
			{
			}
			
			
			
			
			/// Create a complex number from another with different template parameter type.
			template < typename U >
			GSOUND_FORCE_INLINE Complex( const Complex<U>& other )
				:	real( other.real ),
					imaginary( other.imaginary )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Complex Number Operations
			
			
			
			
			/// Return the conjugate of this complex number.
			GSOUND_FORCE_INLINE Complex conjugate() const
			{
				return Complex( real, -imaginary );
			}
			
			
			
			
			/// Return the magnitude of this complex number.
			GSOUND_FORCE_INLINE T getMagnitude() const
			{
				return math::sqrt( real*real + imaginary*imaginary );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Comparison Operators
			
			
			
			
			/// Return whether or not this complex number is equal to another.
			GSOUND_FORCE_INLINE Bool operator == ( const Complex& other ) const
			{
				return real == other.real && imaginary == other.imaginary;
			}
			
			
			
			
			/// Return whether or not this complex number is equal to a real number.
			GSOUND_FORCE_INLINE Bool operator == ( T value ) const
			{
				return real == value && imaginary == T(0);
			}
			
			
			
			
			/// Return whether or not this complex number is not equal to another.
			GSOUND_FORCE_INLINE Bool operator != ( const Complex& other ) const
			{
				return real != other.real || imaginary != other.imaginary;
			}
			
			
			
			
			/// Return whether or not this complex number is not equal to a real number.
			GSOUND_FORCE_INLINE Bool operator != ( T value ) const
			{
				return real != value || imaginary != T(0);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			
			
			/// Return the negation of this complex number.
			GSOUND_FORCE_INLINE Complex operator - () const
			{
				return Complex( -real, -imaginary );
			}
			
			
			
			
			/// Return the positivation of this complex number.
			GSOUND_FORCE_INLINE Complex operator + () const
			{
				return Complex( real, imaginary );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			
			
			/// Return the sum of this complex number with a real number.
			GSOUND_FORCE_INLINE Complex operator + ( T value ) const
			{
				return Complex( real + value, imaginary );
			}
			
			
			
			
			/// Return the difference between this complex number a real number.
			GSOUND_FORCE_INLINE Complex operator - ( T value ) const
			{
				return Complex( real - value, imaginary );
			}
			
			
			
			
			/// Multiply this complex number by real number and return the resulting complex number.
			GSOUND_FORCE_INLINE Complex operator * ( T value ) const
			{
				return Complex( real*value, imaginary*value );
			}
			
			
			
			
			/// Divide this complex number by another and return the resulting complex number.
			GSOUND_FORCE_INLINE Complex operator / ( T value ) const
			{
				T inverseValue = T(1)/value;
				
				return Complex( real*inverseValue, imaginary*inverseValue );
			}
			
			
			
			
			/// Return the sum of this complex number with another.
			GSOUND_FORCE_INLINE Complex operator + ( const Complex& other ) const
			{
				return Complex( real + other.real, imaginary + other.imaginary );
			}
			
			
			
			
			/// Return the difference between this complex number and other.
			GSOUND_FORCE_INLINE Complex operator - ( const Complex& other ) const
			{
				return Complex( real - other.real, imaginary - other.imaginary );
			}
			
			
			
			
			/// Multiply this complex number with another and return the resulting complex number.
			GSOUND_FORCE_INLINE Complex operator * ( const Complex& other ) const
			{
				return Complex( real*other.real - imaginary*other.imaginary,
								imaginary*other.real + real*other.imaginary );
			}
			
			
			
			
			/// Divide this complex number by another and return the resulting complex number.
			GSOUND_FORCE_INLINE Complex operator / ( const Complex& other ) const
			{
				T inverseDenominator = T(1)/(other.real*other.real + other.imaginary*other.imaginary);
				
				return Complex( (real*other.real - imaginary*other.imaginary)*inverseDenominator,
								(imaginary*other.real + real*other.imaginary)*inverseDenominator );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			
			
			/// Add a real number to this complex number, modifying it.
			GSOUND_FORCE_INLINE Complex& operator += ( T value )
			{
				real += value;
				
				return *this;
			}
			
			
			
			
			/// Subtract a real number from this complex number, modifying it.
			GSOUND_FORCE_INLINE Complex& operator -= ( T value )
			{
				real -= value;
				
				return *this;
			}
			
			
			
			
			/// Multiply a real number with this complex number, modifying it.
			GSOUND_FORCE_INLINE Complex& operator *= ( T value )
			{
				real *= value;
				imaginary *= value;
				
				return *this;
			}
			
			
			
			
			/// Divide this complex number by a real number, modifying it.
			GSOUND_FORCE_INLINE Complex& operator /= ( T value )
			{
				T inverseValue = T(1)/value;
				
				real *= inverseValue;
				imaginary *= inverseValue;
				
				return *this;
			}
			
			
			
			
			/// Add a complex number to this complex number, modifying it.
			GSOUND_FORCE_INLINE Complex& operator += ( const Complex& other )
			{
				real += other.real;
				imaginary += other.imaginary;
				
				return *this;
			}
			
			
			
			
			/// Subtract a complex number from this complex number, modifying it.
			GSOUND_FORCE_INLINE Complex& operator -= ( const Complex& other )
			{
				real -= other.real;
				imaginary -= other.imaginary;
				
				return *this;
			}
			
			
			
			
			/// Multiply a complex number with this complex number, modifying it.
			GSOUND_FORCE_INLINE Complex& operator *= ( const Complex& other )
			{
				T realTemp = real*other.real - imaginary*other.imaginary;
				imaginary = imaginary*other.real + real*other.imaginary;
				real = realTemp;
				
				return *this;
			}
			
			
			
			
			/// Divide this complex number by another complex number, modifying it.
			GSOUND_FORCE_INLINE Complex& operator /= ( const Complex& other )
			{
				T inverseDenominator = T(1)/(other.real*other.real + other.imaginary*other.imaginary);
				
				T realTemp = (real*other.real - imaginary*other.imaginary)*inverseDenominator;
				imaginary = (imaginary*other.real + real*other.imaginary)*inverseDenominator;
				real = realTemp;
				
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// The real portion of the complex number.
			T real;
			
			
			
			
			/// The imaginary portion of the complex number.
			T imaginary;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Comparison Operators
//############		
//##########################################################################################
//##########################################################################################




/// Return whether or not a real number is equal to a complex number.
template < typename T >
GSOUND_FORCE_INLINE Bool operator == ( T value, const Complex<T>& complex )
{
	return complex == value;
}




/// Return whether or not a real number is not equal to a complex number.
template < typename T >
GSOUND_FORCE_INLINE Bool operator != ( T value, const Complex<T>& complex )
{
	return complex != value;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a complex number to a real number and return the resulting complex number.
template < typename T >
GSOUND_FORCE_INLINE Complex<T> operator + ( T value, const Complex<T>& complex )
{
	return Complex<T>( value + complex.real, complex.imaginary );
}




/// Subtract a complex number from a real number and return the resulting complex number.
template < typename T >
GSOUND_FORCE_INLINE Complex<T> operator - ( T value, const Complex<T>& complex )
{
	return Complex<T>( value - complex.real, -complex.imaginary );
}




/// Multiply a real number by a complex number and return the resulting complex number.
template < typename T >
GSOUND_FORCE_INLINE Complex<T> operator * ( T value, const Complex<T>& complex )
{
	return Complex<T>( value*complex.real, value*complex.imaginary );
}




/// Divide a real number by a complex number and return the resulting complex number.
template < typename T >
GSOUND_FORCE_INLINE Complex<T> operator / ( T value, const Complex<T>& complex )
{
	return Complex<T>( value ) / complex;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Other Complex Number Functions
//############		
//##########################################################################################
//##########################################################################################




/// Return the absolute value (magnitude) of the specified complex number.
template < typename T >
GSOUND_FORCE_INLINE T abs( const Complex<T>& complex )
{
	return math::sqrt( complex.real*complex.real + complex.imaginary*complex.imaginary );
}




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_GSOUND_COMPLEX_H
