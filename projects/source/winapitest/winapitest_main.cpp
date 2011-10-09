//=============================================================================================================
#include "winapitest.h"
#include <fstream>

WinapiTest::WinapiTest()
{
	posteffects.push_back(bloomrenderer = new DummyFramework::CBloomRenderer9());

	Application.keyup.connect(this, &WinapiTest::application_keyup);
	Application.command.connect(this, &WinapiTest::application_command);
	Application.notify.connect(this, &WinapiTest::application_notify);
}
//=============================================================================================================
WinapiTest::~WinapiTest()
{
}
//=============================================================================================================
bool WinapiTest::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	infile.close();

	return true;
}
//=============================================================================================================
bool WinapiTest::Initialize()
{
	Application.Title = "WinapiTest";

	if( !LoadSettings() )
	{
		DisplayMode.Width = 800;
		DisplayMode.Height = 600;
	}

	// init gui
	InitializeGUI();

	// ide rendereljen
	PresentationParameters.hDeviceWindow = picturebox1.GetHandle();

	bool success = CGame9::Initialize();
	dassert(false, "WinapiTest::Initialize(): Could not initialize game", success);

	return success;
}
//=============================================================================================================
bool WinapiTest::LoadContent()
{
#ifdef _DEBUG
	SetCurrentDirectory("../../../media/");
#else
	SetCurrentDirectory("../media/");
#endif

	bloomrenderer->MediaDir = "./";
	bloomrenderer->Enabled = false;

	current = NULL;
	D3DXMatrixIdentity(&world);
	
	return CGame9::LoadContent();
}
//=============================================================================================================
void WinapiTest::Update()
{
	CGame9::Update();
}
//=============================================================================================================
void WinapiTest::Draw()
{
	if( bloomrenderer->CanBeApplied() )
	{
		Graphics->GetRenderTarget(0, &bloomrenderer->BackBuffer);
		Graphics->SetRenderTarget(0, bloomrenderer->LDR);
	}

	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6694ed, 1.0f, 0);
	Graphics->BeginScene();
	{
		Graphics->SetVertexDeclaration(DummyFramework::CScreenQuad9::Declaration);

		if( current )
		{
			StateManager->SetTexture(0, current);
			DrawImage(world);
		}

		CGame9::Draw();
	}
	Graphics->EndScene();
	
	Graphics->Present(NULL, NULL, NULL, NULL);
	safe_release(bloomrenderer->BackBuffer);
}
//=============================================================================================================
