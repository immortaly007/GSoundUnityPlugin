/*
 * Project:     GSound
 * 
 * File:        gsound/math/SIMDRay3D.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::SIMDRay3D class declaration
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


#ifndef INCLUDE_GSOUND_SIMD_RAY_3D_H
#define INCLUDE_GSOUND_SIMD_RAY_3D_H


#include "GSoundMathConfig.h"


#include "Ray3D.h"
#include "SIMDVector3D.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A template prototype declaration for the SIMDRay3D class.
/**
  * This class is used to store and operate on a set of N 3D rays
  * in a SIMD fashion. The rays are stored in a structure-of-arrays format
  * that accelerates SIMD operations. Each ray is specified by an origin
  * point and a direction vector.
  */
template < typename T, Size dimension >
class SIMDRay3D;




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A specialization for the SIMDRay3D class that has a SIMD width of 4.
/**
  * This class is used to store and operate on a set of 4 3D rays
  * in a SIMD fashion. The rays are stored in a structure-of-arrays format
  * that accelerates SIMD operations. Each ray is specified by an origin
  * point and a direction vector.
  */
template < typename T >
class GSOUND_ALIGN(16) SIMDRay3D<T,4>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a SIMD ray with N copies of the specified ray for a SIMD width of N.
			GSOUND_FORCE_INLINE SIMDRay3D( const Ray3D<T>& ray )
				:	origin( ray.origin ),
					direction( ray.direction )
			{
			}
			
			
			
			
			/// Create a SIMD ray with the 4 rays it contains equal to the specified rays.
			GSOUND_FORCE_INLINE SIMDRay3D( const Ray3D<T>& ray1, const Ray3D<T>& ray2,
											const Ray3D<T>& ray3, const Ray3D<T>& ray4 )
				:	origin( ray1.origin, ray2.origin, ray3.origin, ray4.origin ),
					direction( ray1.direction, ray2.direction, ray3.direction, ray4.direction )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// A SIMD 3D vector indicating the origin of the ray(s).
			SIMDVector3D<T,4> origin;
			
			
			
			
			/// A SIMD 3D vector indicating the direction of the ray(s).
			SIMDVector3D<T,4> direction;
			
			
			
			
};




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SIMD_RAY_3D_H
