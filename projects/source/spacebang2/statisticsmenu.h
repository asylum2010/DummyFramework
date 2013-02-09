//=============================================================================================================
#ifndef _STATISTICSMENU_H_
#define _STATISTICSMENU_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CSprite9.h>

#include "animatedbutton.h"
#include "gameelement.h"

/**
 * \brief Displays some statistics
 */
class StatisticsMenu : public DummyFramework::CForm
{
	struct entry
	{
		DummyFramework::CLabel key;
		DummyFramework::CLabel value;
	};

	static const int NUM_ROWS = 5;

private:
	DummyFramework::syncedanimator<float> alpha;
	DummyFramework::CLabel title;

	AnimatedButton	cont;
	entry			entries[NUM_ROWS];
	int				ticks;

public:
	enum formaction
	{
		Continue
	};

	DummyFramework::CSprite9* Background;

	StatisticsMenu();
	~StatisticsMenu() {}

	bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);
	bool LoadContent();

	void Draw();
	void SetState(unsigned int newstate);
	void Update();

_DUMMY_EVENTS:
	DummyFramework::signal1<size_t> selected;

_DUMMY_EVENTHANDLERS:
	void onfocusgained();
	void onfocuslost();
	void onresetdevice();

	void onkeyup(const DummyFramework::skeyboardstate& kstate);
	void onmouseup(const DummyFramework::smousestate& mstate);
};

#endif
//=============================================================================================================
