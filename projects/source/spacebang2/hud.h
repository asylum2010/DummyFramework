//=============================================================================================================
#ifndef _HUD_H_
#define _HUD_H_

#include <dummyframework/menu/CLabel.h>
#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CSprite9.h>

#include "gameelement.h"
#include "animatedlabel.h"

/**
 * \brief Hovering User Display
 */
class Hud : public DummyFramework::CForm
{
private:
	DummyFramework::CLabel	score;
	AnimatedLabel			message;
	AnimatedLabel			warning;
	D3DXCOLOR				healthcolor;
	D3DXCOLOR				shieldcolor;
	int						ticks;
	float					texcoords[4];

	DummyFramework::syncedanimator<float> alpha;

public:
	enum stateex
	{
		TransitionHalf = Inactive + 1
	};

	DummyFramework::CSprite9*	Side;		/*!< \brief Health bar (side) */
	DummyFramework::CSprite9*	Middle;		/*!< \brief Health bar (middle) */
	DummyFramework::CSprite9*	Panel;		/*!< \brief Black stripes */
	unsigned short				Health;
	unsigned short				Shield;
	unsigned int				Score;

	Hud();
	~Hud() {}

	//! Intiailizes the HUD
	bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);

	//! Announces a message (powerups)
	void Announce(const std::string& str);

	//! Puts a warning on the screen (spacetime distortion)
	void Warn(const std::string& str, size_t delay);

	//! Title text
	void Title(const std::string& str);

	//! Tutorial text
	void Tutorial(const std::string& str);

	//! Resets the HUD
	void Reset();

	//! Changes the HUD's state
	void SetState(unsigned int newstate);

	//! Fixed timestep update
	void Update();

	//! Render
	size_t Write(size_t start, quadbuffer& quad);

_DUMMY_EVENTHANDLERS:
	void onresetdevice();
};

#endif
//=============================================================================================================
