//=============================================================================================================
#ifndef _GAMEOVERMENU_H_
#define _GAMEOVERMENU_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CSprite9.h>

#include "animatedbutton.h"
#include "gameelement.h"

/**
 * \brief Menu that appears when the game is over
 */
class GameOverMenu : public DummyFramework::CForm
{
	static const int NUM_BUTTONS = 2;

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
		Restart,
		MainMenu
	};

	DummyFramework::CSprite9* Background;

	GameOverMenu();
	~GameOverMenu() {}

	//! Initialize
	bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);

	//! Render
	void Draw();

	//! Changse state
	void SetState(unsigned int newstate);

	//! Fixed update
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
