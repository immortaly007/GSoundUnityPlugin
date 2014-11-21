/*
 * Project:     GSound
 * 
 * File:        gsound/internal/ProbePath.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::ProbePath class declaration
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


#ifndef INCLUDE_GSOUND_PROBE_PATH_H
#define INCLUDE_GSOUND_PROBE_PATH_H


#include "GSoundInternalConfig.h"


#include "ObjectSpaceTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




class ProbePath
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			GSOUND_INLINE ProbePath()
				:	hashCode( 0 ),
					foundPaths( false )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Equality Comparison Operators
			
			
			
			
			GSOUND_INLINE Bool operator == ( const ProbePath& other ) const
			{
				if ( hashCode != other.hashCode )
					return false;
				else
					return triangles == other.triangles;
			}
			
			
			
			
			GSOUND_INLINE Bool operator != ( const ProbePath& other ) const
			{
				return !((*this) == other);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Triangle Accessor Methods
			
			
			
			
			GSOUND_INLINE void addTriangle( const ObjectSpaceTriangle& newTriangle )
			{
				triangles.add( newTriangle );
				hashCode += (Hash(triangles.getSize())*Hash(0x8DA6B343)) ^ (newTriangle.triangle->getHashCode()*Hash(0xD8163841));
			}
			
			
			
			
			GSOUND_INLINE void insertTriangle( const ObjectSpaceTriangle& newTriangle, Index triangleIndex )
			{
				triangles.insert( newTriangle, triangleIndex );
				
				// Recompute the path's hash code.
				hashCode = 0;
				
				for ( Index i = 0; i < triangles.getSize(); i++ )
					hashCode += (Hash(i + 1)*Hash(0x8DA6B343)) ^ (triangles[i].triangle->getHashCode()*Hash(0xD8163841));
			}
			
			
			
			
			GSOUND_INLINE const ObjectSpaceTriangle& getTriangle( Index triangleIndex ) const
			{
				GSOUND_DEBUG_ASSERT( triangleIndex < triangles.getSize() );
				
				return triangles[triangleIndex];
			}
			
			
			
			
			GSOUND_INLINE Size getDepth() const
			{
				return triangles.getSize();
			}
			
			
			
			
			GSOUND_INLINE Size getNumberOfTriangles() const
			{
				return triangles.getSize();
			}
			
			
			
			
			GSOUND_INLINE void clearTriangles()
			{
				triangles.clear();
				hashCode = 0;
				foundPaths = false;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Found Paths Accessor Methods
			
			
			
			
			/// Get whether or not any valid propagation paths were found with this probe path.
			GSOUND_INLINE Bool getFoundPaths() const
			{
				return foundPaths;
			}
			
			
			
			
			/// Set whether or not any valid propagation paths were found with this probe path.
			GSOUND_INLINE void setFoundPaths( Bool newFoundPaths )
			{
				foundPaths = newFoundPaths;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Hash Code Accessor Method
			
			
			
			
			/// Get a hash code for this probe path which is automatically generated as triangles are added.
			GSOUND_INLINE Hash getHashCode() const
			{
				return hashCode;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
#if GSOUND_FIXED_MAX_PATH_DEPTH
			StaticArrayList<ObjectSpaceTriangle,GSOUND_MAX_PATH_DEPTH> triangles;
#else
			ArrayList<ObjectSpaceTriangle> triangles;
#endif
			
			
			
			
			Hash hashCode;
			
			
			
			
			Bool foundPaths;
			
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PROBE_PATH_H
