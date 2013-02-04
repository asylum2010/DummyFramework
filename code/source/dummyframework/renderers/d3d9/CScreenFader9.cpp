//=============================================================================================================
#include <dummyframework/renderers/d3d9/CScreenFader9.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/CHelper.h>

namespace DummyFramework
{
	CScreenFader9::CScreenFader9()
	{
		game = NULL;
		texture = NULL;
		ticks = 0;
		Delay = 3;

		Reset(In);

		alpha.set(5, 0, 1);
		alpha = alpha.count() - 1;
		alpha.direction = -1;
	}
	//=============================================================================================================
	bool CScreenFader9::Initialize(CGame9& mygame)
	{
		CEffectRenderer9::Initialize(mygame);

		D3DXMatrixScaling(&world,
			(float)game->DisplayMode.Width,
			(float)game->DisplayMode.Height, 1);

		world._41 = -0.5f;
		world._42 = -0.5f;

		return true;
	}
	//=============================================================================================================
	bool CScreenFader9::LoadContent()
	{
		texture = game->Content.CreateTexture(4, 4, D3DFMT_A8R8G8B8);
		dassert(false, "CScreenFader9::LoadContent(): Could not create texture", texture);

		id = game->Content.GetTextureID(texture);
		game->Content.FillTexture(texture, 0xff000000);

		return CEffectRenderer9::LoadContent();
	}
	//=============================================================================================================
	void CScreenFader9::Update()
	{
		if( ticks < Delay )
		{
			++ticks;
			return;
		}

		if( mode == In )
		{
			if( alpha.current <= 0 )
				faded(mode);
			
			alpha.step();
		}
		else
		{
			if( alpha.current >= alpha.count() - 1 )
				faded(mode);

			alpha.step();
		}
	}
	//=============================================================================================================
	void CScreenFader9::Draw()
	{
		if( !CanBeApplied() )
			return;

		alpha.smooth((float)game->Sync.Alpha());
		color.a = alpha.value;

		if( mode == In )
		{
			if( alpha.previous <= 0 )
				return;
		}

		manager->SetTexture(0, texture);
		manager->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		manager->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		manager->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		manager->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		manager->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
		manager->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

		manager->SetVertexShader(NULL);
		manager->SetPixelShader(NULL);

		game->DrawImage(world, color);

		manager->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		manager->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
	//=============================================================================================================
	void CScreenFader9::Reset(efademode fademode)
	{
		mode = fademode;
		color = D3DXCOLOR(0, 0, 0, (mode == In ? 1.0f : 0.0f));
		alpha = (mode == Out ? 0 : alpha.count() - 1);

		alpha.direction = (mode == Out ? 1 : -1);
	}
	//=============================================================================================================
	void CScreenFader9::onlostdevice()
	{
		texture = NULL;
		CEffectRenderer9::onlostdevice();
	}
	//=============================================================================================================
	void CScreenFader9::onresetdevice()
	{
		texture = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(id);
		game->Content.FillTexture(texture, 0xff000000);

		CEffectRenderer9::onresetdevice();
	}
	//=============================================================================================================
}
