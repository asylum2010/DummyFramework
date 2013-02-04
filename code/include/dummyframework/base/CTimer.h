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
		LONGLONG stoptime;
		LONGLONG basetime;

	public:
		CTimer();
		~CTimer() {}

		double Time();
		double TimeNano();

		void Reset();
		void Start();
		void Stop();

		inline bool IsStopped() const {
			return stopped;
		}

		inline double Nano() const {
			return 1.0 / (double)tickspersec;
		}
	};
}

#endif
//=============================================================================================================
 
