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
		//�������Ͽ�����
		Terminate();
	}
	if (((DWORD)ov == DISCONNECT_FLAG) &&(pSoketContext))
	{
		//�ͻ��˶Ͽ�����
		OutputDebugString(L"�ͻ��˶Ͽ�����");
		continue;
	}
	if ((!bReturn) || (dwBytesTransfered == 0))
	{
	 //�����ǿͻ��˶Ͽ�����
		OutputDebugString(L"�ͻ��˶Ͽ�����");
			continue;

	}
	//�±��ǿͻ�������������д����
	if ((pSoketContext)&&(ov))
	{
		OutputDebugString(L"������д����");
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
