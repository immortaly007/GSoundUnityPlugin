#include "gsound/GSound.h"

#if defined(GSOUND_PLATFORM_APPLE)
#include <mach-o/dyld.h>
#elif defined(GSOUND_PLATFORM_WINDOWS)
#define NOMINMAX
#include <Windows.h>
#endif

using namespace gsound;
using gsound::util::Timer;


// Load a box with the specified dimensions and material.
SoundMesh* loadBox(const AABB3& box, const SoundMaterial& material);


// Sets the current working directory to the one that contains the executable.
void setCurrentDirectory();

// Main method
int main(int argc, char** argv);