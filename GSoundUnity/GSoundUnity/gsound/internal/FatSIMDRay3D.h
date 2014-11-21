/*
 * Project:     GSound
 * 
 * File:        gsound/internal/FatSIMDRay3D.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::FatSIMDRay3D class declaration
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


#ifndef INCLUDE_GSOUND_FAT_SIMD_RAY_3D_H
#define INCLUDE_GSOUND_FAT_SIMD_RAY_3D_H


#include "GSoundInternalConfig.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// A class which is used as an extension to the SIMDRay3D class to encode extra information.
/**
  * The additional information about the ray(s) that is stored is the inverse of the direction
  * vector(s) and the sign of the ray's direction
  */
template < typename T, Size dimension >
class GSOUND_ALIGN(16) FatSIMDRay3D : public math::SIMDRay3D<T,dimension>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			GSOUND_INLINE FatSIMDRay3D( const Ray3D<T>& ray )
				:	SIMDRay3D<T,dimension>( ray ),
					inverseDirection( T(1) / ray.direction )
			{
				sign[0] = ray.direction.x > T(0);
				sign[1] = ray.direction.y > T(0);
				sign[2] = ray.direction.z > T(0);
			}
			
			
			
			
			GSOUND_INLINE FatSIMDRay3D( const Vector3D<T>& newOrigin, const Vector3D<T>& newDirection )
				:	SIMDRay3D<T,dimension>( newOrigin, newDirection ),
					inverseDirection( T(1) / newDirection )
			{
				sign[0] = newDirection.x > T(0);
				sign[1] = newDirection.y > T(0);
				sign[2] = newDirection.z > T(0);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// The inverse of the direction vector of this SIMD Ray.
			SIMDVector3D<T,dimension> inverseDirection;
			
			
			
			
			/// Indices of the sign of the ray's direction along the 3 axes: 1 for positive, 0 for negative.
			/**
			  * The axes are enumerated: 0 = X, 1 = Y, 2 = Z.
			  */
			Index sign[3];
			
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_FAT_SIMD_RAY_3D_H
