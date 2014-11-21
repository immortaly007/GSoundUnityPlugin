/*
 * Project:     GSound
 * 
 * File:        gsound/FrequencyPartition.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::FrequencyPartition class declaration
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


#ifndef INCLUDE_GSOUND_FREQUENCY_PARTITION_H
#define INCLUDE_GSOUND_FREQUENCY_PARTITION_H


#include "GSoundBase.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which describes a partion of frequency space into a number of frequency bands.
/**
  * This is used describe the frequency bands that are used when rendering sound propagation
  * paths. Each SoundPropagationRenderer has FrequencyPartition object that specifies
  * how incoming audio from all sound sources should be split into distinct frequency
  * bands for rendering. By splitting audio into multiple frequency bands, frequency-
  * dependent effects can be efficiently achieved.
  *
  * This class keeps a sorted list of split frequencies, the boundary frequencies
  * of the frequency bands that the FrequencyPartition describes. Above and below
  * each band's bounding split frequencies, the audio is attenuated using a 
  * filter.
  *
  * As of version 1.0.0, it is recommended that split frequencies are at least 2 octaves
  * apart from each other. If this recommendation is not respected, overlap from 
  * frequency bands when summed together will not be all-pass, resulting in comb
  * filtering artifacts in the rendered audio. While all-pass filters are used
  * internally to split the audio, frequency bands that overlap can produce
  * non-all-pass behavior if the amplitudes of the particular frequency ranges are similar.
  * This is not an issue when split frequencies are >=2 octaves apart because
  * the overlap between frequency bands is on the order of -48dB.
  */
class FrequencyPartition
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a frequency partition with no split frequencies.
			FrequencyPartition();
			
			
			
			
			/// Create a frequency partition using the specified split frequencies.
			/**
			  * The specified list of split frequencies is sanitized by removing all
			  * split frequencies that don't lie in the range (0,+infinity) or are
			  * not a number (NaN). The list is then sorted in increasing order and
			  * the result is used as the split frequencies for this FrequencyPartition.
			  * 
			  * @param newSplitFrequencies - the split frequencies to use in this FrequencyPartition.
			  */
			FrequencyPartition( const ArrayList<Real>& newSplitFrequencies );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Equality/Inequality Comparision Operators
			
			
			
			
			/// Compare this FrequencyPartition to another for equality.
			/**
			  * One FrequencyPartition is equal to another if and only if
			  * the lists of split frequencies are equal.
			  * 
			  * @param other - the FrequencyPartition object to compare with this one.
			  * @return whether or not this FrequencyPartition is equal to the other.
			  */
			GSOUND_INLINE Bool operator == ( const FrequencyPartition& other ) const
			{
				return splitFrequencies == other.splitFrequencies;
			}
			
			
			
			
			/// Compare this FrequencyPartition to another for inequality.
			/**
			  * One FrequencyPartition is not equal to another if and only if
			  * the lists of split frequencies are not equal.
			  * 
			  * @param other - the FrequencyPartition object to compare with this one.
			  * @return whether or not this FrequencyPartition is not equal to the other.
			  */
			GSOUND_INLINE Bool operator != ( const FrequencyPartition& other ) const
			{
				return splitFrequencies != other.splitFrequencies;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Split Frequency Accessor Methods
			
			
			
			
			/// Add a split frequency to this FrequencyPartition.
			/**
			  * If the specified split frequency is not within the range (0,+infinity),
			  * or is not a number (NaN), it is ignored. Otherwise, the specified
			  * frequency is inserted in the internal list of split frequencies
			  * at the location which preserves the sorted order of the split
			  * frequencies.
			  * 
			  * @param newSplitFrequency - the split frequency to be added to this FrequencyPartition.
			  */
			void addSplitFrequency( Real newSplitFrequency );
			
			
			
			
			/// Get the split frequencies in this FrequencyPartition.
			/**
			  * This method returns a const reference to the internal list
			  * of split frequencies of this FrequencyPartition.
			  * 
			  * @return a list of split frequencies of this FrequencyPartition.
			  */
			GSOUND_FORCE_INLINE const ArrayList<Real>& getSplitFrequencies() const
			{
				return splitFrequencies;
			}
			
			
			
			
			/// Get the split frequency at the specified index in this FrequencyPartition.
			/**
			  * If the specified frequency index is greater than or equal to the
			  * number of split frequencies in this FrequencyPartition, an assertion
			  * is raised. Otherwise, the split frequency at that index is returned.
			  * 
			  * @param frequencyIndex - the index of the split frequency to query.
			  * @return the split frequency at the specified index in this FrequencyPartition.
			  */
			GSOUND_INLINE Real getSplitFrequency( Index frequencyIndex ) const
			{
				GSOUND_DEBUG_ASSERT( frequencyIndex < splitFrequencies.getSize() );
				
				return splitFrequencies[frequencyIndex];
			}
			
			
			
			
			/// Remove the split frequency at the specified index in this FrequencyPartition.
			/**
			  * If the specified frequency index is greater than or equal to the
			  * number of split frequencies in this FrequencyPartition, an assertion
			  * is raised. Otherwise, the split frequency at that index is removed.
			  * 
			  * @param frequencyIndex - the index of the split frequency to remove.
			  */
			GSOUND_INLINE void removeSplitFrequency( Index frequencyIndex )
			{
				GSOUND_DEBUG_ASSERT( frequencyIndex < splitFrequencies.getSize() );
				
				splitFrequencies.removeAtIndex(frequencyIndex);
			}
			
			
			
			
			/// Clear all split frequencies from this FrequencyPartition.
			/**
			  * This method removes all split frequencies from this FrequencyPartition,
			  * returning it to its default state.
			  */
			GSOUND_INLINE void clearSplitFrequencies()
			{
				splitFrequencies.clear();
			}
			
			
			
			
			/// Get the number of split frequencies in this FrequencyPartition.
			/**
			  * @return the number of split frequencies in this FrequencyPartition.
			  */
			GSOUND_FORCE_INLINE Size getNumberOfSplitFrequencies() const
			{
				return splitFrequencies.getSize();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Frequency Band Accessor Methods
			
			
			
			
			/// Get the number of frequency bands in this FrequencyPartition.
			/**
			  * This number is equal to one more than the total number of split frequencies
			  * in the FrequencyPartition.
			  * 
			  * @return the number of frequency bands in this FrequencyPartition.
			  */
			GSOUND_INLINE Size getNumberOfFrequencyBands() const
			{
				return splitFrequencies.getSize() + 1;
			}
			
			
			
			
			/// Get the minimum and maximum frequencies of the frequency band at the specified index.
			/**
			  * If the specified frequency band index is greater than or equal to the
			  * number of frequency bands in this FrequencyPartition, an assertion is
			  * raised. Otherwise, a 1D range indicating the minimum and maximum 
			  * frequencies in the specified frequency band is returned.
			  * 
			  * @param frequencyBandIndex - the index of the frequency band to query.
			  * @return a 1D range indicating the minimum and maximum frequencies of the specified band.
			  */
			GSOUND_INLINE AABB1 getFrequencyBandRange( Index frequencyBandIndex ) const
			{
				// Make sure that the frequency band index is inside the valid range.
				GSOUND_DEBUG_ASSERT( frequencyBandIndex < this->getNumberOfFrequencyBands() );
				
				AABB1 result( Real(0), math::infinity<Real>() );
				
				if ( frequencyBandIndex != 0 )
					result.min = splitFrequencies[frequencyBandIndex - 1];
				
				if ( frequencyBandIndex != splitFrequencies.getSize() )
					result.max = splitFrequencies[frequencyBandIndex];
				
				return result;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The list of split frequencies that partition the frequency space.
			ArrayList<Real> splitFrequencies;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_FREQUENCY_PARTITION_H
