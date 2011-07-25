//=============================================================================================================
#ifndef _CENVIRONMENTRENDERER9_H_
#define _CENVIRONMENTRENDERER9_H_

#include "../../base/d3d9/CEffectRenderer9.h"
#include <d3dx9.h>

namespace DummyFramework
{
    class CEnvironmentRenderer9 : public CEffectRenderer9
    {
    private:
        LPDIRECT3DSURFACE9     surfaces[7];
        D3DXVECTOR3            look;
        D3DXVECTOR3            viewdir[6];
        D3DXVECTOR3            up[3];
        unsigned int           frequency;
        unsigned long          id;
        
    public:
        LPDIRECT3DCUBETEXTURE9 Texture;
        D3DXMATRIX             View;
        D3DXMATRIX             Projection;
        D3DXVECTOR3            Eye;
        unsigned int           Frequency;
		unsigned int           EnvironmentMapSize;

        CEnvironmentRenderer9();
		~CEnvironmentRenderer9() {}

        bool Initialize(CGame9& mygame);
		bool LoadContent();

        void UnloadContent();
        void Draw();

        inline void SetEyePosition(const D3DXVECTOR3& position) {
            this->Eye = position;
        }

	events:
		signal0 render;

	eventhandlers:
		void onlostdevice();
        void onresetdevice();
    };
}

#endif
//=============================================================================================================
 
 
 
