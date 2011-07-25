//=============================================================================================================
#ifndef _CGAMELOGICSYNCHRONIZER_H_
#define _CGAMELOGICSYNCHRONIZER_H_

#include "../template/signal.hpp"
#include "CTimer.h"

namespace DummyFramework
{
    class CGameLogicSynchronizer
    {
    private:
        CTimer clock;
        double currenttime;
        double accumulator;
        double newtime;
        double lasttime;
        double lastfps;

        unsigned int fps;
        unsigned int frames;

    public:
        CGameLogicSynchronizer();
		~CGameLogicSynchronizer() {}

        double UpdateInterval;

	    bool Elapsed(double interval, bool reset = true);

        void Reset();
        void Update();
        void Jump();

        inline unsigned int Fps() const {
			return fps;
		}

        inline double Alpha() const {
			return accumulator / UpdateInterval;
		}

        inline CTimer& Timer() {
			return clock;
		}

	events:
		signal0 synchronize;
    };
}

#endif
//=============================================================================================================


