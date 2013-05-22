#include "stdafx.h"
#include "AcceptThread.h"



CAcceptThread::CAcceptThread( CIOCPSocket * ListenSocke )
{
	CThread();
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
	while (!m_bTerminated)
	{
		if (m_IOCPSOCKETSERVER != NULL)
		{
			if(listen(m_IOCPSOCKETSERVER->GetIOCPSocket(),SOMAXCONN) != 0 )
			{
				OutputDebugString(L"listen is Failed!");
			}
			sockaddr * toaddr = NULL;
			toaddr = new sockaddr;
			SOCKET sock = INVALID_SOCKET;
			int BufLen = sizeof(sockaddr) ;
			SOCKET s =m_IOCPSOCKETSERVER->GetIOCPSocket(); 
			sock = WSAAccept(s,toaddr,&BufLen,NULL/*这里是一个回调函数*/,(DWORD_PTR)m_IOCPSOCKETSERVER) ;

			delete(toaddr);
			if (m_bTerminated)
				return;
			if (sock == INVALID_SOCKET)
			{
				OutputDebugString(L"WSAAccept is Failed /n");
				WCHAR buf[1024] = {0};
				wsprintf (buf,L"%d",GetLastError());
				OutputDebugString(buf);
			}
			else
			{
#ifdef DEBUG
				OutputDebugString(L"WSAAccept is Sussecced!\n");
				OutputDebugString(L"\n");
				
#endif // DEBUG
				//下边的过程就是客户端连接上来的存储过程了

			}

		}

	}

}

