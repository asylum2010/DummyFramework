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
		unsigned long id;

    public:
		LPDIRECT3DTEXTURE9 ShadowMap;
		unsigned int Size;

        CShadowMapRenderer9();
        ~CShadowMapRenderer9() {}
        
        bool LoadContent();

        void UnloadContent();
        void Draw();

	events:
		DummyFramework::unisignal0 rendershadow;

	eventhandlers:
		void onlostdevice();
        void onresetdevice();
    };
}

#endif
//=============================================================================================================
 
