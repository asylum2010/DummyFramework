//=============================================================================================================
#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CSprite9.h>

#include "animatedbutton.h"
#include "gameelement.h"

/**
 * \brief Appears when the game is paused
 */
class PauseMenu : public DummyFramework::CForm
{
	static const int NUM_BUTTONS = 3;

private:
	DummyFramework::syncedanimator<float> alpha;
	DummyFramework::CLabel title;

	AnimatedButton	buttons[NUM_BUTTONS];
	size_t			selectedindex;
	int				ticks;

	void SelectedIndexChanged(size_t prev);

public:
	enum formaction
	{
		Continue,
		Restart,
		MainMenu
	};

	DummyFramework::CSprite9* Background;

	PauseMenu();
	~PauseMenu() {}

	bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);

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
	void onmousemove(const DummyFramework::smousestate& mstate);
};

#endif
//=============================================================================================================
 
 
