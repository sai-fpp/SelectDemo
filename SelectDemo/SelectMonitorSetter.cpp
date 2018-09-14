#include "pch.h"
#include "SelectMonitorSetter.h"

SelectMonitorSetter SelectMonitorSetter::m_ins;


SelectMonitorSetter& SelectMonitorSetter::Get()
{
	return m_ins;
}

void SelectMonitorSetter::Init(SocketEventDispatcher* pSED)
{
	m_SED = pSED;
}

bool SelectMonitorSetter::SetSocket(SOCKET nSocketNum, int nConditionSet)
{
	return m_SED->SetFdSet(nSocketNum, nConditionSet);
}

SelectMonitorSetter::SelectMonitorSetter()
{
}


SelectMonitorSetter::~SelectMonitorSetter()
{
}
