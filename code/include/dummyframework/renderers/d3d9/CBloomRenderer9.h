//=============================================================================================================
#ifndef _CBLOOMRENDERER9_H_
#define _CBLOOMRENDERER9_H_

#include "../../base/d3d9/CEffectRenderer9.h"
#include <d3dx9.h>

namespace DummyFramework
{
    class CBloomRenderer9 : public CEffectRenderer9
    {
    private:
        LPD3DXEFFECT         effect;
		LPDIRECT3DTEXTURE9   scenetarget;
		LPDIRECT3DTEXTURE9   dstargets[5];
        LPDIRECT3DTEXTURE9   blurtargets[5];

        LPDIRECT3DSURFACE9   dssurfaces[5];
		LPDIRECT3DSURFACE9   blursurfaces[5];
        D3DXMATRIX           world;
        D3DXVECTOR2          size;
        unsigned long        ids[11];

    public:
        LPDIRECT3DSURFACE9 LDR;
        LPDIRECT3DSURFACE9 BackBuffer;

        CBloomRenderer9();
        ~CBloomRenderer9() {}

        bool Initialize(CGame9& mygame);
        bool ResetRenderStates();
        bool LoadContent();

        void UnloadContent();
        void Draw();
		
		inline LPD3DXEFFECT GetEffect() {
			return effect;
		}

	eventhandlers:
		void onlostdevice();
        void onresetdevice();
    };
}

#endif
//=============================================================================================================
 
