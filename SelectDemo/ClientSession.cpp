#include "pch.h"
#include "ClientSession.h"
#include <stdio.h>
#include "PubDef.h"
#include "SocketEventDispatcher.h"

using namespace std;


ClientSession::ClientSession(SOCKET SocketNum, string ip)
{
	m_SocketNum = SocketNum;
	m_ip = ip;
}


ClientSession::~ClientSession()
{
	if (m_SocketNum != INVALID_SOCKET)
	{
		closesocket(m_SocketNum);
	}
}

int ClientSession::HandleRead()
{
	m_nRecvLen = recv(m_SocketNum,m_RecvBuffer, RECV_BUFFER_LEN, 0);
	if (m_nRecvLen > 0)
	{
		printf("%s\n", m_RecvBuffer);
		SendData data;
		strcpy_s(data.data, "result 1");
		data.len = strlen(data.data);
		m_SendDataSet.push(data);
		eventLoop.SetFdSet(m_SocketNum, SOCKET_READABLE | SOCKET_WRITABLE);
	}
	return m_nRecvLen;
}

void ClientSession::HandleWrite()
{
	while (m_SendDataSet.size() > 0)
	{
		SendData data = m_SendDataSet.front();
		m_SendDataSet.pop();
		int n = send(m_SocketNum, data.data, data.len, 0);
		printf("send %d\n", n);
	}
	eventLoop.SetFdSet(m_SocketNum, SOCKET_READABLE);
}

void ClientSession::HandleException()
{

}
