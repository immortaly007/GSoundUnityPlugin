/*
 * Project:     GSound
 * 
 * File:        gsound/dsp/Crossover.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::dsp::Crossover class declaration
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


#ifndef INCLUDE_GSOUND_CROSSOVER_H
#define INCLUDE_GSOUND_CROSSOVER_H


#include "GSoundDSPConfig.h"


#include "SoundProcessor.h"


//##########################################################################################
//****************************  Start GSound DSP Namespace  ********************************
GSOUND_DSP_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which is used to filter input audio data into multiple output frequency bands.
/**
  * The Crossover class uses a series of Linkwitz-Riley 4th order crossover filters to
  * split a stream of input audio into an arbitrary number of frequency bands whose
  * corner frequencies can be between 0Hz and the Nyquist Frequency for the current operating
  * sample rate.
  */
class Crossover : public SoundProcessor
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a default Crossover object with no crossover frequencies and no input source.
			Crossover();
			
			
			
			
			/// Create a Crossover object with the specified input source but no crossover frequencies.
			/**
			  * @param newInput - the object to get input audio from. May be NULL.
			  */
			Crossover( SoundOutput* newInput );
			
			
			
			
			/// Create a Crossover object with the specified input source and crossover frequencies.
			/**
			  * @param newInput - the object to get input audio from. May be NULL.
			  * @param newCrossoverFrequencies - the crossover frequencies to use in this Crossover.
			  */
			Crossover( SoundOutput* newInput, const ArrayList<Float>& newCrossoverFrequencies );
			
			
			
			
			/// Create an exact copy of the specified Crossover object.
			Crossover( const Crossover& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy a Crossover object and release all internal state.
			~Crossover();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign the state of one Crossover object to this object.
			Crossover& operator = ( const Crossover& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Crossover Frequency Accessor Methods
			
			
			
			
			/// Get the total number of output frequency bands that this Crossover produces.
			/**
			  * @param the number of frequency bands that this Crossover produces.
			  */
			GSOUND_INLINE Size getNumberOfFrequencyBands() const
			{
				return filters.getSize() + 1;
			}
			
			
			
			
			/// Get the total number of crossover frequencies that this Crossover has.
			/**
			  * @return the number of crossover frequencies that this Crossover has.
			  */
			GSOUND_INLINE Size getNumberOfCrossoverFrequencies() const
			{
				return filters.getSize();
			}
			
			
			
			
			/// Get the crossover frequency at the specified index.
			/**
			  * If the specified crossover frequency index is within the valid range of
			  * crossover frequencies, the crossover frequency at that index is returned.
			  * Otherwise, an assertion is raised.
			  * 
			  * @param crossoverFrequencyIndex - the index of the crossover frequency to query.
			  */
			Float getCrossoverFrequency( Index crossoverFrequencyIndex ) const;
			
			
			
			
			/// Replace all crossover frequencies of this Crossover with the specified list of frequencies.
			/**
			  * This method clears all existing crossover frequencies from this Crossover and adds
			  * all of the frequencies from the input list, excluding those which are not within
			  * the valid range of 0 to the Nyquist Frequency for the current sample rate.
			  * 
			  * @param newCrossoverFrequencies - the new crossover frequencies to use in this Crossover.
			  */
			void setCrossoverFrequencies( const ArrayList<Float>& newCrossoverFrequencies );
			
			
			
			
			/// Add the specified crossover frequency to this Crossover.
			/**
			  * If the specified frequency is not within the valid range of 0 to the
			  * Nyquist Frequency, this method has no effect and FALSE is returned. Otherwise,
			  * the frequency is added to the Crossover and TRUE is returned.
			  * 
			  * @param newCrossoverFrequency - the crossover frequency to add to this Crossover.
			  * @return whether or not the specified crossover frequency was added successfully.
			  */
			Bool addCrossoverFrequency( Float newCrossoverFrequency );
			
			
			
			
			/// Remove the crossover frequency at the specified index in this Crossover.
			/**
			  * If the specified crossover frequency index is not within the valid range of 
			  * crossover frequency indices, an assertion is raised.
			  * 
			  * @param crossoverFrequencyIndex - the index of the crossover frequency to remove.
			  */
			void removeCrossoverFrequency( Index crossoverFrequencyIndex );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Input Accessor Methods
			
			
			
			
			/// Get a pointer to the object which is providing input audio to this Crossover.
			/**
			  * If there is no input object, NULL is returned.
			  * 
			  * @return a pointer to the object which is providing input audio to this Crossover.
			  */
			virtual SoundOutput* getInput() const;
			
			
			
			
			/// Set a pointer to the object which is providing input audio to this Crossover.
			/**
			  * NULL is a legal value for the input object pointer, effectively removing the
			  * input from this Crossover.
			  * 
			  * @param newInput - a pointer to the object which should provide input audio to this Crossover.
			  */
			virtual void setInput( SoundOutput* newInput );
			
			
			
			
			/// Remove the current audio input object from this Crossover.
			virtual void removeInput();
			
			
			
			
			/// Return whether or not this Crossover has an object which is providing input audio.
			/**
			  * If an input exists, TRUE is returned. Otherwise, FALSE is returned.
			  * 
			  * @param whether or not this Crossover has an input object.
			  */
			virtual Bool hasInput() const;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Output Format Accessor Methods
			
			
			
			
			/// Get the current sample rate of the audio which this Crossover is producing.
			virtual Float getSampleRate() const;
			
			
			
			
			/// Get the number of channels of each stream of audio which this Crossover is producing.
			virtual Size getNumberOfChannels() const;
			
			
			
			
			/// Get the number of outputs of that this Crossover has. This is equal to the number of frequency bands.
			virtual Size getNumberOfOutputs() const;
			
			
			
			
			/// Get whether or not the Crossover has any output audio yet to be produced.
			virtual Bool hasOutputRemaining() const;
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Crossover Filter Class Declaration
			
			
			
			
			/// A class which encapsulates a 2nd order filter.
			class SecondOrderFilter;
			
			
			
			
			/// A class which encapsulates a single crossover filter.
			class CrossoverFilter;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Helper Methods
			
			
			
			
			/// Initialize the crossover filters of this Crossover with the specified crossover frequencies.
			void initializeCrossoverFilters( const ArrayList<Float>& newCrossoverFrequencies );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Crossover Filter Rendering Method
			
			
			
			
			/// Render the output audio of this Crossover and place it in the output stream object.
			virtual Size fillBuffer( SoundStream& stream, Index startIndex, Size numSamples );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A pointer to the object which is producing input audio for this Crossover.
			SoundOutput* input;
			
			
			
			
			/// A list of the filters that split the audio into frequency bands.
			ArrayList<CrossoverFilter> filters;
			
			
			
			
			/// An input stream buffer used to hold intermediate audio data.
			SoundStream inputStream;
			
			
			
			
			/// The current sample rate of the Crossover's output audio.
			Float sampleRate;
			
			
			
			
			/// A mutex which provides thread synchronization of audio rendering with parameter manipulation.
			mutable Mutex renderMutex;
			
			
			
};




//##########################################################################################
//****************************  End GSound DSP Namespace  **********************************
GSOUND_DSP_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_CROSSOVER_H
