/*
 * Project:     GSound
 * 
 * File:        gsound/FrequencyResponse.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::FrequencyResponse class declaration
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


#ifndef INCLUDE_GSOUND_FREQUENCY_RESPONSE_H
#define INCLUDE_GSOUND_FREQUENCY_RESPONSE_H


#include "GSoundBase.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which represents a frequency response over the range of human hearing.
/**
  * The frequency response is partitioned into a fixed number of frequency bands
  * at predetermined frequencies. This implementation uses 8 frequencies to represent
  * a frequency response: 67.5Hz, 125Hz, 250Hz, 500Hz, 1000Hz, 2000Hz, 4000Hz, and
  * 8000Hz. Each frequency has a linear gain coefficient that determines how sound
  * is affected by the frequency response.
  */
class FrequencyResponse
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a default frequency response with unity gain across all frequency bands.
			GSOUND_INLINE FrequencyResponse()
			{
				for ( Index i = 0; i < numFrequencyBands; i++ )
					response[i] = Real(1);
			}
			
			
			
			
			/// Create a frequency response with the specified gain across all frequency bands.
			/**
			  * @param initialGain - the initial gain value used for all frequency bands.
			  */
			GSOUND_INLINE FrequencyResponse( Real initialGain )
			{
				for ( Index i = 0; i < numFrequencyBands; i++ )
					response[i] = initialGain;
			}
			
			
			
			
			/// Create a frequency response with the specified gain at each of the 8 frequency bands.
			GSOUND_INLINE FrequencyResponse( Real gain67, Real gain125, Real gain250, Real gain500,
											Real gain1000, Real gain2000, Real gain4000, Real gain8000 )
			{
				response[0] = gain67;
				response[1] = gain125;
				response[2] = gain250;
				response[3] = gain500;
				response[4] = gain1000;
				response[5] = gain2000;
				response[6] = gain4000;
				response[7] = gain8000;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Frequency Band Accessor Methods
			
			
			
			
			/// Get the response at the specified frequency band index.
			/**
			  * If the specified band index is greater than or equal to the number
			  * of frequency bands in the FrequencyResponse, an assertion is raised.
			  * Otherwise, this operator returns the gain coefficient of the frequency band
			  * at the specified index.
			  * 
			  * @param bandIndex - the index of the frequency band gain coefficient to query.
			  * @return a reference to the gain at the specified frequency band.
			  */
			GSOUND_INLINE Real& operator [] ( Index bandIndex )
			{
				GSOUND_DEBUG_ASSERT( bandIndex < numFrequencyBands );
				
				return response[bandIndex];
			}
			
			
			
			
			/// Get the response at the specified frequency band index.
			/**
			  * If the specified band index is greater than or equal to the number
			  * of frequency bands in the FrequencyResponse, an assertion is raised.
			  * Otherwise, this operator returns the gain coefficient of the frequency band
			  * at the specified index.
			  * 
			  * @param bandIndex - the index of the frequency band gain coefficient to query.
			  * @return the gain at the specified frequency band.
			  */
			GSOUND_INLINE Real operator [] ( Index bandIndex ) const
			{
				GSOUND_DEBUG_ASSERT( bandIndex < numFrequencyBands );
				
				return response[bandIndex];
			}
			
			
			
			
			/// Get the center frequency of the specified frequency band index.
			/**
			  * If the specified band index is greater than or equal to the number
			  * of frequency bands in the FrequencyResponse, an assertion is raised.
			  * Otherwise, this operator returns the center frequency of the frequency band
			  * at the specified index.
			  * 
			  * @param bandIndex - the index of the frequency band centerto query.
			  * @return the center frequency of the specified frequency band.
			  */
			GSOUND_INLINE Real getBandCenterFrequency( Index bandIndex ) const
			{
				GSOUND_DEBUG_ASSERT( bandIndex < numFrequencyBands );
				
				return bandCenterFrequencies[bandIndex];
			}
			
			
			
			
			/// Get the number of bands in this frequency response.
			/**
			  * The value returned by this method fixed at compilation time
			  * in order to allow simple multiplication of FrequencyResponse objects
			  * and to reduce the number of allocations performed.
			  * 
			  * @return the number of band in this FrequencyResponse.
			  */
			GSOUND_INLINE Size getNumberOfBands() const
			{
				return numFrequencyBands;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Frequency Band Average Gain Method
			
			
			
			
			/// Get the average gain over the specified frequency range.
			/**
			  * This method is useful when mapping a higher-resolution frequency response
			  * down to a lower-resolution frequency response, especially when doing DSP
			  * operations on audio. The requested band boundaries are clamped to the 
			  * minimum and maximum frequencies in this frequency response and will be swaped
			  * if they are not specified in ascending order.
			  *
			  * The method returns the average gain over the specified frequency band
			  * range.
			  * 
			  * @param lowFrequency - the low frequency of the range to query for average gain.
			  * @param highFrequency - the high frequency of the range to query for average gain.
			  * @return the average gain over the specified frequency band.
			  */
			Real getBandAverageGain( Real lowFrequency, Real highFrequency ) const;
			
			
			
			
			/// Get the average gain over the specified frequency range.
			/**
			  * This method is useful when mapping a higher-resolution frequency response
			  * down to a lower-resolution frequency response, especially when doing DSP
			  * operations on audio. The requested band boundaries are clamped to the 
			  * minimum and maximum frequencies in this frequency response and will be swaped
			  * if they are not specified in ascending order.
			  *
			  * The method returns the average gain over the specified frequency band
			  * range.
			  * 
			  * @param bandRange - the range of frequencies over which to find the average gain.
			  * @return the average gain over the specified frequency band.
			  */
			GSOUND_INLINE Real getBandAverageGain( const AABB1& bandRange ) const
			{
				return this->getBandAverageGain( bandRange.min, bandRange.max );
			}
			
			
			
			
			/// Get the average gain of this FrequencyResponse over its entire range.
			/**
			  * This method computes and returns the average of the gain coefficients of all
			  * frequency bands in this FrequencyResponse.
			  * 
			  * @return the average gain of all frequency bands in this FrequencyResponse.
			  */
			GSOUND_INLINE Real getAverageGain() const
			{
				Real sum = 0;
				
				for ( Index i = 0; i < numFrequencyBands; i++ )
					sum += response[i];
				
				return sum / Real(numFrequencyBands);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Frequency Response Addition Operators
			
			
			
			
			/// Add this FrequencyResponse to another and return the result.
			/**
			  * The gains for each band in the frequency responses are added together.
			  * 
			  * @param other - the FrequencyResponse to add to this FrequencyResponse object.
			  * @return the result of the addition operation.
			  */
			GSOUND_INLINE FrequencyResponse operator + ( const FrequencyResponse& other ) const
			{
				FrequencyResponse fr = *this;
				
				return fr += other;
			}
			
			
			
			
			/// Add a FrequencyResponse to this one and modify this response.
			/**
			  * The gains for each band in the frequency responses are added together.
			  * 
			  * @param other - the FrequencyResponse to add to this FrequencyResponse object.
			  * @return a reference to this FrequencyReponse in order to allow operator chaining.
			  */
			GSOUND_INLINE FrequencyResponse& operator += ( const FrequencyResponse& other )
			{
				for ( Index i = 0; i < numFrequencyBands; i++ )
					response[i] += other.response[i];
				
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Frequency Response Multiplication Operators
			
			
			
			
			/// Multiply this frequency response by another and return the result.
			/**
			  * The gains for each band in the frequency responses are multiplied together
			  * with the resulting frequency response representing the response if the two
			  * responses were applied in series to incoming audio.
			  * 
			  * @param other - the FrequencyResponse to multiply with this FrequencyResponse object.
			  * @return the result of the multiplication operation.
			  */
			GSOUND_INLINE FrequencyResponse operator * ( const FrequencyResponse& other ) const
			{
				FrequencyResponse fr = *this;
				
				return fr *= other;
			}
			
			
			
			
			/// Multiply this frequency response by another and modify this response.
			/**
			  * The gains for each band in the frequency responses are multiplied together
			  * with the resulting frequency response representing the response if the two
			  * responses were applied in series to incoming audio.
			  * 
			  * @param other - the FrequencyResponse to multiply with this FrequencyResponse object.
			  * @return a reference to this FrequencyReponse in order to allow operator chaining.
			  */
			GSOUND_INLINE FrequencyResponse& operator *= ( const FrequencyResponse& other )
			{
				for ( Index i = 0; i < numFrequencyBands; i++ )
					response[i] *= other.response[i];
				
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Frequency Response/Scalar Multiplication Operators
			
			
			
			
			/// Multiply this frequency response by a frequency-independent gain factor and return the result.
			/**
			  * The gain for each band in this frequency response is multiplied by the specified
			  * constant gain factor.
			  * 
			  * @param other - the gain to multiply with this FrequencyResponse object.
			  * @return the result of the multiplication operation.
			  */
			GSOUND_INLINE FrequencyResponse operator * ( const Real& gain ) const
			{
				FrequencyResponse fr = *this;
				
				return fr *= gain;
			}
			
			
			
			
			/// Multiply this frequency response by a frequency-independent gain factor and modify this response.
			/**
			  * The gain for each band in this frequency response is multiplied by the specified
			  * constant gain factor.
			  * 
			  * @param other - the gain to multiply with this FrequencyResponse object.
			  * @return a reference to this FrequencyReponse in order to allow operator chaining.
			  */
			GSOUND_INLINE FrequencyResponse& operator *= ( const Real& gain )
			{
				for ( Index i = 0; i < numFrequencyBands; i++ )
					response[i] *= gain;
				
				return *this;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Static High Frequency Rolloff Accessor Methods
			
			
			
			
			/// Get a frequency response which has a linear high frequency rolloff above the specified frequency.
			static FrequencyResponse getLinearHighRolloff( Real startFrequency );
			
			
			
			
			/// Get a frequency response which has a linear high frequency rolloff across the whole spectrum.
			GSOUND_INLINE static FrequencyResponse getLinearHighRolloff()
			{
				return getLinearHighRolloff( bandCenterFrequencies[0] );
			}
			
			
			
			
			/// Get a frequency response which has a quadratic high frequency rolloff above the specified frequency.
			GSOUND_INLINE static FrequencyResponse getQuadraticHighRolloff( Real startFrequency )
			{
				FrequencyResponse result = getLinearHighRolloff( startFrequency );
				
				return result*result;
			}
			
			
			
			
			/// Get a frequency response which has a quadratic high frequency rolloff across the whole spectrum.
			GSOUND_INLINE static FrequencyResponse getQuadraticHighRolloff()
			{
				return getQuadraticHighRolloff( bandCenterFrequencies[0] );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Static Low Frequency Rolloff Accessor Methods
			
			
			
			
			/// Get a frequency response which has a linear low frequency rolloff below the specified frequency.
			static FrequencyResponse getLinearLowRolloff( Real startFrequency );
			
			
			
			
			/// Get a frequency response which has a linear low frequency rolloff across the whole spectrum.
			GSOUND_INLINE static FrequencyResponse getLinearLowRolloff()
			{
				return getLinearLowRolloff( bandCenterFrequencies[numFrequencyBands - 1] );
			}
			
			
			
			
			/// Get a frequency response which has a quadratic low frequency rolloff below the specified frequency.
			GSOUND_INLINE static FrequencyResponse getQuadraticLowRolloff( Real startFrequency )
			{
				FrequencyResponse result = getLinearLowRolloff( startFrequency );
				
				return result*result;
			}
			
			
			
			
			/// Get a frequency response which has a quadratic low frequency rolloff across the whole spectrum.
			GSOUND_INLINE static FrequencyResponse getQuadraticLowRolloff()
			{
				return getQuadraticLowRolloff( bandCenterFrequencies[numFrequencyBands - 1] );
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Static Data Members
			
			
			
			
			/// The number of frequency bands in this frequency response.
			static const Size numFrequencyBands = 8;
			
			
			
			
			/// The center frequencies for each band in this frequency response.
			static const Real bandCenterFrequencies[numFrequencyBands];
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The gain response across the range of this frequency response.
			Real response[numFrequencyBands];
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_FREQUENCY_RESPONSE_H
