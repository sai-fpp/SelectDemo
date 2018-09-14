#pragma once
#include <WinSock2.h>
#include <map>
#include "FdSetUpdate.h"
#include "ClientSession.h"

class SocketEventDispatcher
{
public:
	SocketEventDispatcher();
	~SocketEventDispatcher();
	void Run();
private:
	bool StartListen();
	int Select(fd_set& readSet, fd_set& writeSet, fd_set& exceptionSet, long nWaitMS);
	bool SetFdSet(SOCKET nSocketNum, int nConditionSet);
	bool StartListen(int nPort);
	bool SetNoBlock();
private:
	SOCKET m_ListenSocketNum = INVALID_SOCKET;
	SOCKET m_InternalClientSocket = INVALID_SOCKET;
	std::map<SOCKET, ClientSession*> m_mapHandlers;
	fd_set m_ReadSet;
	fd_set m_WriteSet;
	fd_set m_ExceptionSet;
};

