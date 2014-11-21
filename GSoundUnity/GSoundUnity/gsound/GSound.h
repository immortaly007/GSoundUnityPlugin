/*
 * Project:     GSound
 * 
 * File:        gsound/GSound.h
 * 
 * Version:     1.0.0
 * 
 * Contents:    All includes for GSound library, imports into main gsound namespace.
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


#ifndef INCLUDE_GSOUND_H
#define INCLUDE_GSOUND_H


//##########################################################################################
//##########################################################################################
//############		
//############		Library Documentation Main Page
//############		
//##########################################################################################
//##########################################################################################



/** \mainpage
  * 
  * \section main_intro Introduction
  * 
  * GSound is a library written in C++ which is designed to simulate the propagation
  * of sound within a 3D environment. In particular, it is designed to be fast enough
  * to run at real-time rates on modern hardware while consuming as few system resources
  * as possible.
  *
  * This page serves as an introduction to the basic structure and function of the
  * modules of the GSound library. For a more in-depth discussion of various functions
  * and classes, see the linked reference pages.
  *
  * \section main_sound_propagation Sound Propagation
  *
  * Sound Propagation is a name given to the group of physical phenomena that 
  * determine how sound moves through an environment. In reality, sound can propagate
  * via large number complex phenomena: specular reflection, diffuse reflection, transmission,
  * refraction, diffraction, among others. GSound does not model all of these processes
  * because of real-time performance goals, but instead models those phenomena that are
  * most perceptually important to an observer or efficient: specular reflection,
  * transmission, and diffraction.
  *
  * The canonical representation of the results of sound propagation in an environment
  * between a sound source and sound listener is an impulse response (IR). An IR provides
  * all of the information necessary for recreating the sound produced by a particular
  * room, source position, and listener position combination. It is common to split an 
  * IR into two separate components: an early reflection component and a late reverberation
  * component. Early reflections are the first reflections and diffractions that arrive
  * at a listener from a sound source. Late reverberation is the combination of millions
  * of higher-order reflections that sum to produce an audible reverberation tail.
  *
  * GSound models these two components of a room's IR using different two different methods.
  * 
  * \subsection early_propagation Early Sound Propagation
  * 
  * GSound performs early sound propagation by first tracing a large number of rays
  * from the position of the sound listener randomly in all directions. These rays are
  * specularly reflected through the scene up to a user-defined reflection depth. These
  * probe rays are used to generate potential propagation paths from each sound source in
  * the scene to the listener. These potential paths are then validated and the valid
  * paths are returned as the early reflections and diffractions that reach the listener.
  * 
  * \subsection reverb_estimation Late Reverb Estimation
  * 
  * Since late reverberation is a highly complex phenomenon that would be extremely expensive
  * to simulate, a statistical method must be used to estimate the late reverberation tail
  * produced by a room/listener/source configuration. GSound does this by tracing a number
  * of rays randomly in all directions from each sound source. These ray are specularly reflected
  * through the scene up to a user-defined reflection depth. The triangles that these
  * rays strike are used to estimate a reverb time for the sound source in question. The
  * process is repeated for each sound source, allowing for each source to have a different
  * reverb time.
  * 
  * \section main_conventions Library Conventions
  * 
  * GSound uses the following conventions when formating names of types, namespaces,
  * functions, variables, and constants:
  * - Types: Types start with a capital letter and use CamelCase formating to indicate
  *   the breaks between words. Abbreviations are avoided unless they are considered to be
  *   part of common written language. Abbreviations are always written in ALL CAPS.
  * - Namespaces: Namespaces start with a lowercase letter and are always one word or
  *   an abbreviation of a word.
  * - Functions: Functions start with a lowercase letter and use camelCase formating to
  *   indicate the breaks between words. Abbreviations are avoided unless they are
  *   considered to be part of common written language. Abbreviations are always
  *   written in ALL CAPS.
  * - Variables: Variables use naming similar to functions.
  * - Constants: Constants are written in ALL_CAPS with words separated by an underscore.
  *   The previously mentioned rules for abbreviations apply here as well.
  * 
  * \section using_gsound Using GSound
  * 
  * All parts of the GSound library are declared within the namespace gsound. Classes
  * and types declared in that namespace are the public interface of the library.
  * Other internal classes and functions are declared in other sub-namespaces of gsound.
  * These internal classes, though publicly accessable, shouldn't be used directly by the
  * programmer unless one is modifying the source code of the library itself and has
  * complete control over the changes made. The internal classes are subject to change,
  * removal, or addition without notice and should be considered in a constant state of
  * deprecation.
  * 
  * \subsection scene_setup Scene Setup
  * 
  * In order to perform sound propagation, a gsound::SoundScene object must be created.
  * This object contains pointers to all gsound::SoundSource and gsound::SoundObject objects in a scene.
  * A SourceSource object describes the position, velocity, and other properties relating
  * to the audible qualities of a sound source. A SoundSource object corresponds to a point
  * in space which emits sound. A SoundObject object describes the position,
  * velocity, orientation, scaling, and 3D triangle mesh of an object in a scene. A SoundObject
  * can represent world geometry or any other type of object that is sonically significant.
  *
  * In order to create a SoundObject object, one must first create a gsound::SoundMesh object describing
  * the 3D triangle mesh of the SoundObject. A SoundMesh stores an array of vertices, materials,
  * and triangles. Each triangle consists of the indices to its 3 vertices, plus an index of the
  * material which that triangle uses. The SoundMesh class does not contain any functionality for
  * loading 3D model files to and from system storage. This design choice was made in order to 
  * remove the responsibility of mesh loading from the SoundMesh class and because it would be
  * exceedingly difficult to target every model format in common use. Instead, mesh loading is
  * left to the programmer to implement using their favorite model format. Mesh data is submitted
  * to a SoundMesh class as an array of gsound::SoundVertex objects, an array of gsound::SoundTriangle objects, 
  * and an array of gsound::SoundMaterial objects. See the documentation page for each of these classes
  * for a more detailed description of their interfaces. In the future, it is possible that
  * GSound will add support for loading and saving a custom preprocessed model format for quicker
  * load times. This feature is currently under research and development.
  *
  * Once all necessary SoundObject and SoundSource objects have been constructed, these
  * objects should be added to the SoundScene object. A SoundScene object also allows the 
  * user to specify the speed of sound in the scene. This quantity is CRUCIAL for proper
  * auralization of the sound propagation output. If the speed of sound is not in the same
  * units as the positions and velocities of SoundSource, SoundObject, and SoundListener objects,
  * strange pitch shifting and delay interpolation artifacts can occurr. It is recommended
  * that one uses the convention that 1 unit in world space is equal to one meter. Thus, the
  * speed of sound would be set to be approximately 343 meters per second.
  *
  * The final object which must be created in order to perform sound propagation is a 
  * gsound::SoundListener object. A SoundListener object describes the position, velocity,
  * and orientation of a sound reciever. The orientation of a SoundListener is specified using
  * a right-handed 3x3 column-major rotation matrix. The orientation specified must correspond to
  * the visual representation of the listener's orientation in order for proper panning to
  * be performed.
  * 
  * \subsection performing_sound_propagation Performing Sound Propagation
  * 
  * The main class which performs sound propagation in GSound is gsound::SoundPropagator.
  * This class allows one to set parameters which determine what kind of propagation should
  * be performed, as well as performs the sound propagation computations itself. The different
  * parts of the sound propagation algorithm that can be enabled or disable are: direct sound,
  * transmitted sound (through objects), reflected sound, diffracted sound, and reverb estimation.
  * Each type of propagation has some cost involved; some (direct sound, transmission) have
  * almost no or minimal runtime cost and thus should stay enabled. Reflected and diffracted
  * sound should usually be enabled or diabled together: if reflection is already enabled, diffraction
  * can be computed at minimal additional cost with a significant increase in audio quality, and
  * visce versa.
  *
  * Once a SoundPropagator class has been configured, one perfoms sound propagation by 
  * calling the method gsound::SoundPropagator::propagateSound(). This method takes several
  * parameters. The first parameter should be a const reference to a SoundScene object which
  * represents the scene where propagation should be performed. The second parameter should be a
  * const reference to a SoundListener object which denotes the listener which should recieve the
  * sound being propagated. The next two parameters indicate the maximum depth and the number of
  * probe rays to shoot from the SoundListener's perspective. Since shooting probe rays takes
  * a large portion of the time needed to perform sound propagation, it is important to use
  * as few as possible to get the desired quality of simulation. The cost of shooting probe rays
  * scales approximately linearly with the number of rays or the maximum depth. The next two
  * paremeters are similar to the last two but from each sound source's perspective. These
  * source probe rays are at the moment used for only reverb estimation which requires far
  * fewer rays than sound propagation (usually an order of magnitude less). The last parameter
  * of the method is a buffer of the type gsound::SoundPropagationBuffer where the output of 
  * the sound propagation step is to be placed. An instance of this class must be created by the
  * user so that it can later be used to auralize the sound propagation output.
  * 
  * It is possible to gather information about the sound propagation being performed which can
  * be used for debugging. A second version of the propagateSound() method allows for another
  * parameter, a reference to a gsound::DebugDrawingCache object in which this debugging information
  * is placed. Examples of debug information are: probe rays, various types of propagation paths,
  * scene geometry, sound sources, sound listeners, diffraction edges. Once this information is
  * in the DebugDrawingCache, it can later be drawn using your favorite renderer.
  * 
  * \subsection sound_propagation_rendering Sound Propagation Rendering
  * 
  * In order to perform sound propagation rendering, the supplied gsound::SoundPropagationRenderer
  * class can be used or another external renderer if so desired. The purpose of a sound propagation
  * renderer is to take the audio from each sound source and auralize it for a single listener
  * using the sound propagation paths generated in the propagation step. It is recommended that the
  * provided SoundPropagationRenderer be used rather than a pre-existing sound library such as
  * OpenAL, WWise, or FMOD. These libraries are not designed in order to perform the frequency-dependent
  * effects necessary for different SoundMaterial types and diffraction efficiently. However,
  * these libraries can still be used to provide audio data as input to the SoundPropagationRenderer
  * and can also be used to handle sending the output of the renderer to system audio devices
  * (though this functionality is already present in GSound).
  * 
  * The most important parameters to specify when creating a SoundPropagationRenderer is a
  * gsound::SpeakerConfiguration object and a gsound::FrequencyPartition object.
  * A SpeakerConfiguration determines the number of output channels/speakers that the renderer should have,
  * and where these speakers are positioned in relation to the listener. This information is used to
  * pan virtual sound sources between the speakers to localize them in a specific direction.
  * This allows for any type of audio output format that could be desired (stereo, 5.1 surround, 
  * 7.1 surround, etc.). The FrequencyPartition object describes the number and widths of the
  * frequency bands used for rendering. When audio is rendered, it is split into these
  * frequency bands before the main rendering step to allow frequency-dependent rendering effects
  * such as those produced by diffraction, transmission, and reflection. Other parameters of the
  * SoundPropagationRenderer can be changed to suit the user's needs and requirements such as
  * the output sample rate, maximum delay time, maximum number of propagation paths,
  * maximum propagation path age, and others. See the documentation page for the SoundPropagationRenderer
  * class for more information.
  * 
  * Once a frame of sound propagation has been performed, the SoundPropagationBuffer containing
  * the output of that frame should be passed to the SoundPropagationRenderer using the method
  * gsound::SoundPropagationRenderer::updatePropagationPaths(). This updates the state of the 
  * renderer with any new propagation paths or sound sources that are now part of the
  * scene. In order to directly access the output of the renderer, one can call the inherited method
  * gsound::SoundOutput::getSamples() to get a buffer of output audio samples. This is necessary
  * if one is using an external sound library to interface with the system's audio devices.
  * Any library that provides a callback-based sound output approach will be compatable with 
  * GSound (though some sample data conversion may be necessary). GSound uses 32-bit floating
  * point samples for rendering.
  *
  * However, the easiest way to send the output of the rendering step to an audio output
  * device is to use GSound's built-in functionality. In order to do this, first query the 
  * connected audio ouput devices by creating an instance of the class gsound::SoundDeviceManager.
  * This class allows one to enumerate the output audio devices connected to the system or get
  * the default system output device (recommended). The SoundDeviceManager returns a gsound::SoundDeviceID
  * object which is then passed to the constructor of a gsound::SoundOutputDevice object. A
  * SoundOutputDevice is the main interface to any given output device. Using the
  * gsound::SoundOutputDevice::setInput() method, one can pass in any object deriving from
  * gsound::SoundOutput (such as SoundPropagationRenderer). This SoundOutput will then
  * provide all audio requested by the device once the gsound::SoundOutputDevice::start()
  * method is called, starting the output of audio by the device.
  * 
  * \subsection gsound_tuning Performance Tuning
  * 
  * (coming soon)
  */

//##########################################################################################
//##########################################################################################
//############		
//############		Library Include Files
//############		
//##########################################################################################
//##########################################################################################




#include "GSoundConfig.h"



#include "GSoundMath.h"
#include "GSoundUtilities.h"
#include "GSoundDSP.h"



#include "GSoundBase.h"



#include "FrequencyPartition.h"
#include "FrequencyResponse.h"

#include "SoundVertex.h"
#include "SoundTriangle.h"


#include "SoundMesh.h"
#include "SoundObject.h"
#include "SoundMaterial.h"
#include "SoundSource.h"


#include "SoundMeshPreprocessor.h"
#include "SoundMeshSerializer.h"

#include "SoundDistanceAttenuation.h"


#include "PropagationPath.h"
#include "PropagationPathID.h"
#include "PropagationPathDescription.h"
#include "PropagationPathPoint.h"


#include "SoundPropagator.h"
#include "SoundPropagationController.h"

#include "SoundPropagationRenderer.h"




//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
// Import classes in sub-namespaces into the main gsound namespace if they should be public.


/// Import the Sample type into the base gsound namespace to make it more easily accessible.
typedef dsp::Sample Sample;

/// Import the SoundBuffer type into the base gsound namespace to make it more easily accessible.
typedef dsp::SoundBuffer SoundBuffer;

/// Import the SoundStream type into the base gsound namespace to make it more easily accessible.
typedef dsp::SoundStream SoundStream;

/// Import the SoundInput type into the base gsound namespace to make it more easily accessible.
typedef dsp::SoundInput SoundInput;

/// Import the SoundOutput type into the base gsound namespace to make it more easily accessible.
typedef dsp::SoundOutput SoundOutput;

/// Import the SoundProcessor type into the base gsound namespace to make it more easily accessible.
typedef dsp::SoundProcessor SoundProcessor;

/// Import the SoundInput type into the base gsound namespace to make it more easily accessible.
typedef dsp::SoundInput SoundInput;

/// Import the SpeakerConfiguration type into the base gsound namespace to make it more easily accessible.
typedef dsp::SpeakerConfiguration SpeakerConfiguration;

/// Import the SpeakerType type into the base gsound namespace to make it more easily accessible.
typedef dsp::SpeakerType SpeakerType;

/// Import the SoundDeviceManager type into the base gsound namespace to make it more easily accessible.
typedef dsp::SoundDeviceManager SoundDeviceManager;

/// Import the SoundDeviceID type into the base gsound namespace to make it more easily accessible.
typedef dsp::SoundDeviceID SoundDeviceID;

/// Import the SoundOutputDevice type into the base gsound namespace to make it more easily accessible.
typedef dsp::SoundOutputDevice SoundOutputDevice;

/// Import the ChannelIOMap type into the base gsound namespace to make it more easily accessible.
typedef dsp::ChannelIOMap ChannelIOMap;

/// Import the SoundPlayer type into the base gsound namespace to make it more easily accessible.
typedef dsp::SoundPlayer SoundPlayer;

/// Import the WaveDecoder type into the base gsound namespace to make it more easily accessible.
typedef dsp::WaveDecoder WaveDecoder;



//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_H
