/*
 * Project:     GSound
 * 
 * File:        gsound/math/Ray3D.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::Ray3D class declaration
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


#ifndef INCLUDE_GSOUND_RAY_3D_H
#define INCLUDE_GSOUND_RAY_3D_H


#include "GSoundMathConfig.h"


#include "ScalarMath.h"
#include "Vector3D.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which represents a ray in 3D space.
/**
  * This class contains two data members: origin and direction. Origin represents
  * the starting position of the ray and direction represents the positively
  * parameterized direction along the ray.
  */
template < typename T >
class Ray3D
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a ray starting at the origin pointing in the positive Z direction.
			GSOUND_FORCE_INLINE Ray3D()
				:	origin( 0, 0, 0 ),
					direction( 0, 0, 1 )
			{
			}
			
			
			
			
			/// Create a ray with the specified origin and direction.
			GSOUND_FORCE_INLINE Ray3D( const Vector3D<T>& newOrigin, const Vector3D<T>& newDirection )
				:	origin( newOrigin ),
					direction( newDirection )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Ray Methods
			
			
			
			
			/// Get the position along the ray at the specified parameter value.
			/**
			  * This position is calculated using the equation:
			  * position = origin + parameter*direction.
			  */
			GSOUND_FORCE_INLINE Vector3D<T> getPositionAt( T parameter ) const
			{
				return origin + parameter*direction;
			}
			
			
			
			
			/// Return a new ray with a unit-length direction vector.
			GSOUND_FORCE_INLINE Ray3D<T> normalize() const
			{
				return Ray3D<T>( origin, direction.normalize() );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// The origin of the ray in 3D space;
			Vector3D<T> origin;
			
			
			
			
			/// The direction of the ray in 3D space.
			Vector3D<T> direction;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		3D Ray Type Definitions
//############		
//##########################################################################################
//##########################################################################################




typedef Ray3D<int>		Ray3i;
typedef Ray3D<float>	Ray3f;
typedef Ray3D<double>	Ray3d;




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_RAY_3D_H
