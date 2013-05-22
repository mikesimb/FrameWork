#include "StdAfx.h"
#include "Thread.h"
#include <Process.h>

	CThread::CThread(void)
	{
		m_dwThreadID = 0 ;
		m_hThread = INVALID_HANDLE_VALUE ;
		m_bTerminated  = TRUE;
		m_bIsSuspended = false;
	}

	CThread::~CThread(void)
	{
		if (m_hThread != INVALID_HANDLE_VALUE)
		{
			if (m_bIsSuspended) 
			{
				Resume();
			}
			Terminate();
			DWORD time = MAX_WAIT_FOR_TIME;
			WaitFor(time);
		}

	}

	BOOL CThread::Start( BOOL bSuspended /*= FALSE */,unsigned int uStackSize /*= 0*/ )
	{
	   unsigned int dwThreadID;
	   int iFlag = 0 ;
	   if (bSuspended)
	   {
		   iFlag = CREATE_SUSPENDED;
		   m_bIsSuspended = true;

	   }
	   else
	   {
		   m_bIsSuspended = false;
	   }
	   HANDLE hThread = (HANDLE)_beginthreadex(NULL ,uStackSize ,ThreadProc ,(void*)this ,iFlag ,&dwThreadID);

	   if(hThread == INVALID_HANDLE_VALUE)
		   return false;
	   m_dwThreadID = dwThreadID;
	   m_hThread = hThread;
	   return true;

	}

	BOOL CThread::Suspend( void )
	{
		if ( SuspendThread(m_hThread) !=(DWORD) -1 )
		{
			m_bIsSuspended = TRUE;
			return TRUE;
		}
		else
		{
			m_bIsSuspended = FALSE;
			return FALSE;
		}
	}

	BOOL CThread::Resume( void )
	{
	   if (ResumeThread(m_hThread) != (DWORD) -1)
	   {
		   m_bIsSuspended = FALSE;
		   return TRUE;
	   }
	   else
	   {
		   m_bIsSuspended = TRUE;
		   return  FALSE;
	   }
	}

	void CThread::Terminate( void )
	{
		::InterlockedExchange((LONG *)(&m_bTerminated),TRUE);
		//_endthreadex(0);

		
	}

	BOOL CThread::Terminated( void )
	{
	  return m_bTerminated ;
	}

	void CThread::WaitFor( const DWORD dwTimeOut /*= MAX_WAIT_FOR_TIME*/)
	{
	   if(m_hThread != INVALID_HANDLE_VALUE)
	   {
		   WaitForSingleObject(m_hThread,dwTimeOut);
	   }

	}

	unsigned WINAPI CThread::ThreadProc( LPVOID pParam )
	{
	   CThread * pThead =(CThread *) pParam;
	   ::InterlockedExchange((LONG *)&(pThead->m_bTerminated) , FALSE);
	   try
	   {


		   pThead->Execute();

	   }
	   catch(...)
	   {

	   }
	   try
	   {
		   pThead->OnTerminate();
	   }
	   catch(...) 
	   {
	   	
	   }
	   //CloseHandle(pThead->m_hThread);
	   _endthreadex(0);
	   pThead->m_hThread = INVALID_HANDLE_VALUE;
	   pThead->m_dwThreadID = 0 ;
	   pThead->m_bTerminated = true;
	  // pThead->m_dwThreadID  = 0 ;
	  // pThead->m_bTerminated = TRUE:
	   return 0;
	   
	}

