/*
 * Project:     GSound
 * 
 * File:        gsound/SoundPropagationController.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundPropagationController class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_PROPAGATION_CONTROLLER_H
#define INCLUDE_GSOUND_SOUND_PROPAGATION_CONTROLLER_H


#include "GSoundBase.h"


#include "SoundPropagator.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class which automatically adjusts the number of rays shot each frame based on a time requirement.
/**
  * The SoundPropagationController class provides a way for users of GSound to automatically
  * control the number of rays shot each frame in order to meet a certain maximum time per frame.
  * If the maximum frame time is exceeded during propagation, the number of rays that will be shot
  * on the next frame is reduced by the amount necessary to meet the time requirement. In order
  * to balance this, the number of rays is increased additively by a certain amount each frame
  * that the time requirement is not exceeded.
  */
class SoundPropagationController
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a SoundPropagationController with the default ray counts and max frame time.
			/**
			  * This controller starts by shooting 1000 rays from the listener and 100 rays from each
			  * source with a maximum frame time of 50ms.
			  */
			SoundPropagationController();
			
			
			
			
			/// Create a SoundPropagationController with the specified ray counts and max frame time.
			SoundPropagationController( Size numListenerRays, Size numSourceRays, Double maxFrameTime );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Sound Propagation Method
			
			
			
			
			/// Perform sound propagation using the specified SoundPropagator object.
			/**
			  * This method is a wrapper around the SoundPropagator's own
			  * SoundPropagator::propagateSound() method. Omitted paramters are supplied
			  * by the controller and the other parameters are forwarded.
			  */
			void propagateSound( SoundPropagator& propagator, 
								const SoundScene& scene, const SoundListener& listener,
								Size maxListenerProbeDepth, Size maxSourceProbeDepth,
								SoundPropagationPathBuffer& propagationPathBuffer );
			
			
			
			
			/// Perform sound propagation using the specified SoundPropagator object.
			/**
			  * This method is a wrapper around the SoundPropagator's own
			  * SoundPropagator::propagateSound() method. Omitted paramters are supplied
			  * by the controller and the other parameters are forwarded.
			  */
			void propagateSound( SoundPropagator& propagator, 
								const SoundScene& scene, const SoundListener& listener,
								Size maxListenerProbeDepth, Size maxSourceProbeDepth,
								SoundPropagationPathBuffer& propagationPathBuffer,
								DebugDrawingCache& debugCache );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Maximum Frame Time Accessor Methods
			
			
			
			
			/// Get the maximum time that it can take for a frame to be processed in seconds.
			GSOUND_INLINE Double getMaximumFrameTime() const
			{
				return maximumFrameTime;
			}
			
			
			
			
			/// Set the maximum time that it can take for a frame to be processed in seconds.
			GSOUND_INLINE void setMaximumFrameTime( Double newMaxFrameTime )
			{
				maximumFrameTime = math::max( newMaxFrameTime, 0.001 );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Last Frame Time Accessor Method
			
			
			
			
			/// Get the time it took for the last frame to be processed in seconds.
			GSOUND_INLINE Double getLastFrameTime() const
			{
				return lastFrameTime;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Listener Ray Count Accessor Method
			
			
			
			
			/// Get the number of listener rays that are to be shot on the next frame.
			GSOUND_INLINE Size getNumberOfListenerRays() const
			{
				return currentNumberOfListenerRays;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Minimum Number Of Listener Rays Accessor Methods
			
			
			
			
			/// Get the smallest number of rays that can be shot from the listener.
			GSOUND_INLINE Size getMinimumNumberOfListenerRays() const
			{
				return minimumNumberOfListenerRays;
			}
			
			
			
			
			/// Set the smallest number of rays that can be shot from the listener.
			GSOUND_INLINE void setMinimumNumberOfListenerRays( Size newMinimumNumberOfListenerRays )
			{
				minimumNumberOfListenerRays = math::max( newMinimumNumberOfListenerRays, Size(1) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Source Ray Count Accessor Method
			
			
			
			
			/// Get the number of source rays that are to be shot for each source on the next frame.
			GSOUND_INLINE Size getNumberOfSourceRays() const
			{
				return currentNumberOfSourceRays;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Minimum Number Of Source Rays Accessor Methods
			
			
			
			
			/// Get the smallest number of rays that can be shot from each SoundSource.
			GSOUND_INLINE Size getMinimumNumberOfSourceRays() const
			{
				return minimumNumberOfSourceRays;
			}
			
			
			
			
			/// Set the smallest number of rays that can be shot from each SoundSource.
			GSOUND_INLINE void setMinimumNumberOfSourceRays( Size newMinimumNumberOfSourceRays )
			{
				minimumNumberOfSourceRays = math::max( newMinimumNumberOfSourceRays, Size(1) );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Source Ray Ratio Accessor Method
			
			
			
			
			/// Get the ratio of the number of source probe rays to the number of listener probe rays.
			GSOUND_INLINE Float getSourceRayRatio() const
			{
				return sourceRayRatio;
			}
			
			
			
			
			/// Set the ratio of the number of source probe rays to the number of listener probe rays.
			GSOUND_INLINE void setSourceRayRatio( Float newSourceRayRatio )
			{
				sourceRayRatio = math::max( newSourceRayRatio, 0.0f );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Ray Increase Accessor Methods
			
			
			
			
			/// Get the number that is added to the number of rays being shot if the number of rays should increase.
			GSOUND_INLINE Size getRayIncreaseAmount() const
			{
				return increaseAmount;
			}
			
			
			
			
			/// Set the number that is added to the number of rays being shot if the number of rays should increase.
			GSOUND_INLINE void setRayIncreaseAmount( Size newRayIncreaseAmount )
			{
				increaseAmount = math::max( newRayIncreaseAmount, Size(1) );
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A timer used to time each frame.
			util::Timer timer;
			
			
			
			
			/// The maximum time that it can take for a frame to be processed in seconds.
			/**
			  * If the last frame time was less than this value, the number of rays is increased
			  * by the increaseAmount. If the last frame time was greater, the number of rays
			  * is decreased by multiplying it by the ratio of the maximumFrameTime to the lastFrameTime.
			  */
			Double maximumFrameTime;
			
			
			
			
			/// The time it took for the last frame to be processed in seconds.
			Double lastFrameTime;
			
			
			
			
			/// The current number of rays that are being shot from the listener.
			Size currentNumberOfListenerRays;
			
			
			
			
			/// The smallest number of rays that can be shot from the listener.
			Size minimumNumberOfListenerRays;
			
			
			
			
			/// The current number of rays that are being shot from each SoundSource.
			Size currentNumberOfSourceRays;
			
			
			
			
			/// The smallest number of rays that can be shot from each SoundSource.
			Size minimumNumberOfSourceRays;
			
			
			
			
			/// The ratio of the number of source probe rays to the number of listener probe rays.
			Float sourceRayRatio;
			
			
			
			
			/// The number that is added to the number of rays being shot if the number of rays should increase.
			Size increaseAmount;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_PROPAGATOR_H
