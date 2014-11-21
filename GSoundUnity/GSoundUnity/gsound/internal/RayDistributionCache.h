/*
 * Project:     GSound
 * 
 * File:        gsound/internal/RayDistributionCache.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::RayDistributionCache class declaration
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


#ifndef INCLUDE_GSOUND_RAY_DISTRIBUTION_CACHE_H
#define INCLUDE_GSOUND_RAY_DISTRIBUTION_CACHE_H


#include "GSoundInternalConfig.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which represents the spherical distribution of rays that should be traced for sound propagation.
class RayDistributionCache
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Class Declaration
			
			
			
			
			/// A class which iterates over the cells in this ray distribution cache.
			class Iterator;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a ray distribution cache with the default number of cell divisions.
			RayDistributionCache();
			
			
			
			
			/// Create a ray distribution cache with the specified number of cell divisions.
			RayDistributionCache( Size newNumDivisions );
			
			
			
			
			/// Create a deep copy of this ray distribution cache object.
			RayDistributionCache( const RayDistributionCache& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy this ray distribution cache object and its internal state.
			~RayDistributionCache();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the internal state of another ray distribution cache to this cache.
			RayDistributionCache& operator = ( const RayDistributionCache& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Division Count Accessor Methods
			
			
			
			
			/// Return the number of cells in this ray distribution cache.
			GSOUND_FORCE_INLINE Size getNumberOfCells() const
			{
				return numDivisions*(numDivisions >> 1);
			}
			
			
			
			
			
			/// Return the number of angle divisions around the circumference of the distribution sphere.
			GSOUND_FORCE_INLINE Size getNumberOfDivisions() const
			{
				return numDivisions;
			}
			
			
			
			
			/// Set the number of angle divisions around the circumference of the distribution sphere.
			GSOUND_FORCE_INLINE void setNumberOfDivisions( Size newNumDivisions )
			{
				initializeCells( math::nextMultiple( newNumDivisions, Size(2) ) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Ray Affinity Limit Accessor Methods
			
			
			
			
			/// Return the minimum ray affinity that a cell can have.
			GSOUND_FORCE_INLINE Real getMinimumRayAffinity() const
			{
				return minimumRayAffinity;
			}
			
			
			
			
			/// Set the minimum ray affinity that a cell can have, clamped to [0,1].
			GSOUND_FORCE_INLINE void setMinimumRayAffinity( Real newMinimumRayAffinity )
			{
				minimumRayAffinity = math::clamp( newMinimumRayAffinity, Real(0), Real(1) );
			}
			
			
			
			
			/// Return the maximum ray affinity that a cell can have.
			GSOUND_FORCE_INLINE Real getMaximumRayAffinity() const
			{
				return maximumRayAffinity;
			}
			
			
			
			
			/// Set the maximum ray affinity that a cell can have, clamped to [1,100].
			GSOUND_FORCE_INLINE void setMaximumRayAffinity( Real newMaximumRayAffinity )
			{
				maximumRayAffinity = math::clamp( newMaximumRayAffinity, Real(1), Real(100) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Normalization Method
			
			
			
			
			/// Normalize the ray distribution cache.
			/**
			  * This method modifies the distribution so that the sum of the
			  * ray affinities for all cells is equal to the number of cells.
			  */
			void normalize();
			
			
			
			
			/// Get the sum of all cells in this ray distribution cache.
			Real getSum() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iterator Creation Method
			
			
			
			
			/// Get an iterator for this ray distribution cache.
			GSOUND_FORCE_INLINE Iterator getIterator();
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Methods
			
			
			
			
			/// Initialize the contents of this ray distribution cache.
			void initializeCells( Size numDivisions );
			
			
			
			
			/// Destroy the contents of this ray distribution cache.
			void destroyCells();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The number of angle divisions around the circumference of the distribution sphere.
			Size numDivisions;
			
			
			
			
			/// A pointer to an array of ray distribution values, one for each ray cell.
			Real* cells;
			
			
			
			
			/// The minimum ray affinity that a cell can have.
			Real minimumRayAffinity;
			
			
			
			
			/// The maximum ray affinity that a cell can have.
			Real maximumRayAffinity;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Ray Distribution Cache Iterator Class Definition
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which iterates over cells in a ray distribution cache.
class RayDistributionCache::Iterator
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a ray distribution iterator for the specified distribution cache.
			/**
			  * Modifying the dimensions of the cache or its elements while iteration
			  * over them is occuring results in undefined behavior and should be avoided.
			  */
			GSOUND_INLINE Iterator( RayDistributionCache& newCache )
				:	cache( newCache ),
					cell( newCache.cells ),
					heightIndex( 0 ),
					longitudeIndex( 0 ),
					height( 1 ),
					longitude( 0 ),
					halfNumDivisions( newCache.getNumberOfDivisions() >> 1 )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Current Cell Ray Affinity Accessor Methods
			
			
			
			
			/// Return the affinity of the current cell to have rays.
			/**
			  * A value of 1 means that the cell should get no more rays
			  * than any other cell and is the default state. A value greater than
			  * 1 indicates that the cell should get more rays, and a value
			  * less than 1 indicates that the cell should get less rays.
			  */
			GSOUND_FORCE_INLINE Real getRayAffinity() const
			{
				return *cell;
			}
			
			
			
			
			/// Additively increase the ray affinity for the current cell.
			GSOUND_FORCE_INLINE void increaseRayAffinity()
			{
				*cell = math::min( *cell + Real(0.1), cache.getMaximumRayAffinity() );
			}
			
			
			
			
			/// Multiplicatively decrease the ray affinity for the current cell.
			GSOUND_FORCE_INLINE void decreaseRayAffinity()
			{
				*cell = math::max( *cell - Real(0.01), cache.getMinimumRayAffinity() );
			}
			
			
			
			
			/// Set the affinity of the current cell to have rays.
			/**
			  * A value of 1 means that the cell should get no more rays
			  * than any other cell and is the default state. A value greater than
			  * 1 indicates that the cell should get more rays, and a value
			  * less than 1 indicates that the cell should get less rays.
			  */
			GSOUND_FORCE_INLINE void setRayAffinity( Real newRayAffinity )
			{
				*cell = math::clamp( newRayAffinity, cache.getMinimumRayAffinity(), cache.getMaximumRayAffinity() );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Iteration Methods
			
			
			
			
			/// Advance to the next cell in the iteration.
			GSOUND_FORCE_INLINE void operator ++ ( int )
			{
				if ( heightIndex < halfNumDivisions - 1 )
				{
					heightIndex++;
					height = Real(1) - Real(2)*Real(heightIndex)/Real(halfNumDivisions);
					cell++;
				}
				else if ( longitudeIndex < cache.getNumberOfDivisions() - 1 )
				{
					longitudeIndex++;
					longitude = Real(longitudeIndex)*math::pi<Real>() / Real(halfNumDivisions);
					heightIndex = 0;
					height = Real(1);
					cell++;
				}
			}
			
			
			
			
			/// Return whether or not there are any more cells to iterate over.
			GSOUND_FORCE_INLINE operator Bool () const
			{
				return longitudeIndex < cache.getNumberOfDivisions() - 1 || heightIndex < halfNumDivisions - 1;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Cell Bound Accessor Methods
			
			
			
			
			/// Return a range of longitudes which the current distribution cell represents.
			GSOUND_FORCE_INLINE AABB1 getCellLongitudes() const
			{
				return AABB1( longitude, longitude + math::pi<Real>() / Real(halfNumDivisions) );
			}
			
			
			
			
			/// Return a range of latitudes which the current distribution cell represents.
			GSOUND_FORCE_INLINE AABB1 getCellLatitudes() const
			{
				Real height2 = Real(1) - Real(2)*Real(heightIndex + 1)/Real(halfNumDivisions);
				const Real piOver2 = math::pi<Real>()*Real(0.5);
				
				return AABB1( math::asin(height) + piOver2, math::asin(height2) + piOver2 );
			}
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The ray distribution cache that is being iterated over.
			RayDistributionCache& cache;
			
			
			
			
			/// The height row index of the current cell, from 0 to half the number of divisions.
			Index heightIndex;
			
			
			
			
			/// The longitudinal sector index, from 0 to the number of divisions.
			Index longitudeIndex;
			
			
			
			
			/// Half of the number of longitudinal and latitudinal divisions in the distribution.
			Size halfNumDivisions;
			
			
			
			
			/// The minimum height for the current distribution cell.
			Real height;
			
			
			
			
			/// The minimum longitude for the current distribution cell.
			Real longitude;
			
			
			
			
			/// A pointer to the current cell's distribution value.
			Real* cell;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Iterator Accessor Method
//############		
//##########################################################################################
//##########################################################################################




RayDistributionCache::Iterator RayDistributionCache:: getIterator()
{
	return Iterator(*this);
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_RAY_DISTRIBUTION_CACHE_H
