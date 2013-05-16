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
	//TODO:这里需要做几个工作线程
}

bool CIOCPSocket::InitListenSocket()
{
   //初始化listenSocket
	m_listensocket = WSASocket(AF_INET ,SOCK_STREAM ,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(m_listensocket == INVALID_SOCKET)
	{
		OutputDebugString(L"WSASocket is Failed!");
		return false;
	}
	//将listensokcet 和IOCP绑定在一起
	HANDLE temp = CreateIoCompletionPort((HANDLE)m_listensocket,m_IOCPHandle,(DWORD)this,0);
	if(temp == NULL)
	{
		OutputDebugString(L"绑定 Listen Socket至完成端口失败！");
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

	//接下来的listen 和ACCEPT函数的执行都可以放到线程中去执行如果这个放到线程中去执行了，那么还需要跟当前的CreateIoCompletionPort绑定吗?





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


