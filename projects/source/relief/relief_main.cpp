//=============================================================================================================
#include "relief.h"
#include <fstream>

Relief::Relief()
{
	posteffects.push_back(bloomrenderer = new DummyFramework::CBloomRenderer9());
	posteffects.push_back(screenfader = new DummyFramework::CScreenFader9());

	Application.keyup.connect(this, &Relief::application_keyup);
	Application.keydown.connect(&camera, &DummyFramework::CModelViewerCamera::onkeydown);
	Application.keyup.connect(&camera, &DummyFramework::CModelViewerCamera::onkeyup);
	Application.mousedown.connect(&camera, &DummyFramework::CModelViewerCamera::onmousedown);
	Application.mouseup.connect(&camera, &DummyFramework::CModelViewerCamera::onmouseup);
	Application.mousemove.connect(&camera, &DummyFramework::CModelViewerCamera::onmousemove);

	screenfader->faded.connect(this, &Relief::screenfader_faded);
}
//=============================================================================================================
Relief::~Relief()
{
}
//=============================================================================================================
bool Relief::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	infile.close();

	return true;
}
//=============================================================================================================
bool Relief::Initialize()
{
	Application.Title = "Relief";

	if( !LoadSettings() )
	{
		DisplayMode.Width = 800;
		DisplayMode.Height = 600;
	}

	bool success = CGame9::Initialize();
	dassert(false, "Relief::Initialize(): Could not initialize game", success);

	camera.ZoomSpeed = 0.05f;
	camera.SetDistance(1.5f);
	camera.SetClip(0.1f, 100);
	camera.SetAspect((float)DisplayMode.Width / (float)DisplayMode.Height);
	camera.SetPosition(0, 0, 0);
	camera.SetFov(D3DX_PI / 2);
	camera.OrbitRight(-1.2f * D3DX_PI);
	camera.OrbitUp(0.45f);
	camera.ClampPitch(-0.55f, D3DX_PI / 2);

	return success;
}
//=============================================================================================================
bool Relief::LoadContent()
{
#ifdef _DEBUG
	SetCurrentDirectory("../../../media/");
#else
	SetCurrentDirectory("../media/");
#endif

	bloomrenderer->MediaDir = "./";
	objects.resize(3);

	// main obj
	objects[0].Initialize(*this);
	objects[0].Scale(0.7f, 0.7f, 0.7f);
	objects[0].Mesh = Content.LoadMesh("meshes/teapot.x");
	dassert(false, "Relief::LoadContent(): Could not create mesh (0)", objects[0].Mesh);

	objects[0].Material() = Content.LoadMaterial("effects/relief.fx");
	dassert(false, "Relief::LoadContent(): Could not create material (0)", objects[0].Material());

	objects[0].Textures(0) = Content.LoadTexture("textures/green.jpg");
	dassert(false, "Relief::LoadContent(): Could not create texture (0)", objects[0].Textures(0));
	
	objects[0].Textures(1) = Content.LoadTexture("textures/saint_nh.dds");
	dassert(false, "Relief::LoadContent(): Could not create texture (1)", objects[0].Textures(1));
	
	// sky
	objects[1].Initialize(*this);
	objects[1].Scale(20, 20, 20);
	objects[1].Mesh = Content.LoadMesh("meshes/sky.x");
	dassert(false, "Relief::LoadContent(): Could not create mesh (1)", objects[1].Mesh);

	objects[1].Material() = Content.LoadMaterial("effects/sky.fx");
	dassert(false, "Relief::LoadContent(): Could not create material (1)", objects[1].Material());

	objects[1].Textures(0) = Content.LoadCubeTexture("textures/sky1.dds");
	derror(false, "Relief::LoadContent(): Could not create texture (1)", objects[1].Textures(0));
	
	// tiles
	objects[2].Initialize(*this);
	objects[2].Translate(0, -1, 0);
	objects[2].Mesh = Content.CreatePlane(10, 10);
	dassert(false, "Relief::LoadContent(): Could not create mesh (2)", objects[2].Mesh);

	objects[2].Material() = Content.LoadMaterial("effects/relief.fx");
	dassert(false, "Relief::LoadContent(): Could not create material (2)", objects[2].Material());

	objects[2].Textures(0) = Content.LoadTexture("textures/parq.dds");
	dassert(false, "Relief::LoadContent(): Could not create texture (2)", objects[2].Textures(0));

	objects[2].Textures(1) = Content.LoadTexture("textures/parq_nh.dds");
	dassert(false, "Relief::LoadContent(): Could not create texture (2)", objects[2].Textures(1));

	Content.GenerateTangentFrame(&objects[0].Mesh);
	Content.GenerateTangentFrame(&objects[2].Mesh);

	return CGame9::LoadContent();
}
//=============================================================================================================
void Relief::Update()
{
	camera.Update();
	CGame9::Update();
}
//=============================================================================================================
void Relief::Draw()
{
	camera.UpdateTransforms((float)Sync.Alpha());
	camera.GetViewMatrix(view);
	camera.GetProjectionMatrix(proj);
	camera.GetEyePosition(eye);
    
	D3DXMatrixMultiply(&viewproj, &view, &proj);

	objects[1].Translate(eye);
	objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
	objects[1].Material()->SetMatrix("matViewProj", &viewproj);
	objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

	objects[0].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);
	objects[0].Material()->SetMatrix("matViewProj", &viewproj);

	if( bloomrenderer->CanBeApplied() )
	{
		Graphics->GetRenderTarget(0, &bloomrenderer->BackBuffer);
		Graphics->SetRenderTarget(0, bloomrenderer->LDR);
	}

	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6694ed, 1.0f, 0);
	Graphics->BeginScene();
	{
		objects[1].Draw();

		D3DXVECTOR4 uv(5, 5, 0, 1);
		D3DXVECTOR4 lp(-5, 10, 10, 1);

		objects[0].ApplyMaterial();
		{
			objects[0].Material()->SetVector("lightPos", &lp);
			objects[0].Material()->SetVector("uv", &uv);
			objects[0].Material()->SetFloat("heightScale", 0.08f);
			objects[0].Material()->SetFloat("specularIntensity", 0.3f);
			objects[0].Material()->SetMatrix("matWorld", &objects[2].GetTransform());
			objects[0].Material()->SetMatrix("matWorldInv", &objects[2].GetInvTransform());
			objects[0].Material()->CommitChanges();

			objects[2].DrawOptimized();

			uv = D3DXVECTOR4(1, 1, 0, 1);

			objects[0].Material()->SetVector("uv", &uv);
			objects[0].Material()->SetFloat("heightScale", 0.1f);
			objects[0].Material()->SetFloat("specularIntensity", 0.8f);
			objects[0].Material()->SetMatrix("matWorld", &objects[0].GetTransform());
			objects[0].Material()->SetMatrix("matWorldInv", &objects[0].GetInvTransform());
			objects[0].Material()->CommitChanges();

			objects[0].DrawOptimized();
		}
		objects[0].UnapplyMaterial();

		Graphics->SetVertexDeclaration(DummyFramework::CScreenQuad9::Declaration);
		CGame9::Draw();
	}
	Graphics->EndScene();
	
	Graphics->Present(NULL, NULL, NULL, NULL);
	safe_release(bloomrenderer->BackBuffer);
}
//=============================================================================================================
