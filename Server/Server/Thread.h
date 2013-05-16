#pragma once

#include <windows.h>

#define MAX_WAIT_FOR_TIME 6000;

   
   const size_t THREAD_INFO_LEN = 32;

class CThread
{
public:
	CThread(void);
	virtual ~CThread(void);
	//启动线程
    BOOL Start(BOOL bSuspended = FALSE ,unsigned int uStackSize = 0);

	//线程挂起
	BOOL Suspend(void);

	//恢复线程
	BOOL Resume(void);

	//终止线程
	void Terminate(void);

	//等待线程
	void WaitFor(const DWORD dwTimeOut /*= MAX_WAIT_FOR_TIME*/);

	const char * getThreadID() {return (char *) m_dwThreadID;}
	const char * getThreadHandle(){ return (char *)m_hThread;}


protected:
	//线程是否终止
	BOOL Terminated(void);

   //执行过程
	virtual void Execute(void) {} ;
   //终止事件
	virtual void OnTerminate(void) { } ;

	volatile BOOL m_bTerminated;

private:
    //线程ID
	DWORD m_dwThreadID;
    //线程句柄
    HANDLE m_hThread;
	//当前是否挂起
	BOOL m_bIsSuspended;
	//线程过程
	static unsigned WINAPI ThreadProc(LPVOID pParam);



};
