/*
 * Project:     GSound
 * 
 * File:        gsound/internal/SphereTree.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::SphereTree class implementation
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


#include "SphereTree.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SphereTree:: ~SphereTree()
{
	if ( left != NULL )
		util::destruct(left);
	
	if ( right != NULL )
		util::destruct(right);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Tree Update Method
//############		
//##########################################################################################
//##########################################################################################




void SphereTree:: update()
{
	if ( left != NULL )
	{
		left->update();
		
		if ( right != NULL )
		{
			right->update();
			
			boundingVolume = left->boundingVolume + right->boundingVolume;
		}
		else
			boundingVolume = left->boundingVolume;
	}
	else if ( right != NULL )
	{
		right->update();
		
		boundingVolume = right->boundingVolume;
	}
	else
	{
		boundingVolume = object->getBoundingSphere();
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Tree Generation Method
//############		
//##########################################################################################
//##########################################################################################




void SphereTree:: generateTree( ArrayList<SoundObject*>& objects )
{
	generateNode( objects, 0, objects.getSize() - 1, 0 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Node Generation Method
//############		
//##########################################################################################
//##########################################################################################




void SphereTree:: generateNode( ArrayList<SoundObject*>& objects,
								Index start, Index end, Index splitAxis )
{
	Size numberOfObjects = end - start + 1;
	
	if ( numberOfObjects == 1 )
	{
		// This node encapsulates a single object.
		object = objects[start];
		left = NULL;
		right = NULL;
		boundingVolume = object->getBoundingSphere();
		
		return;
	}
	else if ( numberOfObjects == 2 )
	{
		object = NULL;
		
		left = util::allocate<SphereTree>();
		new (left) SphereTree( objects[start], objects[start]->getBoundingSphere() );
		
		right = util::allocate<SphereTree>();
		new (right) SphereTree( objects[end], objects[end]->getBoundingSphere() );
	}
	else
	{
		object = NULL;
		
		left = util::allocate<SphereTree>();
		new (left) SphereTree();
		
		right = util::allocate<SphereTree>();
		new (right) SphereTree();
		
		medianPointSplit( objects, start, end, splitAxis );
		Index medianIndex = (start + end)/2;
		left->generateNode( objects, start, medianIndex, (splitAxis + 1) % 3 );
		right->generateNode( objects, medianIndex + 1, end, (splitAxis + 1) % 3 );
	}
	
	boundingVolume = left->boundingVolume + right->boundingVolume;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Median Point Split Method
//############		
//##########################################################################################
//##########################################################################################




void SphereTree:: medianPointSplit( ArrayList<SoundObject*>& objects,
									Index start, Index end, Size dimension )
{
	Index first = start;
	Index last = end;
	Index middle = (first + last)/2;
	
	while ( 1 )
	{
		Index mid = first;
		Vector3 key = objects[mid]->getBoundingSphere().position;
		
		for ( Index j = first + 1; j <= last; j ++)
		{
			if ( objects[j]->getBoundingSphere().position[dimension] > key[dimension] )
			{
				mid++;
				
				// interchange values.
				SoundObject* temp = objects[mid];
				objects[mid] = objects[j];
				objects[j] = temp;
			}
		}
		
		// interchange the first and mid value.
		SoundObject* temp = objects[mid];
		objects[mid] = objects[first];
		objects[first] = temp;
		
		if ( mid + 1 == middle )
			return;
		
		if ( mid + 1 > middle )
			last = mid - 1;
		else
			first = mid + 1;
	}
}



//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
