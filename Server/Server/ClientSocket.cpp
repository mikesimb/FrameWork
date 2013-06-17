#include "stdafx.h"
#include "ClientSocket.h"


CClientSocket::CClientSocket(void)
{
	m_socket = INVALID_SOCKET;
	InitializeCriticalSection(m_SendBufCS);
	memset(&m_ReviceBuf, 0,sizeof(m_ReviceBuf));
	memset(&m_SendBuf ,0 , sizeof(m_SendBuf));
	m_FirstNode = NULL;
	m_LastNode = NULL;
}


CClientSocket::~CClientSocket(void)
{
	ClearSendBuf();
	DeleteCriticalSection(m_SendBufCS);
}

void CClientSocket::SetSocket( SOCKET _socket )
{
	if (_socket != INVALID_SOCKET)
		m_socket = _socket;
	return;
}

SOCKET CClientSocket::getSocket()
{
	return m_socket;
}

void CClientSocket::SetIpAddress( string ip )
{
	  
	m_stripAddress = ip.c_str();
}

void CClientSocket::SetPort( int _iPort )
{
	m_iport = _iPort;
}

int CClientSocket::getPort()
{
	return m_iport;
}

int CClientSocket::ForceClose()
{
	int Res = 0;
	if (m_socket != INVALID_SOCKET)
	{
		Res = closesocket(m_socket);
		ClearSendBuf();
		memset(&m_ReviceBuf,0 ,sizeof(m_ReviceBuf));
		PostQueuedCompletionStatus((HANDLE)m_socket,0,DWORD(this),LPOVERLAPPED(DISCONNECT_FLAG));
	}

	return Res;
}

void CClientSocket::ClearSendBuf()
{
	EnterCriticalSection(m_SendBufCS);
	 while(m_FirstNode != NULL)
	 {
		 pSendQueueNode Node  = m_FirstNode;
		 m_FirstNode = m_FirstNode->Next ;
		free(Node->szBuf);
		delete(Node);
		Node = NULL;
	 }

	LeaveCriticalSection(m_SendBufCS);

}

void CClientSocket::PrepareSend(pBlock  block ,int iSendLen)
{

	EnterCriticalSection(m_SendBufCS);

	LeaveCriticalSection(m_SendBufCS);

}
