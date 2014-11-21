/*
 * Project:     GSound
 * 
 * File:        gsound/SoundSourcePropagationPathBuffer.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundSourcePropagationPathBuffer class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_SOURCE_PROPAGATION_PATH_BUFFER_H
#define INCLUDE_GSOUND_SOUND_SOURCE_PROPAGATION_PATH_BUFFER_H


#include "GSoundBase.h"


#include "SoundSource.h"
#include "SoundSourceReverbResponse.h"
#include "PropagationPath.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which encapsulates the propagation paths for a sound source.
/**
  * A SoundSourcePropagationPathBuffer contains a pointer to the sound source which
  * it is associated with, as well as a buffer of objects describing propagation paths
  * through a scene to a single SoundListener. It also contains an object which describes
  * the reverberation response of the source in its environment.
  *
  * In practice, one doesn't need to directly interact with any SoundSourcePropagationPathBuffer
  * objects. The manipulation of the data structure happens automatically behind the scenes.
  * However, the interface for querying propagation paths is left public in case one wishes
  * to examine the output of the sound propagation system rather than render it.
  */
class SoundSourcePropagationPathBuffer
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create an empty SoundSourcePropagationPathBuffer not associated with any sound source.
			GSOUND_INLINE SoundSourcePropagationPathBuffer()
				:	source( NULL )
			{
			}
			
			
			
			
			/// Create an empty SoundSourcePropagationPathBuffer which holds paths for the specified sound source.
			GSOUND_INLINE SoundSourcePropagationPathBuffer( SoundSource* newSource )
				:	source( newSource )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Real Sound Source Accessor Methods
			
			
			
			
			/// Get a pointer to the sound source associated with this SoundSourcePropagationPathBuffer.
			GSOUND_INLINE SoundSource* getSource() const
			{
				return source;
			}
			
			
			
			
			/// Set the sound source that this SoundSourcePropagationPathBuffer is associated with.
			GSOUND_INLINE void setSource( SoundSource* newSource )
			{
				source = newSource;
			}
			
			
			
			
			/// Set the sound source that this SoundSourcePropagationPathBuffer is associated with to NULL.
			GSOUND_INLINE void clearSource()
			{
				source = NULL;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Propagation Path Accessor Methods
			
			
			
			
			/// Get the PropagationPath at the specified index in the buffer.
			GSOUND_INLINE const PropagationPath& getPropagationPath( Index propagationPathIndex ) const
			{
				GSOUND_DEBUG_ASSERT( propagationPathIndex < propagationPaths.getSize() );
				
				return propagationPaths[propagationPathIndex];
			}
			
			
			
			
			/// Add a PropagationPath to the SoundSourcePropagationPathBuffer.
			GSOUND_INLINE void addPropagationPath( const PropagationPath& newPropagationPath )
			{
				propagationPaths.add( newPropagationPath );
			}
			
			
			
			
			/// Clear all PropagationPath objects from the SoundSourcePropagationPathBuffer.
			GSOUND_INLINE void clearPropagationPaths()
			{
				propagationPaths.clear();
			}
			
			
			
			
			/// Get the number of PropagationPath objects in the SoundSourcePropagationPathBuffer.
			GSOUND_INLINE Size getNumberOfPropagationPaths() const
			{
				return propagationPaths.getSize();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Reverb Response Accessor Methods
			
			
			
			
			/// Get the reverb response for the environment that the sound source is in.
			GSOUND_INLINE const SoundSourceReverbResponse& getReverbResponse() const
			{
				return reverbResponse;
			}
			
			
			
			
			/// Set the reverb response for the environment that the sound source is in.
			GSOUND_INLINE void setReverbResponse( const SoundSourceReverbResponse& newReverbResponse )
			{
				reverbResponse = newReverbResponse;
			}
			
			
			
			
			/// Reset the reverb response for the environment that the sound source is in.
			GSOUND_INLINE void clearReverbResponse()
			{
				reverbResponse = SoundSourceReverbResponse();
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A pointer to the sound source that this propagation path buffer contains paths for.
			SoundSource* source;
			
			
			
			
			/// A list of all propagation paths for the source associated with this propagation path buffer.
			ArrayList<PropagationPath> propagationPaths;
			
			
			
			
			/// The reverb characteristics for this sound source.
			SoundSourceReverbResponse reverbResponse;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_SOURCE_PROPAGATION_PATH_BUFFER_H
