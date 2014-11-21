/*
 * Project:     GSound
 * 
 * File:        gsound/Timer.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::util::Timer class declaration
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


#ifndef INCLUDE_GSOUND_TIMER_H
#define INCLUDE_GSOUND_TIMER_H


#include "GSoundUtilitiesConfig.h"


//##########################################################################################
//*************************  Start GSound Utilities Namespace  *****************************
GSOUND_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A timer class which behaves like a stopwatch.
/**
  * This class serves to provide a way for the user to mark
  * times and to measure the intervals between them. This can
  * be used to do application profiling/timing, to provide a 
  * frame time interval counter for games, etc. It uses the highest
  * performance timers availible on the system it is compiled under,
  * and therefore should have sub-millisecond accuracy. The timer
  * is also able to be paused, such that it then behaves as if it
  * was stuck in that instance in which it was paused. It can later
  * be resumed and carry on as if it had never been paused, reflecting
  * this in it's attributes accordingly.
  */
class Timer
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			/// Create a new timer and start it's first interval (by calling update()).
			/** 
			  * This constructor creates a new timer, and then
			  * initializes the timer by starting it's first time interval,
			  * done by calling the timer's update() function internally.
			  */
			GSOUND_INLINE Timer()
				:	oldTime( Double(0) ),
					lastInterval( Double(0) ),
					isPaused( false )
			{
				currentTime = this->getTime();
			}
			
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Time Accessor Method
			
			
			
			
			/// Get the current system time in seconds relative to an arbitrary base time.
			static Double getTime();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Stopwatch Timing Methods
			
			
			
			
			/// Update the timer to the current time, store the interval between updates.
			/**
			  * This method updates the timer, and calculates and
			  * stores the time between the last update and this update
			  * internally, so that it can be accessed using the
			  * getElapsedTime() method. If the timer is paused
			  * when this function is called, then the method does nothing.
			  */
			GSOUND_INLINE void update()
			{
				if ( isPaused )
					return;
				
				oldTime = currentTime;
				currentTime = this->getTime();
				
				lastInterval = currentTime - oldTime;
			}
			
			
			
			
			/// Get the time interval of the last update
			/**
			  * This method retrieves the time interval (in seconds)
			  * of the elapsed time between the last call to the update()
			  * method and the second to last call to the update() method.
			  * 
			  * @return the time interval between the last two timer updates
			  */
			GSOUND_INLINE Double getLastInterval() const
			{
				return lastInterval;
			}
			
			
			
			
			/// Get the time interval since the last update without updating the timer.
			/**
			  * This method gets the time passed since the last call to the update()
			  * method in seconds. The method does not reset the timer, and therefore
			  * can be used to get a running total of the time since some arbitrary
			  * moment (set by calling the update method). If the timer is paused, 
			  * this method returns the time between the last call to update() and
			  * the time when the timer was paused.
			  * 
			  * @return the time since the last call to update()
			  */
			GSOUND_INLINE Double getElapsedTime() const
			{
				if ( isPaused )
					return currentTime - oldTime;
				else
					return this->getTime() - currentTime;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Timer State Methods
			
			
			
			
			/// Get whether or not the timer is currently paused.
			/**
			  * If the timer is paused, then this means that the timer
			  * is no longer keeping track of time, and will behave as if
			  * it is stuck in the instant that it was paused in until it
			  * is unpaused. This can be useful in many situations where
			  * one needs to stop timing and later resume timing as if
			  * the timer has never been stopped.
			  * 
			  * @return whether or not the timer is currently paused.
			  */
			GSOUND_INLINE Bool getIsPaused() const
			{
				return isPaused;
			}
			
			
			
			
			/// Set whether or not the timer is paused with a boolean flag.
			/**
			  * If the timer is paused, then this means that the timer
			  * is no longer keeping track of time, and will behave as if
			  * it is stuck in the instant that it was paused in until it
			  * is unpaused. This can be useful in many situations where
			  * one needs to stop timing and later resume timing as if
			  * the timer has never been stopped. If this method is called
			  * with a parameter that is equal to the return value of
			  * getIsPaused(), then it does nothing (the timer's state doesn't
			  * need to change).
			  * 
			  * @param newIsPaused - whether or not the timer should be paused.
			  */
			void setIsPaused( Bool newIsPaused );
			
			
			
			
			/// Pause the timer.
			/**
			  * If the timer is already paused, then this method does
			  * nothing (the timer's state doesn't need to change to
			  * reflect the function call).
			  */
			GSOUND_INLINE void pause()
			{
				this->setIsPaused( true );
			}
			
			
			
			
			/// Resume the timer if it is currently paused.
			/**
			  * If the timer is not paused, then this method does
			  * nothing (the timer's state doesn't need to change to
			  * reflect the function call).
			  */
			GSOUND_INLINE void resume()
			{
				this->setIsPaused( false );
			}
			
			
			
			
			
	private:
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// The second to last time the timer was updated.
			Double oldTime;
			
			
			
			
			/// The last time the timer was updated.
			Double currentTime;
			
			
			
			
			/// The positive time interval between the last and second to last times.
			Double lastInterval;
			
			
			
			
			/// Whether or not the timer is currently paused.
			Bool isPaused;
			
			
			
};




//##########################################################################################
//*************************  End GSound Utilities Namespace  *******************************
GSOUND_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_TIMER_H
