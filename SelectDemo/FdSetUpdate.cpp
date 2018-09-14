#include "pch.h"
#include "FdSetUpdate.h"
#include <WinSock2.h>
#include <stdio.h>

FdSetUpdate FdSetUpdate::ins_;

FdSetUpdate::FdSetUpdate()
{
	m_SocketNum = INVALID_SOCKET;
}


bool FdSetUpdate::Init()
{
	m_SocketNum = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_SocketNum != INVALID_SOCKET)
	{
		struct sockaddr_in serverAddress;
		memset(&serverAddress, 0, sizeof(sockaddr_in));
		serverAddress.sin_family = AF_INET;
		long lip = inet_addr("127.0.0.1");
		if (lip != INADDR_NONE)
		{
			serverAddress.sin_addr.S_un.S_addr = lip;
			serverAddress.sin_port = htons(8000);
			if (connect(m_SocketNum, (sockaddr*)&serverAddress, sizeof(serverAddress)) != SOCKET_ERROR)
			{
				return true;
			}
		}
	}
	return false;
}

void FdSetUpdate::Update(SOCKET num,int condition)
{
	int data[2];
	data[0] = num;
	data[1] = condition;
	int n = send(m_SocketNum, (char*)data,sizeof(data),0);
	printf("FdSetUpdate:%d\n",n);
}

FdSetUpdate& FdSetUpdate::Get()
{
	return ins_;
}

FdSetUpdate::~FdSetUpdate()
{
}
