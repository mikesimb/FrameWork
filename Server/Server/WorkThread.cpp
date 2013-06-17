#include "stdafx.h"
#include "WorkThread.h"
WorkThread::WorkThread(void)
{
}


WorkThread::~WorkThread(void)
{
}

void WorkThread::Execute( void )
{
   DWORD dwBytesTransfered = 0 ;
   PULONG_PTR pSoketContext = 0;
   Block * pOverlapped  = NULL;
   LPOVERLAPPED * ov = NULL;
   ov = (LPOVERLAPPED *)pOverlapped;
   if(! m_IOCPSOCKETSERVER->m_bActived)
   {
	   Sleep(2000);
   }
while (!m_bTerminated)
{
	BOOL bReturn = GetQueuedCompletionStatus((HANDLE)m_IOCPSOCKETSERVER->GetIOCPHandle(),
		&dwBytesTransfered,
		pSoketContext,
		ov,
		INFINITE);

	if ((DWORD)ov == SHUTDOWN_FLAG)
	{
		Terminate();
	}


}
	return;
}
