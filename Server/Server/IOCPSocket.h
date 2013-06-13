#pragma once
#include <WinSock2.h>
#include "AcceptThread.h"
#include "WorkThread.h"
#include <list>
#pragma comment(lib,"ws2_32.lib")


class CAcceptThread;
class WorkThread;
class CIOCPSocket
{
public:
	CIOCPSocket(void);
	virtual ~CIOCPSocket(void);


	bool InitSocketLib();
	bool InitCompleteIOCP();
	bool InitListenSocket();
	SOCKET GetIOCPSocket();
	HANDLE GetIOCPHandle();

	bool CloseListenSocket();

	bool Init();
	//本地服务器的IP；
	CStringA m_strIpaddress;
	//本地服务器的端口号；
	int   m_iport;    
	bool m_bActived;
protected:

	bool InitWorkThread();
	int _GetNoOfProcessors();


private:
	//这个是监听用的SOCKET;
	SOCKET m_listensocket;
	HANDLE  m_IOCPHandle;
    sockaddr_in m_listensockaddr; 
	CAcceptThread * m_AcceptThread;
	CList<WorkThread* > m_WorkThreadList;
};

