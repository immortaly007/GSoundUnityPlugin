/*
 *  StaticArray.h
 *  GSound
 *
 *  Created by Carl Schissler on 1/11/12.
 *  Copyright 2012 Carl Schissler. All rights reserved.
 *
 */

#ifndef INCLUDE_GSOUND_STATIC_ARRAY_H
#define INCLUDE_GSOUND_STATIC_ARRAY_H


#include "GSoundUtilitiesConfig.h"


//##########################################################################################
//*************************  Start GSound Utilities Namespace  *****************************
GSOUND_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A safe wrapper for basic static arrays.
template < class T, Size size >
class StaticArray
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			/// Create an empty static array with all elements default initialized.
			GSOUND_INLINE StaticArray()
			{
				T* current = (T*)data;
				const T* const arrayEnd = current + size;
				
				while ( current != arrayEnd )
				{
					new (current) T();
					current++;
				}
			}
			
			
			
			
			/// Create a static array with all elements initialized to some prototype value.
			GSOUND_INLINE explicit StaticArray( const T& prototype )
			{
				T* current = (T*)data;
				const T* const arrayEnd = (T*)data + size;
				
				while ( current != arrayEnd )
				{
					new (current) T( prototype );
					current++;
				}
			}
			
			
			
			
			/// Create a static array with elements from the specified pointer.
			GSOUND_INLINE explicit StaticArray( const T* newArray )
			{
				copyElements( (T*)data, newArray );
			}
			
			
			
			
			/// Create a static array with elements from the specified pointer of a different type.
			template < typename U >
			GSOUND_INLINE explicit StaticArray( const U* newArray )
			{
				copyElements( (T*)data, newArray );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Copy Constructors
			
			
			
			
			/// Create a copy of the specified static array.
			GSOUND_INLINE StaticArray( const StaticArray& other )
			{
				T* destination = (T*)data;
				const T* const destinationEnd = destination + size;
				const T* source = (const T*)other.data;
				
				while ( destination != destinationEnd )
				{
					new (destination) T( *source );
					source++;
					destination++;
				}
			}
			
			
			
			
			/// Create a copy of the specified static array with different data type.
			template < typename U >
			GSOUND_INLINE StaticArray( const StaticArray<U,size>& other )
			{
				T* destination = (T*)data;
				const T* const destinationEnd = destination + size;
				const U* source = (const U*)other.data;
				
				while ( destination != destinationEnd )
				{
					new (destination) T( *source );
					source++;
					destination++;
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operators
			
			
			
			/// Assign the contents of one static array to this one.
			GSOUND_INLINE StaticArray& operator = ( const StaticArray& other )
			{
				if ( this != &other )
					memberWiseCopy( (T*)data, (T*)other.data, size );
				
				return *this;
			}
			
			
			
			
			/// Assign the contents of one static array to this one.
			template < typename U >
			GSOUND_INLINE StaticArray& operator = ( const StaticArray<U,size>& other )
			{
				memberWiseCopy( (T*)data, (T*)other.data, size );
				
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			/// Destroy an static array object, releasing a reference to the internal static array.
			GSOUND_INLINE ~StaticArray()
			{
				T* current = (T*)data;
				const T* const arrayEnd = (T*)data + size;
				
				while ( current != arrayEnd )
				{
					current->~T();
					current++;
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Cast Operators
			
			
			
			
			/// Convert this static array to a pointer.
			GSOUND_FORCE_INLINE operator T* ()
			{
				return (T*)data;
			}
			
			
			
			
			/// Convert this static array to a pointer, const version.
			GSOUND_FORCE_INLINE operator const T* () const
			{
				return (const T*)data;
			}
			
			
			
			
			/// Convert this static array to a pointer and return the result.
			GSOUND_FORCE_INLINE T* getPointer()
			{
				return (T*)data;
			}
			
			
			
			
			/// Convert this static array to a pointer and return the result, const version.
			GSOUND_FORCE_INLINE const T* getPointer() const
			{
				return (const T*)data;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			
			
			/// Get the size of this static array.
			GSOUND_FORCE_INLINE Size getSize() const
			{
				return size;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Equality Comparison Operators
			
			
			
			
			/// Return whether or not every element in this Static Array is equal to another's elements.
			GSOUND_INLINE Bool operator == ( const StaticArray& other ) const
			{
				const T* compare1 = (const T*)data;
				const T* compare1End = (const T*)data + size;
				const T* compare2 = other.array;
				
				while ( compare1 != compare1End )
				{
					if ( *compare1 != *compare2 )
						return false;
					
					compare1++;
					compare2++;
				}
				
				return true;
			}
			
			
			
			
			/// Return whether or not every element in this Static Array is not equal to another's elements.
			GSOUND_INLINE Bool operator != ( const StaticArray& other ) const
			{
				return !(*this == other);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	StaticArray Concatenation Operators
			
			
			
			
			/// Apend the contents of this static array to another and return the result.
			template < Size otherSize >
			GSOUND_INLINE StaticArray< T, size + otherSize > operator + ( const StaticArray<T,otherSize>& other ) const
			{
				StaticArray< T, size + otherSize > result;
				
				memberWiseCopy( (const T*)result.data, (const T*)data, size );
				memberWiseCopy( (const T*)result.data + size, (const T*)other.data, otherSize );
				
				return result;
			}
			
			
			
			
	private:
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Methods
			
			
			
			
			template < typename A, typename B >
			static A* memberWiseCopy( A* destination, const B* source, Size number )
			{
				const B* const sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					*destination = *source;
					
					destination++;
					source++;
				}
			}
			
			
			
			
			template < typename A, typename B >
			static void copyElements( A* destination, const B* source )
			{
				A* current = destination;
				const A* const destinationEnd = destination + size;
				
				while ( destination != destinationEnd )
				{
					new (destination) A( *source );
					
					destination++;
					source++;
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The static array of bytes which holds the data for this array.
			UByte data[sizeof(T)*size];
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Friend Class
			
			
			
			
			template < typename U, Size otherSize >
			friend class StaticArray;
			
			
			
};




//##########################################################################################
//*************************  End GSound Utilities Namespace  *******************************
GSOUND_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_STATIC_ARRAY_H
