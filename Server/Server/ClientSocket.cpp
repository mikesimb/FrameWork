#include "stdafx.h"
#include "ClientSocket.h"


CClientSocket::CClientSocket(void)
{
	m_socket = INVALID_SOCKET;
	InitializeCriticalSection((LPCRITICAL_SECTION)&m_SendBufCS);
	memset(&m_ReviceBuf, 0,sizeof(m_ReviceBuf));
	memset(&m_SendBuf ,0 , sizeof(m_SendBuf));
	m_FirstNode = NULL;
	m_LastNode = NULL;
}


CClientSocket::~CClientSocket(void)
{
	ClearSendBuf();
	DeleteCriticalSection((LPCRITICAL_SECTION)&m_SendBufCS);
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
	EnterCriticalSection((LPCRITICAL_SECTION)&m_SendBufCS);
	 while(m_FirstNode != NULL)
	 {
		 pSendQueueNode Node  = m_FirstNode;
		 m_FirstNode = m_FirstNode->Next ;
		free(Node->szBuf);
		delete(Node);
		Node = NULL;
	 }

	LeaveCriticalSection((LPCRITICAL_SECTION)&m_SendBufCS);

}

void CClientSocket::PrepareSend(pBlock  block ,int iSendLen)
{

	EnterCriticalSection((LPCRITICAL_SECTION)&m_SendBufCS);

	LeaveCriticalSection((LPCRITICAL_SECTION)&m_SendBufCS);

}

void CClientSocket::Close()
{
	//if(ForceClose() != 0)
	//	SoeckErrorEvent();
	ForceClose();
}

void CClientSocket::PrepareRecv( pBlock Block )
{

	DWORD Transfer;
	DWORD Flage =0 ;
	Block->_enumSocketEvent = seRead;
    Block->_wsabuf.len = MAX_IOCP_SOCKET_BUF;
	Block->_wsabuf.buf = Block->buf;
	memset(&Block->_overlapped,0,sizeof(Block->_overlapped));
	if (m_socket != INVALID_SOCKET)
	{
		if (WSARecv(m_socket,&Block->_wsabuf,1,&Transfer,&Flage,&Block->_overlapped,NULL)==SOCKET_ERROR)
		{
			//说明接收出错
			DWORD ErrorCode = GetLastError();
			if (ErrorCode != ERROR_IO_PENDING)
			{
				//这里说明socketerror;
				Close();
			}
		}
	}


}

pBlock CClientSocket::GetReadBlock()
{
	return &m_ReviceBuf;

}

void CClientSocket::DoClientRead( pBlock Block ,DWORD dwTransfered )
{
//	int SendLen = Block->_wsabuf.len - dwTransfered;
//	if (SendLen == 0)
//	{
//		memmove(Block->buf,Block->_wsabuf.buf,SendLen);

//	}
	if(dwTransfered >0)
	{
		OnReviceEvent(this,Block->buf,dwTransfered);
	}
	PrepareRecv(Block);
}

void CClientSocket::OnReviceEvent( CClientSocket * client ,char* Buf, int Buflen )
{
	OutputDebugStringA(Buf);
}
