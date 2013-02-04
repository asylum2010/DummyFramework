//=============================================================================================================
#include <dummyframework/base/CTimer.h>

namespace DummyFramework
{
	static LARGE_INTEGER qwTime;

	CTimer::CTimer()
	{
		stopped		= true;
		stoptime	= 0;
		basetime	= 0;

		LARGE_INTEGER qwTicksPerSec = { { 0, 0 } };
		QueryPerformanceFrequency(&qwTicksPerSec);

		tickspersec = qwTicksPerSec.QuadPart;
	}
	//=============================================================================================================
	double CTimer::Time()
	{
		if( stopped )
			qwTime.QuadPart = stoptime;
		else
			QueryPerformanceCounter(&qwTime);

		return (double)(qwTime.QuadPart - basetime) / (double)tickspersec;
	}
	//=============================================================================================================
	double CTimer::TimeNano()
	{
		if( stopped )
			qwTime.QuadPart = stoptime;
		else
			QueryPerformanceCounter(&qwTime);

		return (double)(qwTime.QuadPart - basetime);
	}
	//=============================================================================================================
	void CTimer::Reset()
	{
		basetime	= 0;
		stoptime	= 0;
		stopped		= true;
	}
	//=============================================================================================================
	void CTimer::Start()
	{
		QueryPerformanceCounter(&qwTime);

		if( stopped )
			basetime += qwTime.QuadPart - stoptime;

		stopped = false;
	}
	//=============================================================================================================
	void CTimer::Stop()
	{
		if( !stopped )
		{
			QueryPerformanceCounter(&qwTime);

			stoptime	= qwTime.QuadPart;
			stopped		= true;
		}
	}
	//=============================================================================================================
}
