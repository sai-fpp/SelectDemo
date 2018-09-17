#include "pch.h"
#include "SocketEventDispatcher.h"
#include "PubDef.h"
#include "ClientSession.h"

using namespace std;

SocketEventDispatcher::SocketEventDispatcher()
{

}


SocketEventDispatcher::~SocketEventDispatcher()
{
}

bool SocketEventDispatcher::Init()
{
	return StartListen();
}

int SocketEventDispatcher::RunOneStep(long nWaitMS)
{

	fd_set readSet = m_ReadSet;
	fd_set writeSet = m_WriteSet;
	fd_set exceptionSet = m_ExceptionSet;
	int nResult = Select(readSet, writeSet, exceptionSet, nWaitMS);
	if (nResult == 0)
	{
		return 0;
	}
	if (nResult == -2)
	{
		return -1;
	}
	if (FD_ISSET(m_ListenSocketNum, &readSet))
	{
		ClientSession* p = nullptr;
		SOCKADDR_IN addrClient;
		int addrLen = sizeof(SOCKADDR);
		SOCKET sockConn = accept(m_ListenSocketNum, (SOCKADDR*)&addrClient, &addrLen);
		if (sockConn != INVALID_SOCKET)
		{
			string ip = inet_ntoa(addrClient.sin_addr);
			p = new ClientSession(sockConn, ip);
			if (p)
			{
				printf("new connect:%d\n", p->m_SocketNum);
				m_mapHandlers[p->m_SocketNum] = p;
				SetFdSet(p->m_SocketNum, SOCKET_READABLE);
			}
		}
		else
		{
			printf("accept fail\n");
		}
	}
	for (map<SOCKET, ClientSession*>::iterator it = m_mapHandlers.begin(); it != m_mapHandlers.end();)
	{
		if (FD_ISSET(it->first, &readSet))
		{
			int nSize = it->second->HandleRead();
			if (nSize == 0)
			{
				SetFdSet(it->first, 0);
				delete it->second;
				printf("client close connect:%d\n", it->first);
				m_mapHandlers.erase(it++);
				continue;
			}
		}
		if (FD_ISSET(it->first, &writeSet))
		{
			it->second->HandleWrite();
		}
		if (FD_ISSET(it->first, &exceptionSet))
		{
			it->second->HandleException();
		}
		it++;
		continue;
	}
	return 1;
}

bool SocketEventDispatcher::StartListen()
{
	if (StartListen(8000))
	{
		if (SetNoBlock())
		{
			SetFdSet(m_ListenSocketNum, SOCKET_READABLE);
			return true;
		}
	}
	return false;
}

int SocketEventDispatcher::Select(fd_set& readSet, fd_set& writeSet, fd_set& exceptionSet, long nWaitMS)
{	
	struct timeval* pTime = NULL;
	struct timeval waitTime = { 0,nWaitMS * 1000 };
	if (nWaitMS>0)
	{
		pTime = &waitTime;
	}
	printf("before: %d-%d-%d\n", readSet.fd_count, writeSet.fd_count, exceptionSet.fd_count);
	int nResult = select(0, &readSet, &writeSet, &exceptionSet, pTime);
	printf("after: count: %d %d-%d-%d\n",nResult,readSet.fd_count, writeSet.fd_count, exceptionSet.fd_count);
	if (nResult < 0)
	{
		int nError = WSAGetLastError();
		if (nError == WSAEINTR)
		{
			return 0;
		}
		return -1;
	}
	else if(nResult == 0)
	{
		return 0;
	}
	return 1;
}

bool SocketEventDispatcher::SetFdSet(SOCKET nSocketNum,int nConditionSet)
{
	FD_CLR(nSocketNum, &m_ReadSet);
	FD_CLR(nSocketNum, &m_WriteSet);
	FD_CLR(nSocketNum, &m_ExceptionSet);
	if (nConditionSet != 0)
	{
		if ((nConditionSet&SOCKET_READABLE)&&(m_ReadSet.fd_count >= FD_SETSIZE)
			|| (nConditionSet&SOCKET_WRITABLE) && (m_WriteSet.fd_count >= FD_SETSIZE)
			|| (nConditionSet&SOCKET_EXCEPTION) && (m_ExceptionSet.fd_count >= FD_SETSIZE))
		{
			return false;
		}
		else
		{
			if (nConditionSet&SOCKET_READABLE) FD_SET(nSocketNum, &m_ReadSet);
			if (nConditionSet&SOCKET_WRITABLE) FD_SET(nSocketNum, &m_WriteSet);
			if (nConditionSet&SOCKET_EXCEPTION) FD_SET(nSocketNum, &m_ExceptionSet);
		}
	}
	return true;
}


void SocketEventDispatcher::Close()
{
	shutdown(m_ListenSocketNum, SD_BOTH);
	closesocket(m_ListenSocketNum);
}

bool SocketEventDispatcher::StartListen(int nPort)
{
	sockaddr_in localAddr;
	m_ListenSocketNum = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ListenSocketNum == INVALID_SOCKET)
	{
		return false;
	}
	localAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(nPort);
	int ret = bind(m_ListenSocketNum, (struct sockaddr*)&localAddr, sizeof(localAddr));
	if (ret != 0)
	{
		printf("bind fail\n");
		closesocket(m_ListenSocketNum);
		m_ListenSocketNum = INVALID_SOCKET;
		return false;
	}
	ret = listen(m_ListenSocketNum, 5);
	if (ret != 0)
	{
		closesocket(m_ListenSocketNum);
		m_ListenSocketNum = INVALID_SOCKET;
		return false;
	}
	return true;
}


bool SocketEventDispatcher::SetNoBlock()
{
	if (m_ListenSocketNum != INVALID_SOCKET)
	{
		u_long ul = 1;
		if (ioctlsocket(m_ListenSocketNum, FIONBIO, &ul) == 0)
		{
			return true;
		}
		else
		{
			closesocket(m_ListenSocketNum);
			m_ListenSocketNum = INVALID_SOCKET;
		}
	}
	return false;
}
