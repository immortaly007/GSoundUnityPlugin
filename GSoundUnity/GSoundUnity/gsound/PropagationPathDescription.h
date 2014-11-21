/*
 * Project:     GSound
 * 
 * File:        gsound/PropagationPathDescription.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::PropagationPathDescription class declaration
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


#ifndef INCLUDE_GSOUND_PROPAGATION_PATH_DESCRIPTION_H
#define INCLUDE_GSOUND_PROPAGATION_PATH_DESCRIPTION_H


#include "GSoundBase.h"


#include "PropagationPathPoint.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which holds a series of propagation path point objects.
/**
  * If two propagation path descriptions are equal, then it can be said that 
  * the two propagation paths that they describe are the same. By convention, 
  * path points should be placed in the path description in the order in which
  * the sound propagates: source, scene interaction 1, scene interaction 2, ... ,
  * listener.
  */
class PropagationPathDescription
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			/// Create a propagation path description object with no path points.
			GSOUND_INLINE PropagationPathDescription()
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Path Point Accessor Methods
			
			
			
			
			/// Insert a new propagation path point at the specified index.
			GSOUND_FORCE_INLINE void insertPoint( const PropagationPathPoint& newPoint, Index index )
			{
				points.insert( newPoint, index );
			}
			
			
			
			
			/// Add a new propagation path point to the end of the path description.
			GSOUND_FORCE_INLINE void addPoint( const PropagationPathPoint& newPoint )
			{
				points.add( newPoint );
			}
			
			
			
			
			/// Clear all propagation path points from this path description.
			GSOUND_FORCE_INLINE void clearPoints()
			{
				points.clear();
			}
			
			
			
			
			/// Get the propagation path point at the specified index.
			GSOUND_FORCE_INLINE PropagationPathPoint& getPoint( Index i )
			{
				return points[i];
			}
			
			
			
			
			/// Get the propagation path point at the specified index.
			GSOUND_FORCE_INLINE const PropagationPathPoint& getPoint( Index i ) const
			{
				return points[i];
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Path Depth Accessor Methods
			
			
			
			
			/// Get the number of points along the propagaion path, including the source and listener.
			GSOUND_FORCE_INLINE Size getNumberOfPoints() const
			{
				return points.getSize();
			}
			
			
			
			
			/// Get the number of interactions in the scene by the propagation path.
			/**
			  * The value returned by this method is 2 less than the total number of
			  * points in the path description because it does not include the path
			  * endpoints (the source and listener).
			  */
			GSOUND_FORCE_INLINE Size getDepth() const
			{
				return points.getSize() - 2;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Path Description Equality Comparison Methods
			
			
			
			
			/// Return whether or not this path description has the same path points as another.
			/**
			  * Path descriptions with the same points but in a different order are
			  * defined as different.
			  */
			GSOUND_FORCE_INLINE Bool operator == ( const PropagationPathDescription& other ) const
			{
				return points == other.points;
			}
			
			
			
			
			/// Return whether or not this path description has path points that differ from another's.
			/**
			  * Path descriptions with the same points but in a different order are
			  * defined as different.
			  */
			GSOUND_FORCE_INLINE Bool operator != ( const PropagationPathDescription& other ) const
			{
				return points != other.points;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
#if GSOUND_FIXED_MAX_PATH_DEPTH
			/// A list of the points on the propagation path.
			StaticArrayList<PropagationPathPoint,GSOUND_MAX_PATH_DEPTH> points;
#else
			/// A list of the points on the propagation path.
			ArrayList<PropagationPathPoint> points;
#endif
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PROPAGATION_PATH_DESCRIPTION_H
