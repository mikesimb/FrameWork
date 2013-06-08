#include "StdAfx.h"
#include "IOCPSocket.h"

// 释放指针宏
#define RELEASE(x)                      {if(x != NULL ){delete x;x=NULL;}}
// 释放句柄宏
#define RELEASE_HANDLE(x)               {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
// 释放Socket宏
#define RELEASE_SOCKET(x)               {if(x !=INVALID_SOCKET) { closesocket(x);x=INVALID_SOCKET;}}

#define  WORKER_THREADS_PER_PROCESSOR 2;

CIOCPSocket::CIOCPSocket(void)
{
	m_AcceptThread = NULL;
	m_bActived = false;
}


CIOCPSocket::~CIOCPSocket(void)
{
	if (m_AcceptThread)
	delete(m_AcceptThread);
	m_AcceptThread = NULL;
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
// 	//将listensokcet 和IOCP绑定在一起
// 	HANDLE temp = CreateIoCompletionPort((HANDLE)m_listensocket,m_IOCPHandle,(DWORD)this,0);
// 	if(temp == NULL)
// 	{
// 		OutputDebugString(L"绑定 Listen Socket至完成端口失败！");
// 		return false;
// 	}

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
	m_AcceptThread->Start(true);
	m_AcceptThread->Resume();

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
			{
				
				InitWorkThread();
				m_bActived = true; 
				return true;

			}

return false;

}

bool CIOCPSocket::CloseListenSocket()
{
	if( m_listensocket!=INVALID_SOCKET )
	{
		// 激活关闭消息通知
		//SetEvent(m_hShutdownEvent);

// 		for (int i = 0; i < m_nThreads; i++)
// 		{
// 			// 通知所有的完成端口操作退出
// 			PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
// 		}

		// 等待所有的客户端资源退出
		closesocket(m_listensocket);
		m_AcceptThread->Terminate();
		
		WaitForSingleObject((HANDLE)m_AcceptThread->getThreadID(),INFINITE);

//释放工作线程
	//	int i ; 
       while (m_WorkThreadList.GetCount() > 0 )
	   {
			WorkThread * Thread = m_WorkThreadList.GetHead();
			if (Thread)
			{
				Thread->Terminate();
				WaitForSingleObject((HANDLE)Thread->getThreadID(),INFINITE);

			}
			delete Thread;
			//RELEASE(Thread);
			m_WorkThreadList.RemoveHead();
	   }
		//RELEASE_SOCKET(m_listensocket);
		//WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

		// 清除客户端列表信息
		//this->_ClearContextList();

		// 删除客户端列表的互斥量
		//DeleteCriticalSection(&m_csContextList);

		// 关闭系统退出事件句柄
	//	RELEASE_HANDLE(m_hShutdownEvent);

		// 释放工作者线程句柄指针
// 		for( int i=0;i<m_nThreads;i++ )
// 		{
// 			RELEASE_HANDLE(m_phWorkerThreads[i]);
// 		}
		// 删除客户端列表的互斥量
//		DeleteCriticalSection(&m_csContextList);

		// 关闭系统退出事件句柄
	//	RELEASE_HANDLE(m_hShutdownEvent);

		// 释放工作者线程句柄指针
// 		for( int i=0;i<m_nThreads;i++ )
// 		{
// 			RELEASE_HANDLE(m_phWorkerThreads[i]);
// 		}

// 		RELEASE(m_phWorkerThreads);

		// 关闭IOCP句柄
// 		RELEASE_HANDLE(m_hIOCompletionPort);

		// 关闭监听Socket
// 		RELEASE(m_pListenContext);

// 		this->_ShowMessage("释放资源完毕.\n");
// 		RELEASE(m_phWorkerThreads);

		// 关闭IOCP句柄
// 		RELEASE_HANDLE(m_hIOCompletionPort);

		// 关闭监听Socket
// 		RELEASE(m_pListenContext);
// 
// 		this->_ShowMessage("释放资源完毕.\n");
// 
// 		this->_ShowMessage("停止监听\n");
	}	
	m_bActived = false;

	return true;
}

bool CIOCPSocket::InitWorkThread()
{
	int num = _GetNoOfProcessors();
	int processNum = WORKER_THREADS_PER_PROCESSOR;
	int m_nThreadnum = (processNum * num);
    int  i ;
	for(i =0 ;i<m_nThreadnum;i++)
	{
		WorkThread * thread = new WorkThread;
		thread->Start(true);
		thread->Resume();
		
		m_WorkThreadList.AddHead(thread);

	}
    return true;
}

int CIOCPSocket::_GetNoOfProcessors()
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;

}


