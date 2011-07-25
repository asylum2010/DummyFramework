//=============================================================================================================
#include <dummyframework/renderers/d3d9/CBloomRenderer9.h>
#include <dummyframework/base/d3d9/CGame9.h>

namespace DummyFramework
{
    CBloomRenderer9::CBloomRenderer9()
    {
        effect = NULL;
		scenetarget = NULL;

        LDR = NULL;
        BackBuffer = NULL;
        
        memset(dstargets, 0, sizeof(dstargets));
		memset(blurtargets, 0, sizeof(blurtargets));

        memset(dssurfaces, 0, sizeof(dssurfaces));
		memset(blursurfaces, 0, sizeof(blursurfaces));
    }
    //=============================================================================================================
    bool CBloomRenderer9::Initialize(CGame9& mygame)
    {
        CEffectRenderer9::Initialize(mygame);
        D3DXMatrixIdentity(&world);

        world._41 = -0.5f;
        world._42 = -0.5f;

        size = D3DXVECTOR2(
            (float)game->DisplayMode.Width,
            (float)game->DisplayMode.Height);
                
        return true;
    }
    //=============================================================================================================
    bool CBloomRenderer9::LoadContent()
    {
        D3DFORMAT format = D3DFMT_A8R8G8B8;

        effect = game->Content.LoadMaterial(MediaDir + "effects/bloom.fx");
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create material", effect);

        scenetarget = game->Content.CreateRenderTarget((unsigned int)size.x, (unsigned int)size.y, format);
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create scene target", scenetarget);

		// downsample targets
        dstargets[0] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.5f), (unsigned int)(size.y * 0.5f), format);
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create downsample target 0", dstargets[0]);

        dstargets[1] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.25f), (unsigned int)(size.y * 0.25f), format);
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create downsample target 1", dstargets[1]);
        
        dstargets[2] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.125f), (unsigned int)(size.y * 0.125f), format);
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create downsample target 2", dstargets[2]);

        dstargets[3] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.0625f), (unsigned int)(size.y * 0.0625f), format);
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create downsample target 3", dstargets[3]);

        dstargets[4] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.03125f), (unsigned int)(size.y * 0.03125f), format);
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create downsample target 4", dstargets[4]);

		// blur targets
		blurtargets[0] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.5f), (unsigned int)(size.y * 0.5f), format);
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create blur target 0", blurtargets[0]);

        blurtargets[1] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.25f), (unsigned int)(size.y * 0.25f), format);
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create blur target 1", blurtargets[1]);
        
        blurtargets[2] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.125f), (unsigned int)(size.y * 0.125f), format);
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create blur target 2", blurtargets[2]);

        blurtargets[3] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.0625f), (unsigned int)(size.y * 0.0625f), format);
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create blur target 3", blurtargets[3]);

        blurtargets[4] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.03125f), (unsigned int)(size.y * 0.03125f), format);
        dassert(false, "CBloomRenderer9::LoadContent(): Could not create blur target 4", blurtargets[4]);

		ids[0] = game->Content.GetTextureID(scenetarget);
		scenetarget->GetSurfaceLevel(0, &LDR);

		for( int i = 0; i < 5; ++i )
		{
			ids[i + 1] = game->Content.GetTextureID(dstargets[i]);
			ids[i + 6] = game->Content.GetTextureID(blurtargets[i]);

			dstargets[i]->GetSurfaceLevel(0, &dssurfaces[i]);
			blurtargets[i]->GetSurfaceLevel(0, &blursurfaces[i]);
		}
        
        return CEffectRenderer9::LoadContent();
    }
    //=============================================================================================================
    bool CBloomRenderer9::ResetRenderStates()
    {
		manager->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		manager->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		manager->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		manager->SetSamplerState(3, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(3, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		manager->SetSamplerState(4, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(4, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(4, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		manager->SetSamplerState(5, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(5, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(5, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

        return true;
    }
    //=============================================================================================================
    void CBloomRenderer9::UnloadContent()
    {
        for( int i = 0; i < 5; ++i )
        {
            safe_release(dssurfaces[i]);
			safe_release(blursurfaces[i]);
        }

        safe_release(LDR);
		
		memset(dstargets, 0, sizeof(dstargets));
		memset(blurtargets, 0, sizeof(blurtargets));

		CEffectRenderer9::UnloadContent();
    }
    //=============================================================================================================
    void CBloomRenderer9::Draw()
    {
        if( !BackBuffer || !CanBeApplied() )
            return;
        
		D3DXVECTOR4 texelsize(
			1.0f / (size.x * 0.5f),
			1.0f / (size.y * 0.5f), 0, 1);

		float x = size.x * 0.25f;
		float y = size.y * 0.25f;

		manager->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		manager->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		manager->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		manager->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		manager->SetSamplerState(2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		manager->SetSamplerState(2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		manager->SetSamplerState(3, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		manager->SetSamplerState(3, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		manager->SetSamplerState(4, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		manager->SetSamplerState(4, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		manager->SetSamplerState(5, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		manager->SetSamplerState(5, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        // bright pass
        effect->SetTechnique("brightpass");

        world._11 = size.x * 0.5f;
        world._22 = size.y * 0.5f;
        
        game->Graphics->SetRenderTarget(0, dssurfaces[0]);
        manager->SetTexture(0, scenetarget);

        effect->Begin(NULL, 0);
        effect->BeginPass(0);
        {
            game->DrawImage(world);
        }
        effect->EndPass();
        effect->End();
        
        // downsample
		effect->SetTechnique("downsample");

		effect->Begin(NULL, 0);
        effect->BeginPass(0);
        {
			for( int i = 1; i < 5; ++i )
			{
				world._11 = x;
				world._22 = y;

				effect->SetVector("texelSize", &texelsize);
				effect->CommitChanges();

				game->Graphics->SetRenderTarget(0, dssurfaces[i]);
				manager->SetTexture(0, dstargets[i - 1]);

				game->DrawImage(world);
				texelsize *= 2;

				x *= 0.5f;
				y *= 0.5f;
			}
		}
        effect->EndPass();
        effect->End();

		// blur x
		texelsize.x = 1.0f / (size.x * 0.5f);
		texelsize.y = 1.0f / (size.y * 0.5f);

		x = size.x * 0.5f;
		y = size.y * 0.5f;

        effect->SetTechnique("blurx");

		effect->Begin(NULL, 0);
        effect->BeginPass(0);
        {
			for( int i = 0; i < 5; ++i )
			{
				world._11 = x;
				world._22 = y;

				effect->SetVector("texelSize", &texelsize);
				effect->CommitChanges();

				game->Graphics->SetRenderTarget(0, blursurfaces[i]);
				manager->SetTexture(0, dstargets[i]);

				game->DrawImage(world);
				texelsize *= 2;

				x *= 0.5f;
				y *= 0.5f;
			}
		}
        effect->EndPass();
        effect->End();

		// blur y
		texelsize.x = 1.0f / (size.x * 0.5f);
		texelsize.y = 1.0f / (size.y * 0.5f);

		x = size.x * 0.5f;
		y = size.y * 0.5f;

        effect->SetTechnique("blury");

		effect->Begin(NULL, 0);
        effect->BeginPass(0);
        {
			for( int i = 0; i < 5; ++i )
			{
				world._11 = x;
				world._22 = y;

				effect->SetVector("texelSize", &texelsize);
				effect->CommitChanges();

				game->Graphics->SetRenderTarget(0, dssurfaces[i]);
				manager->SetTexture(0, blurtargets[i]);

				game->DrawImage(world);
				texelsize *= 2;

				x *= 0.5f;
				y *= 0.5f;
			}
		}
        effect->EndPass();
        effect->End();

        // final
        effect->SetTechnique("final");

        world._11 = size.x;
        world._22 = size.y;
        
        manager->SetTexture(0, scenetarget);
        manager->SetTexture(1, dstargets[0]);
        manager->SetTexture(2, dstargets[1]);
        manager->SetTexture(3, dstargets[2]);
        manager->SetTexture(4, dstargets[3]);
        manager->SetTexture(5, dstargets[4]);

        game->Graphics->SetRenderTarget(0, BackBuffer);
        
        effect->Begin(NULL, 0);
        effect->BeginPass(0);
        {
            game->DrawImage(world);
        }
        effect->EndPass();
        effect->End();

		manager->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		manager->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		manager->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		manager->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		manager->SetSamplerState(2, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		manager->SetSamplerState(2, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		manager->SetSamplerState(3, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		manager->SetSamplerState(3, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		manager->SetSamplerState(4, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		manager->SetSamplerState(4, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		manager->SetSamplerState(5, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		manager->SetSamplerState(5, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    }
	//=============================================================================================================
    void CBloomRenderer9::onlostdevice()
    {
        UnloadContent();

        memset(dstargets, 0, sizeof(dstargets));
		memset(blurtargets, 0, sizeof(blurtargets));

        memset(dssurfaces, 0, sizeof(dssurfaces));
		memset(blursurfaces, 0, sizeof(blursurfaces));

		scenetarget = NULL;
        LDR = NULL;

        CEffectRenderer9::onlostdevice();
    }
    //=============================================================================================================
    void CBloomRenderer9::onresetdevice()
    {
		// TODO: resizeolni kellhet

		scenetarget = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(ids[0]);
		scenetarget->GetSurfaceLevel(0, &LDR);

        for( int i = 0; i < 5; ++i )
        {
            dstargets[i] = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(ids[i + 1]);
            dstargets[i]->GetSurfaceLevel(0, &dssurfaces[i]);

			blurtargets[i] = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(ids[i + 6]);
            blurtargets[i]->GetSurfaceLevel(0, &blursurfaces[i]);
        }
        
        CEffectRenderer9::onresetdevice();
    }
    //=============================================================================================================
}

