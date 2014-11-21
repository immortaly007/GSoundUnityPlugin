/*
 * Project:     GSound
 * 
 * File:        gsound/Timer.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    Configuration for the util namespace of the GSound library.
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


#ifndef INCLUDE_GSOUND_UTILITIES_CONFIG_H
#define INCLUDE_GSOUND_UTILITIES_CONFIG_H


#include "../GSoundConfig.h"


#include <cstdlib>
#include <new>


//##########################################################################################
//##########################################################################################
//############		
//############		GSound Internal Namespace Configuration
//############		
//##########################################################################################
//##########################################################################################




/// Define the namespace to use for library utility functionality.
#ifndef GSOUND_UTILITIES_NAMESPACE
	#define GSOUND_UTILITIES_NAMESPACE util
#endif




/// Define a macro which indicates the start of the gsound::util namespace.
#ifndef GSOUND_UTILITIES_NAMESPACE_START
	#define GSOUND_UTILITIES_NAMESPACE_START		GSOUND_NAMESPACE_START namespace util {
#endif




/// Define a macro which indicates the end of the gsound::util namespace.
#ifndef GSOUND_UTILITIES_NAMESPACE_END
	#define GSOUND_UTILITIES_NAMESPACE_END		GSOUND_NAMESPACE_END };
#endif




GSOUND_NAMESPACE_START
/// A namespace containing library utility functionality.
/**
  * This namespace consists of classes and functions that provide functionality
  * that is general purpose and not necessarily specific to GSound
  * but that is still necessary. These include common data structures,
  * thread classes, and memory managment functions.
  */
namespace GSOUND_UTILITIES_NAMESPACE {
};
GSOUND_NAMESPACE_END




//##########################################################################################
//*************************  Start GSound Utilities Namespace  *****************************
GSOUND_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//*************************  End GSound Utilities Namespace  *******************************
GSOUND_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_UTILITIES_CONFIG_H
