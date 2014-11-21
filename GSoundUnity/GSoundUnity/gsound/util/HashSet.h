/*
 * Project:     GSound
 * 
 * File:        gsound/HashSet.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::util::HashSet class declaration
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


#ifndef INCLUDE_GSOUND_HASH_SET_H
#define INCLUDE_GSOUND_HASH_SET_H


#include "GSoundUtilitiesConfig.h"


#include "Allocator.h"


//##########################################################################################
//*************************  Start GSound Utilities Namespace  *****************************
GSOUND_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A container class which uses a hash table to keep track of a set of values.
template < typename T >
class HashSet
{
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Entry Class
			
			
			
			
			class Entry
			{
				public:
					
					
					GSOUND_INLINE Entry( Hash newHash, const T& newValue )
						:	next( NULL ),
							hash( newHash ),
							value( newValue )
					{
					}
					
					
					GSOUND_INLINE Entry( const Entry& other )
						:	hash( other.hash ),
							value( other.value )
					{
						if ( other.next )
							next = HashSet::newEntry(*other.next);
						else
							next = NULL;
					}
					
					
					~Entry()
					{
						if ( next )
							delete next;
					}
					
					
					Entry* next;
					
					Hash hash;
					T value;
					
			};
			
			
			
			
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a hash set with the default load factor and number of buckets.
			GSOUND_INLINE HashSet()
				:	loadFactor( DEFAULT_LOAD_FACTOR ),
					loadThreshold( Size(DEFAULT_LOAD_FACTOR*DEFAULT_NUMBER_OF_BUCKETS) ),
					numElements( 0 ),
					numBuckets( DEFAULT_NUMBER_OF_BUCKETS ),
					buckets( util::allocate<Entry*>(DEFAULT_NUMBER_OF_BUCKETS) )
			{
				nullBuckets();
			}
			
			
			
			
			/// Create a hash set with the specified load factor and default number of buckets.
			GSOUND_INLINE HashSet( Float newLoadFactor )
				:	loadFactor( math::clamp( newLoadFactor, 0.1f, 2.0f ) ),
					numElements( 0 ),
					numBuckets( DEFAULT_NUMBER_OF_BUCKETS ),
					buckets( util::allocate<Entry*>(DEFAULT_NUMBER_OF_BUCKETS) )
			{
				loadThreshold = Size(loadFactor*DEFAULT_NUMBER_OF_BUCKETS);
				nullBuckets();
			}
			
			
			
			
			/// Create a hash set with the default load factor and specified number of buckets.
			GSOUND_INLINE HashSet( Hash newNumBuckets )
				:	loadFactor( DEFAULT_LOAD_FACTOR ),
					numElements( 0 ),
					numBuckets( nextPrime(newNumBuckets) )
			{
				buckets = util::allocate<Entry*>(numBuckets);
				loadThreshold = Size(DEFAULT_LOAD_FACTOR*numBuckets);
				nullBuckets();
			}
			
			
			
			
			/// Create a hash set with the specified load factor and number of buckets.
			GSOUND_INLINE HashSet( Hash newNumBuckets, Float newLoadFactor )
				:	loadFactor( math::clamp( newLoadFactor, 0.1f, 2.0f ) ),
					numElements( 0 ),
					numBuckets( nextPrime(newNumBuckets) )
			{
				buckets = util::allocate<Entry*>(numBuckets);
				loadThreshold = Size(loadFactor*numBuckets);
				nullBuckets();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Copy Constructor
			
			
			
			
			/// Create a hash set with the specified load factor and number of buckets.
			GSOUND_INLINE HashSet( const HashSet& other )
				:	loadFactor( other.loadFactor ),
					numElements( other.numElements ),
					numBuckets( other.numBuckets ),
					buckets( util::allocate<Entry*>(other.numBuckets) )
			{
				// Copy the hash table buckets
				const Entry* const * otherBucket = other.buckets;
				const Entry* const * const otherBucketsEnd = otherBucket + numBuckets;
				Entry** bucket = buckets;
				
				while ( otherBucket != otherBucketsEnd )
				{
					if ( *otherBucket )
						*bucket = HashSet::newEntry(**otherBucket);
					else
						*bucket = NULL;
					
					otherBucket++;
					bucket++;
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Copy the contents of one hash set into another.
			GSOUND_INLINE HashSet& operator = ( const HashSet& other )
			{
				if ( this != &other )
				{
					deleteBuckets( buckets, numBuckets );
					
					// Copy the parameters from the other hash set.
					numBuckets = other.numBuckets;
					loadFactor = other.loadFactor;
					numElements = other.numElements;
					buckets = util::allocate<Entry*>(numBuckets);
					
					{
						// Copy the hash table buckets
						const Entry* const * otherBucket = other.buckets;
						const Entry* const * const otherBucketsEnd = otherBucket + numBuckets;
						Entry** bucket = buckets;
						
						while ( otherBucket != otherBucketsEnd )
						{
							if ( *otherBucket )
								*bucket = HashSet::newEntry(**otherBucket);
							else
								*bucket = NULL;
							
							otherBucket++;
							bucket++;
						}
					}
				}
				
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy a hash set and it's contents, deallocating all memory used.
			GSOUND_INLINE ~HashSet()
			{
				deleteBuckets( buckets, numBuckets );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Add Method
			
			
			
			
			/// Add a new element to the hash set if it does not already exist.
			/**
			  * If the element did not previously exist in the set, return TRUE.
			  * Otherwise return FALSE.
			  */
			GSOUND_INLINE Bool add( Hash hash, const T& value )
			{
				// Compute the bucket for the new element.
				Entry** bucket = buckets + hash % numBuckets;
				
				if ( *bucket == NULL )
					*bucket = HashSet::newEntry( hash, value );
				else
				{
					Entry* entry = *bucket;
					
					if ( entry->value == value )
						return false;
					
					while ( entry->next )
					{
						entry = entry->next;
						
						if ( entry->value == value )
							return false;
					}
					
					entry->next = HashSet::newEntry( hash, value );
				}
				
				numElements++;
				return true;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Remove Methods
			
			
			
			
			/// Remove the specified value from the hash set if it exists.
			/**
			  * If the value does not exist in the hash set, then FALSE is returned,
			  * otherwise TRUE is returned.
			  */
			GSOUND_INLINE Bool remove( Hash hash, const T& value )
			{
				// Compute the bucket for the new element.
				Entry** bucket = buckets + hash % numBuckets;
				Entry* entry = *bucket;
				
				if ( !entry )
					return false;
				
				if ( entry->value == value )
				{
					*bucket = entry->next;
					entry->next = NULL;
					delete entry;
					numElements--;
					return true;
				}
				
				Entry* lastEntry = *bucket;
				entry = entry->next;
				
				while ( entry )
				{
					if ( entry->value == value )
					{
						lastEntry->next = entry->next;
						entry->next = NULL;
						delete entry;
						numElements--;
						return true;
					}
					
					lastEntry = entry;
					entry = entry->next;
				}
				
				return false;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Clear Method
			
			
			
			
			/// Clear all elements from the hash set.
			GSOUND_INLINE void clear()
			{
				// Delete all entries
				Entry** bucket = buckets;
				const Entry* const * const bucketsEnd = bucket + numBuckets;
				
				while ( bucket != bucketsEnd )
				{
					if ( *bucket )
					{
						delete (*bucket);
						*bucket = NULL;
					}
					
					bucket++;
				}
				
				numElements = 0;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Contains Methods
			
			
			
		
			/// Query whether or not the specified value is contained in a hash set.
			GSOUND_INLINE Bool contains( Hash hash, const T& value ) const
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + hash % numBuckets);
				
				// Look for the value in the bucket.
				while ( entry )
				{
					if ( entry->value == value )
						return true;
					
					entry = entry->next;
				}
				
				return false;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			
			
			/// Return the number of setpings in a hash set.
			GSOUND_INLINE Size getSize() const
			{
				return numElements;
			}
			
			
			
			
			/// Return whether or not a hash set is empty.
			GSOUND_INLINE Bool isEmpty() const
			{
				return numElements == Size(0);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Class
			
			
			
			
			/// A class which iterates over hash set elements.
			class Iterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new hash set iterator for the specified hash set.
						GSOUND_INLINE Iterator( HashSet<T>& newHashSet )
							:	hashSet( newHashSet ),
								currentBucket( newHashSet.buckets ),
								bucketsEnd( newHashSet.buckets + newHashSet.numBuckets )
						{
							// advance until the first element
							advanceToNextFullBucket();
						}
						
						
						
						
					//********************************************
					//	Destructor
						
						
						
						
						/// Destroy a hash set iterator.
						GSOUND_INLINE ~Iterator()
						{
						}
						
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Increment the location of a hash set iterator by one element.
						GSOUND_INLINE void operator ++ ()
						{
							currentEntry = currentEntry->next;
							
							if ( currentEntry == NULL )
							{
								currentBucket++;
								advanceToNextFullBucket();
							}
						}
						
						
						
						
						/// Increment the location of a hash set iterator by one element.
						GSOUND_INLINE void operator ++ ( int )
						{
							this->operator++();
						}
						
						
						
						
						/// Test whether or not the current element is valid.
						/**
						  * This will return FALSE when the last element of the hash set
						  * has been iterated over.
						  */
						GSOUND_INLINE operator Bool () const
						{
							return currentEntry != NULL;
						}
						
						
						
						/// Return the value of the value-value pair pointed to by the iterator.
						GSOUND_INLINE T& operator * () const
						{
							return currentEntry->value;
						}
						
						
						
						
						/// Access the current iterator element value
						GSOUND_INLINE T* operator -> () const
						{
							return &currentEntry->value;
						}
						
						
						
						/// Get the value hash of the value-value pair pointed to by the iterator.
						GSOUND_INLINE Hash getHash() const
						{
							return currentEntry->hash;
						}
						
						
						
						
						/// Remove the current element from the hash table and advance to the next element.
						void remove()
						{
							// Backup in the bucket so that we can remove the current element.
							// This is potentially inefficient, it would be best if the buckets
							// would use a doublely linked list, but this might add extra overhead
							// elsewhere.
							
							// Handle removing from the start of a bucket separately.
							if ( currentEntry == *currentBucket )
							{
								*currentBucket = currentEntry->next;
								
								if ( *currentBucket != NULL )
								{
									currentEntry->next = NULL;
									delete currentEntry;
									currentEntry = *currentBucket;
								}
								else
								{
									delete currentEntry;
									currentBucket++;
									
									advanceToNextFullBucket();
								}
							}
							else
							{
								// Otherwise, iterate through the bucket until we find the element
								// before this one.
								Entry* previousEntry = *currentBucket;
								
								while ( previousEntry && previousEntry->next != currentEntry )
									previousEntry = previousEntry->next;
								
								previousEntry->next = currentEntry->next;
								Entry* temp = currentEntry;
								operator++();
								temp->next = NULL;
								delete temp;
							}
							
							hashSet.numElements--;
						}
						
						
						
						/// Reset the iterator to the beginning of the hash set.
						GSOUND_INLINE void reset()
						{
							currentBucket = hashSet.buckets;
							
							// advance until the first element
							advanceToNextFullBucket();
						}
						
						
						
						
				private:
					
					//********************************************
					//	Private Methods
						
						
						
						
						/// Advance the iterator to the next non-empty bucket.
						GSOUND_INLINE void advanceToNextFullBucket()
						{
							while ( *currentBucket == NULL && currentBucket != bucketsEnd )
								currentBucket++;
							
							if ( currentBucket == bucketsEnd )
								currentEntry = NULL;
							else
								currentEntry = *currentBucket;
						}
						
						
						
						
					//********************************************
					//	Private Data Members
						
						
						
						
						/// The HashSet that is being iterated over.
						HashSet<T>& hashSet;
						
						
						
						
						/// The current bucket in the HashSet.
						Entry** currentBucket;
						
						
						
						
						/// The last bucket in the HashSet.
						const Entry* const * const bucketsEnd;
						
						
						
						
						/// The current entry in the hash set that the iterator is pointing to.
						Entry* currentEntry;
						
						
						
						
						/// Make the const iterator class a friend.
						friend class HashSet<T>::ConstIterator;
						
						
						
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	ConstIterator Class
			
			
			
			
			/// A class which iterates over hash set elements without the ability to modify them.
			class ConstIterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new hash set iterator for the specified hash set.
						GSOUND_INLINE ConstIterator( const HashSet<T>& newHashSet )
							:	hashSet( newHashSet ),
								currentBucket( newHashSet.buckets ),
								bucketsEnd( newHashSet.buckets + newHashSet.numBuckets )
						{
							// advance until the first element
							advanceToNextFullBucket();
						}
						
						
						
						/// Create a new const hash set iterator from a non-const iterator.
						GSOUND_INLINE ConstIterator( const Iterator& iterator )
							:	hashSet( iterator.hashSet ),
								currentBucket( iterator.currentBucket ),
								bucketsEnd( iterator.bucketsEnd ),
								currentEntry( iterator.currentEntry )
						{
						}
						
						
						
						
					//********************************************
					//	Destructor
						
						
						
						
						/// Destroy a hash set iterator.
						GSOUND_INLINE ~ConstIterator()
						{
						}
						
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Increment the location of a hash set iterator by one element.
						GSOUND_INLINE void operator ++ ()
						{
							currentEntry = currentEntry->next;
							
							if ( currentEntry == NULL )
							{
								currentBucket++;
								advanceToNextFullBucket();
							}
						}
						
						
						
						
						/// Increment the location of a hash set iterator by one element.
						GSOUND_INLINE void operator ++ ( int )
						{
							this->operator++();
						}
						
						
						
						
						/// Test whether or not the current element is valid.
						/**
						  * This will return FALSE when the last element of the hash set
						  * has been iterated over.
						  */
						GSOUND_INLINE operator Bool () const
						{
							return currentEntry != NULL;
						}
						
						
						
						/// Return the value of the value-value pair pointed to by the iterator.
						GSOUND_INLINE const T& operator * () const
						{
							return currentEntry->value;
						}
						
						
						
						
						/// Access the current iterator element
						GSOUND_INLINE const T* operator -> () const
						{
							return &currentEntry->value;
						}
						
						
						
						/// Get the hash of the element pointed to by the iterator.
						GSOUND_INLINE Hash getHash() const
						{
							return currentEntry->valueHash;
						}
						
						
						
						/// Reset the iterator to the beginning of the hash set.
						GSOUND_INLINE void reset()
						{
							currentBucket = hashSet.buckets;
							
							// advance until the first element.
							advanceToNextFullBucket();
						}
						
						
						
						
				private:
					
					//********************************************
					//	Private Methods
						
						
						
						
						/// Advance the iterator to the next non-empty bucket.
						GSOUND_INLINE void advanceToNextFullBucket()
						{
							while ( *currentBucket == NULL && currentBucket != bucketsEnd )
								currentBucket++;
							
							if ( currentBucket == bucketsEnd )
								currentEntry = NULL;
							else
								currentEntry = *currentBucket;
						}
						
						
						
						
					//********************************************
					//	Private Data Members
						
						
						
						
						/// The HashSet that is being iterated over.
						const HashSet<T>& hashSet;
						
						
						
						
						/// The current bucket in the HashSet.
						const Entry* const * currentBucket;
						
						
						
						
						/// The last bucket in the HashSet.
						const Entry* const * bucketsEnd;
						
						
						
						
						/// The current entry in the hash set that the iterator is pointing to.
						const Entry* currentEntry;
						
						
						
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Method
			
			
			
			
			/// Get a const-iterator for the hash set.
			GSOUND_INLINE ConstIterator getIterator() const
			{
				return ConstIterator(*this);
			}
			
			
			
			
			/// Get an iterator for the hash set that can modify the hash set.
			GSOUND_INLINE Iterator getIterator()
			{
				return Iterator(*this);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Load Factor Accessor Methods
			
			
			
			
			GSOUND_INLINE void setLoadFactor( Float newLoadFactor )
			{
				loadFactor = math::clamp( newLoadFactor, 0.1f, 5.0f );
				loadThreshold = Size(loadFactor*numBuckets);
				
				// Check the load constraint, if necessary, increase the size of the table.
				if ( numElements > loadThreshold )
					resize( nextPrime( numBuckets + 1 ) );
			}
			
			
			
			
			GSOUND_INLINE Float getLoadFactor() const
			{
				return loadFactor;
			}
			
			
			
			
	private:
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Methods
			
			
			
			
			void resize( Hash newNumBuckets )
			{
				Entry** oldBuckets = buckets;
				Hash oldNumBuckets = numBuckets;
				
				// initialize all buckets and resize the array
				numBuckets = newNumBuckets;
				loadThreshold = Size(loadFactor*numBuckets);
				buckets = util::allocate<Entry*>(numBuckets);
				nullBuckets();
				
				// add old elements to the hash set.
				Entry** oldBucket = oldBuckets;
				const Entry* const * const oldBucketsEnd = oldBucket + oldNumBuckets;
				
				while ( oldBucket != oldBucketsEnd )
				{
					Entry* oldEntry = *oldBucket;
					
					while ( oldEntry )
					{
						Entry** bucket = buckets + oldEntry->hash % numBuckets;
						
						// Add the new element.
						if ( *bucket == NULL )
							*bucket = HashSet::newEntry( oldEntry->hash, oldEntry->value );
						else
						{
							Entry* entry = *bucket;
							
							while ( entry->next )
								entry = entry->next;
							
							entry->next = HashSet::newEntry( oldEntry->hash, oldEntry->value );
						}
						
						oldEntry = oldEntry->next;
					}
					
					oldBucket++;
				}
								
				// deallocate all memory currently used by the old buckets
				deleteBuckets( oldBuckets, oldNumBuckets );
			}
			
			
			
			
			GSOUND_INLINE void nullBuckets()
			{
				Entry** bucket = buckets;
				const Entry* const * const bucketsEnd = bucket + numBuckets;
				
				while ( bucket != bucketsEnd )
				{
					*bucket = NULL;
					bucket++;
				}
			}
			
			
			
			
			GSOUND_INLINE static void deleteBuckets( Entry** buckets, Hash numBuckets )
			{
				// Delete all entries
				Entry** entry = buckets;
				const Entry* const * const entryEnd = entry + numBuckets;
				
				while ( entry != entryEnd )
				{
					if ( *entry )
						delete (*entry);
					
					entry++;
				}
				
				// Delete the bucket array.
				util::deallocate( buckets );
			}
			
			
			
			
			GSOUND_INLINE static Hash nextPrime( Hash n )
			{
				for ( Index i = 0; i < NUMBER_OF_PRIMES; i++ )
				{
					if ( PRIMES[i] > n )
						return PRIMES[i];
				}
				
				return n;
			}
			
			
			
			
			GSOUND_INLINE static Entry* newEntry( const Entry& other )
			{
				Entry* result = util::allocate<Entry>();
				
				new (result) Entry( other );
				
				return result;
			}
			
			
			
			
			GSOUND_INLINE static Entry* newEntry( Hash hash, const T& value )
			{
				Entry* result = util::allocate<Entry>();
				
				new (result) Entry( hash, value );
				
				return result;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			Entry** buckets;
			Hash numBuckets;
			
			
			Size numElements;
			Float loadFactor;
			Size loadThreshold;
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Data Members
			
			
			
			
			static const Hash DEFAULT_NUMBER_OF_BUCKETS = 19;
			
			static const Size NUMBER_OF_PRIMES = 28;
			static const Hash PRIMES[NUMBER_OF_PRIMES];
			
			static const Float DEFAULT_LOAD_FACTOR;
			
			
			
};




template < typename T >
const Float		HashSet<T>:: DEFAULT_LOAD_FACTOR = 0.5f;




template < typename T >
const Hash	HashSet<T>:: PRIMES[] =
{
	11,				//	Between 2^3 and 2^4
	23,				//	Between 2^4 and 2^5
	53,				//	Between 2^5 and 2^6
	97,				//	Between 2^6 and 2^7
	193,			//	Between 2^7 and 2^8
	389,			//	Between 2^8 and 2^9
	769,			//	Between 2^9 and 2^10
	1543,			//	Between 2^10 and 2^11
	3079,			//	Between 2^11 and 2^12
	6151,			//	Between 2^12 and 2^13
	12289,			//	Between 2^13 and 2^14
	24593,			//	Between 2^14 and 2^15
	49157,			//	Between 2^15 and 2^16
	98317,			//	Between 2^16 and 2^17
	196613,			//	Between 2^17 and 2^18
	393241,			//	Between 2^18 and 2^19
	786433,			//	Between 2^19 and 2^20
	1572869,		//	Between 2^20 and 2^21
	3145739,		//	Between 2^21 and 2^22
	6291469,		//	Between 2^22 and 2^23
	12582917,		//	Between 2^23 and 2^24
	25165843,		//	Between 2^24 and 2^25
	50331653,		//	Between 2^25 and 2^26
	100663319,		//	Between 2^26 and 2^27
	201326611,		//	Between 2^27 and 2^28
	402653189,		//	Between 2^28 and 2^29
	805306457,		//	Between 2^29 and 2^30
	1610612741,		//	Between 2^30 and 2^31
};




//##########################################################################################
//*************************  End GSound Utilities Namespace  *******************************
GSOUND_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################

#endif // INCLUDE_GSOUND_HASH_SET_H
