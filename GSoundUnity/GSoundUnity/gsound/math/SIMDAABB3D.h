/*
 * Project:     GSound
 * 
 * File:        gsound/math/SIMDAABB3D.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::SIMDAABB3D class declaration
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


#ifndef INCLUDE_GSOUND_SIMD_AABB_3D_H
#define INCLUDE_GSOUND_SIMD_AABB_3D_H


#include "GSoundMathConfig.h"


#include "AABB3D.h"
#include "SIMDVector3D.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A template prototype declaration for the SIMDAABB3D class.
/**
  * This class is used to store and operate on a set of N axis-aligned bounding boxes
  * in a SIMD fashion. The bounding boxes are stored in a structure-of-arrays format
  * that accelerates SIMD operations. Each bounding box is specified by a minimum
  * and maximum vertex coordinate.
  */
template < typename T, Size dimension >
class SIMDAABB3D;




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A specialization for the SIMDAABB3D class that has a SIMD width of 4.
/**
  * This class is used to store and operate on a set of 4 axis-aligned bounding boxes
  * in a SIMD fashion. The bounding boxes are stored in a structure-of-arrays format
  * that accelerates SIMD operations. Each bounding box is specified by a minimum
  * and maximum vertex coordinate.
  */
template < typename T>
class GSOUND_ALIGN(16) SIMDAABB3D<T,4>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a SIMD axis-aligned bounding box that has all components initialized to zero.
			GSOUND_FORCE_INLINE SIMDAABB3D()
			{
			}
			
			
			
			
			
			/// Create a SIMD axis-aligned bounding box from a single bounding box.
			GSOUND_FORCE_INLINE SIMDAABB3D( const AABB3D<T>& aabb )
				:	min( aabb.min ),
					max( aabb.max )
			{
			}
			
			
			
			
			/// Create a SIMD axis-aligned bounding box from the four specified bounding boxes.
			GSOUND_FORCE_INLINE SIMDAABB3D( const AABB3D<T>& aabb1, const AABB3D<T>& aabb2,
											const AABB3D<T>& aabb3, const AABB3D<T>& aabb4 )
				:	min( aabb1.min, aabb2.min, aabb3.min, aabb4.min ),
					max( aabb1.max, aabb2.max, aabb3.max, aabb4.max )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Accessor Methods
			
			
			
			
			/// Get either the minimal or maximal vertex of this AABB.
			/**
			  * If the index parameter is 0, the minimal vertex is returned, if the
			  * index parameter is 1, the maximal vertex is returned. Otherwise the
			  * result is undefined.
			  */
			GSOUND_FORCE_INLINE const SIMDVector3D<T,4>& getMinMax( Index i ) const
			{
				return (&min)[i];
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// The minimum coordinate vectors for this SIMD axis-aligned bounding box.
			SIMDVector3D<T,4> min;
			
			
			
			
			/// The maximum coordinate vectors for this SIMD axis-aligned bounding box.
			SIMDVector3D<T,4> max;
			
			
			
			
};




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SIMD_AABB_3D_H
