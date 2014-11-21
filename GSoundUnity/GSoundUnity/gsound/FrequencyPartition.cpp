/*
 * Project:     GSound
 * 
 * File:        gsound/FrequencyPartition.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::FrequencyPartition class implementation
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


#include "FrequencyPartition.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




FrequencyPartition:: FrequencyPartition()
{
}




FrequencyPartition:: FrequencyPartition( const ArrayList<Real>& newSplitFrequencies )
	:	splitFrequencies( newSplitFrequencies )
{
	// Sanitize the split frequencies.
	for ( Index i = 0; i < splitFrequencies.getSize(); )
	{
		register Real splitFrequency = splitFrequencies[i];
		
		if ( splitFrequency <= Real(0) || splitFrequency == math::infinity<Real>() || math::isNAN(splitFrequency) )
			splitFrequencies.removeAtIndexUnordered(i);
		else
			i++;
	}
	
	// Sort the split frequencies.
	register Size numSplitFrequencies = splitFrequencies.getSize();
	register Size numSplitFrequenciesMinusOne = numSplitFrequencies - 1;
	
	for ( Index i = 1; i < numSplitFrequencies; i++ )
	{
		for ( Index j = 0; j < numSplitFrequenciesMinusOne; j++ )
		{
			register Index jPlusOne = j + 1;
			
			if ( splitFrequencies[jPlusOne] < splitFrequencies[j] )
			{
				register Real temp = splitFrequencies[j];
				splitFrequencies[j] = splitFrequencies[jPlusOne];
				splitFrequencies[jPlusOne] = temp;
			}
		}
	}
}




void FrequencyPartition:: addSplitFrequency( Real newSplitFrequency )
{
	// If the split frequency is not within the valid range, ignore it and return from the function.
	if ( newSplitFrequency <= Real(0) || newSplitFrequency == math::infinity<Real>() || math::isNAN(newSplitFrequency) )
		return;
	
	Index insertIndex = splitFrequencies.getSize();
	
	for ( Index i = 0; i < splitFrequencies.getSize(); i++ )
	{
		if ( newSplitFrequency < splitFrequencies[i] )
		{
			insertIndex = i;
			break;
		}
	}
	
	splitFrequencies.insert( newSplitFrequency, insertIndex );
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
