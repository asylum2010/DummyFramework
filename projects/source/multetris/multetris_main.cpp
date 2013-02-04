//=============================================================================================================
#include "multetris.h"
#include <fstream>

MulTetris::MulTetris()
{
	formmanager.InitialDelay = 10;

	formmanager.AddForm(background);
	formmanager.AddForm(mainmenu);
	formmanager.AddForm(versus);
	formmanager.AddForm(single);
	formmanager.AddForm(coop);
	formmanager.AddForm(quit);
	formmanager.AddForm(pausemenu);

	formmanager.SetCurrent(background);
	formmanager.Connect(background, StaticBackground::Visible, mainmenu);
	formmanager.Connect(versus, VersusGame::PauseMenu, pausemenu);
	formmanager.Connect(versus, VersusGame::GameOver, mainmenu);
	formmanager.Connect(single, SingleGame::PauseMenu, pausemenu);
	formmanager.Connect(single, SingleGame::GameOver, mainmenu);
	formmanager.Connect(coop, CoopGame::PauseMenu, pausemenu);
	formmanager.Connect(coop, CoopGame::GameOver, mainmenu);
	formmanager.Connect(pausemenu, PauseMenu::MainMenu, mainmenu);
	formmanager.Connect(mainmenu, MainMenu::MultiVersus, versus);
	formmanager.Connect(mainmenu, MainMenu::SinglePlayer, single);
	formmanager.Connect(mainmenu, MainMenu::MultiCoop, coop);

	versus.Bind(0, VersusGame::Left, 0x41);
	versus.Bind(0, VersusGame::Right, 0x44);
	versus.Bind(0, VersusGame::Down, 0x53);
	versus.Bind(0, VersusGame::Rotate, 0x57);
	versus.Bind(1, VersusGame::Left, VK_LEFT);
	versus.Bind(1, VersusGame::Right, VK_RIGHT);
	versus.Bind(1, VersusGame::Down, VK_DOWN);
	versus.Bind(1, VersusGame::Rotate, VK_UP);

	coop.Bind(0, CoopGame::Left, 0x41);
	coop.Bind(0, CoopGame::Right, 0x44);
	coop.Bind(0, CoopGame::Down, 0x53);
	coop.Bind(0, CoopGame::Rotate, 0x57);
	coop.Bind(1, CoopGame::Left, VK_LEFT);
	coop.Bind(1, CoopGame::Right, VK_RIGHT);
	coop.Bind(1, CoopGame::Down, VK_DOWN);
	coop.Bind(1, CoopGame::Rotate, VK_UP);

	single.Bind(SingleGame::Left, VK_LEFT);
	single.Bind(SingleGame::Right, VK_RIGHT);
	single.Bind(SingleGame::Down, VK_DOWN);
	single.Bind(SingleGame::Rotate, VK_UP);

	Application.keyup.connect(&formmanager, &DummyFramework::CFormManager::onkeyup);
	Application.keydown.connect(&formmanager, &DummyFramework::CFormManager::onkeydown);

	mainmenu.selected.connect(this, &MulTetris::menu_selected);
	mainmenu.hover.connect(this, &MulTetris::menu_hover);
	pausemenu.selected.connect(this, &MulTetris::pause_selected);
}
//=============================================================================================================
MulTetris::~MulTetris()
{
}
//=============================================================================================================
bool MulTetris::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	infile.close();

	return true;
}
//=============================================================================================================
bool MulTetris::Initialize()
{
	Application.Title = "MulTetris";
	ZBuffer = false;

	if( !LoadSettings() )
	{
		DisplayMode.Width = 1024;
		DisplayMode.Height = 768;
	}

	bool success = CGame9::Initialize();
	dassert(false, "MulTetris::Initialize(): Could not initialize game", success);

	GameVariables::RefWidth = 1360;
	GameVariables::RefHeight = 768;

	GameVariables::ScreenWidth = (float)PresentationParameters.BackBufferWidth;
	GameVariables::ScreenHeight = (float)PresentationParameters.BackBufferHeight;

#ifdef _DEBUG
	SetCurrentDirectory("../../../media/multetris/");
#else
	SetCurrentDirectory("../media/multetris/");
#endif

	success = (success && formmanager.Initialize(*this, spritefont));
	return success;
}
//=============================================================================================================
bool MulTetris::LoadContent()
{
	bool success = (0xffffffff != spritefont.AddFont("showcard_100_bold.fnt"));
	dassert(false, "MulTetris::LoadContent(): Could not load font", success);

	success = Content.LoadSprite(menubg, "menu.txt", &GameVariables::CorrelateH);
	dassert(false, "MulTetris::LoadContent(): Could not load 'menu'", success);

	success = Content.LoadSprite(bg, "bg.txt", &GameVariables::CorrelateH);
	dassert(false, "MulTetris::LoadContent(): Could not load 'bg'", success);

	success = Content.LoadSprite(panel, "panel.txt", &GameVariables::CorrelateH);
	dassert(false, "MulTetris::LoadContent(): Could not load 'panel'", success);

	LPDIRECT3DTEXTURE9 tex;

	tex = Content.LoadTextureAtlas(atlas, "atlas.txt", &GameVariables::CorrelateH);
	dassert(false, "Game::LoadContent(): Could not load 'atlas'", tex);

	versus.Atlas = single.Atlas = coop.Atlas = quit.Atlas = tex;
	versus.Cell = single.Cell = coop.Cell = quit.Cell = &atlas["cell"];
	versus.Glow = single.Glow = coop.Glow = quit.Glow = &atlas["glow"];

	mainmenu.GroupID = spritefont.AddGroup(0);
	pausemenu.GroupID = spritefont.AddGroup(0);
	versus.GroupID = spritefont.AddGroup(0);
	single.GroupID = spritefont.AddGroup(0);
	coop.GroupID = spritefont.AddGroup(0);

	spritefont.SetGroupScale(mainmenu.GroupID, GameVariables::CorrelateH(0.333f));
	spritefont.SetGroupScale(pausemenu.GroupID, GameVariables::CorrelateH(0.333f));
	spritefont.SetGroupScale(versus.GroupID, GameVariables::CorrelateH(0.333f));
	spritefont.SetGroupScale(single.GroupID, GameVariables::CorrelateH(0.333f));
	spritefont.SetGroupScale(coop.GroupID, GameVariables::CorrelateH(0.333f));

	background.Panel = &panel;
	background.Background = &bg;
	pausemenu.Background = mainmenu.Background = &menubg;

	success = formmanager.LoadContent();
	background.SetState(DummyFramework::CControl::TransitionIn);

	return success && CGame9::LoadContent();
}
//=============================================================================================================
void MulTetris::ResetRenderStates()
{
	StateManager->SetRenderState(D3DRS_ZENABLE, false);
	StateManager->SetRenderState(D3DRS_LIGHTING, false);
	StateManager->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	StateManager->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	StateManager->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	StateManager->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	StateManager->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	StateManager->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	StateManager->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	StateManager->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	StateManager->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	StateManager->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
}
//=============================================================================================================
void MulTetris::Update()
{
	formmanager.Update();
	CGame9::Update();
}
//=============================================================================================================
void MulTetris::Draw()
{
	Graphics->Clear(0, NULL, D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
	Graphics->SetVertexDeclaration(DummyFramework::CScreenQuad9::Declaration);

	Graphics->BeginScene();
	{
		formmanager.Draw();
	}
	Graphics->EndScene();

	Graphics->Present(NULL, NULL, NULL, NULL);
}
//=============================================================================================================
