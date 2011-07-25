//=============================================================================================================
#include <dummyframework/renderers/d3d9/CShadowMapRenderer9.h>
#include <dummyframework/base/d3d9/CGame9.h>

namespace DummyFramework
{
	CShadowMapRenderer9::CShadowMapRenderer9()
	{
		Size = 512;
		ShadowMap = NULL;

		shadowsurface = NULL;
		depthstencil = NULL;
	}
	//=============================================================================================================
	bool CShadowMapRenderer9::LoadContent()
	{
		ShadowMap = game->Content.CreateRenderTarget(Size, Size, D3DFMT_R32F);

		if( !ShadowMap )
		{
			ShadowMap = game->Content.CreateRenderTarget(Size, Size, D3DFMT_R16F);
			dassert(false, "CShadowMapRenderer9::LoadContent(): Could not create shadow map", ShadowMap);
		}

		HRESULT hr = game->Graphics->CreateDepthStencilSurface(Size, Size, game->PresentationParameters.AutoDepthStencilFormat,
			D3DMULTISAMPLE_NONE, 0, false, &depthstencil, NULL);

		dnassert(false, "CShadowMapRenderer9::LoadContent(): Could not create depth/stencil surface", FAILED(hr));
		std::cout << "CShadowMapRenderer9::LoadContent(): Created new depth stencil surface\n";

		ShadowMap->GetSurfaceLevel(0, &shadowsurface);
		id = game->Content.GetTextureID(ShadowMap);

		return CEffectRenderer9::LoadContent();
	}
	//=============================================================================================================
	void CShadowMapRenderer9::UnloadContent()
	{
		safe_release(shadowsurface);
		safe_release(depthstencil);

		CEffectRenderer9::UnloadContent();
	}
	//=============================================================================================================
	void CShadowMapRenderer9::Draw()
	{
		if( !CanBeApplied() )
			return;

		LPDIRECT3DSURFACE9 backbuffer = NULL;
		LPDIRECT3DSURFACE9 oldds = NULL;

		game->Graphics->GetRenderTarget(0, &backbuffer);
		game->Graphics->GetDepthStencilSurface(&oldds);
		game->Graphics->SetDepthStencilSurface(depthstencil);

		game->Graphics->SetRenderTarget(0, shadowsurface);
		game->Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		rendershadow();

		game->Graphics->SetRenderTarget(0, backbuffer);
		game->Graphics->SetDepthStencilSurface(oldds);

		safe_release(backbuffer);
		safe_release(oldds);
	}
	//=============================================================================================================
	void CShadowMapRenderer9::onlostdevice()
	{
		UnloadContent();

        shadowsurface = NULL;
	    ShadowMap = NULL;

        CEffectRenderer9::onlostdevice();
	}
	//=============================================================================================================
	void CShadowMapRenderer9::onresetdevice()
	{
		ShadowMap = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(id);
		ShadowMap->GetSurfaceLevel(0, &shadowsurface);
		
		HRESULT hr = game->Graphics->CreateDepthStencilSurface(Size, Size, game->PresentationParameters.AutoDepthStencilFormat,
			D3DMULTISAMPLE_NONE, 0, false, &depthstencil, NULL);

		dnassert(, "CShadowMapRenderer9::onresetdevice(): Could not create depth/stencil surface", FAILED(hr));

        CEffectRenderer9::onresetdevice();
	}
	//=============================================================================================================
}
