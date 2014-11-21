/*
 * Project:     GSound
 * 
 * File:        gsound/internal/ProbePathCache.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::ProbePathCache class declaration
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


#ifndef INCLUDE_GSOUND_PROBE_PATH_CACHE_H
#define INCLUDE_GSOUND_PROBE_PATH_CACHE_H


#include "GSoundInternalConfig.h"


#include "ProbePath.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




class ProbePathCache
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Probe Path Cache Iterator Declaration
			
			
			
			
			class Iterator;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create an empy propagation path cache with the default number of hash table buckets.
			ProbePathCache();
			
			
			
			
			/// Create an empy propagation path cache with the specified number of hash table buckets and loadFactor.
			ProbePathCache( Hash newNumBuckets, Float newLoadFactor );
			
			
			
			
			/// Create a copy of another probe path cache.
			ProbePathCache( const ProbePathCache& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy a probe path cache object and deallocate all resources that it has allocated.
			~ProbePathCache();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Copy the contents of one probe path cache to ths cache, replacing this cache's state.
			ProbePathCache& operator = ( const ProbePathCache& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Cache Manipulation Methods
			
			
			
			
			/// If a probe path doesn't currently exists in the cache, add it to the cache.
			/**
			  * If the probe path was already in the cache, return FALSE and leave the
			  * cache unmodified. Otherwise, return TRUE and add the probe path to the
			  * cache.
			  * 
			  * @param newProbePath - the probe path to try to add to this probe path cache.
			  * @return whether or not the path was added to the cache.
			  */
			GSOUND_INLINE Bool addPath( const ProbePath& newProbePath );
			
			
			
			
			/// Get whether or not this probe path cache currently contains the specified probe path.
			/**
			  * If the cache currently contains this probe path, the method returns TRUE.
			  * Otherwise, FALSE is returned.
			  * 
			  * @param probePath - the probe path to be tested to see if it exists in the cache.
			  * @return whether or not the cache currently contains the specified probe path.
			  */
			GSOUND_INLINE Bool containsPath( const ProbePath& probePath ) const;
			
			
			
			
			/// Remove all probe paths from this cache.
			void clear();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Load Factor Constraint Methods
			
			
			
			
			/// Get the load factor used by this probe path cache to avoid bucket clustering.
			GSOUND_INLINE Float getLoadFactor() const
			{
				return loadFactor;
			}
			
			
			
			
			/// Set the load factor used by this probe path cache to avoid bucket clustering.
			/**
			  * The input value is clamped to the range [0.1,+infinity].
			  */
			GSOUND_INLINE void setLoadFactor( Float newLoadFactor )
			{
				loadFactor = math::max( newLoadFactor, Float(0.1) );
			}
			
			
			
			
			/// Check to see if the hash table should be resized, and if necessary, resize it.
			/**
			  * If the number of paths in the cache is greater than the load factor multiplied
			  * by the number of buckets in the cache's hash table, the cache is resized
			  * to a prime number of buckets at least twice the current size and great enough
			  * to satisfy the load factor constraint. This is a potentially expensive operation
			  * and should only performed once per sound propagation frame to avoid massive
			  * reallocation.
			  */
			void checkLoadFactor();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Probe Path Cache Size Accessor Method
			
			
			
			
			/// Get the number of probe paths in this probe path cache.
			GSOUND_INLINE Size getNumberOfPaths() const
			{
				return numPaths;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Probe Path Cache Iterator Accessor Methods
			
			
			
			
			Iterator getIterator();
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Bucket Class Declaration
			
			
			
			
			class Bucket;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Bucket Class Declaration
			
			
			
			
			GSOUND_INLINE static Hash nextPrime( Hash n );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			Bucket* buckets;
			
			
			
			
			Hash numBuckets;
			
			
			
			
			Size numPaths;
			
			
			
			
			Float loadFactor;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Data Members
			
			
			
			
			static const Hash Primes[28];
			
			
			
			
			static const Size DEFAULT_NUMBER_OF_BUCKETS = 97;
			
			
			
			
			static const Size DEFAULT_BUCKET_SIZE;
			
			
			
			
			static const Float DEFAULT_LOAD_FACTOR;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Private Bucket Class Definition
//############		
//##########################################################################################
//##########################################################################################




class ProbePathCache:: Bucket
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			GSOUND_INLINE Bucket( Size newCapacity )
				:	entries( util::allocate<ProbePath>( newCapacity ) ),
					capacity( newCapacity ),
					numEntries( 0 )
			{
			}
			
			
			
			
			GSOUND_INLINE Bucket( const Bucket& other )
				:	entries( util::allocate<ProbePath>( other.capacity ) ),
					capacity( other.capacity ),
					numEntries( other.numEntries )
			{
				copyObjects( entries, other.entries, numEntries );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			GSOUND_INLINE ~Bucket()
			{
				callDestructors( entries, numEntries );
				util::deallocate( entries );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			GSOUND_INLINE Bucket& operator = ( const Bucket& other )
			{
				if ( this != &other )
				{
					callDestructors( entries, numEntries );
					
					if ( capacity < other.numEntries )
					{
						util::deallocate( entries );
						entries = util::allocate<ProbePath>( other.capacity );
						capacity = other.capacity;
					}
					
					copyObjects( entries, other.entries, other.numEntries );
					
					numEntries = other.numEntries;
				}
				
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Probe Path Add Methods
			
			
			
			
			GSOUND_INLINE Bool conditionalAdd( const ProbePath& newEntry )
			{
				ProbePath* entry = entries;
				const ProbePath* const entriesEnd = entries + numEntries;
				
				while ( entry != entriesEnd )
				{
					if ( *entry == newEntry )
						return false;
					
					entry++;
				}
				
				if ( numEntries == capacity )
					doubleCapacity();
				
				new (entries + numEntries) ProbePath(newEntry);
				numEntries++;
				
				return true;
			}
			
			
			
			
			GSOUND_INLINE void add( const ProbePath& newEntry )
			{
				if ( numEntries == capacity )
					doubleCapacity();
				
				new (entries + numEntries) ProbePath(newEntry);
				numEntries++;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Probe Path Contains Method
			
			
			
			
			GSOUND_INLINE Bool contains( const ProbePath& probePath )
			{
				ProbePath* entry = entries;
				const ProbePath* const entriesEnd = entries + numEntries;
				
				while ( entry != entriesEnd )
				{
					if ( *entry == probePath )
						return true;
					
					entry++;
				}
				
				return false;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Probe Path Remove Methods
			
			
			
			
			GSOUND_INLINE void removeAtIndex( Index pathIndex )
			{
				// Make sure that the path index is within the valid bounds.
				GSOUND_DEBUG_ASSERT( pathIndex < numEntries );
				
				ProbePath* removedPath = entries + pathIndex;
				
				// Call the removed object's destructor.
				removedPath->~ProbePath();
				
				// Decrement the number of entries in this bucket.
				numEntries--;
				
				// Move the last elemnt in the bucket to the location of the removed bucket if necessary.
				if ( pathIndex != numEntries )
				{
					ProbePath* lastPath = entries + numEntries;
					
					new (removedPath) ProbePath( *lastPath );
					lastPath->~ProbePath();
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Bucket Clear Method
			
			
			
			
			GSOUND_INLINE void clear()
			{
				callDestructors( entries, numEntries );
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Helper Methods
			
			
			
			
			void doubleCapacity()
			{
				// Update the capacity and allocate a new array.
				capacity *= 2;
				ProbePath* oldEntries = entries;
				entries = util::allocate<ProbePath>( capacity );
				
				// Move the objects from the old array to the new one.
				moveObjects( entries, oldEntries, numEntries );
				
				// Deallocate the old array.
				util::deallocate( oldEntries );
			}
			
			
			
			
			GSOUND_INLINE static void callDestructors( ProbePath* array, Size number )
			{
				const ProbePath* const arrayEnd = array + number;
				
				while ( array != arrayEnd )
				{
					array->~ProbePath();
					array++;
				}
			}
			
			
			
			
			GSOUND_INLINE static void copyObjects( ProbePath* destination, const ProbePath* source, Size number )
			{
				const ProbePath* const sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					new (destination) ProbePath(*source);
					destination++;
					source++;
				}
			}
			
			
			
			
			GSOUND_INLINE static void moveObjects( ProbePath* destination, const ProbePath* source, Size number )
			{
				const ProbePath* const sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					// copy the object from the source to destination
					new (destination) ProbePath(*source);
					
					// call the destructors on the source
					source->~ProbePath();
					
					destination++;
					source++;
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			ProbePath* entries;
			
			
			
			
			Size numEntries;
			
			
			
			
			Size capacity;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Friend Class
			
			
			
			
			friend class ProbePathCache;
			friend class ProbePathCache::Iterator;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Probe Path Cache Iterator Class Defintion
//############		
//##########################################################################################
//##########################################################################################




class ProbePathCache:: Iterator
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			GSOUND_INLINE Iterator( ProbePathCache& newCache )
				:	currentBucket( newCache.buckets ),
					bucketsEnd( newCache.buckets + newCache.numBuckets ),
					cache( &newCache )
			{
				// Advance until the first path in the cache.
				advanceToNextFullBucket();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Advancement Operator
			
			
			
			
			/// Increment the location of a probe path cache iterator by one element.
			GSOUND_INLINE void operator ++ ()
			{
				GSOUND_DEBUG_ASSERT( currentPath != NULL );
				
				currentPath++;
				
				if ( currentPath == pathsEnd )
				{
					currentBucket++;
					advanceToNextFullBucket();
				}
			}
			
			
			
			
			/// Increment the location of a probe path cache iterator by one element.
			GSOUND_INLINE void operator ++ ( int )
			{
				this->operator++();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Current Element Accessor Operators
			
			
			
			
			/// Return whether or not there are any more paths to be iterated over.
			/**
			  * This method will return FALSE when the last element of the cache.
			  * has been iterated over, otherwise TRUE will be returned.
			  */
			GSOUND_INLINE operator Bool () const
			{
				return currentPath != NULL;
			}
			
			
			
			
			/// Get a reference to the probe path that this iterator currently points to.
			GSOUND_INLINE ProbePath& operator * () const
			{
				GSOUND_DEBUG_ASSERT( currentPath != NULL );
				
				return *currentPath;
			}
			
			
			
			
			/// Access the current path pointed to by the iterator as if the iterator was a pointer to that path.
			GSOUND_INLINE ProbePath* operator -> () const
			{
				GSOUND_DEBUG_ASSERT( currentPath != NULL );
				
				return currentPath;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Current Element Remove Operators
			
			
			
			
			/// Remove the current path pointed to by this iterator and advance to the next path.
			GSOUND_INLINE void remove()
			{
				// Remove the path from the bucket.
				currentBucket->removeAtIndex( currentPath - currentBucket->entries );
				
				// Update the pointer to the end of the bucket.
				pathsEnd--;
				
				// Tell the path cache that there is one fewer element.
				cache->numPaths--;
				
				// Advance to the next bucket if we've reached the end of this bucket.
				if ( currentPath == pathsEnd )
				{
					currentBucket++;
					advanceToNextFullBucket();
				}
			}
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Helper Members
			
			
			
			
			/// Advance the iterator to the next non-empty bucket.
			GSOUND_INLINE void advanceToNextFullBucket()
			{
				while ( currentBucket != bucketsEnd && currentBucket->numEntries == 0 )
					currentBucket++;
				
				if ( currentBucket == bucketsEnd )
					currentPath = NULL;
				else
				{
					currentPath = currentBucket->entries;
					pathsEnd = currentBucket->entries + currentBucket->numEntries;
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			ProbePathCache* cache;
			
			
			
			
			ProbePath* currentPath;
			
			
			
			const ProbePath* pathsEnd;
			
			
			
			Bucket* currentBucket;
			
			
			
			const Bucket* const bucketsEnd;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Inline Methods
//############		
//##########################################################################################
//##########################################################################################




GSOUND_INLINE Bool ProbePathCache:: addPath( const ProbePath& newProbePath )
{
	Bucket* bucket = buckets + (newProbePath.getHashCode() % numBuckets);
	
	if ( bucket->conditionalAdd( newProbePath ) )
	{
		numPaths++;
		return true;
	}
	else
		return false;
}




GSOUND_INLINE Bool ProbePathCache:: containsPath( const ProbePath& probePath ) const
{
	Bucket* bucket = buckets + (probePath.getHashCode() % numBuckets);
	
	return bucket->contains( probePath );
}




GSOUND_INLINE ProbePathCache::Iterator ProbePathCache:: getIterator()
{
	return ProbePathCache::Iterator(*this);
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PROBE_PATH_CACHE_H
