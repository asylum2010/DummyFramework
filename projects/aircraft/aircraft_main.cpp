//=============================================================================================================
#include "aircraft.h"
#include <fstream>

Aircraft::Aircraft()
{
	Application.keyup.connect(this, &Aircraft::application_keyup);

	Application.keyup.connect(&plane, &AirPlane::onkeyup);
	Application.keydown.connect(&plane, &AirPlane::onkeydown);
}
//=============================================================================================================
Aircraft::~Aircraft()
{
}
//=============================================================================================================
bool Aircraft::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	infile.close();

	return true;
}
//=============================================================================================================
bool Aircraft::Initialize()
{
	Application.Title = "Aircraft";

	if( !LoadSettings() )
	{
		DisplayMode.Width = 800;
		DisplayMode.Height = 600;
	}
	
	bool success = CGame9::Initialize();
	dassert(false, "Aircraft::Initialize(): Could not initialize game", success);

	camera.ClampZoom(1, 100);
	camera.SetDistance(20);
	camera.SetClip(0.1f, 5000);
	camera.SetAspect((float)DisplayMode.Width / (float)DisplayMode.Height);
	camera.SetPosition(0, 0, 0);
	camera.SetFov(D3DX_PI / 4);
	camera.OrbitRight(D3DX_PI);
	camera.OrbitUp(0.5f);

	return success;
}
//=============================================================================================================
bool Aircraft::LoadContent()
{
#ifdef _DEBUG
	SetCurrentDirectory("../../media/");
#else
	SetCurrentDirectory("../media/");
#endif

	plane.Initialize(*this);

	bool success = Content.CreateObject(plane, "meshes/airplane.x");
	dassert(false, "Aircraft::LoadContent(): Could not load 'airplane.x'", success);
	
	ground.Initialize(*this);
	ground.Translate(0, -5, 0);
	ground.Mesh = Content.CreateBox(1000, 0.2f, 1000);
	dassert(false, "Aircraft::LoadContent(): Could not create 'ground'", ground.Mesh);

	ground.Textures(0) = Content.LoadTexture("textures/tile.jpg");
	dassert(false, "Aircraft::LoadContent(): Could not create 'tile.jpg'", ground.Textures(0));

	return CGame9::LoadContent();
}
//=============================================================================================================
void Aircraft::ResetRenderStates()
{
	StateManager->SetRenderState(D3DRS_LIGHTING, false);
	StateManager->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	StateManager->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	StateManager->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}
//=============================================================================================================
void Aircraft::Update()
{
	camera.Update();
	plane.Update(Sync.Timer().Time());

	CGame9::Update();
}
//=============================================================================================================
void Aircraft::Draw()
{
	plane.UpdateTransforms((float)Sync.Alpha());
	
	camera.SetPosition(plane.GetTranslation());
	camera.UpdateTransforms((float)Sync.Alpha());
	camera.GetViewMatrix(view);
	camera.GetProjectionMatrix(proj);
	
	Graphics->SetTransform(D3DTS_VIEW, &view);
	Graphics->SetTransform(D3DTS_PROJECTION, &proj);
	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6694ed, 1.0f, 0);

	Graphics->BeginScene();
	{
		Graphics->SetTransform(D3DTS_WORLD, &plane.GetTransform());
		plane.Draw();

		Graphics->SetTransform(D3DTS_WORLD, &ground.GetTransform());
		ground.Draw();

		CGame9::Draw();
	}
	Graphics->EndScene();
	
	Graphics->Present(NULL, NULL, NULL, NULL);
}
//=============================================================================================================
