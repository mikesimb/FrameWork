#include "stdafx.h"
#include "AcceptThread.h"
#include "ClientSocket.h"



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
			SOCKADDR_IN  toaddr ;//= NULL;
			SOCKET sock = INVALID_SOCKET;
			int BufLen = sizeof(sockaddr) ;
			SOCKET s =m_IOCPSOCKETSERVER->GetIOCPSocket(); 
			sock = WSAAccept(s,(sockaddr *)&toaddr,&BufLen,NULL/*������һ���ص�����*/,(DWORD_PTR)m_IOCPSOCKETSERVER) ;
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
				//�±ߵĹ��̾��ǿͻ������������Ĵ洢������
				char * str;
				str = inet_ntoa(toaddr.sin_addr);
				m_IOCPSOCKETSERVER->AcceptSocket(sock,str,toaddr.sin_port);

			}

		}

	}

}

