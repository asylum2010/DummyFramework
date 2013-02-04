//=============================================================================================================
#ifndef _CSHADOWMAPRENDERER9_H_
#define _CSHADOWMAPRENDERER9_H_

#include "../../base/d3d9/CEffectRenderer9.h"
#include <d3dx9.h>

namespace DummyFramework
{
	class CShadowMapRenderer9 : public CEffectRenderer9
	{
	private:
		LPDIRECT3DSURFACE9 shadowsurface;
		LPDIRECT3DSURFACE9 depthstencil;
		unsigned int id;

	public:
		LPDIRECT3DTEXTURE9 ShadowMap;
		unsigned int Size;

		CShadowMapRenderer9();
		~CShadowMapRenderer9() {}

		bool LoadContent();

		void UnloadContent();
		void Draw();

	_DUMMY_EVENTS:
		DummyFramework::unisignal0 rendershadow;

	_DUMMY_EVENTHANDLERS:
		void onlostdevice();
		void onresetdevice();
	};
}

#endif
//=============================================================================================================
