// GSoundUnityTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

__declspec(dllimport) void playSound();

int _tmain(int argc, _TCHAR* argv[])
{
	playSound();
	return 0;
}

