//=============================================================================================================
#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CSprite9.h>

#include "animatedbutton.h"
#include "gameelement.h"

/**
 * \brief Main menu
 */
class MainMenu : public DummyFramework::CForm
{
	static const int NUM_BUTTONS = 4;

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
		NewGame,
		LoadGame,
		Options,
		Quit
	};

	DummyFramework::CSprite9* Background;

	MainMenu();
	~MainMenu() {}

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
