// SelectDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "SocketEventDispatcher.h"
#include "WinsockLoader.h"

SocketEventDispatcher eventLoop;

bool ctrlhandler(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
		case CTRL_C_EVENT:
		{
			printf("ctrl-c event\n\n");
			break;
		}
		case CTRL_CLOSE_EVENT:
		{
			eventLoop.Close();
			break;	
		}
		default:
		{
			break;
		}
	}
	return true;
}

int main()
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, true);
	if (!WinsockLoader::Get().InitLib())
	{
		return 0;
	}
	eventLoop.Init();
	int nReslut = 0;
	while (true)
	{
		nReslut = eventLoop.RunOneStep(0);
		if (nReslut < 0)
		{
			break;
		}
		else if(nReslut == 0)
		{

		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
