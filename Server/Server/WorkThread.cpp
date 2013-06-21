#include "stdafx.h"
#include "WorkThread.h"
#include "ClientSocket.h"
WorkThread::WorkThread(void)
{
}


WorkThread::~WorkThread(void)
{
}

void WorkThread::Execute( void )
{
   DWORD dwBytesTransfered = 0 ;
   CClientSocket *  pSoketContext =new CClientSocket();//NULL;
   pBlock  ov =NULL ;

   if(! m_IOCPSOCKETSERVER->m_bActived)
   {
	   Sleep(2000);
   }
while (!m_bTerminated)
{
	BOOL bReturn = GetQueuedCompletionStatus((HANDLE)m_IOCPSOCKETSERVER->GetIOCPHandle(),
		&dwBytesTransfered,
		(PULONG_PTR)pSoketContext,
		(LPOVERLAPPED * )&ov,
		INFINITE);

	if ((DWORD)ov == SHUTDOWN_FLAG)
	{
		//服务器断开连接
		Terminate();
	}
	if (((DWORD)ov == DISCONNECT_FLAG) &&(pSoketContext))
	{
		//客户端断开连接
		OutputDebugString(L"客户端断开连接");
		continue;
	}
	if ((!bReturn) || (dwBytesTransfered == 0))
	{
	 //可能是客户端断开连接
		OutputDebugString(L"客户端断开连接");
			continue;

	}
	//下边是客户端连接正常读写数据
	if ((pSoketContext)&&(ov))
	{
		OutputDebugString(L"正常读写数据");
		switch (ov->_enumSocketEvent)
		{
		case seRead:
			pSoketContext->DoClientRead(ov,dwBytesTransfered);
		default:
			break;
		}
	}
}
return;
}
