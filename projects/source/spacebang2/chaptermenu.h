//=============================================================================================================
#ifndef _CHAPTERMENU_H_
#define _CHAPTERMENU_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CSprite9.h>

#include "animatedbutton.h"
#include "animatedthumb.h"
#include "gameelement.h"

/**
 * \brief Describes a level
 */
struct leveldesc
{
	std::string file;
	std::string title;
	float texcoords[4];
};

typedef std::vector<leveldesc> levellist;

/**
 * \brief Represents a game
 */
struct gamedesc
{
	LPDIRECT3DTEXTURE9 texture;
	levellist levels;
};

/**
 * \brief Displays playable levels
 */
class ChapterMenu : public DummyFramework::CForm
{
	typedef std::vector<AnimatedThumb> thumblist;

private:
	DummyFramework::syncedanimator<float> alpha;
	DummyFramework::CLabel title;

	AnimatedButton	back;
	thumblist		thumbs;
	textureatlas	atlas;
	size_t			selectedindex;
	int				ticks;

	void SelectedIndexChanged(size_t prev);

public:
	enum formaction
	{
		LoadGame,
		MainMenu
	};

	DummyFramework::CSprite9* Background;
	LPDIRECT3DTEXTURE9 Thumbs;				/*!< \brief Thumbnail atlas */
	size_t CurrentLevel;					/*!< Actual level */

	ChapterMenu();
	~ChapterMenu() {}

	//! Initialize the menu
	bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);

	//! Fill the menu with levels
	void Fill(gamedesc& desc);

	//! Render the menu
	void Draw();

	//! Put the menu into a new state
	void SetState(unsigned int newstate);

	//! Fixed timestep update
	void Update();

_DUMMY_EVENTS:
	DummyFramework::signal1<size_t> selected;

_DUMMY_EVENTHANDLERS:
	void onfocusgained();
	void onfocuslost();
	void onkeyup(const DummyFramework::skeyboardstate& kstate);
	void onresetdevice();
};

#endif
//=============================================================================================================
 
 
