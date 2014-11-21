/*
 * Project:     GSound
 * 
 * File:        gsound/SoundMaterial.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundMaterial class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_MATERIAL_H
#define INCLUDE_GSOUND_SOUND_MATERIAL_H


#include "GSoundBase.h"


#include "FrequencyResponse.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// A class which specifies how sound interacts with a material via reflection and transmission.
class SoundMaterial
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			/// Create a default material with reflection attenuation = 1, absorption and transmission attenuations = 0.
			GSOUND_INLINE SoundMaterial()
				:	reflectionAttenuation( Real(1) ),
					absorptionAttenuation( Real(0) ),
					transmissionAttenuation( Real(0) )
			{
			}
			
			
			
			
			/// Create a sound material with the specified reflection, absorption, and transmission attenuations.
			GSOUND_INLINE SoundMaterial( const FrequencyResponse& newReflectionAttenuation,
									const FrequencyResponse& newAbsorptionAttenuation,
									const FrequencyResponse& newTransmissionAttenuation )
				:	reflectionAttenuation( newReflectionAttenuation ),
					absorptionAttenuation( newAbsorptionAttenuation ),
					transmissionAttenuation( newTransmissionAttenuation )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Reflection Attenuation Accessor Methods
			
			
			
			/// Get the reflection attenuation of this material.
			GSOUND_INLINE FrequencyResponse& getReflectionAttenuation()
			{
				return reflectionAttenuation;
			}
			
			
			
			
			/// Get the reflection attenuation of this material.
			GSOUND_INLINE const FrequencyResponse& getReflectionAttenuation() const
			{
				return reflectionAttenuation;
			}
			
			
			
			
			/// Set the reflection attenuation of this material.
			GSOUND_INLINE void setReflectionAttenuation( const FrequencyResponse& newReflectionAttenuation )
			{
				reflectionAttenuation = newReflectionAttenuation;
			}
			
			
			
			
			/// Get the reflection attenuation of this material at the specified frequency band index.
			GSOUND_INLINE Real getReflectionAttenuation( Index frequencyBandIndex ) const
			{
				return reflectionAttenuation[frequencyBandIndex];
			}
			
			
			
			
			/// Set the reflection attenuation of this material at the specified frequency band index.
			GSOUND_INLINE void setReflectionAttenuation( Index frequencyBandIndex, Real newAttenuation )
			{
				reflectionAttenuation[frequencyBandIndex] = math::max( newAttenuation, Real(0) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Absorption Attenuation Accessor Methods
			
			
			
			/// Get the absorption attenuation of this material.
			GSOUND_INLINE FrequencyResponse& getAbsorptionAttenuation()
			{
				return absorptionAttenuation;
			}
			
			
			
			
			/// Get the absorption attenuation of this material.
			GSOUND_INLINE const FrequencyResponse& getAbsorptionAttenuation() const
			{
				return absorptionAttenuation;
			}
			
			
			
			
			/// Set the absorption attenuation of this material.
			GSOUND_INLINE void setAbsorptionAttenuation( const FrequencyResponse& newAbsorptionAttenuation )
			{
				absorptionAttenuation = newAbsorptionAttenuation;
			}
			
			
			
			
			/// Get the absorption attenuation of this material at the specified frequency band index.
			GSOUND_INLINE Real getAbsorptionAttenuation( Index frequencyBandIndex ) const
			{
				return absorptionAttenuation[frequencyBandIndex];
			}
			
			
			
			
			/// Set the absorption attenuation of this material at the specified frequency band index.
			GSOUND_INLINE void setAbsorptionAttenuation( Index frequencyBandIndex, Real newAttenuation )
			{
				absorptionAttenuation[frequencyBandIndex] = math::max( newAttenuation, Real(0) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Transmission Attenuation Accessor Methods
			
			
			
			
			/// Get the transmission attenuation of this material.
			GSOUND_INLINE FrequencyResponse& getTransmissionAttenuation()
			{
				return transmissionAttenuation;
			}
			
			
			
			
			/// Get the transmission attenuation of this material.
			GSOUND_INLINE const FrequencyResponse& getTransmissionAttenuation() const
			{
				return transmissionAttenuation;
			}
			
			
			
			
			/// Set the transmission attenuation of this material.
			GSOUND_INLINE void setTransmissionAttenuation( const FrequencyResponse& newTransmissionAttenuation )
			{
				transmissionAttenuation = newTransmissionAttenuation;
			}
			
			
			
			
			/// Get the transmission attenuation for the specified frequency band index.
			GSOUND_INLINE Real getTransmissionAttenuation( Index frequencyBandIndex ) const
			{
				return transmissionAttenuation[frequencyBandIndex];
			}
			
			
			
			
			/// Set the transmission attenuation for the specified frequency band index.
			GSOUND_INLINE void setTransmissionAttenuation( Index frequencyBandIndex, Real newAttenuation )
			{
				transmissionAttenuation[frequencyBandIndex] = math::max( newAttenuation, Real(0) );
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The attenuation coefficients for sound reflected off of a surface with this material.
			/**
			  * When a sound bounces off of a surface, the final sound will be attenuated
			  * by this amount.
			  */
			FrequencyResponse reflectionAttenuation;
			
			
			
			
			/// The attenuation coefficients for the sound that is transmitted across a material boundary.
			/**
			  * When sound hits a surface, the portion that is not reflected is either absorbed
			  * (disappated as heat) or transmitted through the material. Multiplying by these coefficients
			  * gives the sound that is transmitted through the material.
			  */
			FrequencyResponse absorptionAttenuation;
			
			
			
			
			/// The attenuation coefficients for sound transmitted through an object with this material.
			/**
			  * This value should be specified in units of attenuation per unit length.
			  * Therefore, if a sound ray passes through D units of this material, the 
			  * final sound will be attenuated by this response to the power of D.
			  */
			FrequencyResponse transmissionAttenuation;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_MATERIAL_H
