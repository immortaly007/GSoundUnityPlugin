/*
 * Project:     GSound
 * 
 * File:        gsound/PropagationPathID.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::PropagationPathID class declaration
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


#ifndef INCLUDE_GSOUND_PROPAGATION_PATH_ID_H
#define INCLUDE_GSOUND_PROPAGATION_PATH_ID_H


#include "GSoundBase.h"


#include "PropagationPathDescription.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which is used to uniquely identify and quickly compare sound propagation paths.
/**
  * It contains an object which describes each point of a propagation path, as well as
  * an integral hash code for that propagation path. Two equal propagation paths will
  * have the same hash code. This fact is used to speed up path equality comparisons.
  */
class PropagationPathID
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			/// Create a new propagation path ID for the specified path description.
			GSOUND_INLINE PropagationPathID( const PropagationPathDescription& newDescription )
				:	description( newDescription ),
					hashCode( getDescriptionHash( newDescription ) )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Path ID Data Accessor Methods
			
			
			
			
			/// Get an integral hash code for this propagation path ID.
			GSOUND_FORCE_INLINE Hash getHashCode() const
			{
				return hashCode;
			}
			
			
			
			
			/// Get the propagation path description used by this path ID.
			GSOUND_FORCE_INLINE const PropagationPathDescription& getDescription() const
			{
				return description;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Propagation Path ID Equaliy Comparison Operators.
			
			
			
			
			/// Return whether or not this path ID is equal to another.
			GSOUND_INLINE Bool operator == ( const PropagationPathID& other ) const
			{
				if ( hashCode != other.hashCode )
					return false;
				
				return description == other.description;
			}
			
			
			
			
			/// Return whether or not this path ID is not equal to another.
			GSOUND_INLINE Bool operator != ( const PropagationPathID& other ) const
			{
				if ( hashCode != other.hashCode )
					return true;
				
				return description != other.description;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Hashing Method
			
			
			
			
			/// Get a hash code for the specified path description object.
			static Hash getDescriptionHash( const PropagationPathDescription& description );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A description of the propagation path that this path ID represents.
			PropagationPathDescription description;
			
			
			
			
			/// An integral hash code generated for the propagation path.
			Hash hashCode;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PROPAGATION_PATH_ID_H
