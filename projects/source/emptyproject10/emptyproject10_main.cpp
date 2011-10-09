//=============================================================================================================
#include "emptyproject10.h"
#include <fstream>

EmptyProject10::EmptyProject10()
{
	Application.keyup.connect(this, &EmptyProject10::application_keyup);
}
//=============================================================================================================
EmptyProject10::~EmptyProject10()
{
	if( Graphics )
		Graphics->ClearState();
}
//=============================================================================================================
bool EmptyProject10::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	infile.close();

	return true;
}
//=============================================================================================================
bool EmptyProject10::Initialize()
{
	Application.Title = "EmptyProject10";

	if( !LoadSettings() )
	{
		DisplayMode.Width = 800;
		DisplayMode.Height = 600;
	}

	bool success = CGame10::Initialize();
	dassert(false, "EmptyProject10::Initialize(): Could not initialize game", success);

	// TODO: initialization logic
	return success;
}
//=============================================================================================================
bool EmptyProject10::LoadContent()
{
#ifdef _DEBUG
	SetCurrentDirectory("../../../../media/");
#else
	SetCurrentDirectory("../media/");
#endif

	// TODO: load content
	return CGame10::LoadContent();
}
//=============================================================================================================
void EmptyProject10::Update()
{
	// TODO: fixed time step game logic
	CGame10::Update();
}
//=============================================================================================================
void EmptyProject10::Draw()
{
	// TODO: interpolation
	float color[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

    Graphics->ClearRenderTargetView(RenderTargetView, color);
    {
		// TODO: draw
		CGame10::Draw();
    }
    SwapChain->Present(0, 0);
}
//=============================================================================================================
