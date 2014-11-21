/*
 * Project:     GSound
 * 
 * File:        gsound/internal/ProbedTriangleCache.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::ProbedTriangleCache class declaration
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


#ifndef INCLUDE_GSOUND_PROBED_TRIANGLE_CACHE_H
#define INCLUDE_GSOUND_PROBED_TRIANGLE_CACHE_H


#include "GSoundInternalConfig.h"


#include "ObjectSpaceTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




template < typename DataType >
class ProbedTriangleCache
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Probed Triangle Iterator Declaration
			
			
			
			
			class Iterator;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			GSOUND_INLINE ProbedTriangleCache()
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Cache Manipulation Methods
			
			
			
			
			GSOUND_INLINE Bool add( const ObjectSpaceTriangle& triangle, const DataType& data )
			{
				return triangleMap.set( triangle.getHashCode(), triangle, data );
			}
			
			
			
			
			GSOUND_INLINE Bool find( const ObjectSpaceTriangle& triangle, DataType*& data )
			{
				return triangleMap.find( triangle.getHashCode(), triangle, data );
			}
			
			
			
			
			GSOUND_INLINE Bool find( const ObjectSpaceTriangle& triangle, const DataType*& data ) const
			{
				return triangleMap.find( triangle.getHashCode(), triangle, data );
			}
			
			
			
			
			GSOUND_INLINE Bool remove( const ObjectSpaceTriangle& triangle )
			{
				return triangleMap.remove( triangle.getHashCode(), triangle );
			}
			
			
			
			GSOUND_INLINE void clear()
			{
				triangleMap.clear();
			}
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Cache Size Accessor Method
			
			
			
			GSOUND_INLINE Size getNumberOfTriangles() const
			{
				return triangleMap.getSize();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Accessor Methods
			
			
			
			
			GSOUND_INLINE Iterator getIterator();
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			HashMap<ObjectSpaceTriangle,DataType> triangleMap;
			
			
			
};




template < typename DataType >
class ProbedTriangleCache<DataType>::Iterator
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			GSOUND_INLINE Iterator( ProbedTriangleCache<DataType>& cache )
				:	iterator( cache.triangleMap.getIterator() )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Advancement Methods
			
			
			
			
			GSOUND_INLINE void operator ++ ()
			{
				iterator++;
			}
			
			
			
			
			GSOUND_INLINE void operator ++ ( int )
			{
				iterator++;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Validity Accessor Method
			
			
			
			
			GSOUND_INLINE operator Bool () const
			{
				return iterator;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Element Accessor Methods
			
			
			
			
			GSOUND_INLINE const ObjectSpaceTriangle& getTriangle() const
			{
				return iterator.getKey();
			}
			
			
			
			
			GSOUND_INLINE DataType& getData() const
			{
				return iterator.getValue();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Element Removal Method
			
			
			
			
			GSOUND_INLINE void remove()
			{
				iterator.remove();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Reset Method
			
			
			
			
			GSOUND_INLINE void reset()
			{
				iterator.reset();
			}
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			typename HashMap<ObjectSpaceTriangle,DataType>::Iterator iterator;
			
			
			
			
};




template < typename DataType >
typename ProbedTriangleCache<DataType>::Iterator ProbedTriangleCache<DataType>:: getIterator()
{
	return typename ProbedTriangleCache<DataType>::Iterator( *this );
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PROBED_TRIANGLE_CACHE_H