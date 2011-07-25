//=============================================================================================================
#ifdef _WIN32
#include <dummyframework/thread/CThread.h>

namespace DummyFramework
{
	unsigned long WINAPI CThread::Run(void* param)
	{
		CCallable* obj = reinterpret_cast<CCallable*>(param);
		obj->operator()();

		return 0;
	}
	//=============================================================================================================
	CThread::CThread()
	{
		handle = INVALID_HANDLE_VALUE;
	}
	//=============================================================================================================
	CThread::~CThread()
	{
	}
	//=============================================================================================================
	bool CThread::Attach(CCallable& obj)
	{
		handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&CThread::Run,
			&obj, CREATE_SUSPENDED, &id);

		return (handle != INVALID_HANDLE_VALUE);
	}
	//=============================================================================================================
}

#endif
