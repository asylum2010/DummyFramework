//=============================================================================================================
#ifndef _CSCREENFADER9_H_
#define _CSCREENFADER9_H_

#include "../../base/d3d9/CEffectRenderer9.h"
#include "../../template/defines.h"
#include <d3dx9.h>

namespace DummyFramework
{
	class CScreenFader9 : public CEffectRenderer9
	{
	public:
		enum efademode
		{
			In,
			Out
		};

		unsigned int Delay;

		CScreenFader9();
		~CScreenFader9() {}

		bool Initialize(CGame9& mygame);
		bool LoadContent();
		bool CreateStateBlock();

		void Update();
		void Draw();
		void Reset(efademode fademode);

	private:
		syncedanimator<float> alpha;

		LPDIRECT3DTEXTURE9	texture;
		D3DXCOLOR			color;
		D3DXMATRIX			world;
		efademode			mode;
		unsigned int		ticks;
		unsigned int		id;

	_DUMMY_EVENTS:
		signal1<efademode> faded;

	_DUMMY_EVENTHANDLERS:
		void onlostdevice();
		void onresetdevice();
	};
}

#endif
//=============================================================================================================



