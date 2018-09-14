#include "pch.h"
#include "WinsockLoader.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

WinsockLoader WinsockLoader::socLoader_;


WinsockLoader::WinsockLoader()
{
}


WinsockLoader& WinsockLoader::Get()
{
	return socLoader_;
}

bool WinsockLoader::InitLib()
{
	if (initialized_)
	{
		return initialized_;
	}
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return false;
	}
	initialized_ = true;
	return initialized_;
}

bool WinsockLoader::IsInitialized()
{
	return  initialized_;
}

WinsockLoader::~WinsockLoader()
{
	WSACleanup();
}
