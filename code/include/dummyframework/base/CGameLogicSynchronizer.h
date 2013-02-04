//=============================================================================================================
#ifndef _CGAMELOGICSYNCHRONIZER_H_
#define _CGAMELOGICSYNCHRONIZER_H_

#include "../template/signal.hpp"
#include "CTimer.h"

namespace DummyFramework
{
	/**
	 * \brief Update scheduler
	 *
	 * This class performs the fixed timestep
	 * updateing of the game's logic.
	 *
	 * Note that if your game has a menu then
	 * you should create another synchronizer
	 * object for the game, and use the
	 * one in the game's main class (inherited from CGame9)
	 * only for the menu.
	 */
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

	_DUMMY_EVENTS:
		signal0 synchronize;
	};
}

#endif
//=============================================================================================================


