/*
 * Project:     GSound
 * 
 * File:        gsound/SoundPropagationController.cpp
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::SoundPropagationController class implementation
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


#include "SoundPropagationController.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundPropagationController:: SoundPropagationController()
	:	maximumFrameTime( 0.030 ),
		lastFrameTime( 0.0 ),
		currentNumberOfListenerRays( 1000 ),
		minimumNumberOfListenerRays( 50 ),
		currentNumberOfSourceRays( 100 ),
		minimumNumberOfSourceRays( 10 ),
		sourceRayRatio( 0.1f ),
		increaseAmount( 1 )
{
}




SoundPropagationController:: SoundPropagationController( Size numListenerRays, Size numSourceRays, Double maxFrameTime )
	:	maximumFrameTime( math::max( maxFrameTime, 0.001 ) ),
		lastFrameTime( 0.0 ),
		currentNumberOfListenerRays( math::max( numListenerRays, Size(50) ) ),
		minimumNumberOfListenerRays( 50 ),
		currentNumberOfSourceRays( math::max( numSourceRays, Size(10) ) ),
		minimumNumberOfSourceRays( 10 ),
		sourceRayRatio( 0.1f ),
		increaseAmount( 1 )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Propagation Methods
//############		
//##########################################################################################
//##########################################################################################





void SoundPropagationController:: propagateSound( SoundPropagator& propagator, 
												const SoundScene& scene, const SoundListener& listener,
												Size maxListenerProbeDepth, Size maxSourceProbeDepth,
												SoundPropagationPathBuffer& propagationPathBuffer )
{
	timer.update();
	
	propagator.propagateSound( scene, listener,
								maxListenerProbeDepth, currentNumberOfListenerRays,
								maxSourceProbeDepth, currentNumberOfSourceRays,
								propagationPathBuffer );
	
	timer.update();
	lastFrameTime = timer.getLastInterval();
	
	if ( lastFrameTime > 0.0 )
	{
		if ( lastFrameTime < maximumFrameTime )
		{
			currentNumberOfListenerRays += increaseAmount;
			currentNumberOfSourceRays = Size(currentNumberOfListenerRays*sourceRayRatio);
		}
		else
		{
			Double decreaseMultiplier = lastFrameTime / maximumFrameTime;
			
			currentNumberOfListenerRays = Size(decreaseMultiplier*currentNumberOfListenerRays);
			currentNumberOfSourceRays = Size(decreaseMultiplier*currentNumberOfSourceRays);
		}
	}
}





void SoundPropagationController:: propagateSound( SoundPropagator& propagator, 
												const SoundScene& scene, const SoundListener& listener,
												Size maxListenerProbeDepth, Size maxSourceProbeDepth,
												SoundPropagationPathBuffer& propagationPathBuffer,
												DebugDrawingCache& debugDrawingCache )
{
	timer.update();
	
	propagator.propagateSound( scene, listener,
								maxListenerProbeDepth, currentNumberOfListenerRays,
								maxSourceProbeDepth, currentNumberOfSourceRays,
								propagationPathBuffer,
								debugDrawingCache );
	
	timer.update();
	lastFrameTime = timer.getLastInterval();
	
	if ( lastFrameTime > 0.0 )
	{
		if ( lastFrameTime < maximumFrameTime )
		{
			currentNumberOfListenerRays += increaseAmount;
			currentNumberOfSourceRays = Size(currentNumberOfListenerRays*sourceRayRatio);
		}
		else
		{
			Float decreaseMultiplier = Float(maximumFrameTime / lastFrameTime);
			
			currentNumberOfListenerRays = math::max( Size(decreaseMultiplier*currentNumberOfListenerRays),
													minimumNumberOfListenerRays );
			currentNumberOfSourceRays = math::max( Size(decreaseMultiplier*currentNumberOfSourceRays),
													minimumNumberOfSourceRays );
		}
	}
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
