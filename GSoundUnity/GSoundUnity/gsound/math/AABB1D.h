/*
 * Project:     GSound
 * 
 * File:        gsound/math/AABB1D.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::AABB1D class declaration
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


#ifndef INCLUDE_GSOUND_AABB_1D_H
#define INCLUDE_GSOUND_AABB_1D_H


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
/// A class which represents a range of values in 1D space.
/**
  * This class contains two data members: min and max. These indicate the minimum
  * and maximum values that this axis-aligned bounding box represents. The class
  * invariant is that min is less than max, though this is not enforced. The class
  * supports union, containment, and intersection operations.
  */
template < typename T >
class AABB1D
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a 1D axis-aligned bounding box with no extent centered about the origin.
			GSOUND_FORCE_INLINE AABB1D()
				:	min(),
					max()
			{
			}
			
			
			
			
			/// Create a 1D axis-aligned bounding box with the specified minimum and maximum coodinates.
			GSOUND_FORCE_INLINE AABB1D( T newMin, T newMax )
				:	min( newMin ),
					max( newMax )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	AABB Cast Operator
			
			
			
			
			/// Cast this bounding box to a bounding box with a different underlying primitive type.
			template < typename U >
			GSOUND_FORCE_INLINE operator AABB1D<U> () const
			{
				return AABB1D<U>( (U)min, (U)max );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	AABB Comparison Methods
			
			
			
			
			/// Return whether or not this bounding box completely contains another.
			GSOUND_FORCE_INLINE Bool contains( const AABB1D& bounds )
			{
				return min <= bounds.min && max >= bounds.max;
			}
			
			
			
			
			/// Return whether or not this bounding box contains the specified value.
			GSOUND_FORCE_INLINE Bool contains( T value )
			{
				return value >= min && value <= max;
			}
			
			
			
			
			/// Return whether or not this bounding box intersects another.
			GSOUND_FORCE_INLINE Bool intersects( const AABB1D& bounds )
			{
				return (min < bounds.max) && (max > bounds.min);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Accessor Methods
			
			
			
			
			/// Set the minimum and maximum values of the axis-aligned bounding box.
			GSOUND_FORCE_INLINE void set( T newMin, T newMax )
			{
				min = newMin;
				max = newMax;
			}
			
			
			
			
			/// Get the different between the maximum and minimum coordinates.
			GSOUND_FORCE_INLINE T getWidth() const
			{
				return max - min;
			}
			
			
			
			
			/// Get the center of the bounding box.
			GSOUND_FORCE_INLINE T getCenter() const
			{
				return math::average( min, max );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Enlargement Methods
			
			
			
			
			/// Modify the current bounding box such that it encloses the specified value.
			GSOUND_FORCE_INLINE void enlargeFor( T value )
			{
				if ( value < min )
					min = value;
				else if ( value > max )
					max = value;
			}
			
			
			
			
			/// Modify the current bounding box such that it encloses the specified value.
			GSOUND_FORCE_INLINE AABB1D<T>& operator += ( T value )
			{
				if ( value < min )
					min = value;
				else if ( value > max )
					max = value;
				
				return *this;
			}
			
			
			
			
			/// Return the bounding box necessary to enclose a value and the current bounding box.
			GSOUND_FORCE_INLINE AABB1D<T> operator + ( T value ) const
			{
				return AABB1D<T>( value < min ? value : min, value > max ? value : max );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Union Methods
			
			
			
			
			/// Return the union of this bounding box and another.
			GSOUND_FORCE_INLINE AABB1D<T> getUnion( const AABB1D<T>& bounds ) const
			{
				return AABB1D<T>(	bounds.min < min ? bounds.min : min,
									bounds.max > max ? bounds.max : max );
			}
			
			
			
			
			/// Modify this bounding box such that it contains the specified bounding box.
			GSOUND_FORCE_INLINE AABB1D<T>& operator += ( const AABB1D<T>& bounds )
			{
				if ( bounds.min < min )
					min = bounds.min;
				
				if ( bounds.max > max )
					max = bounds.max;
				
				return *this;
			}
			
			
			
			
			/// Return the union of this bounding box and another.
			GSOUND_FORCE_INLINE AABB1D<T> operator + ( const AABB1D<T>& bounds ) const
			{
				return AABB1D<T>( bounds.min < min ? bounds.min : min,
									bounds.max > max ? bounds.max : max );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// The minumum coordinate of the bounding box.
			T min;
			
			
			
			
			/// The maximum coordinate of the bounding box.
			T max;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		1D Bounding Box Type Definitions
//############		
//##########################################################################################
//##########################################################################################




typedef AABB1D<int>			AABB1i;
typedef AABB1D<float>		AABB1f;
typedef AABB1D<double>		AABB1d;




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_AABB_1D_H
