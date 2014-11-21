// GSoundUnityTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

__declspec(dllimport) void playSound(const char* filename);

__declspec(dllimport) void init();
__declspec(dllimport) void update();
__declspec(dllimport) void stop();

int _tmain(int argc, _TCHAR* argv[])
{

	init();

	update();

	stop();

	//// Perform sound propagation for 30 seconds.
	//while (timer.getElapsedTime() < 5.0)
	//{
	//	std::cout << timer.getElapsedTime();
	//	std::cout << "\n";
	//	update();
	//}
	//playSound("Data/acoustics.wav");
	return 0;
}

