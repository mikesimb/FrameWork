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

	EnterCriticalSection(&m_SendBufCS);
	m_bSending = false;
	while(m_FirstNode)
	{
		SendQueueNode *node = m_FirstNode;
		int iRemainLen = MAX_IOCP_SOCKET_BUF - iSendLen;
		int iDataLen = node->ibuflen - node->iStartPosition;
		if (iDataLen > iRemainLen)
		{
			memmove(block->buf+iSendLen,node->szBuf+node->iStartPosition,iRemainLen);
			iSendLen = MAX_IOCP_SOCKET_BUF;
			node->iStartPosition += iRemainLen;
			break;
		}
		else
		{
			memmove(block->buf+iSendLen,node->szBuf+node->iStartPosition,iDataLen);
			iSendLen += iDataLen;
			m_FirstNode = node->Next;
			if(m_FirstNode == NULL)
				m_LastNode = NULL;
			free(node->szBuf);
			free(node);

		}
	}

	m_iTotleBufLen -= iSendLen;
	
	if(iSendLen > 0)
	{
		m_bSending =true;
		block->_enumSocketEvent = seWrite;
		block->_wsabuf.buf = block->buf;
		block->_wsabuf.len = iSendLen;
		memset(&block->_overlapped,0,sizeof(block->_overlapped));

		if (m_socket != INVALID_SOCKET)
		{
			DWORD Trasnfer = 0 ;
			if (WSASend(m_socket,&block->_wsabuf,1,&Trasnfer,0,&block->_overlapped,NULL) == SOCKET_ERROR )
			{
				DWORD iError = GetLastError();
				OnSocketError(iError);
				Close();
			}

		}

	}

	LeaveCriticalSection(&m_SendBufCS);

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
				OnSocketError(ErrorCode);
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
	//Sleep(3000);
	//char * sendBuf = new char[100];
	//sendBuf = "copy that copy that !!!!";
	//SendBuf(sendBuf , 20,false);
	//delete(sendBuf); 

}


void CClientSocket::SendBuf( char * Buf, int len, bool FreeAfterSend )
{
	//如果长度小于0 或者 SOCKE == invalid_socket那么退出
	if ((len <= 0 ) || (m_socket==INVALID_SOCKET))
	{
		if (FreeAfterSend)
		{
			//这里不是用NEW创建的使用MALLOC			
			free(Buf);
		}

	}
	SendQueueNode * node = new SendQueueNode();
	node->ibuflen = len;
	node->iStartPosition = 0 ;
	node->Next = NULL;
	node->szBuf = (char*)malloc(len);
	memmove(node->szBuf,Buf,len);
if (FreeAfterSend)
	{
		delete(Buf);
	}
	//加入到发送队列里
	EnterCriticalSection(&m_SendBufCS);
	m_iTotleBufLen += len;
	m_iTotleNodeCount += 1;
	if (m_LastNode)
	{
		m_LastNode->Next = node;
	}
	if (!m_FirstNode)
	{
		m_FirstNode = node;
	}
	m_LastNode = node;
	//if(!m_bSending)
	{
		PrepareSend(&m_SendBuf,0);
	}
	
	LeaveCriticalSection(&m_SendBufCS);

}

void CClientSocket::OnSocketError( DWORD ErrorCode )
{

}

void CClientSocket::DoClientWrite( pBlock Block,DWORD dwTransfered )
{
	int iSendLen = Block->_wsabuf.len - dwTransfered;
	if (iSendLen>0)
		memmove(Block->buf+dwTransfered,Block->buf,iSendLen);
    PrepareSend(Block,iSendLen);
}


