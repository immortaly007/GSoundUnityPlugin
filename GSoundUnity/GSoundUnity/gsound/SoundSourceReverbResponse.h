/*
 * Project:     GSound
 * 
 * File:        gsound/SoundSourceReverbResponse.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundSourceReverbResponse class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_SOURCE_REVERB_RESPONSE_H
#define INCLUDE_GSOUND_SOUND_SOURCE_REVERB_RESPONSE_H


#include "GSoundBase.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which provides parameters needed to calculate the reverb time of an environment.
/**
  * The parameters correspond to the input parameters of the Erying reverb estimation
  * equation: volume, surface area, average surface attenuation. In addition, there is a
  * frequency-dependent distance attenuation value which provides an attenuation amount
  * computed based on environmental geometry. This value can be used to adjust the output
  * gain of a reverb audio renderer for semi-realistic reverb attenuation.
  */
class SoundSourceReverbResponse
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a default sound source reverb response.
			GSOUND_INLINE SoundSourceReverbResponse()
				:	volume( 0 ),
					surfaceArea( 0 ),
					averageSurfaceAttenuation( 0 ),
					distanceAttenuation( 0 )
			{
			}
			
			
			
			
			/// Create a default sound source reverb response with the specified parameters.
			GSOUND_INLINE SoundSourceReverbResponse( Real newVolume, Real newSurfaceArea,
											const FrequencyResponse& newAverageSurfaceAttenuation,
											const FrequencyResponse& newDistanceAttenuation )
				:	volume( newVolume ),
					surfaceArea( newSurfaceArea ),
					averageSurfaceAttenuation( newAverageSurfaceAttenuation ),
					distanceAttenuation( newDistanceAttenuation )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Public Data Members
			
			
			
			
			/// The average attenuation frequency response for the space where the sound source is.
			FrequencyResponse averageSurfaceAttenuation;
			
			
			
			
			/// The attenuation amount of the reverb for the sound source relative to the listener.
			FrequencyResponse distanceAttenuation;
			
			
			
			
			/// An estimate of the volume of the space where the sound source is.
			Real volume;
			
			
			
			
			/// An estimate of the total surface area of the space that the sound source is in.
			Real surfaceArea;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_SOURCE_REVERB_RESPONSE_H
