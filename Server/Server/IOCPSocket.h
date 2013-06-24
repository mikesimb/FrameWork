#pragma once
#include <WinSock2.h>
#include "AcceptThread.h"
#include "WorkThread.h"
#include <list>
#include <string>
#include <map>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
const int  MAX_IOCP_SOCKET_BUF = 8*1024;

const DWORD DISCONNECT_FLAG = 0xFFFFFFFE;
const DWORD SHUTDOWN_FLAG   = 0xFFFFFFEE;



class CAcceptThread;
class WorkThread;

class CClientSocket;

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


typedef struct SendQueueNode
{
	char * szBuf;
	int       ibuflen;
	int       iStartPosition;
    SendQueueNode * Next;
} *pSendQueueNode;


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

	void AcceptSocket(SOCKET _socket,string ipaddr,int port);


	


	void ClientSocketReviceData(CClientSocket * client ,char * Buf, int Buflen);

	bool Init();
	//本地服务器的IP；
	CStringA m_strIpaddress;
	//本地服务器的端口号；
	int   m_iport;    
	bool m_bActived;
	 list<CClientSocket * > m_ActiveClientMap;
	 void SendBufferForErery();
protected:

	bool InitWorkThread();
	int _GetNoOfProcessors();
	

private:
	//这个是监听用的SOCKET;
	SOCKET m_listensocket;
	HANDLE  m_IOCPHandle;
    sockaddr_in m_listensockaddr; 
	CAcceptThread * m_AcceptThread;
	CList<WorkThread* > m_WorkThreadList;

	CRITICAL_SECTION   m_ClientCS;

	
   

};

