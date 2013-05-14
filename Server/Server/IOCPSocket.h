#pragma once
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

class CIOCPSocket
{
public:
	CIOCPSocket(void);
	virtual ~CIOCPSocket(void);


	bool InitSocketLib();
	bool InitCompleteIOCP();
	bool InitListenSocket();
protected:


private:
	//这个是监听用的SOCKET;
	SOCKET m_listensocket;
	//本地服务器的IP；
	CStringA m_strIpaddress;
	//本地服务器的端口号；
	int   m_iport;    

	HANDLE  m_IOCPHandle;
    sockaddr_in m_listensockaddr; 
};

