#include "stdafx.h"
#include "AcceptThread.h"



CAcceptThread::CAcceptThread( CIOCPSocket * ListenSocke )
{
	m_IOCPSOCKETSERVER = NULL;
	if(ListenSocke != NULL)
	{
		m_IOCPSOCKETSERVER = ListenSocke;
	}
	
}


CAcceptThread::~CAcceptThread(void)
{
	m_IOCPSOCKETSERVER = NULL;

}



void CAcceptThread::Execute( void )
{

	CThread::Execute();
	while (m_bTerminated)
	{
		if (m_IOCPSOCKETSERVER != NULL)
		{
			if(listen(m_IOCPSOCKETSERVER->GetIOCPSocket(),SOMAXCONN) == 0 )
			{
				OutputDebugString(L"listen is Failed");
			}
			sockaddr * toaddr = NULL;
			SOCKET sock = INVALID_SOCKET;
			int BufLen = 0 ;
			sock = WSAAccept(m_IOCPSOCKETSERVER->GetIOCPSocket(),toaddr,&BufLen,NULL,(DWORD_PTR)m_IOCPSOCKETSERVER) ;
			if (sock == INVALID_SOCKET)
			{
				OutputDebugString(L"WSAAccept is Failed");
			}
			else
			{
#ifdef DEBUG
				OutputDebugString(L"WSAAccept is Failed!");
#endif // DEBUG
				//下边的过程就是客户端连接上来的存储过程了

			}

		}

	}

}

