//=============================================================================================================
#include "godray.h"
#include <fstream>

Godray::Godray()
{
	effects.push_back(reflectionrenderer = new DummyFramework::CWaterRenderer9());

	posteffects.push_back(bloomrenderer = new DummyFramework::CBloomRenderer9());
	posteffects.push_back(godrayrenderer = new DummyFramework::CGodRayRenderer9());
	posteffects.push_back(screenfader = new DummyFramework::CScreenFader9());

	Application.keyup.connect(this, &Godray::application_keyup);
	Application.mousedown.connect(&camera, &DummyFramework::CModelViewerCamera::onmousedown);
	Application.mouseup.connect(&camera, &DummyFramework::CModelViewerCamera::onmouseup);
	Application.mousemove.connect(&camera, &DummyFramework::CModelViewerCamera::onmousemove);
	Application.keydown.connect(&camera, &DummyFramework::CModelViewerCamera::onkeydown);
	Application.keyup.connect(&camera, &DummyFramework::CModelViewerCamera::onkeyup);

	screenfader->faded.connect(this, &Godray::screenfader_faded);
	reflectionrenderer->renderreflection.connect(this, &Godray::reflectionrenderer_renderreflection);
}
//=============================================================================================================
Godray::~Godray()
{
}
//=============================================================================================================
bool Godray::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	infile.close();

	return true;
}
//=============================================================================================================
bool Godray::Initialize()
{
	Application.Title = "Godray";

	if( !LoadSettings() )
	{
		DisplayMode.Width = 800;
		DisplayMode.Height = 600;
	}
	
	bool success = CGame9::Initialize();
	dassert(false, "Godray::Initialize(): Could not initialize game", success);

	// ez kell mert erre tükrözi a kamerát
	reflectionrenderer->SetClipPlane(D3DXPLANE(0, -1, 0, 0.9f));
	reflectionrenderer->Clip = false;
	reflectionrenderer->Frequency = 1;

	camera.ZoomSpeed = 0.05f;
	camera.SetDistance(1.4f);
	camera.SetClip(0.1f, 100);
	camera.SetAspect((float)DisplayMode.Width / (float)DisplayMode.Height);
	camera.SetPosition(0, 0.5f, 0);
	camera.SetFov(D3DX_PI / 2);
	camera.OrbitRight(-1.2f * D3DX_PI);
	camera.OrbitUp(0.45f);
	camera.ClampPitch(-0.3f, D3DX_PI / 2);
	camera.ClampZoom(0.2f, 3.0f);

	return success;
}
//=============================================================================================================
bool Godray::LoadContent()
{
#ifdef _DEBUG
	SetCurrentDirectory("../../../media/");
#else
	SetCurrentDirectory("../media/");
#endif

	bloomrenderer->MediaDir = "./";
	godrayrenderer->MediaDir = "./";

	objects.resize(3);

	// palm
	objects[0].Initialize(*this);
	objects[0].Scale(2.0f, 2.0f, 2.0f);
	objects[0].Translate(0, -0.9f, 0);
	
	Content.CreateObject(objects[0], "meshes/palm.qm");
	dassert(false, "Godray::LoadContent(): Could not create mesh (0)", objects[0].Mesh);
	
	objects[0].Material() = Content.LoadMaterial("effects/blinnphong.fx");
	dassert(false, "Godray::LoadContent(): Could not create material (0)", objects[0].Material(0));
	
	// sky
	objects[1].Initialize(*this);
	objects[1].Scale(20, 20, 20);
	objects[1].Mesh = Content.LoadMesh("meshes/sky.x");
	dassert(false, "Godray::LoadContent(): Could not create mesh (1)", objects[1].Mesh);

	objects[1].Material() = Content.LoadMaterial("effects/sky.fx");
	dassert(false, "Godray::LoadContent(): Could not create material (1)", objects[1].Material());

	objects[1].Textures(0) = Content.LoadCubeTexture("textures/sky2.dds");
	dassert(false, "Godray::LoadContent(): Could not create texture (1)", objects[1].Textures(0));
	
	// tiles
	objects[2].Initialize(*this);
	objects[2].Translate(0, -0.9f, 0);
	objects[2].Mesh = Content.CreatePlane(10, 10);
	dassert(false, "Godray::LoadContent(): Could not create mesh (2)", objects[2].Mesh);

	objects[2].Material() = Content.LoadMaterial("effects/tiles.fx");
	dassert(false, "Godray::LoadContent(): Could not create material (2)", objects[2].Material());

	objects[2].Textures(0) = Content.LoadTexture("textures/tile.jpg");
	dassert(false, "Godray::LoadContent(): Could not create texture (2)", objects[2].Textures(0));

	objects[2].Textures(2) = Content.LoadTexture("textures/tile_normal.bmp");
	dassert(false, "Fresnel::LoadContent(): Could not create texture (2)", objects[2].Textures(2));

	D3DXVECTOR4 uv(5, 5, 0, 1);
	objects[2].Material()->SetVector("uv", &uv);

	return CGame9::LoadContent();
}
//=============================================================================================================
void Godray::Update()
{
	camera.Update();
	CGame9::Update();
}
//=============================================================================================================
void Godray::Draw()
{
    camera.UpdateTransforms((float)Sync.Alpha());
	camera.GetViewMatrix(view);
	camera.GetProjectionMatrix(proj);
	camera.GetEyePosition(eye);

	D3DXMatrixMultiply(&viewproj, &view, &proj);

	godrayrenderer->Sun = D3DXVECTOR3(542, 740, 1000);
	godrayrenderer->Eye = eye;
	godrayrenderer->View = view;
	godrayrenderer->ViewProj = viewproj;

	// ezek végig ugyanazok
	objects[0].Material(0)->SetMatrix("matWorld", &objects[0].GetTransform());
	objects[0].Material(0)->SetMatrix("matWorldInv", &objects[0].GetInvTransform());

	objects[2].Material()->SetMatrix("matWorld", &objects[2].GetTransform());
	objects[2].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);
	
	if( godrayrenderer->CanBeApplied() )
	{
		if( CanUseMultipleRTs() )
		{
			objects[1].Material()->SetTechnique("sky_mrt");
			objects[2].Material()->SetTechnique("tiles_mrt");

			if( bloomrenderer->CanBeApplied() )
				DrawGodrayBloomMRT();
			else
				DrawGodrayMRT();
		}
		else
		{
			objects[1].Material()->SetTechnique("sky");
			objects[2].Material()->SetTechnique("tiles");

			if( bloomrenderer->CanBeApplied() )
				DrawGodrayBloom();
			else
				DrawGodray();
		}
	}
	else
		DrawScene();

	Graphics->Present(NULL, NULL, NULL, NULL);
}
//=============================================================================================================
