/*
 * Project:     GSound
 * 
 * File:        gsound/Mutex.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::util::Mutex class declaration
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


#ifndef INCLUDE_GSOUND_MUTEX_H
#define INCLUDE_GSOUND_MUTEX_H


#include "GSoundUtilitiesConfig.h"


//##########################################################################################
//*************************  Start GSound Utilities Namespace  *****************************
GSOUND_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class whose job is to provide a means of thread synchronization by exclusion.
/** 
  * The class is essentially a wrapper around the host platform's mutex facilities.
  * It allows threads to be synchronized so that access to data or other sensitive
  * items can be restricted to one thread at a time.
  *
  *	In order to use the class properly, call the acquire() method to lock the
  * mutex and call the release() method to unlock it. The acquire() method blocks
  * execution of the calling thread until the mutex has been released by another
  * thread. One can also query the state of the mutex (locked or unlocked) using
  * the method isAvailable().
  */
class Mutex
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a new mutex in the default state of not locked.
			Mutex();
			
			
			
			
			/// Create a copy of a Mutex object, copying its internal state.
			/**
			  * Calling this constructor causes a Mutex to be created which always
			  * has the same state as the parameter Mutex. If the other Mutex object
			  * was locked, so will be the created Mutex object.
			  * 
			  * @param other - the Mutex whose state is to be copied.
			  */
			Mutex( const Mutex& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy a Mutex object, releasing all internal state.
			~Mutex();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Assignment Operator
			
			
			
			
			/// Assign one Mutex to another, obtaining a reference to the Mutex's internal state.
			/**
			  * Calling this assignment operator causes a Mutex to be created which always
			  * has the same state as the specified Mutex object. If the other Mutex object
			  * was locked, so will be this Mutex object.
			  * 
			  * @param other - the Mutex whose state is to be copied.
			  * @return a reference to this Mutex object in order to allow assignment chaining.
			  */
			Mutex& operator = ( const Mutex& other );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mutex Acquisition
			
			
			
			
			/// Wait until the Mutex is available for the current thread of execution.
			/**
			  * This method blocks the current thread until the signal is recieved
			  * that the Mutex has been released, at which time the Mutex is acquired
			  * by the current thread and the method returns. If the Mutex is available,
			  * the method returns immediately and the Mutex is acquired.
			  */
			void acquire();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mutex Release
			
			
			
			
			/// Release the mutex so that another thread can acquire it.
			/**
			  * If the mutex is not already acquired, this method
			  * has no effect.
			  */
			void release();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mutex Status Accessor
			
			
			
			
			/// Get whether or not the mutex is available.
			/**
			  * If the mutex is free for acquisition, TRUE is returned. Otherwise
			  * FALSE is returned.
			  * 
			  * @return whether or not the mutex is available for acquisition.
			  */
			Bool isAvailable() const;
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Mutex Wrapper Class Declaration
			
			
			
			
			/// A class which encapsulates internal platform-specific Mutex code.
			class MutexWrapper;
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A pointer to a wrapper object containing the internal state of the Mutex.
			MutexWrapper* wrapper;
			
			
			
			
			
};




//##########################################################################################
//*************************  End GSound Utilities Namespace  *******************************
GSOUND_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_MUTEX_H
