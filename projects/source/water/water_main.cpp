//=============================================================================================================
#include "water.h"
#include <fstream>

Water::Water()
{
	effects.push_back(waterrenderer = new DummyFramework::CWaterRenderer9());
	posteffects.push_back(bloomrenderer = new DummyFramework::CBloomRenderer9());
	posteffects.push_back(screenfader = new DummyFramework::CScreenFader9());

	Application.keyup.connect(this, &Water::application_keyup);
	Application.mousedown.connect(&camera, &DummyFramework::CModelViewerCamera::onmousedown);
	Application.mouseup.connect(&camera, &DummyFramework::CModelViewerCamera::onmouseup);
	Application.mousemove.connect(&camera, &DummyFramework::CModelViewerCamera::onmousemove);
	Application.keydown.connect(&camera, &DummyFramework::CModelViewerCamera::onkeydown);
	Application.keyup.connect(&camera, &DummyFramework::CModelViewerCamera::onkeyup);

	screenfader->faded.connect(this, &Water::screenfader_faded);
	waterrenderer->renderreflection.connect(this, &Water::waterrenderer_renderreflection);
}
//=============================================================================================================
Water::~Water()
{
}
//=============================================================================================================
bool Water::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	infile.close();

	return true;
}
//=============================================================================================================
bool Water::Initialize()
{
	Application.Title = "Water";

	if( !LoadSettings() )
	{
		DisplayMode.Width = 800;
		DisplayMode.Height = 600;
	}

	bool success = CGame9::Initialize();
	dassert(false, "Water::Initialize(): Could not initialize game", success);

	waterrenderer->SetClipPlane(D3DXPLANE(0, 1, 0, 1));
	waterrenderer->Frequency = 1;
	waterrenderer->Clip = true;

	camera.SetDistance(5.0f);
	camera.SetClip(0.1f, 210);
	camera.SetAspect((float)DisplayMode.Width / (float)DisplayMode.Height);
	camera.SetPosition(0, 1.2f, 0);
	camera.SetFov(D3DX_PI / 2);
	camera.OrbitRight(1.13f * D3DX_PI);
	camera.OrbitUp(0.55f);
	camera.ClampPitch(-0.4f, D3DX_PI / 2 - 0.55f);
	camera.ClampZoom(0.5f, 5.0f);

	return success;
}
//=============================================================================================================
bool Water::LoadContent()
{
#ifdef _DEBUG
	SetCurrentDirectory("../../../media/");
#else
	SetCurrentDirectory("../media/");
#endif

	bloomrenderer->MediaDir = "./";
	objects.resize(4);

	// palm
	objects[0].Initialize(*this);
	objects[0].Translate(0, -2, 0);
	objects[0].Scale(6, 6, 6);

	Content.CreateObject(objects[0], "meshes/palm.qm");
	dassert(false, "Water::LoadContent(): Could not create mesh (0)", objects[0].Mesh);
	
	objects[0].Material() = Content.LoadMaterial("effects/blinnphong.fx");
	dassert(false, "Water::LoadContent(): Could not create material (0)", objects[0].Material());

	// sky
	objects[1].Initialize(*this);
	objects[1].Scale(200, 200, 200);
	objects[1].Mesh = Content.LoadMesh("meshes/sky.x");
	dassert(false, "Water::LoadContent(): Could not create mesh (1)", objects[1].Mesh);

	objects[1].Material() = Content.LoadMaterial("effects/sky.fx");
	dassert(false, "Water::LoadContent(): Could not create material (1)", objects[1].Material());

	objects[1].Textures(0) = Content.LoadCubeTexture("textures/sky2.dds");
	dassert(false, "Water::LoadContent(): Could not create texture (1)", objects[1].Textures(0));
	
	// water
	objects[2].Initialize(*this);
	objects[2].Translate(0, -1, 0);
	objects[2].Mesh = Content.CreatePlane(150, 150);
	dassert(false, "Water::LoadContent(): Could not create mesh (2)", objects[2].Mesh);

	objects[2].Material() = Content.LoadMaterial("effects/water.fx");
	dassert(false, "Water::LoadContent(): Could not create material (2)", objects[2].Material());

	objects[2].Textures(0) = Content.LoadTexture("textures/wave.tga");
	dassert(false, "Water::LoadContent(): Could not create texture (2)", objects[2].Textures(0));

	// tiles
	objects[3].Initialize(*this);
	objects[3].Translate(0, -2, 0);
	objects[3].Mesh = Content.CreatePlane(150, 150);
	dassert(false, "Water::LoadContent(): Could not create mesh (3)", objects[3].Mesh);

	objects[3].Material() = Content.LoadMaterial("effects/diffuse.fx");
	dassert(false, "Water::LoadContent(): Could not create material (3)", objects[3].Material());

	objects[3].Textures(0) = Content.LoadTexture("textures/sand.jpg");
	dassert(false, "Water::LoadContent(): Could not create texture (3)", objects[3].Textures(0));

	D3DXVECTOR2 uv(30, 30);
	objects[3].Material()->SetVector("uv", (D3DXVECTOR4*)&uv);

	return CGame9::LoadContent();
}
//=============================================================================================================
void Water::Update()
{
	camera.Update();
	CGame9::Update();
}
//=============================================================================================================
void Water::Draw()
{
	camera.UpdateTransforms((float)Sync.Alpha());
	camera.GetViewMatrix(view);
	camera.GetProjectionMatrix(proj);
	camera.GetEyePosition(eye);
	
	objects[2].Textures(2) = NULL;
	objects[2].Textures(1) = NULL;

	Graphics->BeginScene();
	{
		objects[0].Material()->SetMatrix("matWorld", &objects[0].GetTransform());
		objects[0].Material()->SetMatrix("matWorldInv", &objects[0].GetInvTransform());
		objects[0].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

		// render water textures
		waterrenderer->SetViewMatrix(view);
		waterrenderer->SetProjectionMatrix(proj);
		waterrenderer->SetEyePosition(eye);
		waterrenderer->Draw();

		// render scene
		D3DXMatrixMultiply(&viewproj, &view, &proj);
		objects[0].Material()->SetMatrix("matViewProj", &viewproj);
		
		objects[1].Translate(eye);
		objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
		objects[1].Material()->SetMatrix("matViewProj", &viewproj);
		objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

		objects[2].Material()->SetMatrix("matWorld", &objects[2].GetTransform());
		objects[2].Material()->SetMatrix("matViewProj", &viewproj);
		objects[2].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);
		objects[2].Material()->SetFloat("time", (float)Sync.Timer().Time());
		
		objects[3].Material()->SetMatrix("matWorld", &objects[3].GetTransform());
		objects[3].Material()->SetMatrix("matViewProj", &viewproj);

		if( bloomrenderer->CanBeApplied() )
		{
			Graphics->GetRenderTarget(0, &bloomrenderer->BackBuffer);
			Graphics->SetRenderTarget(0, bloomrenderer->LDR);
		}

		Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6694ed, 1.0f, 0);

		// alfa maszk
		objects[2].Material()->SetTechnique("alpha");

		objects[1].Draw();
		objects[0].Draw();
		objects[3].Draw();

		StateManager->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);
		objects[2].Draw();

		StateManager->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);
		objects[2].Material()->SetTechnique("water");
	
		// copy amit eddig kirajzoltunk
		Graphics->StretchRect(bloomrenderer->LDR, NULL, waterrenderer->GetRefractionSurface(), NULL, D3DTEXF_LINEAR);

		// amit nyerünk: nem kell kirenderelni a refractiont
		objects[2].Textures(2) = waterrenderer->ReflectionTexture;
		objects[2].Textures(1) = waterrenderer->RefractionTexture;

		objects[2].Draw();
		
		Graphics->SetVertexDeclaration(DummyFramework::CScreenQuad9::Declaration);
		CGame9::Draw();
	}
	Graphics->EndScene();
		
	Graphics->Present(NULL, NULL, NULL, NULL);
	safe_release(bloomrenderer->BackBuffer);
}
//=============================================================================================================
