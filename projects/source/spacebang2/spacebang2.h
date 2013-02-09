//=============================================================================================================
#ifndef _SPACEBANG2_H_
#define _SPACEBANG2_H_

#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/d3d9/CSpriteFont9.h>
#include <dummyframework/base/CHelper.h>
#include <dummyframework/menu/CFormManager.h>

#include "game.h"
#include "mainmenu.h"
#include "loadingmenu.h"
#include "pausemenu.h"
#include "staticbackground.h"
#include "statisticsmenu.h"
#include "optionsmenu.h"
#include "gameovermenu.h"
#include "chaptermenu.h"
#include "staticlogo.h"

// TODO:
// - shoot(Projectile&) helyett shoot(const Projectile&) söt, érték szerint is lehetne
// - worm is haljon meg
// - CForm-ba a controloknak nem lenne jo inkább set?
// - selectedindex helyett is lehetne iterator (CControl-ra mutatva)
// - titlepos uaz legyen mindenhol (statmenu-beli)
// - chatbubble lent vagy fent; kup az avatárhoz; invertkor?
// - visszamenjen a helyére
// - animatedbuttonban scale-t hogy kérni el? (felujitani a spritefontot)
// - uj fv szovegidöre (pl. vmi diszkrét... >5, >10, >20 ...)
// - enemybe a Hit() nézze meg hogy valid-e
// - élet tünjön el halálkor; hud szövegek is

class SpaceBang2 : public DummyFramework::CGame9
{
	typedef std::vector<gamedesc> gamelist;

	public:
	SpaceBang2();
	~SpaceBang2();

	protected:
	bool Initialize();
	bool LoadContent();
	bool LoadGames(gamedesc& out, const std::string& file);
	bool LoadSettings(const std::string& file);
	bool SaveSettings(const std::string& file);

	void ResizeSprites();
	void ResetRenderStates();
	void Update();
	void Draw();

	private:
	DummyFramework::CFormManager	formmanager;
	DummyFramework::CSpriteFont9	spritefont;
	DummyFramework::CSprite9		menubg;
	DummyFramework::CSprite9		bg;
	DummyFramework::CSprite9		logobg;

	Game							game;
	MainMenu						mainmenu;
	LoadingMenu						loading;
	PauseMenu						pausemenu;
	StaticBackground				background;
	StaticLogo						logo;
	StatisticsMenu					statistics;
	OptionsMenu						options;
	GameOverMenu					gameover;
	ChapterMenu						loadgame;
	gamelist						games;
	size_t							currentgame;
	size_t							currentlevel;

	STICKYKEYS stickykeys;

	_DUMMY_EVENTHANDLERS:
	void onresetdevice();
	void onactivate();
	void ondeactivate();

	void logo_focuslost(DummyFramework::CControl& sender);
	void menu_selected(size_t action);
	void loading_startload();
	void pause_selected(size_t action);
	void statistics_selected(size_t action);
	void gameover_selected(size_t action);
	void chapter_selected(size_t action);
	void options_settingschanged();
};

#endif
//=============================================================================================================
