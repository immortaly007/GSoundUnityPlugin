/*
 * Project:     GSound
 * 
 * File:        gsound/internal/QBVHArrayTreeNode.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::QBVHArrayTreeNode class implementation
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


#include "QBVHArrayTreeNode.h"


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




QBVHArrayTreeNode:: QBVHArrayTreeNode( const StaticArray<AABB3,4>& newVolumes, const StaticArray<Index,4>& newIndices,
										const StaticArray<Bool,4>& isALeaf, const StaticArray<Size,4>& numLeafTriangles,
										const StaticArray<Index,3>& newSplitAxes )
	:	volumes( newVolumes[0], newVolumes[1], newVolumes[2], newVolumes[3] )
{
	for ( Index i = 0; i < 4; i++ )
	{
		indices[i] = newIndices[i];
		
		if ( isALeaf[i] )
			flags[i] = LEAF_NODE_FLAG | (UInt8)(numLeafTriangles[i]);
		else
			flags[i] = 0;
	}
	
	splitAxis[0] = newSplitAxes[0];
	splitAxis[1] = newSplitAxes[1];
	splitAxis[2] = newSplitAxes[2];
}




//##########################################################################################
//##########################################################################################
//############		
//############		Composite Volume Determination Method
//############		
//##########################################################################################
//##########################################################################################




AABB3 QBVHArrayTreeNode:: getVolume() const
{
	AABB3 result( volumes.min.x[0], volumes.max.x[0],
					volumes.min.y[0], volumes.max.y[0],
					volumes.min.z[0], volumes.max.z[0] );
	
	for ( Index i = 1; i < 4; i++ )
	{
		result += AABB3( volumes.min.x[i], volumes.max.x[i],
						volumes.min.y[i], volumes.max.y[i],
						volumes.min.z[i], volumes.max.z[i] );
	}
	
	return result;
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
