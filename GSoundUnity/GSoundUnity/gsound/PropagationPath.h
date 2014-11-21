/*
 * Project:     GSound
 * 
 * File:        gsound/PropagationPath.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    gsound::PropagationPath class declaration
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


#ifndef INCLUDE_GSOUND_PROPAGATION_PATH_H
#define INCLUDE_GSOUND_PROPAGATION_PATH_H


#include "GSoundBase.h"


#include "PropagationPathID.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class representing a single sound propagation path through a scene.
/**
  * It contains the direction of the first part of the path in the listener's coordinate
  * frame, the total distance of the path, the relative speed along the path of the
  * source and listener, the total frequency-dependent attenuation along the path, 
  * and an object uniquely describing the interactions of that path with the scene.
  */
class PropagationPath
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructor
			
			
			
			
			/// Create a new propagation path object by initializing all attributes.
			GSOUND_INLINE PropagationPath( const Vector3& newDirection, Real newDistance,
									Real newRelativeSpeed, Real newSpeedOfSound,
									const FrequencyResponse& newFrequencyAttenuation,
									const PropagationPathDescription& pathDescription )
				:	direction( newDirection ),
					distance( newDistance ),
					relativeSpeed( newRelativeSpeed ),
					speedOfSound( newSpeedOfSound ),
					frequencyAttenuation( newFrequencyAttenuation ),
					pathID( pathDescription )
			{
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Direction Accessor Methods
			
			
			
			
			/// Get the direction from the listener to the virtual sound source.
			/**
			  * The vector returned is of unit length and specified in the listener's
			  * coordinate frame.
			  */
			GSOUND_FORCE_INLINE const Vector3& getDirection() const
			{
				return direction;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Distance Accessor Methods
			
			
			
			
			/// Get the distance from the listener to the virtual sound source.
			GSOUND_FORCE_INLINE Real getDistance() const
			{
				return distance;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Relative Speed Accessor Methods
			
			
			
			
			/// Get the relative speed of the source and listener along the propagation path.
			GSOUND_FORCE_INLINE Real getRelativeSpeed() const
			{
				return relativeSpeed;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Relative Speed Accessor Methods
			
			
			
			
			/// Get the average speed of sound along the propagation path.
			GSOUND_FORCE_INLINE Real getSpeedOfSound() const
			{
				return speedOfSound;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Attenuation Accessor Methods
			
			
			
			
			/// Get the frequency-dependent attenuation for the virtual sound source.
			GSOUND_FORCE_INLINE const FrequencyResponse& getFrequencyAttenuation() const
			{
				return frequencyAttenuation;
			}
			
			
			
			
			/// Set the frequency-dependent attenuation for the virtual sound source.
			GSOUND_INLINE void setFrequencyAttenuation( const FrequencyResponse& newFrequencyAttenuation )
			{
				frequencyAttenuation = newFrequencyAttenuation;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Propagation Path Depth Accessor Method
			
			
			
			
			/// Get the number of scene interactions that occurred between the source and listener.
			GSOUND_FORCE_INLINE Size getDepth() const
			{
				return pathID.getDescription().getDepth();
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Propagation Path ID Accessor Methods
			
			
			
			
			/// Get a unique identifier for the propagation path.
			GSOUND_FORCE_INLINE const PropagationPathID& getID() const
			{
				return pathID;
			}
			
			
			
			
			/// Get an integral hash code for this propagation path.
			GSOUND_FORCE_INLINE Hash getHashCode() const
			{
				return pathID.getHashCode();
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// A unique identifier for the propagation path of this virtual source source.
			PropagationPathID pathID;
			
			
			
			
			/// The frequency-dependent attenuation for the virtual sound source.
			/**
			  * This object does not contain any distance attenuation. That must
			  * be separately calculated using the distance term of this propagation
			  * path.
			  */
			FrequencyResponse frequencyAttenuation;
			
			
			
			
			/// The normalized direction from the listener to the virtual sound source.
			/**
			  * This vector is specified in the listener's coordinate frame.
			  */
			Vector3 direction;
			
			
			
			
			/// The distance from the listener to the virtual sound source.
			Real distance;
			
			
			
			
			/// The relative speed of the source and listener along the propagation path.
			Real relativeSpeed;
			
			
			
			
			/// The average speed of sound along the propagation path.
			Real speedOfSound;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PROPAGATION_PATH_H
