//=============================================================================================================
#ifndef _COOPGAME_H_
#define _COOPGAME_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/menu/CLabel.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/template/defines.h>

#include "tetromino.h"
#include "multitable.h"
#include "gamevariables.h"

class CoopGame : public DummyFramework::CForm
{
	typedef union
	{
		unsigned char a[4];
		unsigned int b;
	} binding;

	struct player
	{
		binding		bindings;
		char		movedir;
		bool		rotated;
		bool		firststep;
	};

private:
	MultiTable	table;
	player		players[2];
	int			ticks;
	int			gametime;
	bool		juststarted;

	DummyFramework::syncedanimator<float> blink;
	DummyFramework::CDynamicQuadBuffer9 quads;
	DummyFramework::CLabel labels[5];

	void ResetGUI();
	size_t DrawTetro(size_t start, const Tetromino& tetro, const D3DXVECTOR2& pos);

public:
	LPDIRECT3DTEXTURE9 Atlas;

	DummyFramework::CSprite9* Cell;
	DummyFramework::CSprite9* Glow;

	enum stateex
	{
		BlinkIn = Inactive + 1,
		BlinkOut = Inactive + 2,
		Dead = Inactive + 3
	};

	enum formaction
	{
		PauseMenu,
		GameOver
	};

	enum inputaction
	{
		Left = 0,
		Right = 1,
		Down = 2,
		Rotate = 3
	};

	CoopGame();
	~CoopGame();

	bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);
	bool LoadContent();

	void Draw();
	void Reset(bool clear);
	void SetState(unsigned int newstate);
	void Update();
	
	inline void Bind(size_t player, inputaction act, unsigned char key) {
		players[player].bindings.a[act] = key;
	}

_DUMMY_EVENTHANDLERS:
	void onfocusgained();
	void onfocuslost();
	void onkeyup(const DummyFramework::skeyboardstate& kstate);
	void onkeydown(const DummyFramework::skeyboardstate& kstate);

	void onlostdevice();
	void onresetdevice();
	void ongameover();
};

#endif
//=============================================================================================================
 
