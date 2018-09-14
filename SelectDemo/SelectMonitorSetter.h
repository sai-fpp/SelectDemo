#pragma once
#include "SocketEventDispatcher.h"

class SelectMonitorSetter
{
public:
	static SelectMonitorSetter& Get();
	void Init(SocketEventDispatcher* pSED);
	bool SetSocket(SOCKET nSocketNum, int nConditionSet);
private:
	SelectMonitorSetter();
	~SelectMonitorSetter();
private:
	static SelectMonitorSetter m_ins;
	SocketEventDispatcher* m_SED;
};

