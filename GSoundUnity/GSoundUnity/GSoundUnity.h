#include "gsound/GSound.h"
#include <list>

#if defined(GSOUND_PLATFORM_APPLE)
#include <mach-o/dyld.h>
#elif defined(GSOUND_PLATFORM_WINDOWS)
#define NOMINMAX
#include <Windows.h>
#endif

using namespace gsound;
using gsound::util::Timer;

// The object which performs sound propagation.
static SoundPropagator* propagator;


// The object which contains a collection of all sound objects,
// sound sources, and listeners in the scene.
static SoundScene* scene;

// The object which specifies the location and orientation of
// the sound reciever in the scene.
static SoundListener* listener;

// Create a sound propagation renderer that renders to stereo.
static SoundPropagationRenderer* renderer;

// Create an object which encapsulates the functionality of sending audio
// to a sound output device. Use the default system output device.
static SoundOutputDevice* outputDevice;

// Create a buffer to hold the output of the propagation system.
static SoundPropagationPathBuffer* pathBuffer;

SoundMaterial* getMaterial(float gain67, float gain125, float gain250, float gain500,
	float gain1000, float gain2000, float gain4000, float gain8000,
	float tranRolloffFq, float tranRolloffSpeed,
	float absRolloff);

// Load a box with the specified dimensions and material.
SoundMesh* loadBox(const AABB3& box, const SoundMaterial& material);


// Sets the current working directory to the one that contains the executable.
void setCurrentDirectory();

// Main method
__declspec(dllexport) void playSound(const char *soundFile);

// Test method
__declspec(dllexport) int getSomeInt();

__declspec(dllexport) void init();
__declspec(dllexport) void update();
__declspec(dllexport) void stop();
__declspec(dllexport) void clear();
__declspec(dllexport) void addSource(const char* soundFile, float posX, float posY, float posZ, float volume);
__declspec(dllexport) void addAABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ,
	float gain67, float gain125, float gain250, float gain500,
	float gain1000, float gain2000, float gain4000, float gain8000,
	float tranRolloffFq, float tranRolloffSpeed,
	float absRolloff);
