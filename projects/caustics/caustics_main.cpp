//=============================================================================================================
#include "caustics.h"
#include <fstream>

Caustics::Caustics()
{
	effects.push_back(causticsrenderer = new DummyFramework::CCausticsRenderer9());
	
	causticsrenderer->DistanceMapCount = 3;
	causticsrenderer->DistanceMapSize = 256;
	causticsrenderer->PhotonHitImageSize = 128;

	posteffects.push_back(bloomrenderer = new DummyFramework::CBloomRenderer9());
	posteffects.push_back(screenfader = new DummyFramework::CScreenFader9());

	Application.keyup.connect(this, &Caustics::application_keyup);
	Application.keydown.connect(&camera, &DummyFramework::CModelViewerCamera::onkeydown);
	Application.keyup.connect(&camera, &DummyFramework::CModelViewerCamera::onkeyup);
	Application.mousedown.connect(&camera, &DummyFramework::CModelViewerCamera::onmousedown);
	Application.mouseup.connect(&camera, &DummyFramework::CModelViewerCamera::onmouseup);
	Application.mousemove.connect(&camera, &DummyFramework::CModelViewerCamera::onmousemove);

	Application.mousedown.connect(&light, &DummyFramework::CModelViewerCamera::onmousedown);
	Application.mouseup.connect(&light, &DummyFramework::CModelViewerCamera::onmouseup);
	Application.mousemove.connect(&light, &DummyFramework::CModelViewerCamera::onmousemove);

	camera.Button = DummyFramework::MB_Left;
	light.Button = DummyFramework::MB_Right;

	screenfader->faded.connect(this, &Caustics::screenfader_faded);

	causticsrenderer->renderdistance.connect(this, &Caustics::causticsrenderer_renderdistance);
	causticsrenderer->rendercolor.connect(this, &Caustics::causticsrenderer_rendercolor);
	causticsrenderer->photontrace.connect(this, &Caustics::causticsrenderer_photontrace);
	causticsrenderer->rendershadowmap.connect(this, &Caustics::causticsrenderer_photontrace);
	causticsrenderer->rendershadow.connect(this, &Caustics::causticsrenderer_rendershadow);
}
//=============================================================================================================
Caustics::~Caustics()
{
}
//=============================================================================================================
bool Caustics::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	infile.close();

	return true;
}
//=============================================================================================================
bool Caustics::Initialize()
{
	Application.Title = "Caustics (experimental)";

	if( !LoadSettings() )
	{
		DisplayMode.Width = 800;
		DisplayMode.Height = 600;
	}

	bool success = CGame9::Initialize();
	dassert(false, "Caustics::Initialize(): Could not initialize game", success);
	
	camera.ZoomSpeed = 0.05f;
	camera.SetDistance(1.4f);
	camera.SetClip(0.1f, 100);
	camera.SetAspect((float)DisplayMode.Width / (float)DisplayMode.Height);
	camera.SetPosition(0, 0, 0);
	camera.SetFov(D3DX_PI / 2);
	camera.OrbitRight(D3DX_PI * -0.2f);
	camera.OrbitUp(0.25f);
	camera.ClampPitch(-0.55f, D3DX_PI / 2);

	light.SetDistance(1.4f);
	light.SetClip(0.1f, 100);
	light.SetAspect(1);
	light.SetPosition(0, 0, 0);
	light.SetFov(D3DX_PI / 3);
	light.OrbitUp(0.8f);
	light.OrbitRight(D3DX_PI * -0.55f);
	light.ClampPitch(0.45f, D3DX_PI / 2);

	return success;
}
//=============================================================================================================
bool Caustics::LoadContent()
{
#ifdef _DEBUG
	SetCurrentDirectory("../../media/");
#else
	SetCurrentDirectory("../media/");
#endif

	bloomrenderer->MediaDir = "./";
	causticsrenderer->MediaDir = "./";

	objects.resize(3);

	// caustic generator
	objects[0].Initialize(*this);

#if 0
	Content.CreateObject(objects[0], "meshes/angel.qm");
	dassert(false, "Caustics::LoadContent(): Could not create mesh (0)", objects[0].Mesh);

	objects[0].Scale(0.006f, 0.006f, 0.006f);
	objects[0].Translate(0, -1, 0);
#else
	//objects[0].Scale(0.6f, 0.6f, 0.6f);
	objects[0].Mesh = Content.LoadMesh("meshes/sphere.x");
	dassert(false, "Caustics::LoadContent(): Could not create mesh (0)", objects[0].Mesh);
#endif

	objects[0].Material() = Content.LoadMaterial("effects/refraction.fx");
	dassert(false, "Caustics::LoadContent(): Could not create material (0)", objects[0].Material());

	// sky
	objects[1].Initialize(*this);
	objects[1].Scale(10, 10, 10);
	objects[1].Mesh = Content.LoadMesh("meshes/sky.x");
	dassert(false, "Caustics::LoadContent(): Could not create mesh (1)", objects[1].Mesh);

	objects[1].Material() = Content.LoadMaterial("effects/sky.fx");
	dassert(false, "Caustics::LoadContent(): Could not create material (1)", objects[1].Material());

	objects[1].Textures(0) = Content.LoadCubeTexture("textures/sky3.dds");
	derror(false, "Caustics::LoadContent(): Could not create texture (1)", objects[1].Textures(0));
	
	// caustic receiver
	objects[2].Initialize(*this);
	objects[2].Translate(0, -1, 0);
	objects[2].Mesh = Content.CreatePlane(10, 10);
	dassert(false, "Caustics::LoadContent(): Could not create mesh (2)", objects[2].Mesh);

	objects[2].Material() = Content.LoadMaterial("effects/shadowcaustic.fx");
	dassert(false, "Caustics::LoadContent(): Could not create material (2)", objects[2].Material());

	objects[2].Textures(0) = Content.LoadTexture("textures/burloak.jpg");
	dassert(false, "Caustics::LoadContent(): Could not create texture (2)", objects[2].Textures(0));
	
	D3DXVECTOR4 uv(5, 5, 0, 1);
	objects[2].Material()->SetVector("uv", &uv);
	
	return CGame9::LoadContent();
}
//=============================================================================================================
void Caustics::Update()
{
	camera.Update();
	light.Update();

	CGame9::Update();
}
//=============================================================================================================
void Caustics::Draw()
{
	camera.UpdateTransforms((float)Sync.Alpha());
	light.UpdateTransforms((float)Sync.Alpha());

	objects[0].Textures(0) = NULL;
	objects[0].Textures(1) = NULL;
	objects[0].Textures(2) = NULL;
	objects[0].Textures(3) = NULL;
	objects[0].Textures(4) = NULL;

	objects[2].Textures(1) = NULL;

	Graphics->BeginScene();
	{
		objects[2].Material()->SetMatrix("matWorld", &objects[2].GetTransform());

		LPDIRECT3DSURFACE9 oldsurface = NULL;
		Graphics->GetRenderTarget(0, &oldsurface);
		
		// STEP 1: render color + distance maps
		causticsrenderer->DrawColorDistance();

		objects[0].Textures(0) = causticsrenderer->GetColorMap(1);
		objects[0].Textures(1) = causticsrenderer->GetColorMap(2);
		objects[0].Textures(2) = causticsrenderer->GetDistanceMap(0);
		objects[0].Textures(3) = causticsrenderer->GetDistanceMap(1);
		objects[0].Textures(4) = causticsrenderer->GetDistanceMap(2);

		// STEP 2: photon tracing
		causticsrenderer->PhotonTrace();

		// STEP 3: shadows + reconstruct caustics
		StateManager->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		StateManager->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		StateManager->SetTexture(2, NULL);
		objects[2].Textures(1) = causticsrenderer->ShadowMap;

		causticsrenderer->Draw();

		StateManager->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		StateManager->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		// STEP 4: render scene
		Graphics->SetRenderTarget(0, oldsurface);
		oldsurface->Release();

		//StateManager->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		//StateManager->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		camera.GetViewMatrix(view);
		camera.GetProjectionMatrix(proj);
		camera.GetEyePosition(eye);

		D3DXMatrixMultiply(&viewproj, &view, &proj);
		
		objects[0].Material()->SetTechnique("refraction");
		objects[0].Material()->SetMatrix("matViewProj", &viewproj);
		objects[0].Material()->SetMatrix("matWorld", &objects[0].GetTransform());
		objects[0].Material()->SetMatrix("matWorldInv", &objects[0].GetInvTransform());
		objects[0].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

		objects[1].Translate(eye);
		objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
		objects[1].Material()->SetMatrix("matViewProj", &viewproj);
		objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

		objects[2].Material()->SetTechnique("shadowcaustic");
		objects[2].Material()->SetMatrix("matViewProj", &viewproj);
		objects[2].Material()->SetMatrix("lightVP", &lightvp);
		objects[2].Material()->SetVector("lightPos", (D3DXVECTOR4*)&lightpos);
		objects[2].Material()->SetVector("refPoint", (D3DXVECTOR4*)&causticsrenderer->Position);

		objects[2].Textures(1) = causticsrenderer->Caustics;
		objects[2].Textures(2) = causticsrenderer->GetDistanceMap(2);

		bloomrenderer->Enabled = false;
		if( bloomrenderer->CanBeApplied() )
		{
			Graphics->GetRenderTarget(0, &bloomrenderer->BackBuffer);
			Graphics->SetRenderTarget(0, bloomrenderer->LDR);
		}

		Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6694ed, 1.0f, 0);
		
		objects[1].Draw();
		objects[2].Draw();
		objects[0].Draw();

		Graphics->SetVertexDeclaration(DummyFramework::CScreenQuad9::Declaration);

		///////////////////
		StateManager->SetVertexShader(NULL);
		StateManager->SetPixelShader(NULL);

		D3DXMATRIX t(128, 0, 0, 0, 0, 128, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		StateManager->SetTexture(0, causticsrenderer->PhotonHits);
		DrawImage(t);
		///////////////////

		CGame9::Draw();
	}
	Graphics->EndScene();
	
	Graphics->Present(NULL, NULL, NULL, NULL);
	safe_release(bloomrenderer->BackBuffer);
}
//=============================================================================================================
