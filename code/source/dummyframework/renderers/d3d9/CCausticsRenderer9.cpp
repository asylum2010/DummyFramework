//=============================================================================================================
#include <dummyframework/renderers/d3d9/CCausticsRenderer9.h>
#include <dummyframework/base/d3d9/CGame9.h>

namespace DummyFramework
{
	CCausticsRenderer9::CCausticsRenderer9()
	{
		DistanceMapCount = 1;
		DistanceMapSize = 256;
		PhotonHitImageSize = 128;

		Position = D3DXVECTOR3(0, 0, 0);

		declaration = NULL;
		vertices = NULL;
		indices = NULL;
		effect = NULL;

		photonsurface = NULL;
		shadowsurface = NULL;

		memset(causticsurfaces, 0, sizeof(causticsurfaces));

		PhotonHits = NULL;
		Caustics = NULL;
		ShadowMap = NULL;
	}
	//=============================================================================================================
	CCausticsRenderer9::~CCausticsRenderer9()
	{
		safe_release(vertices);
		safe_release(indices);
		safe_release(declaration);
	}
	//=============================================================================================================
	bool CCausticsRenderer9::Initialize(CGame9& mygame)
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

		D3DVERTEXELEMENT9 elements[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			D3DDECL_END()
		};

		game->Graphics->CreateVertexDeclaration(elements, &declaration);

        D3DXMatrixPerspectiveFovLH(&Projection, D3DX_PI / 2, 1.0f, 0.1f, 100.0f);
		return true;
	}
	//=============================================================================================================
	bool CCausticsRenderer9::LoadContent()
	{
		effect = game->Content.LoadMaterial(MediaDir + "effects/caustics.fx");
		dassert(false, "CCausticsRenderer9::LoadContent(): Could not create effect", effect);
	
		// photon hit location image
		PhotonHits = game->Content.CreateRenderTarget(PhotonHitImageSize, PhotonHitImageSize, D3DFMT_A32B32G32R32F);
		dassert(false, "CCausticsRenderer9::LoadContent(): Could not create photon hit image", PhotonHits);

		PhotonHits->GetSurfaceLevel(0, &photonsurface);
		id1 = game->Content.GetTextureID(PhotonHits);
		
		// shadow map
		ShadowMap = game->Content.CreateRenderTarget(PhotonHitImageSize, PhotonHitImageSize, D3DFMT_R32F);
		dassert(false, "CCausticsRenderer9::LoadContent(): Could not create shadow map", ShadowMap);

		ShadowMap->GetSurfaceLevel(0, &shadowsurface);
		id3 = game->Content.GetTextureID(ShadowMap);

		// distance maps
		distancemaps.resize(DistanceMapCount);

		for( size_t i = 0; i < distancemaps.size(); ++i )
		{
			distancemap& dm = distancemaps[i];

			dm.colors = game->Content.CreateCubeRenderTarget(DistanceMapSize, D3DFMT_A8R8G8B8);
			dassert(false, "CCausticsRenderer9::LoadContent(): Could not create distance map", dm.colors);

			dm.distances = game->Content.CreateCubeRenderTarget(DistanceMapSize, D3DFMT_A16B16G16R16F);
			dassert(false, "CCausticsRenderer9::LoadContent(): Could not create distance map", dm.distances);

			dm.id1 = game->Content.GetTextureID(dm.colors);
			dm.id2 = game->Content.GetTextureID(dm.distances);

			for( int j = 0; j < 6; ++j )
			{
				dm.colors->GetCubeMapSurface((D3DCUBEMAP_FACES)j, 0, &dm.surfaces[j]);
				dm.distances->GetCubeMapSurface((D3DCUBEMAP_FACES)j, 0, &dm.surfaces[j + 6]);
			}
		}

		// caustic map
		Caustics = game->Content.CreateCubeRenderTarget(DistanceMapSize, D3DFMT_A16B16G16R16F);
		dassert(false, "CCausticsRenderer9::LoadContent(): Could not create caustics map", Caustics);

		id2 = game->Content.GetTextureID(Caustics);

		for( int j = 0; j < 6; ++j )
			Caustics->GetCubeMapSurface((D3DCUBEMAP_FACES)j, 0, &causticsurfaces[j]);

		// caustic points
		HRESULT hr;
		D3DXVECTOR3* vdata = NULL;
		unsigned short* idata = NULL;

		hr = game->Graphics->CreateVertexBuffer(
			PhotonHitImageSize * PhotonHitImageSize * sizeof(D3DXVECTOR3),
			0, D3DFVF_XYZ, D3DPOOL_MANAGED, &vertices, NULL);

		dnassert(false, "CCausticsRenderer9::LoadContent(): Could not create vertex buffer", FAILED(hr));

		hr = vertices->Lock(0, 0, (void**)&vdata, 0);
		dnassert(false, "CCausticsRenderer9::LoadContent(): Could not lock vertex buffer", FAILED(hr));

		for( unsigned int i = 0; i < PhotonHitImageSize; ++i )
		{
			for( unsigned int j = 0; j < PhotonHitImageSize; ++j )
			{
				vdata[i * PhotonHitImageSize + j].x = (float)j / (float)PhotonHitImageSize;
				vdata[i * PhotonHitImageSize + j].y = (float)i / (float)PhotonHitImageSize;
				vdata[i * PhotonHitImageSize + j].z = 0;
			}
		}

		vertices->Unlock();
		
		return CEffectRenderer9::LoadContent();
	}
	//=============================================================================================================
	void CCausticsRenderer9::UnloadContent()
	{
		for( size_t i = 0; i < distancemaps.size(); ++i )
		{
			for( int j = 0; j < 12; ++j )
			{
				safe_release(distancemaps[i].surfaces[j]);
			}
		}
		
		safe_release(photonsurface);
		safe_release(shadowsurface);

		for( int j = 0; j < 6; ++j )
			safe_release(causticsurfaces[j]);

		CEffectRenderer9::UnloadContent();
	}
	//=============================================================================================================
	void CCausticsRenderer9::DrawColorDistance()
	{
		if( !CanBeApplied() )
            return;
				
		DrawColors();
		DrawDistances();
	}
	//=============================================================================================================
	void CCausticsRenderer9::DrawColors()
	{
		for( size_t i = 0; i < distancemaps.size(); ++i )
		{
			distancemap& dm = distancemaps[i];

			for( int j = 0; j < 6; ++j )
			{
				int index = max((j % 4) - 1, 0);
				look = viewdir[j] + Position;

				D3DXMatrixLookAtLH(&View, &Position, &look, &up[index]);
				game->Graphics->SetRenderTarget(0, dm.surfaces[j]);

				rendercolor(i);
			}
		}
	}
	//=============================================================================================================
	void CCausticsRenderer9::DrawDistances()
	{
		effect->SetTechnique("distancemap");
		effect->SetMatrix("matProj", &Projection);
		effect->SetVector("refPoint", (D3DXVECTOR4*)&Position);
		
		effect->Begin(NULL, _DUMMY_EFFECT_DONT_SAVE_);
		effect->BeginPass(0);

		for( size_t i = 0; i < distancemaps.size(); ++i )
		{
			distancemap& dm = distancemaps[i];

			for( int j = 0; j < 6; ++j )
			{
				int index = max((j % 4) - 1, 0);
				look = viewdir[j] + Position;

				D3DXMatrixLookAtLH(&View, &Position, &look, &up[index]);
				effect->SetMatrix("matView", &View);
				
				game->Graphics->SetRenderTarget(0, dm.surfaces[j + 6]);
				renderdistance(i);
			}
		}

		effect->EndPass();
		effect->End();
	}
	//=============================================================================================================
	void CCausticsRenderer9::PhotonTrace()
	{
		if( !CanBeApplied() )
            return;
		
		// photon tracing
		game->Graphics->SetRenderTarget(0, photonsurface);

		effect->SetTechnique("photontrace");
		effect->SetVector("refPoint", (D3DXVECTOR4*)&Position);

		effect->Begin(NULL, _DUMMY_EFFECT_DONT_SAVE_);
		effect->BeginPass(0);

		photontrace();

		effect->EndPass();
		effect->End();

		// render shadow map
		game->Graphics->SetRenderTarget(0, shadowsurface);

		effect->SetTechnique("shadowmap");

		effect->Begin(NULL, _DUMMY_EFFECT_DONT_SAVE_);
		effect->BeginPass(0);

		rendershadowmap();

		effect->EndPass();
		effect->End();
	}
	//=============================================================================================================
	void CCausticsRenderer9::Draw()
	{
		manager->SetRenderState(D3DRS_ZENABLE, false);

		// render shadow
		effect->SetTechnique("shadow");
		effect->Begin(NULL, _DUMMY_EFFECT_DONT_SAVE_);
		effect->BeginPass(0);

		for( int j = 0; j < 6; ++j )
		{
			int index = max((j % 4) - 1, 0);
			look = viewdir[j] + Position;

			D3DXMatrixLookAtLH(&View, &Position, &look, &up[index]);
			game->Graphics->SetRenderTarget(0, causticsurfaces[j]);

			effect->SetMatrix("matView", &View);
			effect->SetMatrix("matProj", &Projection);

			game->Graphics->Clear(0, NULL, D3DCLEAR_TARGET, 0xffffffff, 1.0f, 0);
			rendershadow();
		}

		effect->EndPass();
		effect->End();

		// render caustics
		manager->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		manager->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		manager->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		manager->SetTexture(D3DVERTEXTEXTURESAMPLER0, PhotonHits);

		game->Graphics->SetVertexDeclaration(declaration);
		game->Graphics->SetStreamSource(0, vertices, 0, sizeof(D3DXVECTOR3));
		
		effect->SetTechnique("reconstruct");
		effect->Begin(NULL, _DUMMY_EFFECT_DONT_SAVE_);
		effect->BeginPass(0);

		distancemap& dm = distancemaps[distancemaps.size() - 1];

		for( int j = 0; j < 6; ++j )
		{
			int index = max((j % 4) - 1, 0);
			look = viewdir[j] + Position;

			D3DXMatrixLookAtLH(&View, &Position, &look, &up[index]);
			game->Graphics->SetRenderTarget(0, causticsurfaces[j]);

			effect->SetMatrix("matView", &View);
			effect->SetMatrix("matProj", &Projection);
			effect->CommitChanges();

			game->Graphics->DrawPrimitive(D3DPT_POINTLIST, 0, PhotonHitImageSize * PhotonHitImageSize);
		}

		effect->EndPass();
		effect->End();
		
		manager->SetTexture(D3DVERTEXTEXTURESAMPLER0, NULL);
		manager->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		manager->SetRenderState(D3DRS_ZENABLE, true);
	}
	//=============================================================================================================
	void CCausticsRenderer9::onlostdevice()
	{
		UnloadContent();
        
		for( size_t i = 0; i < distancemaps.size(); ++i )
			distancemaps[i].colors = distancemaps[i].distances = NULL;

		PhotonHits = NULL;
		Caustics = NULL;
		ShadowMap = NULL;
		
		CEffectRenderer9::onlostdevice();
	}
	//=============================================================================================================
    void CCausticsRenderer9::onresetdevice()
	{
		PhotonHits = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(id1);
		PhotonHits->GetSurfaceLevel(0, &photonsurface);

		ShadowMap = (LPDIRECT3DTEXTURE9)game->Content.GetTextureByID(id3);
		ShadowMap->GetSurfaceLevel(0, &shadowsurface);

		Caustics = (LPDIRECT3DCUBETEXTURE9)game->Content.GetTextureByID(id2);

		for( int j = 0; j < 6; ++j )
			Caustics->GetCubeMapSurface((D3DCUBEMAP_FACES)j, 0, &causticsurfaces[j]);

		for( size_t i = 0; i < distancemaps.size(); ++i )
		{
			distancemap& dm = distancemaps[i];

			dm.colors = (LPDIRECT3DCUBETEXTURE9)game->Content.GetTextureByID(dm.id1);
			dm.distances = (LPDIRECT3DCUBETEXTURE9)game->Content.GetTextureByID(dm.id2);

			for( int j = 0; j < 6; ++j )
			{
				dm.colors->GetCubeMapSurface((D3DCUBEMAP_FACES)j, 0, &dm.surfaces[j]);
				dm.distances->GetCubeMapSurface((D3DCUBEMAP_FACES)j, 0, &dm.surfaces[j + 6]);
			}
		}

        CEffectRenderer9::onresetdevice();
	}
	//=============================================================================================================
}
