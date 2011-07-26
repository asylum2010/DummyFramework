//=============================================================================================================
#ifndef _CTIMER_H_
#define _CTIMER_H_

#include <windows.h>

namespace DummyFramework
{
	/**
	 * \brief Win32-based high resolution timer
	 */
    class CTimer
    {
    protected:
		bool stopped;
        LONGLONG tickspersec;
        LONGLONG lastelapsedtime;
        LONGLONG stoptime;
        LONGLONG basetime;
    
        LARGE_INTEGER GetAdjustedCurrentTime();

    public:
        CTimer();
		~CTimer() {}

        double ElapsedTime();
        double Time();

        void Reset();
        void Start();
        void Stop();

        inline bool IsStopped() const {
			return stopped;
		}
    };
}

#endif
//=============================================================================================================
 
