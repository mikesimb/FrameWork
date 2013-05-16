#pragma once

#include <windows.h>

#define MAX_WAIT_FOR_TIME 6000;

   
   const size_t THREAD_INFO_LEN = 32;

class CThread
{
public:
	CThread(void);
	virtual ~CThread(void);
	//�����߳�
    BOOL Start(BOOL bSuspended = FALSE ,unsigned int uStackSize = 0);

	//�̹߳���
	BOOL Suspend(void);

	//�ָ��߳�
	BOOL Resume(void);

	//��ֹ�߳�
	void Terminate(void);

	//�ȴ��߳�
	void WaitFor(const DWORD dwTimeOut /*= MAX_WAIT_FOR_TIME*/);

	const char * getThreadID() {return (char *) m_dwThreadID;}
	const char * getThreadHandle(){ return (char *)m_hThread;}


protected:
	//�߳��Ƿ���ֹ
	BOOL Terminated(void);

   //ִ�й���
	virtual void Execute(void) {} ;
   //��ֹ�¼�
	virtual void OnTerminate(void) { } ;

	volatile BOOL m_bTerminated;

private:
    //�߳�ID
	DWORD m_dwThreadID;
    //�߳̾��
    HANDLE m_hThread;
	//��ǰ�Ƿ����
	BOOL m_bIsSuspended;
	//�̹߳���
	static unsigned WINAPI ThreadProc(LPVOID pParam);



};
