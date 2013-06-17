#pragma once
#include <string>
#include "IOCPSocket.h"
using namespace std;

class CClientSocket
{
public:
	CClientSocket(void);
	~CClientSocket(void);


	void SetSocket(SOCKET _socket);
	SOCKET getSocket();

	void SetIpAddress(string ip);
	string getIpAddress();

	void SetPort(int _iPort);
	int getPort();

	int  ForceClose();

	void ClearSendBuf();
	void PrepareSend(pBlock block ,int iSendLen);
	//void ConnectEvent();

	//void  PrepareRevice();
	//void PrepareSend();

	//void SocketRead();


private:
	 SOCKET m_socket;
	 string m_stripAddress;
	 int    m_iport;

	 Block  m_ReviceBuf;
	 Block  m_SendBuf;

	 SendQueueNode * m_FirstNode;
	 SendQueueNode * m_LastNode;


	 LPCRITICAL_SECTION  m_SendBufCS;



};

