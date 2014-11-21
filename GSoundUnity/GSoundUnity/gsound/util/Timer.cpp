/*
 * Project:     GSound
 * 
 * File:        gsound/Timer.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::util::Timer class implementation
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


#include "Timer.h"


#if defined(GSOUND_PLATFORM_APPLE)
	#include <sys/time.h>
	
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
//############		Platform Specific Static Data
//############		
//##########################################################################################
//##########################################################################################




#if defined(GSOUND_PLATFORM_WINDOWS)
	
	/// The period of the window's timer.
	static Double windowsTimerPeriod = Double(0);
	static Bool hasQueriedTimerPeriod = false;
	
#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Time Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Double Timer:: getTime()
{
#if defined(GSOUND_PLATFORM_APPLE)
	
	// A struct representing the current state of the system timer on Mac OS X.
	timeval timeData;
	
	int error = gettimeofday( &timeData, NULL );
	
	// Make sure that there was not an error encountered in querying the current time.
	GSOUND_ASSERT_MESSAGE( error == 0, "An error was encountered while querying the current system time." );
	
	// Compute the current time.
	return Double(timeData.tv_sec) + Double(timeData.tv_usec)*Double(1.0e-6);
	
#elif defined(GSOUND_PLATFORM_WINDOWS)
	
	// Query the performance counter frequency if it has not already been queried.
	if ( !hasQueriedTimerPeriod )
	{
		unsigned __int64 performanceCounterFrequency;
		
		BOOL success = QueryPerformanceFrequency( (LARGE_INTEGER*)&performanceCounterFrequency );
		
		// Make sure that there was not an error encountered in querying the performance counter's frequency.
		GSOUND_ASSERT_MESSAGE( success == TRUE, "An error was encountered while querying the current performance counter frequency." );
		
		windowsTimerPeriod = Double(1) / Double(performanceCounterFrequency);
		hasQueriedTimerPeriod = true;
	}
	
	unsigned __int64 time;
	
	BOOL success = QueryPerformanceCounter( (LARGE_INTEGER*)&time );
	
	// Make sure that there was not an error encountered in querying the current time.
	GSOUND_ASSERT_MESSAGE( success == TRUE, "An error was encountered while querying the current system time." );
	
	return ((double)time)*windowsTimerPeriod;
	
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		Timer Is Paused Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void Timer:: setIsPaused( Bool newIsPaused )
{
	if ( newIsPaused )
	{
		// is the timer already paused? if so, then return.
		if ( isPaused )
			return;
		
		// do some clever timer data manipulation to avoid
		// having another data member for the time when paused.
		oldTime = currentTime;
		currentTime = getTime();
		isPaused = newIsPaused;
	}
	else
	{
		
		// is the timer already active? if so, then return.
		if ( !isPaused )
			return;
		
		// do some clever timer data manipulation to avoid
		// having another data member for the time when paused.
		currentTime = getTime() - (currentTime - oldTime);
		isPaused = newIsPaused;
	}
}




//##########################################################################################
//*************************  End GSound Utilities Namespace  *******************************
GSOUND_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
