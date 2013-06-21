#pragma once
#include <string>
#include "IOCPSocket.h"

using namespace std;


class ClientSocketCallBack;

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

	pBlock GetReadBlock();
	void PrepareRecv(pBlock Block);
	void DoClientRead(pBlock Block ,DWORD dwTransfered);
	void OnReviceEvent(CClientSocket * client ,char* Buf, int Buflen);
	void Close();
private:
	 SOCKET m_socket;
	 string m_stripAddress;
	 int    m_iport;

	 Block  m_ReviceBuf;
	 Block  m_SendBuf;

	 SendQueueNode * m_FirstNode;
	 SendQueueNode * m_LastNode;


	 //CRITICAL_SECTION  criticalSection
	 CRITICAL_SECTION  m_SendBufCS;



	

	// LPCRITICAL_SECTION  m_SendBufCS;



};


