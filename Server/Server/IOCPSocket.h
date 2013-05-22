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

	bool CloseListenSocket();

	bool Init();
	//���ط�������IP��
	CStringA m_strIpaddress;
	//���ط������Ķ˿ںţ�
	int   m_iport;    
	bool m_bActived;
protected:


private:
	//����Ǽ����õ�SOCKET;
	SOCKET m_listensocket;
	HANDLE  m_IOCPHandle;
    sockaddr_in m_listensockaddr; 
	CAcceptThread * m_AcceptThread;
};

