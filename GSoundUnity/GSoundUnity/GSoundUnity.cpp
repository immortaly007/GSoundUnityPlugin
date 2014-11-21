// GSoundUnity.cpp : Defines the exported functions for the DLL application.
//
#include "GSoundUnity.h"
#include <iostream>

void playSound()
{
	// Make the current directory the same as the directory containing the executable.
	setCurrentDirectory();

	// The object which performs sound propagation.
	SoundPropagator propagator;

	// Enable all types of propagation paths.
	propagator.setDirectSoundIsEnabled(true);
	propagator.setTransmissionIsEnabled(true);
	propagator.setReflectionIsEnabled(true);
	propagator.setDiffractionIsEnabled(true);
	propagator.setReverbIsEnabled(true);

	//***********************************************************************

	// The object which contains a collection of all sound objects,
	// sound sources, and listeners in the scene.
	SoundScene scene;

	// The object which specifies the location and orientation of
	// the sound reciever in the scene.
	SoundListener listener;

	// Set the listener's starting position, at head height and centered at the XZ origin.
	listener.setPosition(Vector3(0, 1.5, 0));

	//***********************************************************************

	// Create a material object which will be the material for the box's surface.
	// The material is specified in terms of FrequencyResponse objects that dictate
	// how sound is affected when it undergoes the associated interaction.
	SoundMaterial defaultMaterial(
		// The reflection attenuation for the material.
		FrequencyResponse::getLinearHighRolloff(1000)*
		FrequencyResponse::getLinearLowRolloff(200)*0.9,
		// The transmisison attenuation per world unit for the material.
		FrequencyResponse::getQuadraticHighRolloff(800)*0.9,
		// The absorption attenuation for the material.
		FrequencyResponse::getLinearHighRolloff()*0.5);


	// Create an axis-aligned box that is 4x8x3 meters, centered
	// at the origin that uses the default material.
	SoundMesh* box = loadBox(AABB3(-2, 2, -1.5, 1.5, -4, 4), defaultMaterial);

	SoundObject* boxObject = new SoundObject(box);

	// Set the position of the box so that it is now 1.5 units higher along the Y-axis.
	boxObject->setPosition(Vector3(0, 1.5, 0));

	// Add the box to the scene
	scene.addObject(boxObject);

	//***********************************************************************

	SoundSource* source = new SoundSource();

	// Set the position of the source so that it is on one side of the box.
	source->setPosition(Vector3(0, 1, -3));

	// Set the source to have an intensity of 1.
	// This is the gain applied to the source's audio when there is no
	// distance attenuation.
	source->setIntensity(1);

	// Create a distance attenuation object which specifies how the source's
	// audio decreases in intensity with distance.
	SoundDistanceAttenuation attenuation(1, // Constant attenuation of 1.
		1, // Linear attenuation of 1.
		0); // Quadratic attenuation of 0.

	// Set the distance attenuation for the source.
	source->setDistanceAttenuation(attenuation);

	// Set the reverb distance attenuation for the source to slightly less
	// than the normal distance attenuation.
	source->setDistanceAttenuation(SoundDistanceAttenuation(1, 0.5, 0));

	// Create a WAVE file decoder that will allow the source to use a WAVE file
	// as its source audio. Use the file "sound.wav" in the current working directory.
	WaveDecoder* decoder = new WaveDecoder("Data/acoustics.wav");

	// Create a SoundPlayer object which handles sound playback from a seekable
	// stream of audio. A decoder for another format (such as OGG) can be substituted
	SoundPlayer* player = new SoundPlayer(decoder);

	// Configure the sound player to start playing as soon as audio is requested and to loop.
	player->setIsPlaying(true);
	player->setIsLooping(true);

	// Set the source of the sound source's audio.
	source->setSoundInput(player);

	// Add the sound source to the scene.
	scene.addSource(source);


	//***********************************************************************

	// Create a sound propagation renderer that renders to stereo.
	SoundPropagationRenderer* renderer = new SoundPropagationRenderer(
		SpeakerConfiguration::getStereo());

	// Create a FrequencyPartition object which specifies how the audio
	// is split into frequency band when rendering. It is recommended to use
	// band numbers that are a multiple of 4 in order to take full advantage
	// of SIMD processing.
	FrequencyPartition frequencyPartition;

	// The four frequency bands will be: 0Hz to 250Hz, 250Hz to 1000Hz,
	// 1000Hz to 4000Hz, and 4000Hz to the Nyquist Frequency.
	frequencyPartition.addSplitFrequency(250);
	frequencyPartition.addSplitFrequency(1000);
	frequencyPartition.addSplitFrequency(4000);

	// Tell the propagation renderer to use this frequency partition when rendering.
	renderer->setFrequencyPartition(frequencyPartition);

	//***********************************************************************

	// Create a device manager object which enumerates all connected sound devices.
	SoundDeviceManager deviceManager;

	// Create an object which encapsulates the functionality of sending audio
	// to a sound output device. Use the default system output device.
	SoundOutputDevice* outputDevice =
		new SoundOutputDevice(deviceManager.getDefaultOutputDeviceID());

	// Set the output device so that it gets audio from the sound propagation renderer.
	outputDevice->setInput(renderer);

	// Start outputing audio to the device.
	outputDevice->start();

	//***********************************************************************

	// Create a buffer to hold the output of the propagation system.
	SoundPropagationPathBuffer pathBuffer;

	Timer timer;

	// Perform sound propagation for 30 seconds.
	while (timer.getElapsedTime() < 5.0)
	{
		std::cout << timer.getElapsedTime();
		std::cout << "\n";
		// Perform sound propagation in the scene.
		propagator.propagateSound(scene, // The scene in which to perform propagation.
			listener, // The listener to use as the sound receiver.
			4, // The maximum depth of the rays shot from the listener.
			1000, // The number of rays to shoot from the listener,
			// influences the quality of the early reflection paths.
			4, // The maximum depth of the rays shot from each sound source.
			100, // The number of rays to shoot from each sound source, 
			// influences reverb estimation quality.
			pathBuffer); // The buffer in which to put the propagation paths.

		// Update the state of the sound propagation renderer.
		renderer->updatePropagationPaths(pathBuffer);
	}

	//***********************************************************************

	// Stop outputing audio to the device and destroy it.
	std::cout << "Stopping output device...\n";
	outputDevice->stop(); // Bas: Apparently never ends
	delete outputDevice;
	std::cout << "Output device deleted...\n";

	// Destroy the various objects used for propagation.
	delete renderer;
	delete player;
	delete decoder;
	delete source;
	delete boxObject;
	delete box;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Box Loading Method
//############		
//##########################################################################################
//##########################################################################################




SoundMesh* loadBox(const AABB3& box, const SoundMaterial& material)
{
	ArrayList<SoundVertex> vertices;
	ArrayList<SoundTriangle> triangles;

	vertices.add(SoundVertex(box.min.x, box.min.y, box.min.z));
	vertices.add(SoundVertex(box.max.x, box.min.y, box.min.z));
	vertices.add(SoundVertex(box.max.x, box.max.y, box.min.z));
	vertices.add(SoundVertex(box.max.x, box.max.y, box.max.z));
	vertices.add(SoundVertex(box.min.x, box.min.y, box.max.z));
	vertices.add(SoundVertex(box.min.x, box.max.y, box.max.z));
	vertices.add(SoundVertex(box.min.x, box.max.y, box.min.z));
	vertices.add(SoundVertex(box.max.x, box.min.y, box.max.z));

	// negative z face
	triangles.add(SoundTriangle(0, 1, 2, 0));
	triangles.add(SoundTriangle(0, 2, 6, 0));

	// positive z face
	triangles.add(SoundTriangle(3, 5, 4, 0));
	triangles.add(SoundTriangle(3, 7, 4, 0));

	// positive y face
	triangles.add(SoundTriangle(6, 3, 5, 0));
	triangles.add(SoundTriangle(6, 2, 3, 0));

	// negative y face
	triangles.add(SoundTriangle(0, 7, 4, 0));
	triangles.add(SoundTriangle(0, 1, 7, 0));

	// negative x face
	triangles.add(SoundTriangle(0, 4, 6, 0));
	triangles.add(SoundTriangle(4, 6, 5, 0));

	// positive x face
	triangles.add(SoundTriangle(1, 7, 2, 0));
	triangles.add(SoundTriangle(7, 2, 3, 0));

	ArrayList<SoundMaterial> materials;
	materials.add(material);

	return new SoundMesh(vertices, triangles, materials);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Current Working Directory Set Method
//############		
//##########################################################################################
//##########################################################################################




/// Set the current working directory to be the directory containing the executable.
void setCurrentDirectory()
{
	Size bufferSize = 1024;

#if defined(GSOUND_PLATFORM_APPLE)
	char pathToExecutable[1024];
	_NSGetExecutablePath(pathToExecutable, (uint32_t*)&bufferSize);
#elif defined(GSOUND_PLATFORM_WINDOWS)
	TCHAR pathToExecutable[1024];
	bufferSize = GetModuleFileName(NULL, pathToExecutable, 1024);
#endif

	Index i;
	for (i = 0; pathToExecutable[i] != '\0'; i++);

	for (; i >= 1; i--)
	{
#if defined(GSOUND_PLATFORM_APPLE)
		if (pathToExecutable[i] == '/')
#elif defined(GSOUND_PLATFORM_WINDOWS)
		if (pathToExecutable[i] == '/' || pathToExecutable[i] == '\\')
#endif
		{
			pathToExecutable[i + 1] = '\0';
			break;
		}
	}

#if defined(GSOUND_PLATFORM_APPLE)
	chdir(pathToExecutable);
#elif defined(GSOUND_PLATFORM_WINDOWS)
	SetCurrentDirectory(pathToExecutable);
#endif
}


int getSomeInt()
{
	return 42;
}