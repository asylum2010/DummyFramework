//=============================================================================================================
#include "fresnel.h"
#include <fstream>
#include <dummyframework/base/CHelper.h>

Fresnel::Fresnel()
{
	effects.push_back(reflectionrenderer = new DummyFramework::CWaterRenderer9());
	effects.push_back(environmentrenderer = new DummyFramework::CEnvironmentRenderer9());
	
	posteffects.push_back(bloomrenderer = new DummyFramework::CBloomRenderer9());
	posteffects.push_back(screenfader = new DummyFramework::CScreenFader9());

	Application.keyup.connect(this, &Fresnel::application_keyup);
	Application.keydown.connect(&camera, &DummyFramework::CModelViewerCamera::onkeydown);
	Application.keyup.connect(&camera, &DummyFramework::CModelViewerCamera::onkeyup);
	Application.mousedown.connect(&camera, &DummyFramework::CModelViewerCamera::onmousedown);
	Application.mouseup.connect(&camera, &DummyFramework::CModelViewerCamera::onmouseup);
	Application.mousemove.connect(&camera, &DummyFramework::CModelViewerCamera::onmousemove);

	screenfader->faded.connect(this, &Fresnel::screenfader_faded);
	environmentrenderer->render.connect(this, &Fresnel::environmentrenderer_render);
	reflectionrenderer->renderreflection.connect(this, &Fresnel::reflectionrenderer_renderreflection);
}
//=============================================================================================================
Fresnel::~Fresnel()
{
}
//=============================================================================================================
bool Fresnel::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	infile.close();

	return true;
}
//=============================================================================================================
bool Fresnel::Initialize()
{
	Application.Title = "Fresnel";

	if( !LoadSettings() )
	{
		DisplayMode.Width = 800;
		DisplayMode.Height = 600;
	}

	bool success = CGame9::Initialize();
	dassert(false, "Fresnel::Initialize(): Could not initialize game", success);

	// ez kell mert erre tükrözi a kamerát
	reflectionrenderer->SetClipPlane(D3DXPLANE(0, -1, 0, 1));
	reflectionrenderer->Clip = false;
	reflectionrenderer->Frequency = 1;
	environmentrenderer->Frequency = 1;
	
	camera.ZoomSpeed = 0.05f;
	camera.SetDistance(1.4f);
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
bool Fresnel::LoadContent()
{
#ifdef _DEBUG
	SetCurrentDirectory("../../../../media/");
#else
	SetCurrentDirectory("../media/");
#endif

	bloomrenderer->MediaDir = "./";
	objects.resize(3);

	// teapot
	objects[0].Initialize(*this);
	objects[0].Scale(0.6f, 0.6f, 0.6f);
	objects[0].Mesh = Content.LoadMesh("meshes/teapot.x");
	dassert(false, "Fresnel::LoadContent(): Could not create mesh (0)", objects[0].Mesh);

	objects[0].Material() = Content.LoadMaterial("effects/fresnel.fx");
	dassert(false, "Fresnel::LoadContent(): Could not create material (0)", objects[0].Material());

	objects[0].Textures(0) = Content.LoadTexture("textures/wood.tga");
	dassert(false, "Fresnel::LoadContent(): Could not create texture (0)", objects[0].Textures(0));
	
	// sky
	objects[1].Initialize(*this);
	objects[1].Scale(20, 20, 20);
	objects[1].Mesh = Content.LoadMesh("meshes/sky.x");
	dassert(false, "Fresnel::LoadContent(): Could not create mesh (1)", objects[1].Mesh);

	objects[1].Material() = Content.LoadMaterial("effects/sky.fx");
	dassert(false, "Fresnel::LoadContent(): Could not create material (1)", objects[1].Material());

	objects[1].Textures(0) = Content.LoadCubeTexture("textures/sky2.dds");
	derror(false, "Fresnel::LoadContent(): Could not create texture (1)", objects[1].Textures(0));

	objects[1].Textures(1) = Content.LoadCubeTexture("textures/sky1.dds");
	dassert(false, "Fresnel::LoadContent(): Could not create texture (1)", objects[1].Textures(1));

	// tiles
	objects[2].Initialize(*this);
	objects[2].Translate(0, -1, 0);
	objects[2].Mesh = Content.CreatePlane(10, 10);
	dassert(false, "Fresnel::LoadContent(): Could not create mesh (2)", objects[2].Mesh);

	objects[2].Material() = Content.LoadMaterial("effects/tiles.fx");
	dassert(false, "Fresnel::LoadContent(): Could not create material (2)", objects[2].Material());

	objects[2].Textures(0) = Content.LoadTexture("textures/tile.jpg");
	dassert(false, "Fresnel::LoadContent(): Could not create texture (2)", objects[2].Textures(0));

	objects[2].Textures(2) = Content.LoadTexture("textures/tile_normal.bmp");
	dassert(false, "Fresnel::LoadContent(): Could not create texture (2)", objects[2].Textures(2));

	D3DXVECTOR4 uv(5, 5, 0, 1);
	objects[2].Material()->SetVector("uv", &uv);

	return CGame9::LoadContent();
}
//=============================================================================================================
void Fresnel::Update()
{
	camera.Update();
	CGame9::Update();
}
//=============================================================================================================
void Fresnel::Draw()
{
	camera.UpdateTransforms((float)Sync.Alpha());
	camera.GetViewMatrix(view);
	camera.GetProjectionMatrix(proj);
	camera.GetEyePosition(eye);

	objects[0].Textures(1) = NULL;
	objects[2].Textures(1) = NULL;

	Graphics->BeginScene();
	{
		objects[0].Material()->SetMatrix("matWorld", &objects[0].GetTransform());
		objects[0].Material()->SetMatrix("matWorldInv", &objects[0].GetInvTransform());
		objects[2].Material()->SetMatrix("matWorld", &objects[2].GetTransform());

		environmentrenderer->Draw();
		objects[0].Textures(1) = environmentrenderer->Texture;
		
		// render floor textures
		reflectionrenderer->SetViewMatrix(view);
		reflectionrenderer->SetProjectionMatrix(proj);
		reflectionrenderer->SetEyePosition(eye);
		reflectionrenderer->Draw();

		// render scene
		D3DXMatrixMultiply(&viewproj, &view, &proj);

		objects[0].Material()->SetTechnique("fresnel");
		objects[0].Material()->SetMatrix("matViewProj", &viewproj);
		objects[0].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

		objects[1].Translate(eye);
		objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
		objects[1].Material()->SetMatrix("matViewProj", &viewproj);
		objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

		objects[2].Textures(1) = reflectionrenderer->ReflectionTexture;
		objects[2].Material()->SetTechnique("tiles");
		objects[2].Material()->SetMatrix("matViewProj", &viewproj);
		objects[2].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

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
		CGame9::Draw();
	}
	Graphics->EndScene();
	
	Graphics->Present(NULL, NULL, NULL, NULL);
	safe_release(bloomrenderer->BackBuffer);
}
//=============================================================================================================
