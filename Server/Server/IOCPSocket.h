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
	//����Ǽ����õ�SOCKET;
	SOCKET m_listensocket;
	//���ط�������IP��
	CStringA m_strIpaddress;
	//���ط������Ķ˿ںţ�
	int   m_iport;    

	HANDLE  m_IOCPHandle;
    sockaddr_in m_listensockaddr; 
};

