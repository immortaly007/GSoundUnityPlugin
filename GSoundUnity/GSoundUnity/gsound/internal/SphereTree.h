/*
 * Project:     GSound
 * 
 * File:        gsound/internal/SphereTree.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::internal::SphereTree class declaration
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


#ifndef INCLUDE_GSOUND_SPHERE_TREE_H
#define INCLUDE_GSOUND_SPHERE_TREE_H


#include "GSoundInternalConfig.h"


#include "../SoundObject.h"
#include "BoundingSphere.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




class SphereTree
{
	public:
		
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			GSOUND_INLINE SphereTree( ArrayList<SoundObject*>& objects )
				:	object( NULL ),
					left( NULL ),
					right( NULL )
			{
				generateTree( objects );
			}
			
			
			
			
			GSOUND_INLINE SphereTree( const SphereTree& other )
				:	object( other.object ),
					left( other.left ? util::construct<SphereTree>(*other.left) : NULL ),
					right( other.right ? util::construct<SphereTree>(*other.right) : NULL ),
					boundingVolume( other.boundingVolume )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			~SphereTree();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			GSOUND_INLINE SphereTree& operator = ( const SphereTree& other )
			{
				if ( &other != this )
				{
					if ( left != NULL )
						util::destruct(left);
					
					if ( right != NULL )
						util::destruct(right);
					
					left = other.left ? util::construct<SphereTree>(*other.left) : NULL;
					right = other.right ? util::construct<SphereTree>(*other.right) : NULL;
					object = other.object;
					boundingVolume = other.boundingVolume;
				}
				
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Property Accessor Methods
			
			
			
			
			GSOUND_INLINE const SphereTree* getLeftChild() const
			{
				return left;
			}
			
			
			
			GSOUND_INLINE const SphereTree* getRightChild() const
			{
				return right;
			}
			
			
			
			
			GSOUND_INLINE SoundObject* getObject() const
			{
				return object;
			}
			
			
			
			
			GSOUND_INLINE Bool isLeaf() const
			{
				return object != NULL;
			}
			
			
			
			
			GSOUND_INLINE const BoundingSphere& getVolume() const
			{
				return boundingVolume;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Tree Update Method
			
			
			
			
			/// Update the bounding volumes of this BVH so that they are consistent with new object sizes/positions.
			/**
			  * This method does not change the internal structure of the tree and thus
			  * will not provide an optimally-split BVH. However, it is much faster to call
			  * this method than to rebuild the entire BVH.
			  */
			void update();
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Constructors
			
			
			
			
			GSOUND_INLINE SphereTree()
				:	object( NULL ),
					left( NULL ),
					right( NULL )
			{
			}
			
			
			
			
			GSOUND_INLINE SphereTree( SoundObject* newObject, const BoundingSphere& newBoundingVolume )
				:	object( newObject ),
					boundingVolume( newBoundingVolume ),
					left( NULL ),
					right( NULL )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Tree Generation Helper Method
			
			
			
			
			void generateTree( ArrayList<SoundObject*>& objects );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Node Generation Method
			
			
			
			
			void generateNode( ArrayList<SoundObject*>& objects, Index start, Index end, Index splitAxis );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Median-Point Split Method
			
			
			
			
			static void medianPointSplit( ArrayList<SoundObject*>& objects,
										Index start, Index end, Size dimension );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			BoundingSphere boundingVolume;
			
			
			
			
			
			SoundObject* object;
			
			
			
			
			SphereTree* left;
			
			
			
			
			SphereTree* right;
			
			
			
			
};





//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SPHERE_TREE_H
