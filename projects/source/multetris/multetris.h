//=============================================================================================================
#ifndef _MULTETRIS_H_
#define _MULTETRIS_H_

#define _DUMMY_DX 9
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/menu/CFormManager.h>

#include "staticbackground.h"
#include "mainmenu.h"
#include "pausemenu.h"
#include "versusgame.h"
#include "singlegame.h"
#include "coopgame.h"
#include "exitgame.h"

class MulTetris : public DummyFramework::CGame9
{
public:
	MulTetris();
	~MulTetris();

protected:
	bool Initialize();
	bool LoadContent();
	bool LoadSettings();

	void ResetRenderStates();
	void Update();
	void Draw();

private:
	DummyFramework::CContentManager9::textureatlas atlas;

	DummyFramework::CFormManager	formmanager;
	DummyFramework::CSpriteFont9	spritefont;
	DummyFramework::CSprite9		menubg;
	DummyFramework::CSprite9		bg;
	DummyFramework::CSprite9		panel;

	StaticBackground	background;
	MainMenu			mainmenu;
	PauseMenu			pausemenu;
	VersusGame			versus;
	SingleGame			single;
	CoopGame			coop;
	ExitGame			quit;
	size_t				currentgame;

_DUMMY_EVENTHANDLERS:
	void menu_hover(size_t action);
	void menu_selected(size_t action);
	void pause_selected(size_t action);
};

#endif
//===============================================================================