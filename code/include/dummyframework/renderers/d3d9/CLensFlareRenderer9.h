//=============================================================================================================
#ifndef _CLENSFLARERENDERER9_H_
#define _CLENSFLARERENDERER9_H_

#include "../../base/d3d9/CEffectRenderer9.h"
#include <d3dx9.h>
#include <list>

#define _FLARECOUNT_ 9

namespace DummyFramework
{
    class CLensFlareRenderer9 : public CEffectRenderer9
    {
        struct sflare
        {
            float scale;
            float size;
            unsigned char index;
        };

        struct soccluder
        {
            D3DXVECTOR3 center;
            float radius;

            soccluder(const D3DXVECTOR3& c, const float r)
                : center(c), radius(r) {}
        };

        typedef std::list<soccluder> occluderlist;

    private:
        LPDIRECT3DTEXTURE9  lens[4];
        D3DVIEWPORT9        viewport;
        D3DXMATRIX          id, transform, translate, scale;
        D3DXVECTOR3         lightpos, viewdir;
        D3DXVECTOR2         center, scaleddir, ctol;
        D3DXVECTOR3         etoc, etos;
        sflare              flares[_FLARECOUNT_];
        occluderlist        occluders;

        D3DXCOLOR           color;
        float               halfdiagonal;
        float               vdotl, alpha, length;
        float               dist, cosa, x, rad;

    public:
        D3DXMATRIX View, Projection;
        D3DXVECTOR3 Sun, Eye;

        CLensFlareRenderer9();
		~CLensFlareRenderer9() {}

        bool Initialize(CGame9& game);
        bool LoadContent();
        void Draw();

        inline void AddOccluder(const D3DXVECTOR3& center, const float radius) {
            occluders.push_back(soccluder(center, radius));
        }
    };
}

#endif
//=============================================================================================================
 
 
