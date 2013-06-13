#pragma once
#include <WinSock2.h>
#include "AcceptThread.h"
#include "WorkThread.h"
#include <list>
#pragma comment(lib,"ws2_32.lib")
const int  MAX_IOCP_SOCKET_BUF = 8*1024;


class CAcceptThread;
class WorkThread;

typedef  enum SOCKETEVENT
{
	seInitIOPort,seInitSocket
	,seConnect,seDisconnect,
	seListen,seAccept,
	seRead,seWrite
};

typedef struct structBlock
{
	OVERLAPPED _overlapped;
	WSABUF   _wsabuf;
	SOCKETEVENT _enumSocketEvent;
	char buf[MAX_IOCP_SOCKET_BUF];

} Block,*pBlock ;
class CIOCPSocket
{
public:
	CIOCPSocket(void);
	virtual ~CIOCPSocket(void);


	bool InitSocketLib();
	bool InitCompleteIOCP();
	bool InitListenSocket();
	SOCKET GetIOCPSocket();
	HANDLE GetIOCPHandle();

	bool CloseListenSocket();

	bool Init();
	//���ط�������IP��
	CStringA m_strIpaddress;
	//���ط������Ķ˿ںţ�
	int   m_iport;    
	bool m_bActived;
protected:

	bool InitWorkThread();
	int _GetNoOfProcessors();


private:
	//����Ǽ����õ�SOCKET;
	SOCKET m_listensocket;
	HANDLE  m_IOCPHandle;
    sockaddr_in m_listensockaddr; 
	CAcceptThread * m_AcceptThread;
	CList<WorkThread* > m_WorkThreadList;
};

