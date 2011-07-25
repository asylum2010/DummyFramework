 //=============================================================================================================
#ifndef _CGODRAYRENDERER9_H_
#define _CGODRAYRENDERER9_H_

#include "../../base/d3d9/CEffectRenderer9.h"
#include <d3dx9.h>

namespace DummyFramework
{
    class CGodRayRenderer9 : public CEffectRenderer9
    {
    private:
        LPD3DXEFFECT         effect;
        LPDIRECT3DTEXTURE9   rendertargets[4];
        LPDIRECT3DSURFACE9   surfaces[2];
        D3DXMATRIX           world;
        D3DXVECTOR2          size;
        D3DXVECTOR4          lightpos;
        D3DXVECTOR3          lightdir, viewdir;
        unsigned long        ids[4];

    public:
        LPDIRECT3DSURFACE9   Color;
        LPDIRECT3DSURFACE9   Occluders;
        LPDIRECT3DSURFACE9   BackBuffer;
        D3DXVECTOR3          Eye, Sun;
        D3DXMATRIX           View, ViewProj;

        CGodRayRenderer9();
        ~CGodRayRenderer9() {}

        bool Initialize(CGame9& mygame);
        bool ResetRenderStates();
        bool LoadContent();

        void UnloadContent();
        void Draw();

	eventhandlers:
		void onlostdevice();
        void onresetdevice();
    };
}

#endif
//=============================================================================================================
 
