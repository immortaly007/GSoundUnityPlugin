/*
 * Project:     GSound
 * 
 * File:        gsound/math/RandomVariable.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::math::RandomVariable class declaration
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


#ifndef INCLUDE_GSOUND_RANDOM_VARIABLE_H
#define INCLUDE_GSOUND_RANDOM_VARIABLE_H


#include "GSoundMathConfig.h"


#include "../util/Timer.h"


//##########################################################################################
//***************************  Start GSound Math Namespace  ********************************
GSOUND_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################





template < typename T >
class RandomVariable;




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'char' type
//############		
//##########################################################################################
//##########################################################################################




/// Random variable class for the char data type.
template <>
class RandomVariable<char>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (unsigned char)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( unsigned char newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE char sample()
			{
				advanceSeed();
				
				return seed;
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE char sample( char min, char max )
			{
				advanceSeed();
				
				unsigned char uMin = *((unsigned char*)&min) + math::min<char>();
				unsigned char uMax = *((unsigned char*)&max) - math::min<char>();
				
				unsigned char a = (seed % (uMax - uMin + 1)) + *((unsigned char*)&min);
				
				return *((char*)&a);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( unsigned char newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE unsigned char getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = (1664525*seed + 1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			unsigned char seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'unsigned char' type
//############		
//##########################################################################################
//##########################################################################################




/// Random variable class for the unsigned char data type.
template <>
class RandomVariable<unsigned char>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (unsigned char)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( unsigned char newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE unsigned char sample()
			{
				advanceSeed();
				
				return seed;
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE unsigned char sample( unsigned char min, unsigned char max )
			{
				advanceSeed();
				
				return (seed % (max - min + 1)) + min;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( unsigned char newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE unsigned char getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = (1664525*seed + 1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			unsigned char seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'short' type
//############		
//##########################################################################################
//##########################################################################################




/// Random variable class for the short data type.
template <>
class RandomVariable<short>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (unsigned short)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( unsigned short newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE short sample()
			{
				advanceSeed();
				
				return seed;
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE short sample( short min, short max )
			{
				advanceSeed();
				
				unsigned short uMin = *((unsigned short*)&min) + math::min<short>();
				unsigned short uMax = *((unsigned short*)&max) - math::min<short>();
				
				unsigned short a = (seed % (uMax - uMin + 1)) + *((unsigned short*)&min);
				
				return *((short*)&a);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( unsigned short newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE unsigned short getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = (1664525*seed + 1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			unsigned short seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'unsigned short' type
//############		
//##########################################################################################
//##########################################################################################




/// Random variable class for the unsigned short data type.
template <>
class RandomVariable<unsigned short>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (unsigned short)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( unsigned short newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE unsigned short sample()
			{
				advanceSeed();
				
				return seed;
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE unsigned short sample( unsigned short min, unsigned short max )
			{
				advanceSeed();
				
				return (seed % (max - min + 1)) + min;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( unsigned short newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE unsigned short getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = (1664525*seed + 1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			unsigned short seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'int' type
//############		
//##########################################################################################
//##########################################################################################





/// Random variable class for the int data type.
template <>
class RandomVariable<int>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (unsigned int)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( unsigned int newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE int sample()
			{
				advanceSeed();
				
				return seed;
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE int sample( int min, int max )
			{
				advanceSeed();
				
				unsigned int uMin = *((unsigned int*)&min) + math::min<int>();
				unsigned int uMax = *((unsigned int*)&max) - math::min<int>();
				
				unsigned int a = (seed % (uMax - uMin + 1)) + *((unsigned int*)&min);
				
				return *((int*)&a);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( unsigned int newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE unsigned int getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = (1664525*seed + 1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			unsigned int seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'unsigned int' type
//############		
//##########################################################################################
//##########################################################################################




/// Random variable class for the unsigned int data type.
template <>
class RandomVariable<unsigned int>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (unsigned int)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( unsigned int newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE unsigned int sample()
			{
				advanceSeed();
				
				return seed;
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE unsigned int sample( unsigned int min, unsigned int max )
			{
				advanceSeed();
				
				return (seed % (max - min + 1)) + min;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( unsigned int newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE unsigned int getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = (1664525*seed + 1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			unsigned int seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'long' type
//############		
//##########################################################################################
//##########################################################################################




/// Random variable class for the long data type.
template <>
class RandomVariable<long>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (unsigned long)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( unsigned long newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE long sample()
			{
				advanceSeed();
				
				return seed;
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE long sample( long min, long max )
			{
				advanceSeed();
				
				unsigned long uMin = *((unsigned long*)&min) + math::min<long>();
				unsigned long uMax = *((unsigned long*)&max) - math::min<long>();
				
				unsigned long a = (seed % (uMax - uMin + 1)) + *((unsigned long*)&min);
				
				return *((long*)&a);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( unsigned long newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE unsigned long getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = (1664525*seed + 1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			unsigned long seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'unsigned long' type
//############		
//##########################################################################################
//##########################################################################################




/// Random variable class for the unsigned long data type.
template <>
class RandomVariable<unsigned long>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (unsigned long)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( unsigned long newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE unsigned long sample()
			{
				advanceSeed();
				
				return seed;
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE unsigned long sample( unsigned long min, unsigned long max )
			{
				advanceSeed();
				
				return (seed % (max - min + 1)) + min;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( unsigned long newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE unsigned long getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = (1664525*seed + 1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			unsigned long seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'long long' type
//############		
//##########################################################################################
//##########################################################################################




/// Random variable class for the long long data type.
template <>
class RandomVariable<long long>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (unsigned long long)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( unsigned long long newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE long long sample()
			{
				advanceSeed();
				
				return seed;
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE long long sample( long long min, long long max )
			{
				advanceSeed();
				
				unsigned long long uMin = *((unsigned long long*)&min) + math::min<long long>();
				unsigned long long uMax = *((unsigned long long*)&max) - math::min<long long>();
				
				unsigned long long a = (seed % (uMax - uMin + 1)) + *((unsigned long long*)&min);
				
				return *((long long*)&a);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( unsigned long long newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE unsigned long long getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = (1664525*seed + 1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			unsigned long long seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'unsigned long long' type
//############		
//##########################################################################################
//##########################################################################################




/// Random variable class for the unsigned long long data type.
template <>
class RandomVariable<unsigned long long>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (unsigned long long)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( unsigned long long newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE unsigned long long sample()
			{
				advanceSeed();
				
				return seed;
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE unsigned long long sample( unsigned long long min, unsigned long long max )
			{
				advanceSeed();
				
				return (seed % (max - min + 1)) + min;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( unsigned long long newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE unsigned long long getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = (1664525*seed + 1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			unsigned long long seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'float' type
//############		
//##########################################################################################
//##########################################################################################





/// Random variable class for the float data type.
template <>
class RandomVariable<float>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (UInt32)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( UInt32 newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE float sample()
			{
				advanceSeed();
				
				unsigned long a = (seed & 0x007FFFFFul) | 0x3F800000ul;
				
				return (*((float*)&a) - 1.5f)*2.0f*math::max<float>();
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE float sample( float min, float max )
			{
				advanceSeed();
				
				unsigned long a = (seed & 0x007FFFFFul) | 0x3F800000ul;
				
				return (*((float*)&a) - 1.0f)*(max - min) + min;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( UInt32 newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE UInt32 getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = UInt32(1664525)*seed + UInt32(1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			UInt32 seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class for the 'double' type
//############		
//##########################################################################################
//##########################################################################################




/// Random variable class for the double data type.
template <>
class RandomVariable<double>
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a random variable with a default initial seed value.
			GSOUND_FORCE_INLINE RandomVariable()
				:	seed( (UInt64)util::Timer::getTime() )
			{
			}
			
			
			
			
			/// Create a random variable with the specified initial seed value.
			GSOUND_FORCE_INLINE RandomVariable( UInt64 newSeed )
				:	seed( newSeed )
			{
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sample Methods
			
			
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			GSOUND_FORCE_INLINE double sample()
			{
				advanceSeed();
				
				unsigned long long a = (seed & 0x000FFFFFFFFFFFFFull) | 0x3FF0000000000000ull;
				
				return (*((double*)&a) - 1.5)*2.0*math::max<double>();
			}
			
			
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			GSOUND_FORCE_INLINE double sample( double min, double max )
			{
				advanceSeed();
				
				unsigned long long a = (seed & 0x000FFFFFFFFFFFFFull) | 0x3FF0000000000000ull;
				
				return (*((double*)&a) - 1.0)*(max - min) + min;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			
			
			/// Set the seed for the random variable.
			/**
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			GSOUND_FORCE_INLINE void setSeed( UInt64 newSeed )
			{
				seed = newSeed;
			}
			
			
			
			
			/// Get the current state of the random variable.
			GSOUND_FORCE_INLINE UInt64 getSeed() const
			{
				return seed;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Seed Advance Method
			
			
			
			
			/// Advance the random variable's seed to its next value.
			GSOUND_FORCE_INLINE void advanceSeed()
			{
				seed = UInt64(1664525)*seed + UInt64(1013904223);
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Member
			
			
			
			
			/// The current state of the random variable.
			UInt64 seed;
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Global Random Number Generation Method
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
GSOUND_FORCE_INLINE static RandomVariable<T>& getGlobalRandomVariable()
{
	static RandomVariable<T> randomVariable;
	
	return randomVariable;
}




template < typename T >
GSOUND_FORCE_INLINE T random()
{
	return getGlobalRandomVariable<T>().sample();
}




template < typename T >
GSOUND_FORCE_INLINE T random( T min, T max )
{
	return getGlobalRandomVariable<T>().sample( min, max );
}




//##########################################################################################
//***************************  End GSound Math Namespace  **********************************
GSOUND_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_RANDOM_VARIABLE_H
