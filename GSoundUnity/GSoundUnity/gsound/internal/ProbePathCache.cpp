/*
 * Project:     GSound
 * 
 * File:        gsound/internal/ProbePathCache.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::ProbePathCache class implementation
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


#include "ProbePathCache.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ProbePathCache:: ProbePathCache()
	:	numBuckets( DEFAULT_NUMBER_OF_BUCKETS ),
		numPaths( 0 ),
		loadFactor( DEFAULT_LOAD_FACTOR ),
		buckets( util::constructArray<Bucket>( DEFAULT_NUMBER_OF_BUCKETS, DEFAULT_BUCKET_SIZE ) )
{
}




ProbePathCache:: ProbePathCache( Hash newNumBuckets, Float newLoadFactor )
	:	numBuckets( nextPrime( newNumBuckets ) ),
		numPaths( 0 ),
		loadFactor( math::max( newLoadFactor, Float(0.1) ) )
{
	buckets = util::constructArray<Bucket>( numBuckets, DEFAULT_BUCKET_SIZE );
}




ProbePathCache:: ProbePathCache( const ProbePathCache& other )
	:	numBuckets( other.numBuckets ),
		numPaths( other.numPaths ),
		loadFactor( other.loadFactor ),
		buckets( util::allocate<Bucket>( other.numBuckets ) )
{
	register Bucket* bucket = buckets;
	register const Bucket* otherBucket = other.buckets;
	register const Bucket* const otherBucketsEnd = other.buckets + other.numBuckets;
	
	while ( otherBucket != otherBucketsEnd )
	{
		// Copy each bucket.
		new (bucket) Bucket( *otherBucket );
		
		otherBucket++;
		bucket++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




ProbePathCache:: ~ProbePathCache()
{
	util::destructArray( buckets, numBuckets );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




ProbePathCache& ProbePathCache:: operator = ( const ProbePathCache& other )
{
	if ( this != &other )
	{
		// Destroy the old bucket array.
		util::destructArray( buckets, numBuckets );
		
		// Initialize the state for this cache from the other bucket.
		numBuckets = other.numBuckets;
		numPaths = other.numPaths;
		loadFactor = other.loadFactor;
		buckets = util::allocate<Bucket>( other.numBuckets );
		
		
		// Copy the buckets from the other cache.
		register Bucket* bucket = buckets;
		register const Bucket* otherBucket = other.buckets;
		register const Bucket* const otherBucketsEnd = other.buckets + other.numBuckets;
		
		while ( otherBucket != otherBucketsEnd )
		{
			// Copy each bucket.
			new (bucket) Bucket( *otherBucket );
			
			otherBucket++;
			bucket++;
		}
	}
	
	return *this;
}





//##########################################################################################
//##########################################################################################
//############		
//############		Cache Manipulation Methods
//############		
//##########################################################################################
//##########################################################################################




void ProbePathCache:: clear()
{
	register Bucket* bucket = buckets;
	register const Bucket* const bucketsEnd = buckets + numBuckets;
	
	while ( bucket != bucketsEnd )
	{
		bucket->clear();
		bucket++;
	}
	
	numPaths = 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Load Factor Check Method
//############		
//##########################################################################################
//##########################################################################################




void ProbePathCache:: checkLoadFactor()
{
	// Caculate the maximum number of paths that can be in the cache before it must be resized.
	Size maxNumPaths = Size(loadFactor*numBuckets);
	
	// Check to see if the number of paths is below the limit.
	if ( numPaths <= maxNumPaths )
		return;
	else
	{
		// Otherise, reallocate the buckets in the cache and reinsert all paths.
		
		// Save the current state of the cache.
		Bucket* oldBuckets = buckets;
		Hash oldNumBuckets = numBuckets;
		
		// Compute the new number of buckets.
		numBuckets = nextPrime( Hash(numPaths/loadFactor) );
		
		// Allocate a new bucket array.
		buckets = util::constructArray<Bucket>( numBuckets, DEFAULT_BUCKET_SIZE );
		
		// Reinsert all paths into the new buckets.
		register const Bucket* oldBucket = oldBuckets;
		register const Bucket* const oldBucketsEnd = oldBuckets + oldNumBuckets;
		
		while ( oldBucket != oldBucketsEnd )
		{
			// For each path in the bucket, add it to the new buckets.
			register const ProbePath* path = oldBucket->entries;
			register const ProbePath* pathsEnd = path + oldBucket->numEntries;
			
			while ( path != pathsEnd )
			{
				Bucket* bucket = buckets + (path->getHashCode() % numBuckets);
				bucket->add( *path );
				
				path++;
			}
			
			oldBucket++;
		}
		
		// Deallocate the old bucket array.
		util::destructArray( oldBuckets, oldNumBuckets );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Private Static Prime Number Finding Method
//############		
//##########################################################################################
//##########################################################################################




GSOUND_INLINE Hash ProbePathCache:: nextPrime( Hash n )
{
	for ( Index i = 0; i < 28; i++ )
	{
		if ( Primes[i] > n )
			return Primes[i];
	}
	
	return n;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Hash Table Size Prime Numbers
//############		
//##########################################################################################
//##########################################################################################




const Hash ProbePathCache:: Primes [] =
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
//##########################################################################################
//############		
//############		Hash Table Default Load Factor Definition
//############		
//##########################################################################################
//##########################################################################################




const Float ProbePathCache:: DEFAULT_LOAD_FACTOR = 0.75;




const Size ProbePathCache:: DEFAULT_BUCKET_SIZE = 2;




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
