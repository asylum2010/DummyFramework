//=============================================================================================================
#include "networktest.h"
#include <fstream>

NetworkTest::NetworkTest()
{
	cansend = false;
	service = NetworkServiceProvider::Create();

	airplanes.reserve(20);
	airplanes.push_back(AirPlane());

	Application.keyup.connect(this, &NetworkTest::application_keyup);
	Application.closed.connect(this, &NetworkTest::application_closed);

	Application.mousedown.connect(&camera, &DummyFramework::CModelViewerCamera::onmousedown);
	Application.mouseup.connect(&camera, &DummyFramework::CModelViewerCamera::onmouseup);
	Application.mousemove.connect(&camera, &DummyFramework::CModelViewerCamera::onmousemove);

	AirPlane& avatar = *airplanes.begin();

	Application.keyup.connect(&avatar, &AirPlane::onkeyup);
	Application.keydown.connect(&avatar, &AirPlane::onkeydown);
}
//=============================================================================================================
NetworkTest::~NetworkTest()
{
	worker.Close();

	safe_delete(service);
}
//=============================================================================================================
bool NetworkTest::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	infile.close();

	return true;
}
//=============================================================================================================
bool NetworkTest::Initialize()
{
	Application.Title = "NetworkTest (experimental)";

	MultiThreaded = true;
	RenderDeactive = true;

	if( !LoadSettings() )
	{
		DisplayMode.Width = 800;
		DisplayMode.Height = 600;
	}

	bool success = CGame9::Initialize();
	dassert(false, "NetworkTest::Initialize(): Could not initialize game", success);

	camera.ClampZoom(1, 100);
	camera.SetDistance(20);
	camera.SetClip(0.1f, 5000);
	camera.SetAspect((float)DisplayMode.Width / (float)DisplayMode.Height);
	camera.SetPosition(0, 0, 0);
	camera.SetFov(D3DX_PI / 4);
	camera.OrbitRight(D3DX_PI);
	camera.OrbitUp(0.5f);

	// determine connection type
	isserver = false;

	std::cout << "Do you wish to host (1) or connect (2)?: ";
	int answer = -1;

	while( answer != 1 && answer != 2 )
	{
		std::cin >> answer;
	}

	if( answer == 2 )
	{
		std::cout << "Host ip: ";
		std::cin >> hostip;

		SetWindowText(Application.GetWindowHandle(), "NetworkTest - Client");
	}
	else
		SetWindowText(Application.GetWindowHandle(), "NetworkTest - Server");

	isserver = (answer == 1);

	success = worker.Attach(*this);
	dassert(false, "NetworkTest::Initialize(): Could not attach thread", success);

	return success;
}
//=============================================================================================================
bool NetworkTest::LoadContent()
{
#ifdef _DEBUG
	SetCurrentDirectory("../../media/");
#else
	SetCurrentDirectory("../media/");
#endif

	AirPlane& avatar = *airplanes.begin();
	avatar.Initialize(*this);

	bool success = Content.CreateObject(avatar, "meshes/airplane.x");
	dassert(false, "Aircraft::LoadContent(): Could not load 'airplane.x'", success);

	ground.Initialize(*this);
	ground.Translate(0, -5, 0);
	ground.Mesh = Content.CreateBox(1000, 0.2f, 1000);
	dassert(false, "Aircraft::LoadContent(): Could not create 'ground'", ground.Mesh);

	ground.Textures(0) = Content.LoadTexture("textures/tile.jpg");
	dassert(false, "Aircraft::LoadContent(): Could not create 'tile.jpg'", ground.Textures(0));

	success = CGame9::LoadContent();

	if( success )
		worker.Start();

	return success;
}
//=============================================================================================================
void NetworkTest::ResetRenderStates()
{
	StateManager->SetRenderState(D3DRS_LIGHTING, false);
	StateManager->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	StateManager->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	StateManager->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}
//=============================================================================================================
void NetworkTest::Update()
{
	cansend = airplanes.begin()->Interacting();

	// a többit packeten keresztül updateli
	for( size_t i = 0; i < airplanes.size(); ++i )
		airplanes[i].Update(Sync.Timer().Time());

	camera.Update();
	CGame9::Update();
}
//=============================================================================================================
void NetworkTest::Draw()
{
	for( size_t i = 0; i < airplanes.size(); ++i )
		airplanes[i].UpdateTransforms((float)Sync.Alpha());

	AirPlane& avatar = *airplanes.begin();

	camera.SetPosition(avatar.GetTranslation());
	camera.UpdateTransforms((float)Sync.Alpha());
	camera.GetViewMatrix(view);
	camera.GetProjectionMatrix(proj);

	Graphics->SetTransform(D3DTS_VIEW, &view);
	Graphics->SetTransform(D3DTS_PROJECTION, &proj);

	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6694ed, 1.0f, 0);

	Graphics->BeginScene();
	{
		Graphics->SetTransform(D3DTS_WORLD, &ground.GetTransform());
		ground.Draw();

		for( planelist::iterator it = airplanes.begin(); it != airplanes.end(); ++it )
		{
			Graphics->SetTransform(D3DTS_WORLD, &it->GetTransform());
			avatar.Draw();
		}

		CGame9::Draw();
	}
	Graphics->EndScene();
	
	Graphics->Present(NULL, NULL, NULL, NULL);
}
//=============================================================================================================
