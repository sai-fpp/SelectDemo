#pragma once
#include <WinSock2.h>

class FdSetUpdate
{
public:
	bool Init();
	void Update(SOCKET num, int condition);
	static FdSetUpdate& Get();
private:
	FdSetUpdate();
	~FdSetUpdate();
private:
	static FdSetUpdate ins_;
public:
	SOCKET m_SocketNum;
};

