/*
 * Project:     GSound
 * 
 * File:        gsound/HashMap.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::util::HashMap class declaration
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


#ifndef INCLUDE_GSOUND_HASH_MAP_H
#define INCLUDE_GSOUND_HASH_MAP_H


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
/// A container class which uses a hash table to map key objects to value objects.
template < typename K, typename V >
class HashMap
{
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Bucket Class
			
			
			
			
			class Entry
			{
				public:
					
					
					GSOUND_INLINE Entry( Hash newKeyHash, const K& newKey, const V& newValue )
						:	next( NULL ),
							keyHash( newKeyHash ),
							key( newKey ),
							value( newValue )
					{
					}
					
					Entry( const Entry& other )
						:	keyHash( other.keyHash ),
							key( other.key ),
							value( other.value )
					{
						if ( other.next )
							next = util::construct<Entry>(*other.next);
						else
							next = NULL;
					}
					
					
					~Entry()
					{
						if ( next )
							util::destruct(next);
					}
					
					
					Entry* next;
					
					Hash keyHash;
					K key;
					V value;
					
			};
			
			
			
			
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a hash map with the default load factor and number of buckets.
			GSOUND_INLINE HashMap()
				:	loadFactor( DEFAULT_LOAD_FACTOR ),
					loadThreshold( Size(DEFAULT_LOAD_FACTOR*DEFAULT_NUMBER_OF_BUCKETS) ),
					numElements( 0 ),
					numBuckets( DEFAULT_NUMBER_OF_BUCKETS ),
					buckets( util::allocate<Entry*>(DEFAULT_NUMBER_OF_BUCKETS) )
			{
				nullBuckets();
			}
			
			
			
			
			/// Create a hash map with the specified load factor and default number of buckets.
			GSOUND_INLINE HashMap( Float newLoadFactor )
				:	loadFactor( math::clamp( newLoadFactor, 0.1f, 2.0f ) ),
					numElements( 0 ),
					numBuckets( DEFAULT_NUMBER_OF_BUCKETS ),
					buckets( util::allocate<Entry*>(DEFAULT_NUMBER_OF_BUCKETS) )
			{
				loadThreshold = loadFactor*DEFAULT_NUMBER_OF_BUCKETS;
				nullBuckets();
			}
			
			
			
			
			/// Create a hash map with the default load factor and specified number of buckets.
			GSOUND_INLINE HashMap( Hash newNumBuckets )
				:	loadFactor( DEFAULT_LOAD_FACTOR ),
					numElements( 0 ),
					numBuckets( nextPrime(newNumBuckets) )
			{
				buckets = util::allocate<Entry*>(numBuckets);
				loadThreshold = DEFAULT_LOAD_FACTOR*numBuckets;
				nullBuckets();
			}
			
			
			
			
			/// Create a hash map with the specified load factor and number of buckets.
			GSOUND_INLINE HashMap( Hash newNumBuckets, Float newLoadFactor )
				:	loadFactor( math::clamp( newLoadFactor, 0.1f, 2.0f ) ),
					numElements( 0 ),
					numBuckets( nextPrime(newNumBuckets) )
			{
				buckets = util::allocate<Entry*>(numBuckets);
				loadThreshold = loadFactor*numBuckets;
				nullBuckets();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Copy Constructor
			
			
			
			
			/// Create a hash map with the specified load factor and number of buckets.
			GSOUND_INLINE HashMap( const HashMap& other )
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
						*bucket = util::construct<Entry>(**otherBucket);
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
			
			
			
			
			/// Copy the contents of one hash map into another.
			GSOUND_INLINE HashMap& operator = ( const HashMap& other )
			{
				if ( this != &other )
				{
					deleteBuckets( buckets, numBuckets );
					
					// Copy the parameters from the other hash map.
					numBuckets = other.numBuckets;
					loadFactor = other.loadFactor;
					numElements = other.numElements;
					buckets = util::allocate<Entry*>( numBuckets );
					
					{
						// Copy the hash table buckets
						const Entry* const * otherBucket = other.buckets;
						const Entry* const * const otherBucketsEnd = otherBucket + numBuckets;
						Entry** bucket = buckets;
						
						while ( otherBucket != otherBucketsEnd )
						{
							if ( *otherBucket )
								*bucket = util::construct<Entry>(**otherBucket);
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
			
			
			
			
			/// Destroy a hash map and it's contents, deallocating all memory used.
			GSOUND_INLINE ~HashMap()
			{
				deleteBuckets( buckets, numBuckets );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Add Method
			
			
			
			
			/// Add a new mapping to the hash map, associating the given key with the given value.
			GSOUND_INLINE void add( Hash keyHash, const K& key, const V& value )
			{
				// Check the load constraint, if necessary, increase the size of the table.
				if ( numElements > loadThreshold )
					resize( nextPrime( numBuckets + 1 ) );
				
				// Compute the bucket for the new element.
				Entry** bucket = buckets + keyHash % numBuckets;
				numElements++;
				
				// Add the new element.
				if ( *bucket == NULL )
					*bucket = util::construct<Entry>( keyHash, key, value );
				else
				{
					Entry* entry = *bucket;
					
					while ( entry->next )
						entry = entry->next;
					
					entry->next = util::construct<Entry>( keyHash, key, value );
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Set Method
			
			
			
			
			/// Set the mapping of a key to be the given value, regardless of it's previous state.
			/**
			  * The method returns TRUE if the key did not previously exist in the HashMap.
			  * Otherwise the method returns FALSE.
			  */
			GSOUND_INLINE Bool set( Hash keyHash, const K& key, const V& value )
			{
				// Compute the bucket for the new element.
				Entry** bucket = buckets + keyHash % numBuckets;
				
				if ( *bucket == NULL )
					*bucket = util::construct<Entry>( keyHash, key, value );
				else
				{
					Entry* entry = *bucket;
					
					if ( entry->key == key )
					{
						entry->value = value;
						return false;
					}
					
					while ( entry->next )
					{
						entry = entry->next;
						
						if ( entry->key == key )
						{
							entry->value = value;
							return false;
						}
					}
					
					entry->next = util::construct<Entry>( keyHash, key, value );
				}
				
				numElements++;
				
				return true;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Remove Methods
			
			
			
			
			/// Remove the first mapping of the given key and return the mapped value.
			/**
			  * If the key does not exist in the hash map, then FALSE is returned,
			  * otherwise TRUE is returned.
			  */
			GSOUND_INLINE Bool remove( Hash keyHash, const K& key )
			{
				// Compute the bucket for the new element.
				Entry** bucket = buckets + keyHash % numBuckets;
				Entry** previousNext = bucket;
				Entry* entry = *bucket;
				
				while ( entry )
				{
					if ( entry->key == key )
					{
						*previousNext = entry->next;
						entry->next = NULL;
						
						util::destruct(entry);
						
						numElements--;
						
						return true;
					}
					
					previousNext = &(*previousNext)->next;
					entry = entry->next;
				}
				
				return false;
			}
			
			
			
			
			/// Remove a mapping from the hash map if it was found, returning the success.
			GSOUND_INLINE Bool remove( Hash keyHash, const K& key, const V& value )
			{
				// Compute the bucket for the new element.
				Entry** bucket = buckets + keyHash % numBuckets;
				Entry** previousNext = bucket;
				Entry* entry = *bucket;
				
				while ( entry )
				{
					if ( entry->key == key && entry->value == value )
					{
						*previousNext = entry->next;
						entry->next = NULL;
						
						util::destruct(entry);
						
						numElements--;
						
						return true;
					}
					
					previousNext = &(*previousNext)->next;
					entry = entry->next;
				}
				
				return false;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Clear Method
			
			
			
			
			/// Clear all mappings from the hash map.
			GSOUND_INLINE void clear()
			{
				// Delete all entries
				Entry** entry = buckets;
				const Entry* const * const entryEnd = entry + numBuckets;
				
				while ( entry != entryEnd )
				{
					if ( *entry )
					{
						util::destruct(*entry);
						*entry = NULL;
					}
					
					entry++;
				}
				
				numElements = 0;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Contains Methods
			
			
			
			
			/// Query whether or not the specified key is contained in a hash map and retrieve its value.
			GSOUND_INLINE Bool find( Hash keyHash, const K& key, V*& value ) 
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->key == key )
					{
						value = &entry->value;
						return true;
					}
					
					entry = entry->next;
				}
				
				return false;
			}
			
			
			
			
			/// Query whether or not the specified key is contained in a hash map and retrieve its value.
			GSOUND_INLINE Bool find( Hash keyHash, const K& key, const V*& value ) const
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->key == key )
					{
						value = &entry->value;
						return true;
					}
					
					entry = entry->next;
				}
				
				return false;
			}
			
			
			
		
			/// Query whether or not the specified key is contained in a hash map.
			GSOUND_INLINE Bool contains( Hash keyHash, const K& key ) const
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->key == key )
						return true;
					
					entry = entry->next;
				}
				
				return false;
			}
			
			
			
			
			/// Query whether or not a particular key-value mapping exists in the hash map.
			GSOUND_INLINE Bool contains( Hash keyHash, const K& key, const V& value ) const
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->key == key && entry->value == value )
						return true;
					
					entry = entry->next;
				}
				
				return false;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			
			
			/// Return the number of mappings in this hash map.
			GSOUND_FORCE_INLINE Size getSize() const
			{
				return numElements;
			}
			
			
			
			
			/// Return whether or not a hash map has any mappings stored.
			GSOUND_FORCE_INLINE Bool isEmpty() const
			{
				return numElements == Size(0);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Get Methods
			
			
			
			
			/// Return a pointer to the value associated with the given key.
			/**
			  * If the key does not exist in the hash map, a NULL pointer is
			  * returned.
			  */
			GSOUND_INLINE V* get( Hash keyHash, const K& key )
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->key == key )
						return &entry->value;
					
					entry = entry->next;
				}
				
				return NULL;
			}
			
			
			
			
			/// Return a const pointer to the value associated with the given key.
			/**
			  * If the key does not exist in the hash map, a NULL pointer is
			  * returned.
			  */
			GSOUND_INLINE const V* get( Hash keyHash, const K& key ) const
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->key == key )
						return &entry->value;
					
					entry = entry->next;
				}
				
				return NULL;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Class
			
			
			
			
			/// A class which iterates over hash map elements.
			class Iterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new hash map iterator for the specified hash map.
						GSOUND_INLINE Iterator( HashMap<K,V>& newHashMap )
							:	hashMap( newHashMap ),
								currentBucket( newHashMap.buckets ),
								bucketsEnd( newHashMap.buckets + newHashMap.numBuckets )
						{
							// advance until the first element
							advanceToNextFullBucket();
						}
						
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Increment the location of a hash map iterator by one element.
						GSOUND_INLINE void operator ++ ()
						{
							currentEntry = currentEntry->next;
							
							if ( currentEntry == NULL )
							{
								currentBucket++;
								advanceToNextFullBucket();
							}
						}
						
						
						
						
						/// Increment the location of a hash map iterator by one element.
						GSOUND_INLINE void operator ++ ( int )
						{
							this->operator++();
						}
						
						
						
						
						/// Test whether or not the current element is valid.
						/**
						  * This will return FALSE when the last element of the hash map
						  * has been iterated over.
						  */
						GSOUND_INLINE operator Bool () const
						{
							return currentEntry != NULL;
						}
						
						
						
						/// Return the value of the key-value pair pointed to by the iterator.
						GSOUND_INLINE V& operator * () const
						{
							return currentEntry->value;
						}
						
						
						
						
						/// Access the current iterator element value
						GSOUND_INLINE V* operator -> () const
						{
							return &currentEntry->value;
						}
						
						
						
						
						/// Get the value of the key-value pair pointed to by the iterator.
						GSOUND_INLINE V& getValue() const
						{
							return currentEntry->value;
						}
						
						
						
						/// Get the key of the key-value pair pointed to by the iterator.
						GSOUND_INLINE K& getKey() const
						{
							return currentEntry->key;
						}
						
						
						
						/// Get the key hash of the key-value pair pointed to by the iterator.
						GSOUND_INLINE Hash getKeyHash() const
						{
							return currentEntry->keyHash;
						}
						
						
						
						
						/// Remove the current element from the hash table.
						GSOUND_INLINE void remove()
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
									util::destruct( currentEntry );
									currentEntry = *currentBucket;
								}
								else
								{
									util::destruct( currentEntry );
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
								util::destruct( temp );
							}
							
							hashMap.numElements--;
						}
						
						
						
						/// Reset the iterator to the beginning of the hash map.
						GSOUND_INLINE void reset()
						{
							currentBucket = hashMap.buckets;
							
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
						
						
						
						
						/// The HashMap that is being iterated over.
						HashMap<K,V>& hashMap;
						
						
						
						
						/// The current bucket in the HashMap.
						Entry** currentBucket;
						
						
						
						
						/// The last bucket in the HashMap.
						const Entry* const * const bucketsEnd;
						
						
						
						
						/// The current entry in the hash map that the iterator is pointing to.
						Entry* currentEntry;
						
						
						
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	ConstIterator Class
			
			
			
			
			/// A class which iterates over hash map elements without the ability to modify them.
			class ConstIterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new hash map iterator for the specified hash map.
						GSOUND_INLINE ConstIterator( const HashMap<K,V>& newHashMap )
							:	hashMap( newHashMap ),
								currentBucket( newHashMap.buckets ),
								bucketsEnd( newHashMap.buckets + newHashMap.numBuckets )
						{
							// advance until the first element
							advanceToNextFullBucket();
						}
						
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Increment the location of a hash map iterator by one element.
						GSOUND_INLINE void operator ++ ()
						{
							currentEntry = currentEntry->next;
							
							if ( currentEntry == NULL )
							{
								currentBucket++;
								
								advanceToNextFullBucket();
							}
						}
						
						
						
						
						/// Increment the location of a hash map iterator by one element.
						GSOUND_INLINE void operator ++ ( int )
						{
							this->operator++();
						}
						
						
						
						
						/// Test whether or not the current element is valid.
						/**
						  * This will return FALSE when the last element of the hash map
						  * has been iterated over.
						  */
						GSOUND_INLINE operator Bool () const
						{
							return currentEntry != NULL;
						}
						
						
						
						/// Return the value of the key-value pair pointed to by the iterator.
						GSOUND_INLINE const V& operator * () const
						{
							return currentEntry->value;
						}
						
						
						
						
						/// Access the current iterator element value
						GSOUND_INLINE const V* operator -> () const
						{
							return &currentEntry->value;
						}
						
						
						
						/// Get the value of the key-value pair pointed to by the iterator.
						GSOUND_INLINE const V& getValue() const
						{
							return currentEntry->value;
						}
						
						
						
						/// Get the key of the key-value pair pointed to by the iterator.
						GSOUND_INLINE const K& getKey() const
						{
							return currentEntry->key;
						}
						
						
						
						/// Get the key hash of the key-value pair pointed to by the iterator.
						GSOUND_INLINE Hash getKeyHash() const
						{
							return currentEntry->keyHash;
						}
						
						
						
						/// Reset the iterator to the beginning of the hash map.
						GSOUND_INLINE void reset()
						{
							currentBucket = hashMap.buckets;
							
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
						
						
						
						
						/// The HashMap that is being iterated over.
						const HashMap<K,V>& hashMap;
						
						
						
						
						/// The current bucket in the HashMap.
						const Entry* const * currentBucket;
						
						
						
						
						/// The last bucket in the HashMap.
						const Entry* const * bucketsEnd;
						
						
						
						
						/// The current entry in the hash map that the iterator is pointing to.
						const Entry* currentEntry;
						
						
						
			};
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Method
			
			
			
			
			/// Get a const-iterator for the hash map.
			GSOUND_INLINE ConstIterator getIterator() const
			{
				return ConstIterator(*this);
			}
			
			
			
			
			/// Get an iterator for the hash map that can modify the hash map.
			GSOUND_INLINE Iterator getIterator()
			{
				return Iterator(*this);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Load Factor Accessor Methods
			
			
			
			
			/// Set the load factor which this hash table should use.
			/**
			  * When the number of elements exceeds this value times the
			  * number of buckets in the hash map, the table is resized.
			  * The new load factor is clamped to the range [0.1 - 5.0]
			  * to avoid unreasonable use and the table is resized if necessary.
			  */
			GSOUND_INLINE void setLoadFactor( Float newLoadFactor )
			{
				loadFactor = math::clamp( newLoadFactor, 0.1f, 5.0f );
				loadThreshold = Size(loadFactor*numBuckets);
				
				// Check the load constraint, if necessary, increase the size of the table.
				if ( numElements > loadThreshold )
					resize( nextPrime( numBuckets + 1 ) );
			}
			
			
			
			
			/// Get the load factor which this hash table is using.
			/**
			  * When the number of elements exceeds this value times the
			  * number of buckets in the hash map, the table is resized.
			  */
			GSOUND_INLINE Float getLoadFactor() const
			{
				return loadFactor;
			}
			
			
			
			
	private:
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Methods
			
			
			
			
			GSOUND_NO_INLINE void resize( Hash newNumBuckets )
			{
				Entry** oldBuckets = buckets;
				Hash oldNumBuckets = numBuckets;
				
				// initialize all buckets and resize the array
				numBuckets = newNumBuckets;
				loadThreshold = Size(loadFactor*numBuckets);
				buckets = util::allocate<Entry*>(numBuckets);
				nullBuckets();
				
				// add old elements to the hash map.
				Entry** oldBucket = oldBuckets;
				const Entry* const * const oldBucketsEnd = oldBucket + oldNumBuckets;
				
				while ( oldBucket != oldBucketsEnd )
				{
					Entry* oldEntry = *oldBucket;
					
					while ( oldEntry )
					{
						Entry** bucket = buckets + oldEntry->keyHash % numBuckets;
						
						// Add the old element to the end of the bucket.
						if ( *bucket == NULL )
						{
							*bucket = oldEntry;
							oldEntry = oldEntry->next;
							(*bucket)->next = NULL;
						}
						else
						{
							Entry* entry = *bucket;
							
							while ( entry->next )
								entry = entry->next;
							
							entry->next = oldEntry;
							oldEntry = oldEntry->next;
							entry->next->next = NULL;
						}
					}
					
					oldBucket++;
				}
								
				// deallocate all memory currently used by the old buckets
				util::deallocate(oldBuckets);
			}
			
			
			
			
			GSOUND_INLINE void nullBuckets()
			{
				Entry** bucket = buckets;
				const Entry* const * const bucketsEnd = buckets + numBuckets;
				
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
						util::destruct(*entry);
					
					entry++;
				}
				
				// Delete the bucket array.
				util::deallocate(buckets);
			}
			
			
			
			
			/// Return a prime number greater than or equal to the specified number.
			GSOUND_INLINE static Hash nextPrime( Hash n )
			{
				for ( Index i = 0; i < NUMBER_OF_PRIMES; i++ )
				{
					if ( PRIMES[i] >= n )
						return PRIMES[i];
				}
				
				return n;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// An array of pointers to buckets in this hash map.
			Entry** buckets;
			
			
			
			
			/// The number of buckets in this hash map.
			Hash numBuckets;
			
			
			
			
			/// The number of elements that this hash map contains.
			Size numElements;
			
			
			
			
			/// The load factor of the hash map.
			/**
			  * When the number of elements exceeds this value times the
			  * number of buckets in the hash map, the table is resized.
			  */
			Float loadFactor;
			
			
			
			
			/// The pre-computed load threshold of the hash map.
			/**
			  * When the number of elements exceeds this value, the table is resized.
			  */
			Size loadThreshold;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Data Members
			
			
			
			
			/// The default number of buckets to use in a hash map.
			static const Hash DEFAULT_NUMBER_OF_BUCKETS = 19;
			
			
			
			
			/// The number of prime numbers in this hash map's table of primes.
			static const Size NUMBER_OF_PRIMES = 28;
			
			
			
			
			/// A table of prime numbers where each is around double the last.
			/**
			  * These prime numbers are used to keep the number of buckets in the
			  * hash map prime, resulting in less clustering.
			  */
			static const Hash PRIMES[NUMBER_OF_PRIMES];
			
			
			
			
			/// The default load factor to use for a hash map.
			static const Float DEFAULT_LOAD_FACTOR;
			
			
			
};




template < typename K, typename V >
const Float		HashMap<K,V>:: DEFAULT_LOAD_FACTOR = 0.5f;




template < typename K, typename V >
const Hash	HashMap<K,V>:: PRIMES[] =
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

#endif // INCLUDE_GSOUND_HASH_MAP_H
