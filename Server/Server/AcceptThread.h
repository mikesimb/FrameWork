#pragma once
#include "thread.h"
#include "IOCPSocket.h"

class CIOCPSocket;
class CAcceptThread :
	public CThread
{
public:
	CAcceptThread(CIOCPSocket*  m_ListenSocke);
	~CAcceptThread(void);

	CIOCPSocket  * m_IOCPSOCKETSERVER;	
protected:
	virtual  void Execute(void);
};

