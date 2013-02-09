//=============================================================================================================
#ifndef _OPTIONSMENU_H_
#define _OPTIONSMENU_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CSprite9.h>

#include "animatedbutton.h"
#include "gameelement.h"

/**
 * \brief A menu to change settings
 */
class OptionsMenu : public DummyFramework::CForm
{
	struct settings
	{
		std::string quality;
		std::string resolution;
		bool shaders;
		bool fullscreen;
		bool vsync;
	};

	static const int NUM_ROWS = 5;
	static const int NUM_BUTTONS = NUM_ROWS + 2;

private:
	typedef DummyFramework::resolutionset::iterator iterator;

	DummyFramework::resolutionset resolutions;
	DummyFramework::syncedanimator<float> alpha;
	DummyFramework::CLabel title;
	DummyFramework::CLabel labels[NUM_ROWS];

	AnimatedButton	buttons[NUM_BUTTONS];
	settings		current;
	iterator		resolution;
	size_t			selectedindex;
	int				ticks;
	char			modified;

	void QuerySettings();
	void SelectedIndexChanged(size_t prev);

public:
	enum formaction
	{
		Finished
	};

	DummyFramework::CSprite9* Background;

	OptionsMenu();
	~OptionsMenu() {}

	bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);
	bool LoadContent();

	void Draw();
	void SetState(unsigned int newstate);
	void Update();

	inline DummyFramework::resolutionset& GetResolutionSet() {
		return resolutions;
	}

_DUMMY_EVENTS:
	DummyFramework::signal0 settingschanged;

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
