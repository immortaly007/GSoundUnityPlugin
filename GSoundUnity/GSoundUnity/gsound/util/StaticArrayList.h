/*
 * Project:     GSound
 * 
 * File:        gsound/StaticArrayList.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::util::StaticArrayList class declaration
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


#ifndef INCLUDE_GSOUND_STATIC_ARRAY_LIST_H
#define INCLUDE_GSOUND_STATIC_ARRAY_LIST_H


#include "GSoundUtilitiesConfig.h"


//##########################################################################################
//*************************  Start GSound Utilities Namespace  *****************************
GSOUND_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// An array-based list class with a static element capacity.
/**
  * The StaticArrayList class allows basic list operations: add(), remove(),
  * insert(), clear() and getSize(). Once the static capacity of the list is
  * reached, no more elements can be added to the list.
  */
template < typename T, Size capacity >
class StaticArrayList
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a new empty static array list.
			GSOUND_INLINE StaticArrayList()
				:	numElements( 0 ),
					array( (T*)data )
			{
			}
			
			
			
			
			/// Create a copy of another static array list, performing a deep copy.
			GSOUND_INLINE StaticArrayList( const StaticArrayList& otherArray )
				:	numElements( otherArray.numElements ),
					array( (T*)data )
			{
				StaticArrayList::copyObjects( array, otherArray.array, otherArray.numElements );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Equality Operators
			
			
			
			
			/// Return whether or not whether every entry in this list is equal to another list's entries.
			GSOUND_INLINE Bool operator == ( const StaticArrayList<T,capacity>& other ) const
			{
				// If the arraylists point to the same data, they are equal.
				if ( array == other.array )
					return true;
				else if ( numElements != other.numElements )
					return false;
				
				// Do an element-wise comparison otherwise.
				const T* a = array;
				const T* b = other.array;
				const T* const aEnd = a + numElements;
				
				while ( a != aEnd )
				{
					if ( !(*a == *b) )
						return false;
					
					a++;
					b++;
				}
				
				return true;
			}
			
			
			
			
			/// Return whether or not whether any entry in this list is not equal to another list's entries.
			GSOUND_INLINE Bool operator != ( const StaticArrayList<T,capacity>& other ) const
			{
				return !(*this == other);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Add Methods
			
			
			
			
			/// Add an element to the end of the static list.
			/** 
			  * If the capacity of the static array list is not great enough to hold
			  * the new element, then FALSE is returned and the static array list is
			  * unaffected. Otherwise, the element is successfully added and TRUE
			  * is returned.
			  * 
			  * @param newElement - the new element to add to the end of the static array list.
			  * @return whether or not the element was successfully added.
			  */
			GSOUND_INLINE Bool add( const T& newElement )
			{
				if ( numElements != capacity )
				{
					new (array + numElements) T( newElement );
					numElements++;
					
					return true;
				}
				else
					return false;
			}
			
			
			
			
			/// Add the contents of one static array list to another.
			/**
			  * This method has the effect of adding each element of
			  * the given list to the end of this array list in order.
			  * 
			  * @param list - the list to be added to the end of this list
			  */
			template < Size otherCapacity >
			void addAll( const StaticArrayList<T,otherCapacity>& list )
			{
				if ( numElements + list.numElements <= capacity )
				{
					StaticArrayList::copyObjects( array + numElements, list.array, list.numElements );
					numElements += list.numElements;
					
					return true;
				}
				else
					return false;
			}
			
			
			
			
			/// Insert an element at the specified index of the static array list.
			/** 
			  * The method returns TRUE if the element was successfully inserted
			  * into the static array list. If the index is outside of the bounds of the
			  * static array list, then FALSE is returned, indicating that the element
			  * was not inserted. FALSE will also be returned if there is no
			  * more room in the static array list. This method has an average case
			  * time complexity of O(n/2) because all subsequent elements in
			  * the static array list have to be moved towards the end of the array by one
			  * index.
			  * 
			  * @param newElement - the new element to insert into the static array list.
			  * @param index - the index at which to insert the new element.
			  * @return whether or not the element was successfully inserted into the static array list.
			  */
			GSOUND_INLINE Bool insert( const T& newElement, Index index )
			{
				if ( index >= 0 && index <= numElements && numElements != capacity )
				{
					T* destination = array + numElements;
					const T* source = array + numElements - 1;
					const T* const sourceEnd = array + index - 1;
					
					while ( source != sourceEnd )
					{
						new (destination) T(*source);
						source->~T();
						
						source--;
						destination--;
					}
					
					new (array + index) T( newElement );
					numElements++;
					
					return true;
				}
				else
					return false;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Set Method
			
			
			
			
			/// Set an element at the specified index of the static array list to a new value.
			/** 
			  * This method returns TRUE if the specified index is within the bounds
			  * of the static array list, indicating that the element was successfully set
			  * at that position in the static array list. Otherwise, FALSE is returned, 
			  * indicating that the index was out of bounds of the static array list. This
			  * method has worst-case time complexity of O(1).
			  * 
			  * @param newElement - the new element to set in the static array list.
			  * @param index - the index at which to set the new element.
			  * @return whether or not the element was successfully set to the new value.
			  */
			GSOUND_INLINE Bool set( const T& newElement, Index index )
			{
				if ( index < numElements )
				{
					// destroy the old element.
					array[index].~T();
					
					// replace it with the new element.
					new (array + index) T(newElement);
					
					return true;
				}
				else
					return false;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Remove Methods
			
			
			
			
			/// Remove the element at the specified index, ordered version.
			/** 
			  * If the index is within the bounds of the static array list ( >= 0 && < getSize() ),
			  * then the static array list element at that index is removed and TRUE is returned,
			  * indicating that the remove operation was successful.
			  * Otherwise, FALSE is returned and the static array list
			  * is unaffected. The order of the static array list is unaffected, meaning that 
			  * all of the elements after the removed element must be copied one 
			  * index towards the beginning of the static array list. This gives the method
			  * an average case performance of O(n/2) where n is the number of 
			  * elements in the static array list.
			  * 
			  * @param index - the index of the static array list element to remove.
			  * @return whether or not the element was successfully removed.
			  */
			GSOUND_INLINE Bool removeAtIndex( Index index )
			{
				if ( index < numElements )
				{
					// shift all elements forward in the array one index.
					numElements--;
					
					// Destroy the element to be removed.
					array[index].~T();
					
					// Move the objects to fill the hole in the array.
					StaticArrayList::moveObjects( array + index, array + index + 1, numElements - index );
					
					return true;
				}
				else
					return false;
			}
			
			
			
			
			/// Remove the element at the specified index, unordered version.
			/** 
			  * If the index is within the bounds of the static array list ( >= 0 && < getSize() ),
			  * then the static array list element at that index is removed and TRUE is returned,
			  * indicating that the remove operation was successful.
			  * Otherwise, FALSE is returned and the static array list is unaffected. 
			  * The order of the static array list is affected when this method
			  * successfully removes the element. It works by replacing the element
			  * at the index to be removed with the last element in the static array list. This 
			  * gives the method a worst case time complexity of O(1), which is
			  * much faster than the ordered remove methods.
			  * 
			  * @param index - the index of the static array list element to remove.
			  * @return whether or not the element was successfully removed.
			  */
			GSOUND_INLINE Bool removeAtIndexUnordered( Index index )
			{
				if ( index < numElements )
				{
					numElements--;
					
					// Destroy the element to be removed.
					T* destination = array + index;
					destination->~T();
					
					// Replace it with the last element if necessary.
					if ( index != numElements )
					{
						T* source = array + numElements;
						new (destination) T(*source);
						source->~T();
					}
					
					return true;
				}
				else
					return false;
			}
			
			
			
			
			/// Remove the first element equal to the parameter element, ordered version.
			/** 
			  * If this element is found, then it is removed and TRUE is returned.
			  * Otherwise, FALSE is returned and the static array list is unaffected.
			  * The order of the static array list is unaffected, meaning that all of the elements after 
			  * the removed element must be copied one index towards the beginning 
			  * of the static array list. This gives the method an average case performance
			  * of O(n) where n is the number of elements in the static array list. This
			  * method's complexity is worse than the ordered index remove method
			  * because it must search through the static array list for the element and then
			  * copy all subsequent elements one position nearer to the start of the
			  * list.
			  *
			  * @param element - the element to remove the first instance of.
			  * @return whether or not the element was successfully removed.
			  */
			GSOUND_INLINE Bool remove( const T& element )
			{
				Index index;
				
				if ( getIndex( element, index ) )
				{
					// shift all elements forward in the array one index.
					numElements--;
					
					// Destroy the element to be removed.
					array[index].~T();
					
					// Move the objects to fill the hole in the array.
					StaticArrayList::moveObjects( array + index, array + index + 1, numElements - index );
					
					return true;
				}
				else
					return false;
			}
			
			
			
			
			/// Remove the first element equal to the parameter element, unordered version.
			/** 
			  * If this element is found, then it is removed and TRUE is returned. 
			  * Otherwise, FALSE is returned and the static array list is unaffected.
			  * The order of the static array list is affected when this method
			  * successfully removes the element. It works by replacing the element
			  * at the index to be removed with the last element in the static array list. This 
			  * gives the method a worst case time complexity of O(1), which is
			  * much faster than the ordered remove methods.
			  *
			  * @param element - the static array list element to remove the first instance of.
			  * @return whether or not the element was successfully removed.
			  */
			GSOUND_INLINE Bool removeUnordered( const T& element )
			{
				Index index;
				
				if ( getIndex( element, index ) )
				{
					numElements--;
					
					// Destroy the element to be removed.
					T* destination = array + index;
					destination->~T();
					
					// Replace it with the last element if necessary.
					if ( index != numElements )
					{
						T* source = array + numElements;
						new (destination) T(*source);
						source->~T();
					}
					
					return true;
				}
				else
					return false;
			}
			
			
			
			
			/// Remove the last element in the static array list.
			/** 
			  * If the static array list has elements remaining in it, then
			  * the last element in the array list is removed and TRUE is returned.
			  * If the static array list has no remaining elements, then FALSE is returned,
			  * indicating that the static array list was unaffected. This method has worst
			  * case O(1) time complexity.
			  * 
			  * @return whether or not the last element was successfully removed.
			  */
			GSOUND_INLINE Bool removeLast()
			{
				if ( numElements != Size(0) )
				{
					numElements--;
					
					// destroy the last element.
					array[numElements].~T();
					
					return true;
				}
				else
					return false;
			}
			
			
			
			
			/// Remove the last N elements from the static array list.
			/** 
			  * If the static array list has at least N elements remaining in it, then
			  * the last N elements in the array list are removed and N is returned.
			  * If the array list has less than N elements, then the list will be
			  * completely cleared, resulting in an empty list. The method returns the
			  * number of elements successfully removed.
			  * 
			  * @return the number of elements removed from the end of the list.
			  */
			GSOUND_INLINE Size removeLast( Size number )
			{
				number = numElements > number ? number : numElements;
				numElements -= number;
				
				// destroy the elements that were removed.
				ArrayList<T>::callDestructors( array + numElements, number );
				
				return number;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Clear Method
			
			
			
			
			/// Clear the contents of this static array list.
			/**
			  * This method calls the destructors of all elements in the static
			  * array and sets the number of elements to zero while maintaining the
			  * array's capacity.
			  */
			GSOUND_INLINE void clear()
			{
				StaticArrayList::callDestructors( array, numElements );
				numElements = Size(0);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Contains Method
			
			
			
			
			/// Return whether or not the specified element is in this static array list.
			/**
			  * The method has average case O(n/2) time complexity, where
			  * n is the number of elements in the static array list. This method
			  * is here for convenience. It just calls the static array list's
			  * getIndex() method, and tests to see if the return value is
			  * not equal to -1. It is recommended that if one wants the 
			  * index of the element as well as whether or not it is contained
			  * in the static array list, they should use the getIndex() method exclusively,
			  * and check the return value to make sure that the element is in the
			  * static array list. This avoids the double O(n/2) lookup that would be performed
			  * one naively called this method and then that method.
			  * 
			  * @param element - the element to check to see if it is contained in the static array list.
			  * @return whether or not the specified element is in the static array list.
			  */
			GSOUND_INLINE Bool contains( const T& anElement ) const
			{
				T* element = array;
				const T* const end = array + numElements;
				
				while ( element != end )
				{
					if ( *element == anElement )
						return true;
				}
				
				return false;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Element Find Method
			
			
			
			
			/// Return the index of the  element equal to the parameter element.
			/** 
			  * If the specified element is not found within the static array list,
			  * then -1 is returned. Otherwise, the index of the element 
			  * equal to the parameter is returned.
			  * 
			  * @param element - the element to find in the static array list.
			  * @return the index of the element which was found, or -1 if it was not found.
			  */
			GSOUND_INLINE Bool getIndex( const T& object, Index& index ) const
			{
				T* element = array;
				const T* const end = array + numElements;
				
				while ( element != end )
				{
					if ( *element == object )
					{
						index = element - array;
						return true;
					}
				}
				
				return false;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Element Accessor Methods
			
			
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the static array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the const version
			  * of the get() method, disallowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			GSOUND_INLINE const T& get( Index index ) const
			{
				GSOUND_DEBUG_ASSERT( index < numElements );
				
				return array[index];
			}
			
			
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the static array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the non-const version
			  * of the get() method, allowing modification of the element via the
			  * returned non-const reference.
			  * 
			  * @param index - the index of the desired element.
			  * @return a reference to the element at the index specified by the parameter.
			  */
			GSOUND_INLINE T& get( Index index )
			{
				GSOUND_DEBUG_ASSERT( index < numElements );
				
				return array[index];
			}
			
			
			
			
			/// Return a reference to the first element in the static array list.
			GSOUND_INLINE T& getFirst()
			{
				GSOUND_DEBUG_ASSERT( numElements != Size(0) );
				
				return *array;
			}
			
			
			
			
			/// Return a const reference to the first element in the static array list.
			GSOUND_INLINE const T& getFirst() const
			{
				GSOUND_DEBUG_ASSERT( numElements != Size(0) );
				
				return *array;
			}
			
			
			
			
			/// Return a reference to the last element in the static array list.
			GSOUND_INLINE T& getLast()
			{
				GSOUND_DEBUG_ASSERT( numElements != Size(0) );
				
				return *(array + numElements - 1);
			}
			
			
			
			
			/// Return a const reference to the last element in the static array list.
			GSOUND_INLINE const T& getLast() const
			{
				GSOUND_DEBUG_ASSERT( numElements != Size(0) );
				
				return *(array + numElements - 1);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Element Accessor Operators
			
			
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the static array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the const version
			  * of the operator (), disallowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			GSOUND_INLINE const T& operator () ( Index index ) const
			{
				GSOUND_DEBUG_ASSERT( index < numElements );
				
				return array[index];
			}
			
			
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the static array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the non-const version
			  * of the operator (), allowing modification of the element via the
			  * returned non-const reference.
			  * 
			  * @param index - the index of the desired element.
			  * @return a reference to the element at the index specified by the parameter.
			  */
			GSOUND_INLINE T& operator () ( Index index )
			{
				GSOUND_DEBUG_ASSERT( index < numElements );
				
				return array[index];
			}
			
			
			
			
			/// Get a const pointer to the first element in the static array list.
			GSOUND_INLINE operator const T* () const
			{
				return array;
			}
			
			
			
			
			/// Get a pointer to the first element in the static array list.
			GSOUND_INLINE operator T* ()
			{
				return array;
			}
			
			
			
			
			/// Return a const pointer to the beginning of the internal array.
			GSOUND_INLINE const T* getArrayPointer() const
			{
				return array;
			}
			
			
			
			
			/// Return a pointer to the beginning of the internal array.
			GSOUND_INLINE T* getArrayPointer()
			{
				return array;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			
			
			/// Return whether or not the static array list has any elements.
			/** 
			  * This method returns TRUE if the size of the static array list
			  * is greater than zero, and FALSE otherwise.
			  * This method is here for convenience.
			  * 
			  * @return whether or not the static array list has any elements.
			  */
			GSOUND_INLINE Bool isEmpty() const
			{
				return numElements == 0;
			}
			
			
			
			
			/// Get the number of elements in the static array list.
			/** 
			  * @return the number of elements in the static array list.
			  */
			GSOUND_INLINE Size getSize() const
			{
				return numElements;
			}
			
			
			
			
			/// Get the capacity of the static array list.
			/**
			  * The capacity is the maximum number of elements that the
			  * static array list can hold. This value does not change during
			  * the lifetime of the static array list, hence the name.
			  * 
			  * @return the current capacity of the static array list.
			  */
			GSOUND_INLINE Size getCapacity() const
			{
				return capacity;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Class
			
			
			
			
			/// Iterator class for an static array list.
			/** 
			  * The purpose of this class is to iterate through all
			  * or some of the elements in the static array list, making changes as
			  * necessary to the elements.
			  */
			class Iterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new static array list iterator from a reference to a list.
						GSOUND_INLINE Iterator( StaticArrayList<T,capacity>& newList )
							:	list( newList ),
								current( newList.array ),
								end( newList.array + newList.numElements )
						{
						}
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Prefix increment operator.
						GSOUND_INLINE void operator ++ ()
						{
							current++;
						}
						
						
						
						
						/// Postfix increment operator.
						GSOUND_INLINE void operator ++ ( int )
						{
							current++;
						}
						
						
						
						
						/// Return whether or not the iterator is at the end of the list.
						/** 
						  * If the iterator is at the end of the list, return FALSE.
						  * Otherwise, return TRUE, indicating that there are more
						  * elements to iterate over.
						  * 
						  * @return FALSE if at the end of list, otherwise TRUE.
						  */
						GSOUND_INLINE operator Bool () const
						{
							return current < end;
						}
						
						
						
						
						/// Return a reference to the current iterator element.
						GSOUND_INLINE T& operator * ()
						{
							return *current;
						}
						
						
						
						
						/// Access the current iterator element.
						GSOUND_INLINE T* operator -> ()
						{
							return current;
						}
						
						
						
						
						/// Remove the current element from the list.
						/**
						  * This method calls the removeAtIndex() method of the 
						  * iterated static array list, and therefore has an average
						  * time complexity of O(n/2) where n is the size of the
						  * array list.
						  */
						GSOUND_INLINE void remove()
						{
							list.removeAtIndex( getIndex() );
							current = current == list.array ? current : current - 1;
							end--;
						}
						
						
						
						
						/// Remove the current element from the list.
						/**
						  * This method calls the removeAtIndexUnordered() method of the 
						  * iterated static array list, and therefore has an average
						  * time complexity of O(1).
						  */
						GSOUND_INLINE void removeUnordered()
						{
							list.removeAtIndexUnordered( getIndex() );
							current = current == list.array ? current : current - 1;
							end--;
						}
						
						
						
						
						/// Reset the iterator to the beginning of the list.
						GSOUND_INLINE void reset()
						{
							current = list.array;
							end = current + list.numElements;
						}
						
						
						
						
						/// Get the index of the next element to be iterated over.
						GSOUND_INLINE Index getIndex()
						{
							return current - list.array;
						}
						
						
						
					
				private:
					
					//********************************************
					//	Private Data Members
						
						
						
						/// The current position of the iterator
						T* current;
						
						
						
						
						/// A pointer to one element past the end of the list.
						const T* end;
						
						
						
						
						/// The list that is being iterated over.
						StaticArrayList<T,capacity>& list;
						
						
						
						
						/// Make the const iterator class a friend.
						friend class StaticArrayList<T,capacity>::ConstIterator;
						
						
						
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	ConstIterator Class
			
			
			
			
			/// An iterator class for a static array list which can't modify it.
			/** 
			  * The purpose of this class is to iterate through all
			  * or some of the elements in the static array list.
			  */
			class ConstIterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new static array list iterator from a reference to a list.
						GSOUND_INLINE ConstIterator( const StaticArrayList<T,capacity>& newList )
							:	list( newList ),
								current( newList.array ),
								end( newList.array + newList.numElements )
						{
						}
						
						
						
						/// Create a new const static array list iterator from a non-const iterator.
						GSOUND_INLINE ConstIterator( const Iterator& iterator )
							:	list( iterator.list ),
								current( iterator.current ),
								end( iterator.end )
						{
						}
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Prefix increment operator.
						GSOUND_INLINE void operator ++ ()
						{
							current++;
						}
						
						
						
						
						/// Postfix increment operator.
						GSOUND_INLINE void operator ++ ( int )
						{
							current++;
						}
						
						
						
						
						/// Return whether or not the iterator is at the end of the list.
						/** 
						  * If the iterator is at the end of the list, return FALSE.
						  * Otherwise, return TRUE, indicating that there are more
						  * elements to iterate over.
						  * 
						  * @return FALSE if at the end of list, otherwise TRUE.
						  */
						GSOUND_INLINE operator Bool () const
						{
							return current < end;
						}
						
						
						
						
						/// Return a const-reference to the current iterator element.
						GSOUND_INLINE const T& operator * () const
						{
							return *current;
						}
						
						
						
						
						/// Access the current iterator element.
						GSOUND_INLINE const T* operator -> () const
						{
							return current;
						}
						
						
						
						
						/// Reset the iterator to the beginning of the list.
						GSOUND_INLINE void reset()
						{
							current = list.array;
							end = current + list.numElements;
						}
						
						
						
						
						/// Get the index of the next element to be iterated over.
						GSOUND_INLINE Index getIndex() const
						{
							return current - list.array;
						}
						
						
						
					
				private:
					
					//********************************************
					//	Private Data Members
						
						
						
						/// The current position of the iterator
						const T* current;
						
						
						
						
						/// A pointer to one element past the end of the list.
						const T* end;
						
						
						
						
						/// The list that is being iterated over.
						const StaticArrayList<T,capacity>& list;
						
						
						
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Creation Methods
			
			
			
			
			/// Return an iterator for the static array list.
			/**
			  * The iterator serves to provide a way to efficiently iterate
			  * over the elements of the static array list. It is more useful for
			  * a linked list type of data structure, but it is provided for
			  * uniformity among data structures.
			  * 
			  * @return an iterator for the static array list.
			  */
			GSOUND_INLINE Iterator getIterator()
			{ 
				return Iterator(*this);
			}
			
			
			
			
			/// Return a const iterator for the static array list.
			/**
			  * The iterator serves to provide a way to efficiently iterate
			  * over the elements of the static array list. It is more useful for
			  * a linked list type of data structure, but it is provided for
			  * uniformity among data structures.
			  * 
			  * @return an iterator for the static array list.
			  */
			GSOUND_INLINE ConstIterator getIterator() const
			{ 
				return ConstIterator(*this);
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Methods
			
			
			
			
			GSOUND_INLINE static void callDestructors( T* array, Size number )
			{
				const T* const arrayEnd = array + number;
				
				while ( array != arrayEnd )
				{
					array->~T();
					array++;
				}
			}
			
			
			
			
			GSOUND_INLINE static void copyObjects( T* destination, const T* source, Size number )
			{
				const T* const sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					new (destination) T(*source);
					destination++;
					source++;
				}
			}
			
			
			
			
			GSOUND_INLINE static void moveObjects( T* destination, const T* source, Size number )
			{
				const T* const sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					// copy the object from the source to destination
					new (destination) T(*source);
					
					// call the destructors on the source
					source->~T();
					
					destination++;
					source++;
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The array holding all elements in this static array list.
			T* array;
			
			
			
			/// The number of elements in the static array list.
			Size numElements;
			
			
			
			
			/// The array of bytes used to allocate memory for the array.
			UByte data[capacity*sizeof(T)];
			
			
			
			
};




//##########################################################################################
//*************************  End GSound Utilities Namespace  *******************************
GSOUND_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_STATIC_ARRAY_LIST_H
