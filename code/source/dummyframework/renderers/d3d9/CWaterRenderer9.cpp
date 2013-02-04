//=============================================================================================================
#include <dummyframework/renderers/d3d9/CWaterRenderer9.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/CHelper.h>

namespace DummyFramework
{
	CWaterRenderer9::CWaterRenderer9()
	{
		oldsurface = NULL;
		reflectsurface = NULL;
		refractsurface = NULL;

		ReflectionTexture = NULL;
		RefractionTexture = NULL;

		Frequency = 10;
		frequency = 0;
		Clip = true;

		D3DXMatrixIdentity(&view);
		D3DXMatrixIdentity(&projection);
		eye = D3DXVECTOR3(0, 0, 0);
	}
	//=============================================================================================================
	bool CWaterRenderer9::LoadContent()
	{
		ReflectionTexture = game->Content.CreateRenderTarget(game->DisplayMode.Width / 2, game->DisplayMode.Height / 2, D3DFMT_A8R8G8B8);
		dassert(false, "CWaterRenderer9::LoadContent(): Could not create reflection texture", ReflectionTexture);

		id1 = game->Content.GetTextureID(ReflectionTexture);
		ReflectionTexture->GetSurfaceLevel(0, &reflectsurface);

		RefractionTexture = game->Content.CreateRenderTarget(game->DisplayMode.Width / 2, game->DisplayMode.Height / 2, D3DFMT_A8R8G8B8);
		dassert(false, "CWaterRenderer9::LoadContent(): Could not create refraction texture", RefractionTexture);

		id2 = game->Content.GetTextureID(RefractionTexture);
		RefractionTexture->GetSurfaceLevel(0, &refractsurface);

		return CEffectRenderer9::LoadContent();
	}
	//=============================================================================================================
	void CWaterRenderer9::UnloadContent()
	{
		safe_release(reflectsurface);
		safe_release(refractsurface);

		CEffectRenderer9::UnloadContent();
	}
	//=============================================================================================================
	void CWaterRenderer9::Draw()
	{
		if( !CanBeApplied() )
			return;

		frequency = (frequency + 1) % Frequency;

		if( Frequency > 1 && frequency != 1 )
			return;

		Eye = eye;

		look.x = view._13 + eye.x;
		look.y = view._23 + eye.y;
		look.z = view._33 + eye.z;

		Eye.y = -eye.y - plane.d * 2.0f;
		look.y = -look.y - plane.d * 2.0f;
		
		D3DXVECTOR3 right(view._11, view._21, view._31);
		D3DXVECTOR3 fw(view._13, -view._23, view._33);

		D3DXVec3Cross(&up, &right, &fw);
		D3DXMatrixLookAtLH(&View, &Eye, &look, &up);

		Eye.y = eye.y;
		Projection = projection;

		if( Clip )
		{
			D3DXPLANE tp = plane;
			D3DXMATRIX iv, ip;
						
			D3DXPlaneNormalize(&tp, &tp);
			D3DXMatrixInverse(&iv, NULL, &View);
			D3DXMatrixTranspose(&iv, &iv);
			D3DXPlaneTransform(&tp, &tp, &iv);

			D3DXMatrixInverse(&ip, NULL, &Projection);
			D3DXMatrixTranspose(&ip, &ip);
			D3DXPlaneTransform(&tp, &tp, &ip);

			manager->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
			game->Graphics->SetClipPlane(0, tp);
		}

		game->Graphics->GetRenderTarget(0, &oldsurface);
		game->Graphics->SetRenderTarget(0, reflectsurface);

		renderreflection();

		manager->SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
		game->Graphics->SetRenderTarget(0, oldsurface);

		safe_release(oldsurface);
	}
	//=============================================================================================================
	void CWaterRenderer9::onlostdevice()
	{
		UnloadContent();

		ReflectionTexture = NULL;
		RefractionTexture = NULL;

		CEffectRenderer9::onlostdevice();
	}
	//=============================================================================================================
	void CWaterRenderer9::onresetdevice()
	{
		ReflectionTexture = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(id1);
		ReflectionTexture->GetSurfaceLevel(0, &reflectsurface);

		RefractionTexture = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(id2);
		RefractionTexture->GetSurfaceLevel(0, &refractsurface);

		CEffectRenderer9::onresetdevice();
	}
	//=============================================================================================================
}
