/*
 * Project:     GSound
 * 
 * File:        gsound/SoundPropagationPathBuffer.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundPropagationPathBuffer class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_PROPAGATION_PATH_BUFFER_H
#define INCLUDE_GSOUND_SOUND_PROPAGATION_PATH_BUFFER_H


#include "GSoundBase.h"


#include "SoundSourcePropagationPathBuffer.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which encapsulates the propagation path information for one frame of sound propagation.
/**
  * A SoundPropagationPathBuffer contains all of the sound propagation information for
  * a single SoundListener in a scene. The class itself contains a buffer of
  * SoundSourcePropagationPathBuffer objects which contain the propagation paths for
  * each SoundSource in the scene.
  *
  * In practice, one only needs to create a SoundPropagationPathBuffer object and pass that
  * object between SoundPropagator and SoundPropagationRenderer objects. The manipulation
  * of the data structure happens automatically in these classes. However, the interface
  * for querying propagation paths is left public in case one wishes to examine the output
  * of the sound propagation system rather than render it.
  */
class SoundPropagationPathBuffer
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create an empty sound propagation buffer which holds data for 0 sound sources.
			GSOUND_INLINE SoundPropagationPathBuffer()
				:	numSources( 0 )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Source Contribution Number Accessor Methods
			
			
			
			
			/// Get the number of sound sources that this SoundPropagationPathBuffer can hold data for.
			GSOUND_INLINE Size getNumberOfSources() const
			{
				return numSources;
			}
			
			
			
			
			/// Set the number of sound sources that this SoundPropagationPathBuffer can hold data for.
			/**
			  * If necessary, this method increases the number of SoundSourcePropagationPathBuffer objects
			  * that this propagation path buffer holds. A typical implementation will avoid deallocating
			  * existing SoundSourcePropagationPathBuffer objects if the number of sound sources is
			  * decreased.
			  * 
			  * @param newNumSources - the new number of sound sources that this buffer should hold data for.
			  */
			GSOUND_INLINE void setNumberOfSources( Size newNumSources )
			{
				for ( Index i = sourceBuffers.getSize(); i < newNumSources; i++ )
					sourceBuffers.add( SoundSourcePropagationPathBuffer() );
				
				numSources = newNumSources;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Source Propagation Buffer Accessor Methods
			
			
			
			
			/// Get the SoundSourcePropagationPathBuffer for the sound source at the specified index.
			/**
			  * If the specified sound source index is a valid index, a reference to the
			  * SoundSourcePropagationPathBuffer for the sound source at that index is returned.
			  * Otherwise, an assertion is raised.
			  * 
			  * @param sourceIndex - the index of the sound source whose propagation path buffer should be accessed.
			  * @return a reference to the SoundSourcePropagationPathBuffer for the specified sound source.
			  */
			GSOUND_INLINE SoundSourcePropagationPathBuffer& getSourceBuffer( Index sourceIndex )
			{
				GSOUND_DEBUG_ASSERT( sourceIndex < numSources );
				
				return sourceBuffers[sourceIndex];
			}
			
			
			
			
			/// Get the SoundSourcePropagationPathBuffer for the sound source at the specified index.
			/**
			  * If the specified sound source index is a valid index, a const reference to the
			  * SoundSourcePropagationPathBuffer for the sound source at that index is returned.
			  * Otherwise, an assertion is raised.
			  * 
			  * @param sourceIndex - the index of the sound source whose propagation path buffer should be accessed.
			  * @return a const reference to the SoundSourcePropagationPathBuffer for the specified sound source.
			  */
			GSOUND_INLINE const SoundSourcePropagationPathBuffer& getSourceBuffer( Index sourceIndex ) const
			{
				GSOUND_DEBUG_ASSERT( sourceIndex < numSources );
				
				return sourceBuffers[sourceIndex];
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Propagation Path Clear Method
			
			
			
			
			/// Clear all propagation paths from this buffer.
			/**
			  * This method does not change the number of sources that the buffer supports.
			  * It only resets every SoundSourcePropagationPathBuffer to a default state.
			  */
			GSOUND_INLINE void clearPropagationPaths()
			{
				for ( Index i = 0; i < sourceBuffers.getSize(); i++ )
				{
					SoundSourcePropagationPathBuffer& buffer = sourceBuffers[i];
					
					buffer.clearPropagationPaths();
					buffer.clearReverbResponse();
					buffer.clearSource();
				}
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Propagation Path Count Accessor Method
			
			
			
			
			/// Get the total number of propagation paths that this SoundPropagationPathBuffer is holding.
			/**
			  * This method iterates over all internal SoundSourcePropagationPathBuffer objects
			  * and adds together the number of propagation paths for every buffer. This value
			  * is then returned.
			  * 
			  * @return the total number of propagation paths that this SoundPropagationPathBuffer is holding.
			  */
			GSOUND_INLINE Size getTotalNumberOfPropagationPaths() const
			{
				Size total = 0;
				
				for ( Index i = 0; i < sourceBuffers.getSize(); i++ )
					total += sourceBuffers[i].getNumberOfPropagationPaths();
				
				return total;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A list of the propagation buffers for each sound source.
			ArrayList<SoundSourcePropagationPathBuffer> sourceBuffers;
			
			
			
			
			/// The number of sound sources that this buffer holds propagation paths for.
			/**
			  * The number of sound sources is stored separately in order to not depend on
			  * the size of the SoundSourcePropagationPathBuffer array to indicate the number
			  * of sound sources. This results in less reallocations of the propagation path bufffers
			  * in typical use cases.
			  */
			Size numSources;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_PROPAGATION_PATH_BUFFER_H
