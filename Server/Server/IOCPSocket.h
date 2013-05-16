#pragma once
#include <WinSock2.h>
#include "AcceptThread.h"
#pragma comment(lib,"ws2_32.lib")


class CAcceptThread;

class CIOCPSocket
{
public:
	CIOCPSocket(void);
	virtual ~CIOCPSocket(void);


	bool InitSocketLib();
	bool InitCompleteIOCP();
	bool InitListenSocket();
	SOCKET GetIOCPSocket();

	bool Init();
	//本地服务器的IP；
	CStringA m_strIpaddress;
	//本地服务器的端口号；
	int   m_iport;    
protected:


private:
	//这个是监听用的SOCKET;
	SOCKET m_listensocket;


	HANDLE  m_IOCPHandle;
    sockaddr_in m_listensockaddr; 
	CAcceptThread * m_AcceptThread;
};

