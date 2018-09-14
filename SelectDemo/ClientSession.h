#pragma once
#include <WinSock2.h>
#include <queue>
#include <string>


typedef struct SendData
{
	char data[100];
	int len;
}SendData;

#define RECV_BUFFER_LEN 1600

class ClientSession
{
public:
	ClientSession(SOCKET SocketNum,std::string ip);
	virtual ~ClientSession();
public:
	int HandleRead();
	void HandleWrite();
	void HandleException();
public:
	SOCKET m_SocketNum = INVALID_SOCKET;
	std::string m_ip;
private:
	char m_RecvBuffer[RECV_BUFFER_LEN];
	int m_nRecvLen;
	std::queue<SendData> m_SendDataSet;
};

