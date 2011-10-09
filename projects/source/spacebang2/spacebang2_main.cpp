//=============================================================================================================
#include "spacebang2.h"

#include <ctime>
#include <fstream>

SpaceBang2::SpaceBang2()
{
	stickykeys.cbSize = sizeof(STICKYKEYS);
	stickykeys.dwFlags = 0;

	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &stickykeys, 0);
	STICKYKEYS sk = stickykeys;

	// azért 0 mert ha be van már kapcsolva akkor valszeg van oka...
	if( (sk.dwFlags & SKF_STICKYKEYSON) == 0 )
	{
		dhappy("Sticky keys inactive, disabling");
		std::cout << "\n";

		sk.dwFlags &= (~SKF_HOTKEYACTIVE);
        sk.dwFlags &= (~SKF_CONFIRMHOTKEY);
 
        SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &sk, 0);
	}
	else
		dsad("Sticky keys are active, switch them off from control panel");

    currentgame = 0;
    currentlevel = 0;

    formmanager.InitialDelay = 10;

    formmanager.AddForm(logo);
    formmanager.AddForm(background);
    formmanager.AddForm(mainmenu);
    formmanager.AddForm(loading);
    formmanager.AddForm(pausemenu);
    formmanager.AddForm(statistics);
    formmanager.AddForm(options);
    formmanager.AddForm(gameover);
    formmanager.AddForm(loadgame);

    formmanager.SetCurrent(logo);
	formmanager.Connect(logo, StaticLogo::Exit, background);
    formmanager.Connect(background, StaticBackground::Visible, mainmenu);
    formmanager.Connect(background, StaticBackground::Invisible, loading);
    formmanager.Connect(mainmenu, MainMenu::NewGame, background);
    formmanager.Connect(mainmenu, MainMenu::LoadGame, loadgame);
    formmanager.Connect(mainmenu, MainMenu::Options, options);
    formmanager.Connect(options, OptionsMenu::Finished, mainmenu);
    formmanager.Connect(loadgame, ChapterMenu::MainMenu, mainmenu);
    formmanager.Connect(loadgame, ChapterMenu::LoadGame, background);

    GameVariables::Sync.synchronize.connect(&game, &Game::InternalUpdate);
    GameVariables::Sync.UpdateInterval = 0.1f;

    Application.keyup.connect(&formmanager, &DummyFramework::CFormManager::onkeyup);
    Application.keydown.connect(&formmanager, &DummyFramework::CFormManager::onkeydown);

	logo.focuslost.connect(this, &SpaceBang2::logo_focuslost);
    mainmenu.selected.connect(this, &SpaceBang2::menu_selected);
    loading.startload.connect(this, &SpaceBang2::loading_startload);
    pausemenu.selected.connect(this, &SpaceBang2::pause_selected);
    statistics.selected.connect(this, &SpaceBang2::statistics_selected);
    gameover.selected.connect(this, &SpaceBang2::gameover_selected);
    loadgame.selected.connect(this, &SpaceBang2::chapter_selected);
	options.settingschanged.connect(this, &SpaceBang2::options_settingschanged);

    srand((unsigned int)time(NULL));
}
//=============================================================================================================
SpaceBang2::~SpaceBang2()
{
	SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &stickykeys, 0);
	dhappy("Sticky keys restored");
}
//=============================================================================================================
bool SpaceBang2::Initialize()
{
    Application.Title = "SpaceBang2";
	ZBuffer = false;

    // idehackeljük mert kelleni fog (nem ront el semmit)
    Direct3D = Direct3DCreate9(D3D_SDK_VERSION);
    dassert(false, "SpaceBang2::Initialize(): Could not create direct3d", Direct3D);

    bool success = LoadSettings("settings.ini");
    dassert(false, "SpaceBang2::Initialize(): Could not find suitable resolution", success);

    // ezt mindenképp elöbb kell meghivni, hiszen ekkor kreálodik meg a d3d device
    success = CGame9::Initialize();
    
    GameVariables::ScreenWidth = (float)PresentationParameters.BackBufferWidth;
    GameVariables::ScreenHeight = (float)PresentationParameters.BackBufferHeight;

    if( GameVariables::EnableShaders )
    {
        if( DeviceCaps.PixelShaderVersion < D3DPS_VERSION(2, 0) )
        {
            dsad("Pixel shader 2.0 not supported");
            GameVariables::EnableShaders = false;
        }
    }

    // TODO: FindFirstFileA(); stb.

#ifdef _DEBUG
	SetCurrentDirectory("../../../media/spacebang2/");
#else
	SetCurrentDirectory("../media/spacebang2/");
#endif

    games.resize(1);
    success = (success && LoadGames(games[0], "scripts/game.ini"));

    if( success )
        loadgame.Fill(games[0]);

    success = (success && formmanager.Initialize(*this, spritefont));
    return success;
}
//=============================================================================================================
bool SpaceBang2::LoadContent()
{
    bool success = (0xffffffff != spritefont.AddFont("comic_100_bold.fnt"));
    dassert(false, "SpaceBang2::LoadContent(): Could not load font", success);

    success = Content.LoadSprite(menubg, "menu.txt", &GameVariables::CorrelateH);
    dassert(false, "SpaceBang2::LoadContent(): Could not load 'menu'", success);

    std::string path;
    GameVariables::Qualify(path, "menubg.txt");

    success = Content.LoadSprite(bg, path, &GameVariables::CorrelateH);
    dassert(false, "SpaceBang2::LoadContent(): Could not load 'menubg'", success);

    GameVariables::Qualify(path, "logo.txt");

	success = Content.LoadSprite(logobg, path, &GameVariables::CorrelateH);
    dassert(false, "SpaceBang2::LoadContent(): Could not load 'logobg'", success);

    // automatikusan átadja a controljainak a LoadContent()-ben
    game.GroupID = spritefont.AddGroup(0);
    mainmenu.GroupID = spritefont.AddGroup(0);
    loading.GroupID = spritefont.AddGroup(0);
    pausemenu.GroupID = spritefont.AddGroup(0);
    statistics.GroupID = spritefont.AddGroup(0);
    options.GroupID = spritefont.AddGroup(0);
    gameover.GroupID = spritefont.AddGroup(0);
    loadgame.GroupID = spritefont.AddGroup(0);
    
    ResizeSprites();

	logo.Background = &logobg;
    background.Background = &bg;
    mainmenu.Background = pausemenu.Background = statistics.Background =
        options.Background = gameover.Background = loadgame.Background = &menubg;

    success = formmanager.LoadContent();
    logo.SetState(DummyFramework::CControl::TransitionIn);
	
    return success && CGame9::LoadContent();
}
//=============================================================================================================
bool SpaceBang2::LoadGames(gamedesc& out, const std::string& file)
{
    std::ifstream infile(file.c_str());
    dnassert(false, "SpaceBang2::Initialize(): Could not find any game descriptors", infile.fail());
    
    size_t count;
    std::string thumbsatlas, path;

    infile >> count;
    infile >> thumbsatlas;
    
    out.levels.resize(count);

    for( size_t i = 0; i < count; ++i )
    {
        leveldesc& desc = out.levels[i];

        infile >> desc.file >> desc.title >>
            desc.texcoords[0] >> desc.texcoords[1] >>
            desc.texcoords[2] >> desc.texcoords[3];

        DummyFramework::CHelper::Replace(desc.title, "\\s", " ", desc.title);

        desc.texcoords[2] += desc.texcoords[0];
        desc.texcoords[3] += desc.texcoords[1];
    }

    infile.close();

    DummyFramework::CHelper::GetFile(path, file);
    std::cout << "Loaded '" << path << "' (" << count << ")\n";

    out.texture = Content.LoadTexture(thumbsatlas);
    dassert(false, "SpaceBang2::Initialize(): Could not load thumb atlas", out.texture);
    
    currentlevel = 0;
    return true;
}
//=============================================================================================================
bool SpaceBang2::LoadSettings(const std::string& file)
{
    std::ifstream infile(file.c_str());

    // ha nincs fájl akkor marad a default
    if( !infile.fail() )
    {
        infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync >>
			GameVariables::EnableShaders >> GameVariables::Quality;

        infile.close();
    }
	else
	{
		DisplayMode.Width = 1024;
		DisplayMode.Height = 768;

		FullScreen = false;
		VSync = true;

		GameVariables::EnableShaders = true;
		GameVariables::Quality = "medium";
	}

    DummyFramework::resolutionset& resolutions = options.GetResolutionSet();
    DummyFramework::resolutionset::iterator it, end;

    resolutions.clear();
    EnumDisplayModes(resolutions);

    if( resolutions.size() == 0 )
        return false;

    std::string res(DummyFramework::CHelper::DiscreteToString(DisplayMode.Width));
    res = res + "x" + DummyFramework::CHelper::DiscreteToString(DisplayMode.Height);

    // jo-e ez a felbontás?
    end = resolutions.find(res);
    
    if( end == resolutions.end() )
    {
        size_t pos;
        int w, h, d, md = 10000, mw, mh;

        // megkeressük a legközelebbit
        for( it = resolutions.begin(); it != end; ++it )
        {
            pos = it->find('x');

            w = atoi(it->substr(0, pos).c_str());
            h = atoi(it->substr(pos + 1).c_str());

            d = abs((int)DisplayMode.Width - w) + abs((int)DisplayMode.Height - h);

            if( d < md )
            {
                md = d;
                mw = w;
                mh = h;
            }
        }

        DisplayMode.Width = mw;
        DisplayMode.Height = mh;
    }

    return true;
}
//=============================================================================================================
bool SpaceBang2::SaveSettings(const std::string& file)
{
	std::ofstream of(file.c_str());

	if( of.fail() )
		return false;

	of << DisplayMode.Width << "\n" << DisplayMode.Height << "\n" << FullScreen << "\n" << VSync <<
		"\n" << GameVariables::EnableShaders << "\n" << GameVariables::Quality << "\n\n";

	of.close();
	return true;
}
//=============================================================================================================
void SpaceBang2::ResizeSprites()
{
    spritefont.SetGroupScale(game.GroupID, GameVariables::CorrelateH(0.8f));
    spritefont.SetGroupScale(mainmenu.GroupID, GameVariables::CorrelateH(0.8f));
    spritefont.SetGroupScale(loading.GroupID, GameVariables::CorrelateH(0.8f));
    spritefont.SetGroupScale(pausemenu.GroupID, GameVariables::CorrelateH(0.8f));
    spritefont.SetGroupScale(statistics.GroupID, GameVariables::CorrelateH(0.8f));
    spritefont.SetGroupScale(options.GroupID, GameVariables::CorrelateH(0.8f));
    spritefont.SetGroupScale(gameover.GroupID, GameVariables::CorrelateH(0.8f));
    spritefont.SetGroupScale(loadgame.GroupID, GameVariables::CorrelateH(0.8f));

    bg.Size.x = GameVariables::CorrelateH(bg.OriginalSize.x);
    bg.Size.y = GameVariables::CorrelateH(bg.OriginalSize.y);

    menubg.Size.x = GameVariables::CorrelateH(menubg.OriginalSize.x);
    menubg.Size.y = GameVariables::CorrelateH(menubg.OriginalSize.y);
}
//=============================================================================================================
void SpaceBang2::ResetRenderStates()
{
    StateManager->SetRenderState(D3DRS_ZENABLE, false);
    StateManager->SetRenderState(D3DRS_LIGHTING, false);
    StateManager->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    StateManager->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    StateManager->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    StateManager->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    StateManager->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    StateManager->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    StateManager->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    StateManager->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    StateManager->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    StateManager->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    StateManager->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    StateManager->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    StateManager->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    StateManager->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    // az invertáláskor ebbe van a kép
    StateManager->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    StateManager->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    StateManager->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    StateManager->SetSamplerState(2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    StateManager->SetSamplerState(2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    StateManager->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    StateManager->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    StateManager->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
}
//=============================================================================================================
void SpaceBang2::Update()
{
    formmanager.Update();
    CGame9::Update();
}
//=============================================================================================================
void SpaceBang2::Draw()
{
    GameVariables::Sync.Update();

    Graphics->Clear(0, NULL, D3DCLEAR_TARGET, 0xff000000, 1.0f, 0); //0xff6694ed
    Graphics->SetVertexDeclaration(DummyFramework::CScreenQuad9::Declaration);

    Graphics->BeginScene();
    {
        formmanager.Draw();
    }
    Graphics->EndScene();

    Graphics->Present(NULL, NULL, NULL, NULL);

    //Sleep(100); // fps teszt
}
//=============================================================================================================
