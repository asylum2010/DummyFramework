//=============================================================================================================
#include <dummyframework/renderers/d3d9/CGodRayRenderer9.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/CHelper.h>

namespace DummyFramework
{
	CGodRayRenderer9::CGodRayRenderer9()
	{
		effect = NULL;

		Color = NULL;
		BackBuffer = NULL;
		Occluders = NULL;

		memset(rendertargets, 0, sizeof(rendertargets));
		memset(surfaces, 0, sizeof(surfaces));
	}
	//=============================================================================================================
	bool CGodRayRenderer9::Initialize(CGame9& mygame)
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
	bool CGodRayRenderer9::LoadContent()
	{
		D3DFORMAT format = D3DFMT_A8R8G8B8;

		effect = game->Content.LoadMaterial(MediaDir + "effects/godray.fx");
		dassert(false, "CGodRayRenderer9::LoadContent(): Could not create material", effect);

		rendertargets[0] = game->Content.CreateRenderTarget((unsigned int)size.x, (unsigned int)size.y, format);
		dassert(false, "CGodRayRenderer9::LoadContent(): Could not create render target", rendertargets[0]);

		rendertargets[1] = game->Content.CreateRenderTarget((unsigned int)size.x, (unsigned int)size.y, format);
		dassert(false, "CGodRayRenderer9::LoadContent(): Could not create render target", rendertargets[1]);

		rendertargets[2] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.25f), (unsigned int)(size.y * 0.25f), format);
		dassert(false, "CGodRayRenderer9::LoadContent(): Could not create render target", rendertargets[2]);

		rendertargets[3] = game->Content.CreateRenderTarget((unsigned int)(size.x * 0.25f), (unsigned int)(size.y * 0.25f), format);
		dassert(false, "CGodRayRenderer9::LoadContent(): Could not create render target", rendertargets[2]);

		ids[0] = game->Content.GetTextureID(rendertargets[0]);
		ids[1] = game->Content.GetTextureID(rendertargets[1]);
		ids[2] = game->Content.GetTextureID(rendertargets[2]);
		ids[3] = game->Content.GetTextureID(rendertargets[3]);

		rendertargets[0]->GetSurfaceLevel(0, &Color);
		rendertargets[1]->GetSurfaceLevel(0, &Occluders);
		rendertargets[2]->GetSurfaceLevel(0, &surfaces[0]);
		rendertargets[3]->GetSurfaceLevel(0, &surfaces[1]);

		lightpos = D3DXVECTOR4(4.0f / size.x, 4.0f / size.y, 0, 1);
		effect->SetVector("texelSize", (D3DXVECTOR4*)&lightpos);

		return CEffectRenderer9::LoadContent();
	}
	//=============================================================================================================
	bool CGodRayRenderer9::ResetRenderStates()
	{
		manager->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		manager->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		
		return true;
	}
	//=============================================================================================================
	void CGodRayRenderer9::UnloadContent()
	{
		safe_release(surfaces[0]);
		safe_release(surfaces[1]);
		safe_release(Occluders);
		safe_release(Color);

		memset(rendertargets, 0, sizeof(rendertargets));
		CEffectRenderer9::UnloadContent();
	}
	//=============================================================================================================
	void CGodRayRenderer9::Draw()
	{
		if( !BackBuffer || !CanBeApplied() )
			return;

		viewdir.x = View._13;
		viewdir.y = View._23;
		viewdir.z = View._33;

		D3DXVec3Normalize(&lightdir, &Sun);
		float exposure = CHelper::Clamp<float>(D3DXVec3Dot(&lightdir, &viewdir), 0, 1);

		if( exposure > 0.0f )
		{
			D3DXVECTOR3 sun = Eye + Sun;
			D3DXVec3Transform(&lightpos, &sun, &ViewProj);

			lightpos.x = (1.0f + lightpos.x / lightpos.w) * 0.5f;
			lightpos.y = (1.0f - lightpos.y / lightpos.w) * 0.5f;

			manager->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			manager->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

			effect->SetTechnique("godray");

			effect->SetFloat("exposure", exposure);
			effect->SetVector("lightPos", (D3DXVECTOR4*)&lightpos);

			world._11 = size.x * 0.25f;
			world._22 = size.y * 0.25f;

			game->Graphics->SetRenderTarget(0, surfaces[0]);
			manager->SetTexture(0, rendertargets[1]);

			effect->Begin(NULL, 0);
			effect->BeginPass(0);
			{
				game->DrawImage(world);
			}
			effect->EndPass();
			effect->End();

			// blur
			effect->SetTechnique("blur");

			game->Graphics->SetRenderTarget(0, surfaces[1]);
			manager->SetTexture(0, rendertargets[2]);

			effect->Begin(NULL, 0);
			effect->BeginPass(0);
			{
				game->DrawImage(world);
			}
			effect->EndPass();
			effect->End();

			// final
			effect->SetTechnique("final");

			world._11 = size.x;
			world._22 = size.y;

			game->Graphics->SetRenderTarget(0, BackBuffer);
			manager->SetTexture(0, rendertargets[0]);
			manager->SetTexture(1, rendertargets[3]);

			effect->Begin(NULL, 0);
			effect->BeginPass(0);
			{
				game->DrawImage(world);
			}
			effect->EndPass();
			effect->End();

			manager->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			manager->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		}
		else
		{
			effect->SetTechnique("nothing");

			world._11 = size.x;
			world._22 = size.y;

			game->Graphics->SetRenderTarget(0, BackBuffer);
			manager->SetTexture(0, rendertargets[0]);

			effect->Begin(NULL, 0);
			effect->BeginPass(0);
			{
				game->DrawImage(world);
			}
			effect->EndPass();
			effect->End();
		}
	}
	//=============================================================================================================
	void CGodRayRenderer9::onlostdevice()
	{
		UnloadContent();

		memset(rendertargets, 0, sizeof(rendertargets));
		memset(surfaces, 0, sizeof(surfaces));

		Color = NULL;
		Occluders = NULL;

		CEffectRenderer9::onlostdevice();
	}
	//=============================================================================================================
	void CGodRayRenderer9::onresetdevice()
	{
		rendertargets[0] = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(ids[0]);
		rendertargets[1] = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(ids[1]);
		rendertargets[2] = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(ids[2]);
		rendertargets[3] = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(ids[3]);

		rendertargets[0]->GetSurfaceLevel(0, &Color);
		rendertargets[1]->GetSurfaceLevel(0, &Occluders);
		rendertargets[2]->GetSurfaceLevel(0, &surfaces[0]);
		rendertargets[3]->GetSurfaceLevel(0, &surfaces[1]);

		CEffectRenderer9::onresetdevice();
	}
	//=============================================================================================================
}
