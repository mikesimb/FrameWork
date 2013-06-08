#include "StdAfx.h"
#include "IOCPSocket.h"

// �ͷ�ָ���
#define RELEASE(x)                      {if(x != NULL ){delete x;x=NULL;}}
// �ͷž����
#define RELEASE_HANDLE(x)               {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
// �ͷ�Socket��
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
// 	//��listensokcet ��IOCP����һ��
// 	HANDLE temp = CreateIoCompletionPort((HANDLE)m_listensocket,m_IOCPHandle,(DWORD)this,0);
// 	if(temp == NULL)
// 	{
// 		OutputDebugString(L"�� Listen Socket����ɶ˿�ʧ�ܣ�");
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
		// ����ر���Ϣ֪ͨ
		//SetEvent(m_hShutdownEvent);

// 		for (int i = 0; i < m_nThreads; i++)
// 		{
// 			// ֪ͨ���е���ɶ˿ڲ����˳�
// 			PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
// 		}

		// �ȴ����еĿͻ�����Դ�˳�
		closesocket(m_listensocket);
		m_AcceptThread->Terminate();
		
		WaitForSingleObject((HANDLE)m_AcceptThread->getThreadID(),INFINITE);

//�ͷŹ����߳�
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

		// ����ͻ����б���Ϣ
		//this->_ClearContextList();

		// ɾ���ͻ����б�Ļ�����
		//DeleteCriticalSection(&m_csContextList);

		// �ر�ϵͳ�˳��¼����
	//	RELEASE_HANDLE(m_hShutdownEvent);

		// �ͷŹ������߳̾��ָ��
// 		for( int i=0;i<m_nThreads;i++ )
// 		{
// 			RELEASE_HANDLE(m_phWorkerThreads[i]);
// 		}
		// ɾ���ͻ����б�Ļ�����
//		DeleteCriticalSection(&m_csContextList);

		// �ر�ϵͳ�˳��¼����
	//	RELEASE_HANDLE(m_hShutdownEvent);

		// �ͷŹ������߳̾��ָ��
// 		for( int i=0;i<m_nThreads;i++ )
// 		{
// 			RELEASE_HANDLE(m_phWorkerThreads[i]);
// 		}

// 		RELEASE(m_phWorkerThreads);

		// �ر�IOCP���
// 		RELEASE_HANDLE(m_hIOCompletionPort);

		// �رռ���Socket
// 		RELEASE(m_pListenContext);

// 		this->_ShowMessage("�ͷ���Դ���.\n");
// 		RELEASE(m_phWorkerThreads);

		// �ر�IOCP���
// 		RELEASE_HANDLE(m_hIOCompletionPort);

		// �رռ���Socket
// 		RELEASE(m_pListenContext);
// 
// 		this->_ShowMessage("�ͷ���Դ���.\n");
// 
// 		this->_ShowMessage("ֹͣ����\n");
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


