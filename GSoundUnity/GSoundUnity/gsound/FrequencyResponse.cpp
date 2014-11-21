/*
 * Project:     GSound
 * 
 * File:        gsound/FrequencyResponse.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::FrequencyResponse class implementation
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


#include "FrequencyResponse.h"

 
//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




const Real FrequencyResponse:: bandCenterFrequencies[numFrequencyBands] =
{
	Real(67.5),
	Real(125),
	Real(250),
	Real(500),
	Real(1000),
	Real(2000),
	Real(4000),
	Real(8000),
};




Real FrequencyResponse:: getBandAverageGain( Real lowFrequency, Real highFrequency ) const
{
	lowFrequency = math::clamp( lowFrequency, bandCenterFrequencies[0],
								bandCenterFrequencies[numFrequencyBands - 1] );
	
	highFrequency = math::clamp( highFrequency, bandCenterFrequencies[0],
								bandCenterFrequencies[numFrequencyBands - 1] );
	
	// Swap the frequencies if they are out of order.
	if ( highFrequency < lowFrequency )
	{
		Real temp = highFrequency;
		highFrequency = lowFrequency;
		lowFrequency = temp;
	}
	
	Index i = 0;
	Index lowBand;
	Index highBand;
	
	for ( ; bandCenterFrequencies[i] < lowFrequency; i++ );
	
	lowBand = i;
	
	for ( ; bandCenterFrequencies[i] < highFrequency; i++ );
	
	highBand = i;
	
	Real average = Real(0);
	
	for ( Index i = lowBand; i <= highBand; i++ )
		average += response[i];
	
	return average / Real(highBand - lowBand + 1);
}




FrequencyResponse FrequencyResponse:: getLinearHighRolloff( Real startFrequency )
{
	// Clamp the starting frequency to the minimum and maximum allowed frequencies.
	startFrequency = math::clamp( startFrequency,
								bandCenterFrequencies[0],
								bandCenterFrequencies[numFrequencyBands - 1] );
	
	// Find the starting frequency band index of the rolloff.
	Index startBandIndex;
	
	for ( startBandIndex = 0;
		startBandIndex < numFrequencyBands && bandCenterFrequencies[startBandIndex] < startFrequency;
		startBandIndex++ );
	
	FrequencyResponse result;
	
	Real startBandIndexFloat = (Real)startBandIndex;
	Real numRolloffBands = Real(numFrequencyBands - startBandIndex) - 1.0f;
	Real inverseNumRolloffBands = Real(1)/numRolloffBands;
	
	// calculate the rolloff for each frequency band.
	for ( Index i = startBandIndex; i < numFrequencyBands; i++ )
		result[i] = (numRolloffBands - ((Real)(i) - startBandIndexFloat))*inverseNumRolloffBands;

	return result;
}




FrequencyResponse FrequencyResponse:: getLinearLowRolloff( Real startFrequency )
{
	// Clamp the starting frequency to the minimum and maximum allowed frequencies.
	startFrequency = math::clamp( startFrequency,
								bandCenterFrequencies[0],
								bandCenterFrequencies[numFrequencyBands - 1] );
	
	// Find the starting frequency band index of the rolloff.
	Index startBandIndex;
	
	for ( startBandIndex = 0;
		startBandIndex < numFrequencyBands && bandCenterFrequencies[startBandIndex] < startFrequency;
		startBandIndex++ );
	
	FrequencyResponse result;
	
	Real startBandIndexFloat = (Real)startBandIndex;
	Real numRolloffBands = Real(startBandIndex);
	Real inverseNumRolloffBands = Real(1)/numRolloffBands;
	Index endIndex = startBandIndex + 1;
	
	/// calculate the rolloff for each frequency band.
	for ( Index i = 0; i < endIndex; i++ )
		result[i] = (numRolloffBands - (startBandIndexFloat - (Real)(i)))*inverseNumRolloffBands;
	
	return result;
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
