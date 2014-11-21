/*
 * Project:     GSound
 * 
 * File:        gsound/SoundDistanceAttenuation.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundDistanceAttenuation class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_DISTANCE_ATTENUATION_H
#define INCLUDE_GSOUND_SOUND_DISTANCE_ATTENUATION_H


#include "GSoundBase.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which specifies how sound intensity decreases with distance from a source.
/**
  * The class uses an inverse 2nd-order polynomial attenuation model, where the 
  * attenuation is calculated using the formula: 1/(C + L*R^1 + Q*R^2), where
  * C is the constant attenuation factor, L is the linear attenuation factor, and
  * Q is the quadratic attenuation factor.
  */
class SoundDistanceAttenuation
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a default distance attenuation object.
			/**
			  * This object is created to have a constant attenuation factor of 1, 
			  * a linear attenuation factor of 0, and a quadratic attenuation factor of 0.
			  */
			GSOUND_INLINE SoundDistanceAttenuation()
				:	constantAttenuation( 1 ),
					linearAttenuation( 0 ),
					quadraticAttenuation( 0 )
			{
			}
			
			
			
			
			/// Create a distance attenuation object with the specified constant, linear, and quadratic attenuation factors.
			GSOUND_INLINE SoundDistanceAttenuation( const Real& newConstantAttenuation, const Real& newLinearAttenuation,
											const Real& newQuadraticAttenuation)
				:	constantAttenuation( newConstantAttenuation ),
					linearAttenuation( newLinearAttenuation ),
					quadraticAttenuation( newQuadraticAttenuation )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constant Attenuation Accessor Methods
			
			
			
			
			/// Get the constant attenuation factor for this distance attenuation object.
			GSOUND_FORCE_INLINE Real getConstantAttenuation() const
			{
				return constantAttenuation;
			}
			
			
			
			
			/// Set the constant attenuation factor for this distance attenuation object.
			GSOUND_INLINE void setConstantAttenuation( Real newConstantAttenuation )
			{
				constantAttenuation = math::max( newConstantAttenuation, Real(0) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Linear Attenuation Accessor Methods
			
			
			
			
			/// Get the linear attenuation factor for this distance attenuation object.
			GSOUND_FORCE_INLINE Real getLinearAttenuation() const
			{
				return linearAttenuation;
			}
			
			
			
			
			/// Set the linear attenuation factor for this distance attenuation object.
			GSOUND_INLINE void setLinearAttenuation( Real newLinearAttenuation )
			{
				linearAttenuation = math::max( newLinearAttenuation, Real(0) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Quadratic Attenuation Accessor Methods
			
			
			
			
			/// Get the quadratic attenuation factor for this distance attenuation object.
			GSOUND_FORCE_INLINE Real getQuadraticAttenuation() const
			{
				return quadraticAttenuation;
			}
			
			
			
			
			/// Set the quadratic attenuation factor for this distance attenuation object.
			GSOUND_INLINE void setQuadraticAttenuation( Real newQuadraticAttenuation )
			{
				quadraticAttenuation = math::max( newQuadraticAttenuation, Real(0) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Distance Attenuation Calculation Methods
			
			
			
			
			/// Calculate and return the attenuation amount at the specified distance.
			GSOUND_INLINE Real getAttenuation( Real distance ) const
			{
				return Real(1) / (constantAttenuation + distance*linearAttenuation + 
									distance*distance*quadraticAttenuation);
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The amount that constant attenuation is applied to the sound source's audio.
			/**
			  * This value will generally range between 0 and 1 (and cannot be negative). It
			  * indicates the 'c' coefficient of the R^1 term in the denominator of the attenuation
			  * equation: attenuation = 1/(c + a*R^1 + q*R^2).
			  */
			Real constantAttenuation;
			
			
			
			
			/// The amount that linear attenuation is applied to the sound source's audio.
			/**
			  * This value will generally range between 0 and 1 (and cannot be negative). It
			  * indicates the 'a' coefficient of the R^1 term in the denominator of the attenuation
			  * equation: attenuation = 1/(c + a*R^1 + q*R^2).
			  */
			Real linearAttenuation;
			
			
			
			
			/// The amount that quadratic attenuation is applied to the sound source's audio.
			/**
			  * This value will generally range between 0 and 1 (and cannot be negative). It
			  * indicates the 'q' coefficient of the R^2 term in the denominator of the attenuation
			  * equation: attenuation = 1/(c + a*R^1 + q*R^2).
			  */
			Real quadraticAttenuation;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_DISTANCE_ATTENUATION_H
