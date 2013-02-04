//=============================================================================================================
#ifndef _EXITGAME_H_
#define _EXITGAME_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/template/defines.h>

#include "gamevariables.h"

class ExitGame : public DummyFramework::CForm
{
private:
	DummyFramework::syncedanimator<float, 3> blink;
	DummyFramework::CDynamicQuadBuffer9 quads;

public:
	LPDIRECT3DTEXTURE9 Atlas;

	DummyFramework::CSprite9* Cell;
	DummyFramework::CSprite9* Glow;

	enum stateex
	{
		BlinkIn = Inactive + 1
	};
	
	ExitGame();
	~ExitGame();

	bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);

	void Draw();
	void SetState(unsigned int newstate);
	void Update();
	
_DUMMY_EVENTHANDLERS:
	void onfocusgained();
	void onfocuslost();
	void onkeyup(const DummyFramework::skeyboardstate& kstate);

	void onlostdevice();
	void onresetdevice();
};

#endif
//=============================================================================================================
 
