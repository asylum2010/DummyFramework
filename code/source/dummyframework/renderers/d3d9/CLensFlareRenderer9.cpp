//=============================================================================================================
#include <dummyframework/renderers/d3d9/CLensFlareRenderer9.h>
#include <dummyframework/base/d3d9/CGame9.h>

namespace DummyFramework
{
    CLensFlareRenderer9::CLensFlareRenderer9()
    {
        game = NULL;
        memset(lens, 0, sizeof(lens));
    }
    //=============================================================================================================
    bool CLensFlareRenderer9::Initialize(CGame9& game)
    {
        CEffectRenderer9::Initialize(game);
        game.Graphics->GetViewport(&viewport);

        D3DXMatrixIdentity(&id);
        color = D3DXCOLOR(1, 1, 1, 1);

        halfdiagonal = sqrt((float)(
            game.DisplayMode.Width * game.DisplayMode.Width +
            game.DisplayMode.Height * game.DisplayMode.Height)) * 0.5f;

        center.x = (float)game.DisplayMode.Width / 2;
        center.y = (float)game.DisplayMode.Height / 2;

        flares[0].index = 2;   flares[0].scale =  0.5f;  flares[0].size = 40;
        flares[1].index = 0;   flares[1].scale =  0.2f;  flares[1].size = 40;
        flares[2].index = 1;   flares[2].scale =  0.1f;  flares[2].size = 80;
        flares[3].index = 0;   flares[3].scale = -0.1f;  flares[3].size = 40;
        flares[4].index = 1;   flares[4].scale = -0.3f;  flares[4].size = 100;
        flares[5].index = 2;   flares[5].scale = -0.6f;  flares[5].size = 100;
        flares[6].index = 0;   flares[6].scale = -0.7f;  flares[6].size = 40;
        flares[7].index = 0;   flares[7].scale = -0.9f;  flares[7].size = 40;
        flares[8].index = 1;   flares[8].scale = -1.2f;  flares[8].size = 80;

        return true;
    }
    //=============================================================================================================
    bool CLensFlareRenderer9::LoadContent()
    {
        lens[0] = game->Content.LoadTexture("lens1.jpg");
        lens[1] = game->Content.LoadTexture("lens2.jpg");
        lens[2] = game->Content.LoadTexture("lens3.jpg");
        lens[3] = game->Content.LoadTexture("lens4.jpg");

		if( !(lens[0] && lens[1] && lens[2] && lens[3]) )
			dnassert(false, "CLensFlareRenderer9::LoadContent(): Could not create lens texture", true);

		return CEffectRenderer9::LoadContent();
    }
    //=============================================================================================================
    void CLensFlareRenderer9::Draw()
    {
		if( !CanBeApplied() )
			return;

        // a nézeti mátrix elsö oszlopa a nézési irány
        viewdir.x = View._13;
        viewdir.y = View._23;
        viewdir.z = View._33;
        
        lightpos = Eye + Sun;
        vdotl = D3DXVec3Dot(&viewdir, &lightpos);

        // ha háttal vagyunk a fénynek akkor nincs mit rajzolni
        if( vdotl / D3DXVec3Length(&lightpos) < 0 ) return;
        D3DXVec3Project(&lightpos, &lightpos, &viewport, &Projection, &View, &id);
        
        ctol.x = lightpos.x - center.x;
        ctol.y = lightpos.y - center.y;

        length = D3DXVec2Length(&ctol);
        alpha = 1.0f - (length / halfdiagonal);

        // ha valami eltakarja a fényforrást
        etos = Sun;
        D3DXVec3Normalize(&etos, &etos);

        for( occluderlist::iterator it = occluders.begin(); it != occluders.end(); ++it )
        {
            etoc = it->center - Eye;
            dist = D3DXVec3Length(&etoc);

            D3DXVec3Normalize(&etoc, &etoc);
            cosa = D3DXVec3Dot(&etoc, &etos);

            x = cosa * dist;
            rad = dist * dist - x * x;

            rad /= (it->radius * it->radius);
            alpha *= min(1, rad * rad);
        }

        color.a = min(1, alpha);
        D3DXVec2Normalize(&ctol, &ctol);

        // render
        manager->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
        manager->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        manager->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

        for( int i = 0; i < _FLARECOUNT_; ++i )
        {
            scaleddir = center + ctol * length * flares[i].scale;

            D3DXMatrixScaling(&scale, flares[i].size, flares[i].size, 1);
            D3DXMatrixTranslation(&translate, scaleddir.x - flares[i].size * 0.5f, scaleddir.y - flares[i].size * 0.5f, 0);
            D3DXMatrixMultiply(&transform, &scale, &translate);

            manager->SetTexture(0, lens[flares[i].index]);
            game->DrawImage(transform, color);
        }

		manager->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    }
    //=============================================================================================================
}
