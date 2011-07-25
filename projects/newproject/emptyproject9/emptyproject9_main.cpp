//=============================================================================================================
#include "emptyproject9.h"
#include <fstream>

EmptyProject9::EmptyProject9()
{
	Application.keyup.connect(this, &EmptyProject9::application_keyup);
}
//=============================================================================================================
EmptyProject9::~EmptyProject9()
{
}
//=============================================================================================================
bool EmptyProject9::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	return true;
}
//=============================================================================================================
bool EmptyProject9::Initialize()
{
	Application.Title = "EmptyProject9";

	if( !LoadSettings() )
	{
		DisplayMode.Width = 800;
		DisplayMode.Height = 600;
	}

	bool success = CGame9::Initialize();
	
	// TODO: initialization logic
	return success;
}
//=============================================================================================================
bool EmptyProject9::LoadContent()
{
	// TODO: load content
	return CGame9::LoadContent();
}
//=============================================================================================================
void EmptyProject9::ResetRenderStates()
{
}
//=============================================================================================================
void EmptyProject9::Update()
{
	// TODO: fixed time step game logic
	CGame9::Update();
}
//=============================================================================================================
void EmptyProject9::Draw()
{
	// TODO: interpolation

	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6694ed, 1.0f, 0);

	Graphics->BeginScene();
	{
		// TODO: draw
		CGame9::Draw();
	}
	Graphics->EndScene();
	
	Graphics->Present(NULL, NULL, NULL, NULL);
}
//=============================================================================================================
