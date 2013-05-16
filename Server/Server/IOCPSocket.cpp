#include "StdAfx.h"
#include "IOCPSocket.h"


CIOCPSocket::CIOCPSocket(void)
{
	m_AcceptThread = NULL;
}


CIOCPSocket::~CIOCPSocket(void)
{
}

bool CIOCPSocket::InitSocketLib()
{
	WORD wVersionRequested = MAKEWORD(2,2);
	WSADATA Wsadata;
	int Err = WSAStartup(wVersionRequested,&Wsadata);
	if (Err != NO_ERROR)
	{
		OutputDebugString(L"WSAStartup is Error");
		return false;
	}
	return true;
}

bool CIOCPSocket::InitCompleteIOCP()
{
	m_IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,NULL,0);
	if (m_IOCPHandle == NULL)
	{
		OutputDebugString(L"CreateIoCompletePort is Failed!");
		return false;
	}
	return true;
	//TODO:������Ҫ�����������߳�
}

bool CIOCPSocket::InitListenSocket()
{
   //��ʼ��listenSocket
	m_listensocket = WSASocket(AF_INET ,SOCK_STREAM ,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(m_listensocket == INVALID_SOCKET)
	{
		OutputDebugString(L"WSASocket is Failed!");
		return false;
	}
	//��listensokcet ��IOCP����һ��
	HANDLE temp = CreateIoCompletionPort((HANDLE)m_listensocket,m_IOCPHandle,(DWORD)this,0);
	if(temp == NULL)
	{
		OutputDebugString(L"�� Listen Socket����ɶ˿�ʧ�ܣ�");
		return false;
	}

	ZeroMemory(&m_listensockaddr,sizeof(sockaddr_in));

	m_listensockaddr.sin_family = AF_INET;
	m_listensockaddr.sin_port = htons(m_iport);
	m_listensockaddr.sin_addr.S_un.S_addr = inet_addr(m_strIpaddress.GetString());
	if(SOCKET_ERROR == bind(m_listensocket,(sockaddr *)&m_listensockaddr,sizeof(m_listensockaddr)))
	{
		OutputDebugString(L"Bind is Failed!");
		return false;
	}
	m_AcceptThread = new CAcceptThread(this);

	//��������listen ��ACCEPT������ִ�ж����Էŵ��߳���ȥִ���������ŵ��߳���ȥִ���ˣ���ô����Ҫ����ǰ��CreateIoCompletionPort����?





	return true;



}

SOCKET CIOCPSocket::GetIOCPSocket()
{
	return m_listensocket;
}

bool CIOCPSocket::Init()
{
	if (InitSocketLib())
		if(	InitCompleteIOCP())
			if	(InitListenSocket())
				   return true;

return false;

}


