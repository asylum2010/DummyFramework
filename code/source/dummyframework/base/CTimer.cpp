//=============================================================================================================
#include <dummyframework/base/CTimer.h>

namespace DummyFramework
{
    CTimer::CTimer()
    {
        lastelapsedtime     = 0;
        stopped             = true;
        stoptime            = 0;
        basetime            = 0;

        LARGE_INTEGER qwTicksPerSec = { 0, 0 };
        QueryPerformanceFrequency(&qwTicksPerSec);

        tickspersec = qwTicksPerSec.QuadPart;
    }
    //=============================================================================================================
    double CTimer::ElapsedTime()
    {
        LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

        double elapsedtime = (double)(qwTime.QuadPart - lastelapsedtime) / (double)tickspersec;
        lastelapsedtime = qwTime.QuadPart;
        
        if( elapsedtime < 0.0 )
            elapsedtime = 0.0;

        return elapsedtime;
    }
    //=============================================================================================================
    double CTimer::Time()
    {
        LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

        double fAppTime = (double)(qwTime.QuadPart - basetime) / (double)tickspersec;
        return fAppTime;
    }
    //=============================================================================================================
    void CTimer::Reset()
    {
        LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
    
        basetime         = qwTime.QuadPart;
        lastelapsedtime  = qwTime.QuadPart;
        stoptime         = 0;
        stopped          = false;
    }
    //=============================================================================================================
    void CTimer::Start()
    {
        LARGE_INTEGER qwTime = { 0, 0 };
        QueryPerformanceCounter(&qwTime);

        if( stopped )
            basetime += qwTime.QuadPart - stoptime;

        stoptime = 0;
        lastelapsedtime = qwTime.QuadPart;
        stopped = false;
    }
    //=============================================================================================================
    void CTimer::Stop()
    {
        if( !stopped )
        {
            LARGE_INTEGER qwTime = { 0, 0 };
            QueryPerformanceCounter(&qwTime);

            stoptime = qwTime.QuadPart;
            lastelapsedtime = qwTime.QuadPart;
            stopped = true;
        }
    }
    //=============================================================================================================
    LARGE_INTEGER CTimer::GetAdjustedCurrentTime()
    {
        LARGE_INTEGER qwTime;

        if( stoptime != 0 )
            qwTime.QuadPart = stoptime;
        else
            QueryPerformanceCounter(&qwTime);

        return qwTime;
    }
    //=============================================================================================================
}
