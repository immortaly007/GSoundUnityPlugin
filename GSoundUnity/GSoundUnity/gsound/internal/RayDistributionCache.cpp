/*
 * Project:     GSound
 * 
 * File:        gsound/internal/RayDistributionCache.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::RayDistributionCache class implementation
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


#include "RayDistributionCache.h"


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




RayDistributionCache:: RayDistributionCache()
	:	numDivisions( 10 ),
		cells( NULL ),
		minimumRayAffinity( 0.05 ),
		maximumRayAffinity( 1 )
{
	initializeCells( numDivisions );
}




RayDistributionCache:: RayDistributionCache( Size newNumDivisions )
	:	numDivisions( math::nextMultiple( newNumDivisions, Size(2) ) ),
		cells( NULL ),
		minimumRayAffinity( 0.05 ),
		maximumRayAffinity( 1 )
{
	initializeCells( numDivisions );
}




RayDistributionCache:: RayDistributionCache( const RayDistributionCache& other )
	:	numDivisions( other.numDivisions ),
		minimumRayAffinity( other.minimumRayAffinity ),
		maximumRayAffinity( other.maximumRayAffinity )
{
	cells = util::copyArray<Real>( other.cells, this->getNumberOfCells() );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




RayDistributionCache:: ~RayDistributionCache()
{
	destroyCells();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




RayDistributionCache& RayDistributionCache:: operator = ( const RayDistributionCache& other )
{
	if ( this != &other )
	{
		destroyCells();
		
		minimumRayAffinity = other.minimumRayAffinity;
		maximumRayAffinity = other.maximumRayAffinity;
		numDivisions = other.numDivisions;
		cells = util::copyArray<Real>( other.cells, this->getNumberOfCells() );
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Distribution Sum Method
//############		
//##########################################################################################
//##########################################################################################




Real RayDistributionCache:: getSum() const
{
	Real* cell = cells;
	const Real* const cellsEnd = cells + this->getNumberOfCells();
	Real sum = 0;
	
	while ( cell != cellsEnd )
	{
		sum += *cell;
		cell++;
	}
	
	return sum;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Distribution Normalization Method
//############		
//##########################################################################################
//##########################################################################################




void RayDistributionCache:: normalize()
{
	Size numCells = this->getNumberOfCells();
	Real* cell = cells;
	const Real* const cellsEnd = cells + numCells;
	Real sum = 0;
	
	while ( cell != cellsEnd )
	{
		sum += *cell;
		cell++;
	}
	
	cell = cells;
	Real multiplier = Real(numCells) / sum;
	
	while ( cell != cellsEnd )
	{
		*cell *= multiplier;
		cell++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cell Initialization and Destruction Methods
//############		
//##########################################################################################
//##########################################################################################




void RayDistributionCache:: initializeCells( Size newNumDivisions )
{
	destroyCells();
	
	numDivisions = newNumDivisions;
	cells = util::constructArray<Real>( this->getNumberOfCells(), Real(1) );
}




void RayDistributionCache:: destroyCells()
{
	if ( cells != NULL )
		util::deallocate( cells );
	
	cells = NULL;
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
