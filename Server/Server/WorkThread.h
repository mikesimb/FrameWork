#pragma once
#include "thread.h"

#include "IOCPSocket.h"

class CIOCPSocket;

class WorkThread :
	public CThread
{
public:
	WorkThread(void);
	~WorkThread(void);

	 CIOCPSocket  * m_IOCPSOCKETSERVER;	
protected:
	void Execute(void);
};

