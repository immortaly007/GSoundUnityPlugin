/*
 * Project:     GSound
 * 
 * File:        gsound/Mutex.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::util::Mutex class implementation
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


#include "Mutex.h"


#include "Allocator.h"


// Include platform-specific header files
#if defined(GSOUND_PLATFORM_APPLE)
	#include <pthread.h>
#elif defined(GSOUND_PLATFORM_WINDOWS)
	#include <Windows.h>
#endif


//##########################################################################################
//*************************  Start GSound Utilities Namespace  *****************************
GSOUND_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Mac OS X Mutex Wrapper Class
//############		
//##########################################################################################
//##########################################################################################




class Mutex:: MutexWrapper
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			GSOUND_INLINE MutexWrapper()
			{
#if defined(GSOUND_PLATFORM_APPLE)
				
				// Create a new Mutex object.
				int result = pthread_mutex_init( &mutex, NULL );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE( result == 0, "An error was encountered while creating a Mutex object." );
				
#elif defined(GSOUND_PLATFORM_WINDOWS)
				
				// Create a new Mutex object.
				mutex = CreateMutex( NULL, FALSE, NULL );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE( mutex != NULL, "An error was encountered while creating a Mutex object." );
				
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			GSOUND_INLINE ~MutexWrapper()
			{
#if defined(GSOUND_PLATFORM_APPLE)
				
				// acquire the mutex before destroying it.
				int result = pthread_mutex_lock( &mutex );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE( result == 0, "An error was encountered while locking a Mutex object." );
				
				// release the mutex
				result = pthread_mutex_unlock( &mutex );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE( result == 0, "An error was encountered while unlocking a Mutex object." );
				
				// now we can destroy it safely
				result = pthread_mutex_destroy( &mutex );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE( result == 0, "An error was encountered while destroying a Mutex object." );
				
#elif defined(GSOUND_PLATFORM_WINDOWS)
				
				// Destroy a mutex
				BOOL success = CloseHandle( mutex );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE( success != 0, "An error was encountered while destroying a Mutex object." );
				
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mutex Availability Test Method
			
			
			
			
			GSOUND_INLINE Bool isAvailable() const
			{
#if defined(GSOUND_PLATFORM_APPLE)
				
				// Test the mutex to see if it is available at this moment.
				int result = pthread_mutex_trylock( &mutex );
				
				if ( result == -1 )
					return false;
				else
				{
					// We acquired the mutex, so release it again to make it available.
					int result = pthread_mutex_unlock( &mutex );
					
					// Make sure that an error has not been encountered.
					GSOUND_ASSERT_MESSAGE( result == 0, "An error was encountered while unlocking a Mutex object." );
					
					return true;
				}
				
#elif defined(GSOUND_PLATFORM_WINDOWS)
				
				// Try to acquire the mutex with an instant time-out if it is already acquired.
				DWORD result = WaitForSingleObject( mutex, 0 );
				
				// If the wait operation timed out, the mutex is not available.
				if ( result == WAIT_TIMEOUT )
					return false;
				else
				{
					// We acquired the mutex, so release it again to make it available.
					BOOL success = ReleaseMutex( mutex );
					
					// Make sure that an error has not been encountered.
					GSOUND_ASSERT_MESSAGE( success != 0, "An error was encountered while unlocking a Mutex object." );
					
					return true;
				}
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mutex Acquisition Method
			
			
			
			
			GSOUND_INLINE void acquire()
			{
#if defined(GSOUND_PLATFORM_APPLE)
				
				int result = pthread_mutex_lock( &mutex );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE( result == 0, "An error was encountered while locking a Mutex object." );
				
#elif defined(GSOUND_PLATFORM_WINDOWS)
				
				DWORD result = WaitForSingleObject( mutex, INFINITE );
				
				GSOUND_ASSERT_MESSAGE( result == WAIT_OBJECT_0 || result == WAIT_ABANDONED,
								"An error was encountered while locking a Mutex object." );
				
#endif
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mutex Release Method
			
			
			
			
			GSOUND_INLINE void release()
			{
#if defined(GSOUND_PLATFORM_APPLE)
				
				// Unlock the mutex.
				int result = pthread_mutex_unlock( &mutex );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE( result == 0, "An error was encountered while unlocking a Mutex object." );
				
#elif defined(GSOUND_PLATFORM_WINDOWS)
				
				// Release the mutex.
				BOOL success = ReleaseMutex( mutex );
				
				// Make sure that an error has not been encountered.
				GSOUND_ASSERT_MESSAGE( success != 0, "An error was encountered while unlocking a Mutex object." );
#endif
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
#if defined(GSOUND_PLATFORM_APPLE)
			
			/// A handle to a pthread mutex object.
			mutable pthread_mutex_t mutex;
			
#elif defined(GSOUND_PLATFORM_WINDOWS)
			
			/// A handle to a windows mutex object.
			mutable HANDLE mutex;
			
#endif
			
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Platform-Independent Code
//############		
//##########################################################################################
//##########################################################################################




Mutex:: Mutex()
	:	wrapper( util::construct<MutexWrapper>() )
{
}




Mutex:: Mutex( const Mutex& other )
	:	wrapper( util::construct<MutexWrapper>() )
{
	// Acquire the mutex if the other mutex is not available.
	if ( !other.isAvailable() )
		this->acquire();
}



Mutex:: ~Mutex()
{
	// Destoy the wrapper object.
	util::destruct( wrapper );
}



Mutex& Mutex:: operator = ( const Mutex& other )
{
	if ( this != &other )
	{
		// Release and destroy the previous mutex.
		wrapper->release();
		util::destruct( wrapper );
		
		// Create a new mutex wrapper object.
		wrapper = util::construct<MutexWrapper>();
		
		// Acquire the mutex if the other mutex is not available.
		if ( !other.isAvailable() )
			this->acquire();
	}
	
	return *this;
}




void Mutex:: acquire()
{
	wrapper->acquire();
}




void Mutex:: release()
{
	wrapper->release();
}




Bool Mutex:: isAvailable() const
{
	return wrapper->isAvailable();
}




//##########################################################################################
//*************************  End GSound Utilities Namespace  *******************************
GSOUND_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
