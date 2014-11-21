/*
 * Project:     GSound
 * 
 * File:        gsound/GSoundMath.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    All includes for the math portion of the GSound library.
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


#ifndef INCLUDE_GSOUND_MATH_H
#define INCLUDE_GSOUND_MATH_H


// Math configuration file
#include "math/GSoundMathConfig.h"


// 1D math files
#include "math/ScalarMath.h"
#include "math/AABB1D.h"
#include "math/Complex.h"


// 2D math files
#include "math/Vector2D.h"


// 3D math files
#include "math/Vector3D.h"
#include "math/Matrix3D.h"
#include "math/Transformation3D.h"
#include "math/Plane3D.h"
#include "math/Ray3D.h"
#include "math/AABB3D.h"


// Random variable files
#include "math/RandomVariable.h"


// SIMD include files
#include "math/SIMDScalar.h"
#include "math/SIMDScalarBool4.h"
#include "math/SIMDScalarBoolN.h"
#include "math/SIMDScalarInt4.h"
#include "math/SIMDScalarIntN.h"
#include "math/SIMDScalarFloat4.h"
#include "math/SIMDScalarFloatN.h"
#include "math/SIMDVector3D.h"
#include "math/SIMDRay3D.h"
#include "math/SIMDAABB3D.h"
#include "math/SIMDTriangle3D.h"


#endif // INCLUDE_GSOUND_MATH_H
