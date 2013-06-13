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
   DWORD pSoketContext = 0;
	OVERLAPPED * pOverlapped = NULL;
   if(! m_IOCPSOCKETSERVER->m_bActived)
   {
	   Sleep(2000);
   }
while (!m_bTerminated)
{
	BOOL bReturn = GetQueuedCompletionStatus((HANDLE)m_IOCPSOCKETSERVER->GetIOCPHandle(),
		&dwBytesTransfered,
		(PULONG_PTR)&pSocketContext,
		&pOverlapped,
		INFINITE);

}
	return;
}
