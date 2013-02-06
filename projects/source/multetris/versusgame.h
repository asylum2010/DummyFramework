//=============================================================================================================
#ifndef _VERSUSGAME_H_
#define _VERSUSGAME_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/menu/CLabel.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/template/defines.h>

#include "tetromino.h"
#include "singletable.h"
#include "gamevariables.h"

class VersusGame : public DummyFramework::CForm
{
	typedef union
	{
		unsigned char a[4];
		unsigned int b;
	} binding;

	struct player
	{
		SingleTable	table;
		binding		bindings;
		char		movedir;
		bool		rotated;
		bool		firststep;
	};

	struct sine
	{
		inline static float y(float x) {
			return 0.5f * (sin((2 * x - 1) * 1.570796327f) + 1);
		}
	};

private:
	player	players[2];
	int		ticks;
	int		swapticks;
	int		gametime;
	int		swaptime;
	bool	swapped;
	bool	juststarted;

	DummyFramework::syncedanimator<float> blink;
	DummyFramework::syncedanimator<float, sine> swap;
	DummyFramework::CDynamicQuadBuffer9 quads;
	DummyFramework::CLabel labels[9];

	void ResetGUI();
	size_t DrawTetro(size_t start, const Tetromino& tetro, const D3DXVECTOR2& pos);

public:
	LPDIRECT3DTEXTURE9 Atlas;

	DummyFramework::CSprite9* Cell;
	DummyFramework::CSprite9* Glow;

	size_t SwapDelay;

	enum stateex
	{
		BlinkIn = Inactive + 1,
		BlinkOut = Inactive + 2,
		Swapping = Inactive + 3,
		Dead = Inactive + 4
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

	VersusGame();
	~VersusGame();

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

	void ongame1over();
	void ongame2over();
	void ongameover();
};

#endif
//=============================================================================================================
 
