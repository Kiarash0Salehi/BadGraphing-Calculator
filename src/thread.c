#include <windows.h>
#include <process.h>
#include "header/global.h"


void startThread(thread* Thread, void* data)
{
	Thread->thread_id = (HANDLE)_beginthread(Thread->func, 0, data);
}

void removeThread(thread* Thread)
{
	CloseHandle(Thread->thread_id);
}

BOOL stopThread(const thread* Thread)
{
	DWORD e = SuspendThread(Thread->thread_id);
	return (e > (DWORD)-1) ? TRUE : FALSE;
}

BOOL resumeThread(const thread* Thread)
{
	DWORD e = ResumeThread(Thread->thread_id);
	return (e > (DWORD)-1) ? TRUE : FALSE;
}
