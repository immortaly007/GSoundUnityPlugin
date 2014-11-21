/*
 * Project:     GSound
 * 
 * File:        gsound/Allocator.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    Functions for memory allocation, deallocation and management
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


#ifndef INCLUDE_GSOUND_ALLOCATOR_H
#define INCLUDE_GSOUND_ALLOCATOR_H


#include "GSoundUtilitiesConfig.h"


//##########################################################################################
//*************************  Start GSound Utilities Namespace  *****************************
GSOUND_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Object Allocation Methods
//############		
//##########################################################################################
//##########################################################################################




/// Allocate memory for a single object of type T.
/**
  * This method returns a pointer to the start of the object in memory.
  * The constructor of the allocated object is not called. It is the
  * responsibility of the user to use placement new to initialize the
  * object. Alternatively, one can call the construct() method instead
  * which initializes the memory location by calling placement new after
  * allocating the memory.
  */
template < typename T >
GSOUND_FORCE_INLINE T* allocate()
{
	// Call a macro defined in RimConfig,h to allocate the memory.
	register T* memory = (T*)GSOUND_MALLOC(sizeof(T));
	
	// Make sure that the memory location returned is not null.
	GSOUND_DEBUG_ASSERT( memory != NULL );
	
	return memory;
}




/// Allocate memory for the specified number of objects of type T.
/**
  * This method returns a pointer to the start of the first object in memory.
  * No constructors are called on the allocated memory locations. One must use
  * placement new in order to initialize the objects.
  * 
  * @param count - the number of objects to allocate space for.
  */
template < typename T >
GSOUND_FORCE_INLINE T* allocate( Size count )
{
	// Call a macro defined in RimConfig,h to allocate the memory.
	register T* memory = (T*)GSOUND_MALLOC(count*sizeof(T));
	
	// Make sure that the memory location returned is not null.
	GSOUND_DEBUG_ASSERT( memory != NULL );
	
	return memory;
}




/// Allocate aligned memory for the specified number of objects of type T.
/**
  * This method returns a pointer to the start of the first object in memory.
  * No constructors are called on the allocated memory locations. One must use
  * placement new in order to initialize the objects.
  * 
  * @param count - the number of objects to allocate space for.
  * @param alignment - the required alignment for the memory in bytes.
  */
template < typename T >
GSOUND_FORCE_INLINE T* allocateAligned( Size count, Size alignment )
{
	// Call a macro defined in RimConfig,h to allocate the memory.
	register T* memory = (T*)GSOUND_ALIGNED_MALLOC(count*sizeof(T), alignment);
	
	// Make sure that the memory location returned is not null.
	GSOUND_DEBUG_ASSERT( memory != NULL );
	
	return memory;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Pointer Deallocation Method
//############		
//##########################################################################################
//##########################################################################################




/// Free the memory block pointed to by the specified pointer.
/**
  * If the pointer is NULL, an assertion is raised.
  * 
  * @param pointer - a pointer to the memory to be deallocated.
  */
template < typename T >
GSOUND_FORCE_INLINE void deallocate( T* pointer )
{
	// Make sure that the memory location to free is not null.
	GSOUND_DEBUG_ASSERT_MESSAGE( pointer != NULL, "Cannot deallocate NULL pointer." );
	
	// Call a macro defined in GSoundConfig.h to free the memory.
	GSOUND_FREE( pointer );
}




/// Free the aligned memory block pointed to by the specified pointer.
/**
  * If the pointer is NULL, an assertion is raised.
  * 
  * @param pointer - a pointer to the memory to be deallocated.
  */
template < typename T >
GSOUND_FORCE_INLINE void deallocateAligned( T* pointer )
{
	// Make sure that the memory location to free is not null.
	GSOUND_DEBUG_ASSERT_MESSAGE( pointer != NULL, "Cannot deallocate NULL pointer." );
	
	// Call a macro defined in GSoundConfig.h to free the memory.
	GSOUND_ALIGNED_FREE( pointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Object Construction Methods
//############		
//##########################################################################################
//##########################################################################################




/// Construct and allocate an object of the templated type T, calling the default constructor.
template < typename T >
GSOUND_FORCE_INLINE T* construct()
{
	register T* object = allocate<T>();
	
	new (object) T();
	
	return object;
}




/// Construct and allocate an object of the templated type T with one constructor parameter.
template < typename T, typename P1 >
GSOUND_FORCE_INLINE T* construct( const P1& p1 )
{
	register T* object = allocate<T>();
	
	new (object) T( p1 );
	
	return object;
}




/// Construct and allocate an object of the templated type T with 2 constructor parameters.
template < typename T, typename P1, typename P2 >
GSOUND_FORCE_INLINE T* construct( const P1& p1, const P2& p2 )
{
	register T* object = allocate<T>();
	
	new (object) T( p1, p2 );
	
	return object;
}




/// Construct and allocate an object of the templated type T with 3 constructor parameters.
template < typename T, typename P1, typename P2, typename P3 >
GSOUND_FORCE_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3 )
{
	register T* object = allocate<T>();
	
	new (object) T( p1, p2, p3 );
	
	return object;
}




/// Construct and allocate an object of the templated type T with 4 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4 >
GSOUND_FORCE_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
{
	register T* object = allocate<T>();
	
	new (object) T( p1, p2, p3, p4 );
	
	return object;
}




/// Construct and allocate an object of the templated type T with 5 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5 >
GSOUND_FORCE_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
{
	register T* object = allocate<T>();
	
	new (object) T( p1, p2, p3, p4, p5 );
	
	return object;
}




/// Construct and allocate an object of the templated type T with 6 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6 >
GSOUND_FORCE_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 )
{
	register T* object = allocate<T>();
	
	new (object) T( p1, p2, p3, p4, p5, p6 );
	
	return object;
}




/// Construct and allocate an object of the templated type T with 7 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7 >
GSOUND_FORCE_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7 )
{
	register T* object = allocate<T>();
	
	new (object) T( p1, p2, p3, p4, p5, p6, p7 );
	
	return object;
}




/// Construct and allocate an object of the templated type T with 8 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7, typename P8 >
GSOUND_FORCE_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8 )
{
	register T* object = allocate<T>();
	
	new (object) T( p1, p2, p3, p4, p5, p6, p7, p8 );
	
	return object;
}




/// Construct and allocate an object of the templated type T with 9 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7, typename P8, typename P9 >
GSOUND_FORCE_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9 )
{
	register T* object = allocate<T>();
	
	new (object) T( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	
	return object;
}




/// Construct and allocate an object of the templated type T with 10 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7, typename P8, typename P9, typename P10 >
GSOUND_FORCE_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10 )
{
	register T* object = allocate<T>();
	
	new (object) T( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
	
	return object;
}





//##########################################################################################
//##########################################################################################
//############		
//############		Object Destruction Method
//############		
//##########################################################################################
//##########################################################################################




/// Destruct and free an object of the templated type T at the specified location.
/**
  * This method calls the destructor of the pointed-to object and then deallocates
  * the memory used by that object. If the object pointer is NULL, an assertion is raised.
  * 
  * @param pointer - a pointer to the object to be destroyed.
  */
template < typename T >
GSOUND_FORCE_INLINE void destruct( T* object )
{
	GSOUND_DEBUG_ASSERT_MESSAGE( object != NULL, "Cannot deallocate NULL object pointer." );
	
	// Call the object's destructor.
	object->~T();
	
	// Deallocate the memory use by the object.
	deallocate( object );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Object Array Construction Methods
//############		
//##########################################################################################
//##########################################################################################




/// Construct and allocate an array of objects of the templated type T, calling the default constructor.
template < typename T >
GSOUND_INLINE T* constructArray( Size number )
{
	register T* objects = allocate<T>( number );
	
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		new (object) T();
		object++;
	}
	
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with one constructor parameter.
template < typename T, typename P1 >
GSOUND_INLINE T* constructArray( Size number, const P1& p1 )
{
	register T* objects = allocate<T>( number );
	
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		new (object) T( p1 );
		object++;
	}
	
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 2 constructor parameters.
template < typename T, typename P1, typename P2 >
GSOUND_INLINE T* constructArray( Size number, const P1& p1, const P2& p2 )
{
	register T* objects = allocate<T>( number );
	
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		new (object) T( p1, p2 );
		object++;
	}
	
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 3 constructor parameters.
template < typename T, typename P1, typename P2, typename P3 >
GSOUND_INLINE T* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3 )
{
	register T* objects = allocate<T>( number );
	
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		new (object) T( p1, p2, p3 );
		object++;
	}
	
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 4 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4 >
GSOUND_INLINE T* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
{
	register T* objects = allocate<T>( number );
	
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		new (object) T( p1, p2, p3, p4 );
		object++;
	}
	
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 5 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5 >
GSOUND_INLINE T* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
{
	register T* objects = allocate<T>( number );
	
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		new (object) T( p1, p2, p3, p4, p5 );
		object++;
	}
	
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 6 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6 >
GSOUND_INLINE T* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 )
{
	register T* objects = allocate<T>( number );
	
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		new (object) T( p1, p2, p3, p4, p5, p6 );
		object++;
	}
	
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 7 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7 >
GSOUND_INLINE T* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7 )
{
	register T* objects = allocate<T>( number );
	
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		new (object) T( p1, p2, p3, p4, p5, p6, p7 );
		object++;
	}
	
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 8 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7, typename P8 >
GSOUND_INLINE T* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8 )
{
	register T* objects = allocate<T>( number );
	
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		new (object) T( p1, p2, p3, p4, p5, p6, p7, p8 );
		object++;
	}
	
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 9 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7, typename P8, typename P9 >
GSOUND_INLINE T* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9 )
{
	register T* objects = allocate<T>( number );
	
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		new (object) T( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
		object++;
	}
	
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 10 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7, typename P8, typename P9, typename P10 >
GSOUND_INLINE T* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10 )
{
	register T* objects = allocate<T>( number );
	
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		new (object) T( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
		object++;
	}
	
	return objects;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Array Copy Methods
//############		
//##########################################################################################
//##########################################################################################




/// Allocate a new array and copy the specified number of elements from an old array.
template < typename T >
GSOUND_INLINE T* copyArray( const T* oldArray, Size number )
{
	register T* newArray = allocate<T>( number );
	
	const T* oldObject = oldArray;
	const T* const oldArrayEnd = oldArray + number;
	T* newObject = newArray;
	
	while ( oldObject != oldArrayEnd )
	{
		new (newObject) T(*oldObject);
		oldObject++;
		newObject++;
	}
	
	return newArray;
}




/// Allocate a new array and copy the specified number of elements from an old array.
template < typename T >
GSOUND_INLINE T* copyArrayAligned( const T* oldArray, Size number, Size alignment )
{
	register T* newArray = allocateAligned<T>( number, alignment );
	
	const T* oldObject = oldArray;
	const T* const oldArrayEnd = oldArray + number;
	T* newObject = newArray;
	
	while ( oldObject != oldArrayEnd )
	{
		new (newObject) T(*oldObject);
		oldObject++;
		newObject++;
	}
	
	return newArray;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Object Array Destruction Method
//############		
//##########################################################################################
//##########################################################################################




/// Destruct and free an array of objects of the templated type T at the specified location.
template < typename T >
GSOUND_INLINE void destructArray( T* objects, Size number )
{
	GSOUND_DEBUG_ASSERT( objects != NULL );
	
	// Call the objects' destructors.
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		object->~T();
		object++;
	}
	
	// Deallocate the memory use by the objects.
	deallocate( objects );
}




//##########################################################################################
//*************************  End GSound Utilities Namespace  *******************************
GSOUND_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_ALLOCATOR_H
