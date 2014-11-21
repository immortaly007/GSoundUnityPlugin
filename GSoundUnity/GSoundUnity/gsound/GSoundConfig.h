/*
 * Project:     GSound
 * 
 * File:        gsound/GSoundConfig.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    Main configuration for the GSound library.
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


#ifndef INCLUDE_GSOUND_CONFIG_H
#define INCLUDE_GSOUND_CONFIG_H


#include <cstddef>
#include <cstdlib>
#include <cstdio>


//##########################################################################################
//##########################################################################################
//############
//############		Library Debugging Configuration
//############
//##########################################################################################
//##########################################################################################




#ifndef GSOUND_DEBUG
	#if defined(_DEBUG)
		/// Define whether or not internal debugging checks should be force-enabled.
		#define GSOUND_DEBUG
	#endif
#endif




//##########################################################################################
//##########################################################################################
//############
//############		Library Configuration
//############
//##########################################################################################
//##########################################################################################




/// Define an integer representing the major version number of the GSound library.
/**
  * Major version changes indicate large revisions to the overall structure or function
  * of the library and may produce incompatability with code designed to use older
  * versions. Functions or classes that have been deprecated previously may be removed
  * after a major version change.
  */
#ifndef GSOUND_MAJOR_VERSION
	#define GSOUND_MAJOR_VERSION 1
#endif




/// Define an integer representing the minor version number of the GSound library.
/**
  * Minor version changes indicate that functionality has been improved while
  * keeping the existing API, or that new functionality has been added without
  * disrupting older functionality. Minor version changes can also indicate
  * major bug fixes. Old functionality may become deprecated with a minor version
  * update.
  */
#ifndef GSOUND_MINOR_VERSION
	#define GSOUND_MINOR_VERSION 0
#endif




/// Define an integer representing the revision version number of the GSound library.
/**
  * Library revisions indicate that bug fixes or minor improvements
  * to the backend codebase have occurred. New functionality may also be
  * added that doesn't disrupt existing use of the library.
  */
#ifndef GSOUND_REVISION
	#define GSOUND_REVISION 0
#endif




/// Define an integral number exactly representing the version of the GSound library.
/**
  * This definition is primarily useful when performing compilation switching
  * based on the version of GSound being used. More recent versions of GSound
  * will always have a version greater than that of older versions.
  */
#ifndef GSOUND_VERSION
	#define GSOUND_VERSION (GSOUND_MAJOR_VERSION*10000 + GSOUND_MINOR_VERSION*100 + GSOUND_REVISION)
#endif




/// A constant C-string representing the version of the GSound library.
/**
  * The string will be of the format "A.B.C" where A, B, and C are all
  * positive integers in decimal notation.
  */
#ifndef GSOUND_VERSION_STRING
	#define GSOUND_VERSION_STRING "1.0.0"
#endif




/// Determine whether or not SIMD code should be used.
/**
  * If set to 1, many operations will be parallelized using SIMD vector operations.
  * This will generally increase performance but may not work on all hardware. If set to
  * 0, no SIMD operations will be used. If enabled but the hardware doesn't support SIMD
  * instructions, a serial fallback implementation will be used.
  */
#ifndef GSOUND_USE_SIMD
	#define GSOUND_USE_SIMD 1
#endif




/// Define the newest major version of SSE that can be used by GSound.
/**
  * This value can be used to limit the complexity of the SSE operations
  * performed when compiling for hardware that doesn't support newer SSE versions.
  * Only SSE versions up to this version (specified as an integer number)
  * can be used.
  */
#ifndef GSOUND_SSE_MAX_MAJOR_VERSION
	#define GSOUND_SSE_MAX_MAJOR_VERSION 3
#endif




/// Define the newest minor version of SSE that can be used by GSound.
/**
  * This value can be used to limit the complexity of the SSE operations
  * performed when compiling for hardware that doesn't support newer SSE versions.
  * Only SSE versions up to this version (specified as an integer number)
  * can be used.
  */
#ifndef GSOUND_SSE_MAX_MINOR_VERSION
	#define GSOUND_SSE_MAX_MINOR_VERSION 0
#endif




/// Determine whether or not OpenCL code should be used.
/**
  * If set to 1, GSound will test to see if any OpenCL devices
  * are available. If so, it will use these devices to perform
  * various operations. Otherwise, a fallback CPU implementation
  * will be used.
  */
#ifndef GSOUND_USE_OPEN_CL
	#define GSOUND_USE_OPEN_CL 0
#endif




/// Determine whether or not a fixed maximum path depth should be used.
/**
  * This option is available in order to reduce the number of small allocations
  * performed during simulation. If set to 1, no propagation paths with a depth
  * greater than GSOUND_MAX_PATH_DEPTH will be found. Internal arrays that would
  * normally be dynamically allocated are set to be a fixed size.
  */
#ifndef GSOUND_FIXED_MAX_PATH_DEPTH
	#define GSOUND_FIXED_MAX_PATH_DEPTH 1
#endif




#if GSOUND_FIXED_MAX_PATH_DEPTH
	/// Set the maximum propagation path depth allowed when a fixed max path depth is enabled.
	/**
	  * Increasing this value will allow deeper propagation paths to be found, but will increase
	  * the size of static arrays allocated internally. This can reduce cache performance if these
	  * arrays become too large. Thus, it is best to set this value to be as small as is required
	  * for your particular application.
	  */
	#ifndef GSOUND_MAX_PATH_DEPTH
		#define GSOUND_MAX_PATH_DEPTH 8
	#endif
#endif




/// Define the function to use for all library memory allocations.
#ifndef GSOUND_MALLOC
	#define GSOUND_MALLOC(X) (std::malloc(X))
#endif




/// Define the function to use for all library memory deallocations.
#ifndef GSOUND_FREE
	#define GSOUND_FREE(X) (std::free(X))
#endif




/// Define whether or not to turn off all assertion (including assertions active during release-mode builds).
#ifndef GSOUND_DISABLE_ASSERTIONS
	#define GSOUND_DISABLE_ASSERTIONS 0
#endif




//##########################################################################################
//##########################################################################################
//############
//############		Library Platform Configuration
//############
//##########################################################################################
//##########################################################################################




/// Determine if the platform is Mac OS X based.
#ifdef __APPLE__
	#define GSOUND_PLATFORM_APPLE
#endif




/// Determine if the platform is Windows 32-bit based.
#if defined(_WIN32) || defined(__WIN32__)
	#define GSOUND_PLATFORM_WIN32
#endif




/// Determine if the platform is Windows 64-bit based.
#if defined(_WIN64) || defined(__WIN64__)
	#define GSOUND_PLATFORM_WIN64
#endif




/// Determine if the platform is Windows based.
#if defined(GSOUND_PLATFORM_WIN32) || defined(GSOUND_PLATFORM_WIN64) || defined(__WINDOWS__)
	#define GSOUND_PLATFORM_WINDOWS
#endif




/// Determine if the platform is Linux based.
#if defined(__linux__) || defined(__linux)
	#define GSOUND_PLATFORM_LINUX
#endif




/// Determine the compiler that is being used.
#if defined( __GNUC__ )
	#define GSOUND_COMPILER_GCC __GNUC__
	#define GSOUND_COMPILER GSOUND_COMPILER_GCC
	#define GSOUND_GCC_VERSION( Major, Minor ) (Major*10000 + Minor*100 )
	#define GSOUND_COMPILER_VERSION GSOUND_GCC_VERSION( __GNUC__, __GNUC_MINOR__ )
	
#elif defined( _MSC_VER )
	#define GSOUND_COMPILER_MSVC _MSC_VER
	#define GSOUND_COMPILER GSOUND_COMPILER_MSVC
	#define GSOUND_COMPILER_VERSION _MSC_VER

#endif



#if defined( GSOUND_COMPILER_GCC )
	
	#ifdef GSOUND_DEBUG
		#define GSOUND_INLINE inline
		#define GSOUND_FORCE_INLINE inline
	#else
		#define GSOUND_INLINE inline
		#define GSOUND_FORCE_INLINE __attribute__((__always_inline__)) inline
	#endif
	
	#define GSOUND_NO_INLINE __attribute__((noinline))
	
	#define GSOUND_ALIGN(alignment) __attribute__((aligned(alignment)))

	#if defined(GSOUND_PLATFORM_APPLE)
        #if !defined(__MAC_10_6)
			#define GSOUND_ALIGNED_MALLOC( size, alignment ) (GSOUND_MALLOC( size ))
        #else
			namespace gsound { namespace util {

			GSOUND_FORCE_INLINE void* posix_memalign_wrapper( size_t size, size_t alignment )
			{
				void* pointer;
				posix_memalign( &pointer, alignment, size );
				return pointer;
			}

			}; };

			#include <malloc/malloc.h>
			#define GSOUND_ALIGNED_MALLOC( size, alignment ) (posix_memalign_wrapper( size, alignment ))

        #endif
	#else
		#define GSOUND_ALIGNED_MALLOC( size, alignment ) (GSOUND_MALLOC( size ))
	#endif
	
	#define GSOUND_ALIGNED_FREE(X) (std::free(X))

	#define GSOUND_DEPRECATED __attribute__((deprecated))

#elif defined( GSOUND_COMPILER_MSVC )

	#include <malloc.h>

	#ifdef GSOUND_DEBUG
		#define GSOUND_INLINE inline
		#define GSOUND_FORCE_INLINE inline
	#else
		#if GSOUND_COMPILER_VERSION >= 1200
			#define GSOUND_INLINE inline
			#define GSOUND_FORCE_INLINE __forceinline
		#else
			#define GSOUND_INLINE inline
			#define GSOUND_FORCE_INLINE inline
		#endif
	#endif
	
	#define GSOUND_NO_INLINE __declspec(noinline)

	#define GSOUND_ALIGN(alignment) __declspec(align(alignment))

	#define GSOUND_ALIGNED_MALLOC( size, alignment ) (_aligned_malloc( size, alignment ))
	
	#define GSOUND_ALIGNED_FREE(X) (_aligned_free(X))

	#define GSOUND_DEPRECATED __declspec(deprecated)
#else

	/// Define the inlining procedure to use for methods that should be inlined.
	#define GSOUND_INLINE inline
	#define GSOUND_FORCE_INLINE inline
	#define GSOUND_NO_INLINE 

	/// Define the alignment declaration to be use when aligning structure/class members.
	#define GSOUND_ALIGN(alignment) 

	/// Define the function to use when allocating aligned blocks of memory.
	#define GSOUND_ALIGNED_MALLOC( size, alignment ) (GSOUND_MALLOC(size))
	
	/// Define the function to use when freeing an aligned block of memory.
	#define GSOUND_ALIGNED_FREE(X) (GSOUND_FREE(X))

	/// Define the marker which is used when defining methods, types, and variables as deprecated.
	#define GSOUND_DEPRECATED

#endif




#if defined(_M_PPC) || defined(__ppc__) || defined(__POWERPC__)
	#define GSOUND_CPU_POWER_PC

#elif defined(_M_X64) || defined(_M_IX86) || defined(__x86_64__) || defined(__i386__)
	#define GSOUND_CPU_INTEL
#else
	#error Unsupported CPU type.
#endif




#if GSOUND_USE_SIMD
#if defined(GSOUND_CPU_POWER_PC) && defined(__ALTIVEC__)
	/// Define that Altivec instructions are available.
	#define GSOUND_SIMD_ALTIVEC
	
	/// Redfine the keyword associated with an Altivec vector to avoid name collisions.
	#ifdef GSOUND_COMPILER_GCC
		#undef vector
		#define GSOUND_ALTIVEC_VECTOR __vector
	#endif
	
	/// A macro which produces a boolean value indicating whether the specified (major,minor) version of SSE is supported.
	#define GSOUND_SSE_VERSION_IS_SUPPORTED( majorVersion, minorVersion ) 0

#elif defined(GSOUND_CPU_INTEL)
	/// Define that SSE instructions are available.
	#define GSOUND_SIMD_SSE
	
	/// Define a macro which determines whether a specified (major,minor) version of SSE is allowed by the user.
	#define GSOUND_SSE_VERSION_IS_ALLOWED( majorVersion, minorVersion ) \
				(GSOUND_SSE_MAX_MAJOR_VERSION > majorVersion || \
				(GSOUND_SSE_MAX_MAJOR_VERSION == majorVersion && GSOUND_SSE_MAX_MINOR_VERSION >= minorVersion))
	
	// Determine the newest available version of SSE and include its header.
	#if defined(GSOUND_COMPILER_GCC)
		#if defined(__SSE4_2__)
			#define GSOUND_SSE_MAJOR_VERSION 4
			#define GSOUND_SSE_MINOR_VERSION 2
			
		#elif defined(__SSE4_1__)
			#define GSOUND_SSE_MAJOR_VERSION 4
			#define GSOUND_SSE_MINOR_VERSION 1
			
		#elif defined(__SSSE3__)
			#define GSOUND_SSE_MAJOR_VERSION 3
			#define GSOUND_SSE_MINOR_VERSION 1
			
		#elif defined(__SSE3__)
			#define GSOUND_SSE_MAJOR_VERSION 3
			#define GSOUND_SSE_MINOR_VERSION 0
			
		#elif defined(__SSE2__)
			#define GSOUND_SSE_MAJOR_VERSION 2
			#define GSOUND_SSE_MINOR_VERSION 0
		
		#elif defined(__SSE__)
			#define GSOUND_SSE_MAJOR_VERSION 1
			#define GSOUND_SSE_MINOR_VERSION 0
		#endif
	#elif defined(GSOUND_COMPILER_MSVC)
		// Assume the newest version because MSVC has no way to determine this.
		#define GSOUND_SSE_MAJOR_VERSION 4
		#define GSOUND_SSE_MINOR_VERSION 2
	#endif
	
	
	/// A macro which produces a boolean value indicating whether the specified (major,minor) version of SSE is supported.
	#define GSOUND_SSE_VERSION_IS_SUPPORTED( majorVersion, minorVersion ) \
				(GSOUND_SSE_VERSION_IS_ALLOWED( majorVersion, minorVersion ) && \
				((majorVersion < GSOUND_SSE_MAJOR_VERSION) || \
				(majorVersion == GSOUND_SSE_MAJOR_VERSION && minorVersion <= GSOUND_SSE_MINOR_VERSION)))
	
	
	#if GSOUND_SSE_VERSION_IS_SUPPORTED(1,0)
		#include <xmmintrin.h> // Include for SSE intrinsics
	#endif

	#if GSOUND_SSE_VERSION_IS_SUPPORTED(2,0)
		#include <emmintrin.h> // Include for SSE2 intrinsics
	#endif

	#if GSOUND_SSE_VERSION_IS_SUPPORTED(3,0)
		#include <pmmintrin.h> // Include for SSE3 intrinsics
	#endif

	#if GSOUND_SSE_VERSION_IS_SUPPORTED(3,1)
		#include <tmmintrin.h> // Include for SSSE3 intrinsics
	#endif

	#if GSOUND_SSE_VERSION_IS_SUPPORTED(4,1)
		#include <smmintrin.h> // Include for SSE4.2 intrinsics
	#endif

	#if GSOUND_SSE_VERSION_IS_SUPPORTED(4,2)
		#include <nmmintrin.h> // Include for SSE4.1 intrinsics
	#endif

#endif
#else // !GSOUND_USE_SIMD
	
	/// A macro which produces a boolean value indicating whether the specified (major,minor) version of SSE is supported.
	#define GSOUND_SSE_VERSION_IS_SUPPORTED( majorVersion, minorVersion ) 0
	
#endif




//##########################################################################################
//##########################################################################################
//############
//############		Library Assertion Configuration
//############
//##########################################################################################
//##########################################################################################



#if !GSOUND_DISABLE_ASSERTIONS

	// Define the assertion to use in only debug builds.
	#ifdef GSOUND_DEBUG
		// Make sure that debugging is enabled.
		#ifdef NDEBUG
			#undef NDEBUG

			#include <assert.h>
			#define GSOUND_DEBUG_ASSERT(X) assert(X)
			#define GSOUND_DEBUG_ASSERT_MESSAGE(X, MESSAGE) { ( X ? 0 : std::printf("%s\n",MESSAGE)); assert(X); }
			#define GSOUND_DEBUG_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) { ( X ? 0 : \
					std::printf("%s\nError Code: %d\n",MESSAGE,(int)CODE)); assert(X); }

			#define NDEBUG
		#else
			#include <assert.h>
			#define GSOUND_DEBUG_ASSERT(X) assert(X)
			#define GSOUND_DEBUG_ASSERT_MESSAGE(X, MESSAGE) { ( X ? 0 : std::printf("%s\n",MESSAGE)); assert(X); }
			#define GSOUND_DEBUG_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) { ( X ? 0 : \
					std::printf("%s\nError Code: %d\n",MESSAGE,(int)CODE)); assert(X); }
		#endif
	#else
		#define GSOUND_DEBUG_ASSERT(X) ((void)0)
		#define GSOUND_DEBUG_ASSERT_MESSAGE(X, MESSAGE) ((void)0)
	#endif


	// Define the assertion to use for release builds.
	#ifdef NDEBUG
		#undef NDEBUG
		#include <assert.h>
		#define GSOUND_ASSERT(X) assert(X)
		#define GSOUND_ASSERT_MESSAGE(X, MESSAGE) { ( X ? 0 : std::printf("%s\n",MESSAGE)); assert(X); }
		#define GSOUND_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) { ( X ? 0 : \
					std::printf("%s\nError Code: %d\n",MESSAGE,(int)CODE)); assert(X); }
		#define NDEBUG
	#else
		#include <assert.h>
		#define GSOUND_ASSERT(X) assert(X)
		#define GSOUND_ASSERT_MESSAGE(X, MESSAGE) { ( X ? 0 : std::printf("%s\n",MESSAGE)); assert(X); }
		#define GSOUND_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) { ( X ? 0 : \
					std::printf("%s\nError Code: %d\n",MESSAGE,(int)CODE)); assert(X); }
	#endif

#else

	// Define dummy assertion macros if assertions are disabled.
	#define GSOUND_ASSERT(X) ((void)0)
	#define GSOUND_ASSERT_MESSAGE(X, MESSAGE) ((void)0)
	#define GSOUND_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) ((void)0)
	#define GSOUND_DEBUG_ASSERT(X) ((void)0)
	#define GSOUND_DEBUG_ASSERT_MESSAGE(X, MESSAGE) ((void)0)

#endif




//##########################################################################################
//##########################################################################################
//############
//############		Library Endian-ness Configuration
//############
//##########################################################################################
//##########################################################################################




#if	defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
	#define GSOUND_BIG_ENDIAN
#else
	#define GSOUND_LITTLE_ENDIAN
#endif




//##########################################################################################
//##########################################################################################
//############
//############		Library Namespace Configuration
//############
//##########################################################################################
//##########################################################################################




/// Define the name of the main GSound library namespace.
#ifndef GSOUND_NAMESPACE
	#define GSOUND_NAMESPACE gsound
#endif




/// Define a macro used to start the gsound namespace.
#ifndef GSOUND_NAMESPACE_START
	#define GSOUND_NAMESPACE_START		namespace GSOUND_NAMESPACE {
#endif




/// Define a macro used to end the gsound namespace.
#ifndef GSOUND_NAMESPACE_END
	#define GSOUND_NAMESPACE_END		};
#endif




/// The enclosing namespace for the entire GSound library.
/**
  * All members of this namespace are considered 'public' and will not change
  * without a deprecation notice. All classes that are necessary for necessary
  * use of the GSound library are either declared in or imported into this namespace.
  */
GSOUND_NAMESPACE_START
GSOUND_NAMESPACE_END




//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############
//############		Define NULL (if not already avalible)
//############
//##########################################################################################
//##########################################################################################




#ifndef NULL
	#ifndef __cplusplus
		#define NULL ((void *)0)
	#else
		#define NULL 0
	#endif
#endif




//##########################################################################################
//##########################################################################################
//############
//############		Sized Floating-Point Primitive Type Definitions
//############
//##########################################################################################
//##########################################################################################




/// Define the type used to represent a 32-bit floating point number.
typedef float				Float32;


/// Define the type used to represent a 64-bit floating point number.
typedef double				Float64;




//##########################################################################################
//##########################################################################################
//############
//############		Sized Integer Primitive Type Definitions
//############
//##########################################################################################
//##########################################################################################




/// Define the type used to represent an 8-bit signed integral number.
typedef char				Int8;


/// Define the type used to represent an 8-bit unsigned integral number.
typedef unsigned char		UInt8;


/// Define the type used to represent a 16-bit signed integral number.
typedef short				Int16;


/// Define the type used to represent a 16-bit unsigned integral number.
typedef unsigned short		UInt16;


/// Define the type used to represent a 32-bit signed integral number.
typedef int					Int32;


/// Define the type used to represent a 32-bit unsigned integral number.
typedef unsigned int		UInt32;


/// Define the type used to represent a 64-bit signed integral number.
typedef long long			Int64;


/// Define the type used to represent a 64-bit unsigned integral number.
typedef unsigned long long	UInt64;




//##########################################################################################
//##########################################################################################
//############
//############		Standard Primitive Type Redefinitions
//############
//##########################################################################################
//##########################################################################################




/// Redefine the standard 'bool' primitive type to use the library's type naming conventions.
typedef bool Bool;


/// Redefine the standard 'short' primitive type to use the library's type naming conventions.
typedef short				Short;


/// Redefine the standard 'unsigned short' primitive type to use the library's type naming conventions.
typedef unsigned short		UShort;


/// Redefine the standard 'int' primitive type to use the library's type naming conventions.
typedef int					Int;


/// Redefine the standard 'unsigned int' primitive type to use the library's type naming conventions.
typedef unsigned int		UInt;


/// Redefine the standard 'long' primitive type to use the library's type naming conventions.
typedef long				Long;


/// Redefine the standard 'unsigned long' primitive type to use the library's type naming conventions.
typedef unsigned long		ULong;


/// Redefine the standard 'long long' primitive type to use the library's type naming conventions.
typedef long long			LongLong;


/// Redefine the standard 'unsigned long long' primitive type to use the library's type naming conventions.
typedef unsigned long long	ULongLong;


/// Redefine the standard 'float' primitive type to use the library's type naming conventions.
typedef float				Float;


/// Redefine the standard 'double' primitive type to use the library's type naming conventions.
typedef double				Double;




//##########################################################################################
//##########################################################################################
//############
//############		Application-Specific Primitive Type Definitions
//############
//##########################################################################################
//##########################################################################################




/// Define the type to use when holding signed data, should be 8 bits wide.
typedef Int8				Byte;


/// Define the type to use when holding generic data, should be 8 bits wide.
typedef UInt8				UByte;


/// Define the type to use when working with ASCII character data.
typedef char				Char;


/// Define the type to use for hash codes in hash tables, should be an unsigned integer.
typedef UInt32				Hash;


/// Define the type to use to represent a quanitity of something, should be an unsigned integer.
typedef std::size_t			Size;


/// Define the type to use to represent an offset (index) in an array or sequence of things.
typedef Size				Index;




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################




#endif // INCLUDE_GSOUND_CONFIG_H
