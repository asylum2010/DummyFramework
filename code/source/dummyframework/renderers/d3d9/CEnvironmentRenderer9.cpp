//=============================================================================================================
#include <dummyframework/renderers/d3d9/CEnvironmentRenderer9.h>
#include <dummyframework/base/d3d9/CGame9.h>

namespace DummyFramework
{
	CEnvironmentRenderer9::CEnvironmentRenderer9()
	{
		Texture = NULL;
		Frequency = 10;
		Eye = D3DXVECTOR3(0, 0, 0);
		EnvironmentMapSize = 128;

		frequency = 0;
		memset(surfaces, 0, sizeof(surfaces));
	}
	//=============================================================================================================
	bool CEnvironmentRenderer9::Initialize(CGame9& mygame)
	{
		CEffectRenderer9::Initialize(mygame);

		viewdir[0] = D3DXVECTOR3(1, 0, 0);
		viewdir[1] = D3DXVECTOR3(-1, 0, 0);

		viewdir[2] = D3DXVECTOR3(0, 1, 0);
		viewdir[3] = D3DXVECTOR3(0, -1, 0);

		viewdir[4] = D3DXVECTOR3(0, 0, 1);
		viewdir[5] = D3DXVECTOR3(0, 0, -1);

		up[0] = D3DXVECTOR3(0, 1, 0);
		up[1] = D3DXVECTOR3(0, 0, -1);
		up[2] = D3DXVECTOR3(0, 0, 1);

		D3DXMatrixPerspectiveFovLH(&Projection, D3DX_PI / 2, 1.0f, 0.1f, 100.0f);
		return true;
	}
	//=============================================================================================================
	bool CEnvironmentRenderer9::LoadContent()
	{
		Texture = game->Content.CreateCubeRenderTarget(EnvironmentMapSize, D3DFMT_A8R8G8B8);
		dassert(false, "CEnvironmentRenderer9::LoadContent(): Could not create render target", Texture);

		id = game->Content.GetTextureID(Texture);

		for( int i = 0; i < 6; ++i )
			Texture->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, &surfaces[i]);

		return CEffectRenderer9::LoadContent();
	}
	//=============================================================================================================
	void CEnvironmentRenderer9::UnloadContent()
	{
		for( int i = 0; i < 6; ++i )
		{
			safe_release(surfaces[i]);
		}

		CEffectRenderer9::UnloadContent();
	}
	//=============================================================================================================
	void CEnvironmentRenderer9::Draw()
	{
		if( !CanBeApplied() )
			return;

		frequency = (frequency + 1) % Frequency;

		if( Frequency > 1 && frequency != 1 )
			return;

		game->Graphics->GetRenderTarget(0, &surfaces[6]);

		for( int i = 0; i < 6; ++i )
		{
			const int& index = max((i % 4) - 1, 0);
			look = viewdir[i] + Eye;

			D3DXMatrixLookAtLH(&View, &Eye, &look, &up[index]);
			game->Graphics->SetRenderTarget(0, surfaces[i]);

			render();
		}

		game->Graphics->SetRenderTarget(0, surfaces[6]);

		surfaces[6]->Release();
		surfaces[6] = NULL;
	}
	//=============================================================================================================
	void CEnvironmentRenderer9::onlostdevice()
	{
		UnloadContent();
		Texture = NULL;

		CEffectRenderer9::onlostdevice();
	}
	//=============================================================================================================
	void CEnvironmentRenderer9::onresetdevice()
	{
		Texture = (LPDIRECT3DCUBETEXTURE9)game->Content.GetTextureByID(id);

		for( int i = 0; i < 6; ++i )
			Texture->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, &surfaces[i]);

		CEffectRenderer9::onresetdevice();
	}
	//=============================================================================================================
}
