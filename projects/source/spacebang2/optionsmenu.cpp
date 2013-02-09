//=============================================================================================================
#include "optionsmenu.h"
#include <dummyframework/base/CHelper.h>

#define ENTRY(i, k, v) \
	labels[i].SetText(k);\
	buttons[i].SetText(v);\
	labels[i].Alignment = DT_LEFT|DT_VCENTER;\
	buttons[i].Alignment = DT_RIGHT|DT_VCENTER;

OptionsMenu::OptionsMenu()
	: CForm()
{
	Background = NULL;
	modified = 0;

	alpha.direction = -1;
	alpha.set(MENU_TRANSITION, 0, 1.0f);
	alpha = 0;

	for( size_t i = 0; i < NUM_ROWS; ++i )
	{
		AddControl(labels[i]);
		AddControl(buttons[i]);
	}

	AddControl(buttons[NUM_ROWS]);
	AddControl(buttons[NUM_ROWS + 1]);
	AddControl(title);
}
//=============================================================================================================
void OptionsMenu::Draw()
{
	alpha.smooth((float)game->Sync.Alpha());

	D3DXMATRIX transform(Background->Size.x, 0, 0, 0, 0, Background->Size.y, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	D3DXCOLOR color(1, 1, 1, alpha.value);

	transform._41 = (GameVariables::ScreenWidth - Background->Size.x) * 0.5f - 0.5f;
	transform._42 = (GameVariables::ScreenHeight - Background->Size.y) * 0.5f - 0.5f;

	game->StateManager->SetTexture(0, Background->Texture);
	game->DrawImage(transform, Background->Texcoords, color);

	buttons[NUM_ROWS].Color = (modified ? 0xffffffff : 0xffaaaaaa);

	for( size_t i = 0; i < NUM_ROWS + 2; ++i )
	{
		if( i < NUM_ROWS )
			labels[i].Color.a = alpha.value;

		buttons[i].Color.a = alpha.value;
	}

	title.Color.a = alpha.value;

	CForm::Draw();
	spritefont->Draw(GroupID);
}
//=============================================================================================================
bool OptionsMenu::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
	ENTRY(0, "Resolution", "1024x768");
	ENTRY(1, "Texture quality", "high");
	ENTRY(2, "Shaders", "on");
	ENTRY(3, "Fullscreen", "off");
	ENTRY(4, "Vertical sync", "off");

	title.SetText("Options");
	buttons[NUM_ROWS].SetText("Apply");
	buttons[NUM_ROWS + 1].SetText("Back");

	return CForm::Initialize(mygame, font);
}
//=============================================================================================================
bool OptionsMenu::LoadContent()
{
	CForm::LoadContent();
	onresetdevice();

	return true;
}
//=============================================================================================================
void OptionsMenu::SetState(unsigned int newstate)
{
	switch( newstate )
	{
	case TransitionIn:
		alpha = 0;
		ticks = 0;
		alpha.direction = 1;

		for( size_t i = 0; i < NUM_ROWS + 2; ++i )
		{
			if( i < NUM_ROWS )
				labels[i].SetState(Idle);

			buttons[i].SetState(Inactive);
		}

		title.SetState(Idle);
		break;

	case TransitionOut:
		alpha = alpha.count() - 1;
		ticks = 0;
		alpha.direction = -1;

		for( size_t i = 0; i < NUM_ROWS + 2; ++i )
			buttons[i].SetState(Inactive);
		break;

	default:
		break;
	}

	CControl::SetState(newstate);
}
//=============================================================================================================
void OptionsMenu::Update()
{
	switch( state )
	{
	case TransitionIn:
		if( ticks > alpha.count() + 1 )
		{
			SetState(Idle);
			SelectedIndexChanged(2);
		}
		else
		{
			++ticks;
			alpha.step();
		}
		break;

	case TransitionOut:
		if( ticks > alpha.count() + 1 )
		{
			focuslost(*this);
		}
		else
		{
			++ticks;
			alpha.step();
		}
		break;

	default:
		break;
	}

	CForm::Update();
}
//=============================================================================================================
void OptionsMenu::SelectedIndexChanged(size_t prev)
{
	buttons[prev].SetState(Inactive);
	buttons[selectedindex].SetState(Idle);

	action = Finished;
}
//=============================================================================================================
void OptionsMenu::QuerySettings()
{
	std::string res(DummyFramework::CHelper::DiscreteToString(game->DisplayMode.Width));
	res = res + "x" + DummyFramework::CHelper::DiscreteToString(game->DisplayMode.Height);

	resolution = resolutions.find(res);

	if( resolution == resolutions.end() )
	{
		derror(, "OptionsMenu::QuerySettings(): Unexisting resolution", false);
		resolution = resolutions.begin();
	}

	if( game->DeviceCaps.PixelShaderVersion < D3DPS_VERSION(2, 0) )
	{
		GameVariables::EnableShaders = false;
		buttons[2].SetText("off");
	}

	current.resolution	= (*resolution);
	current.shaders		= GameVariables::EnableShaders;
	current.fullscreen	= game->FullScreen;
	current.vsync		= game->VSync;
	current.quality		= GameVariables::Quality;
}
//=============================================================================================================
void OptionsMenu::onfocusgained()
{
	CForm::onfocusgained();
	selectedindex = 0;

	QuerySettings();

	buttons[0].SetText(*resolution);
	buttons[1].SetText(GameVariables::Quality);
	buttons[2].SetText(GameVariables::EnableShaders ? "on" : "off");
	buttons[3].SetText(game->FullScreen ? "on" : "off");
	buttons[4].SetText(game->VSync ? "on" : "off");

	SetState(TransitionIn);
}
//=============================================================================================================
void OptionsMenu::onfocuslost()
{
	CForm::onfocuslost();
	SetState(Hidden);

	for( size_t i = 0; i < NUM_ROWS + 2; ++i )
	{
		if( i < NUM_ROWS )
			labels[i].SetState(Hidden);

		buttons[i].SetState(Hidden);
	}

	title.SetState(Hidden);
}
//=============================================================================================================
void OptionsMenu::onkeyup(const DummyFramework::skeyboardstate& kstate)
{
	if( state == TransitionIn || state == TransitionOut )
		return;

	switch( kstate.key )
	{
	case VK_RETURN:
	case VK_SPACE:
		if( selectedindex == NUM_ROWS + 1 )
		{
			modified = 0;
			SetState(TransitionOut);
		}
		else if( selectedindex == NUM_ROWS )
		{
			bool needreset = false;

			if( current.resolution != *resolution ||
				current.vsync != game->VSync ||
				current.fullscreen != game->FullScreen )
			{
				needreset = true;
			}

			if( needreset )
			{
				size_t pos = resolution->find("x");

				game->DisplayMode.Width = atoi(resolution->substr(0, pos).c_str());
				game->DisplayMode.Height = atoi(resolution->substr(pos + 1).c_str());

				game->Reset();
			}

			settingschanged();
			QuerySettings();

			modified = 0;
			selectedindex = NUM_ROWS + 1;
			SelectedIndexChanged(NUM_ROWS);
		}
		break;

	case VK_DOWN:
	case 0x53:
		{
			size_t prev = selectedindex;

			if( selectedindex == NUM_ROWS - 1 )
			{
				selectedindex = (modified ? NUM_ROWS : NUM_ROWS + 1);
			}
			else if( selectedindex >= NUM_ROWS )
			{
				selectedindex = 0;
			}
			else
				selectedindex = (selectedindex + 1) % (NUM_ROWS + 2);

			SelectedIndexChanged(prev);
		}
		break;

	case VK_UP:
	case 0x57:
		{
			size_t prev = selectedindex;

			if( selectedindex == 0 )
			{
				selectedindex = (modified ? NUM_ROWS : NUM_ROWS + 1);
			}
			else if( selectedindex == NUM_ROWS + 1 )
			{
				selectedindex = (modified ? NUM_ROWS : NUM_ROWS - 1);
			}
			else
				selectedindex = ((NUM_ROWS + 2) + selectedindex - 1) % (NUM_ROWS + 2);

			SelectedIndexChanged(prev);
		}
		break;

	case VK_LEFT:
	case 0x41:
		if( selectedindex == 0 )
		{
			if( resolution == resolutions.begin() )
				resolution = --resolutions.end();
			else
				--resolution;

			buttons[0].SetText(*resolution);

			if( (*resolution) == current.resolution )
				--modified;
			else
				++modified;
		}
		else if( selectedindex == 2 )
		{
			GameVariables::EnableShaders = !GameVariables::EnableShaders;
			buttons[2].SetText(GameVariables::EnableShaders ? "on" : "off");

			if( GameVariables::EnableShaders == current.shaders )
				--modified;
			else
				++modified;
		}
		else if( selectedindex == 3 )
		{
			game->FullScreen = !game->FullScreen;
			buttons[3].SetText(game->FullScreen ? "on" : "off");

			if( game->FullScreen == current.fullscreen )
				--modified;
			else
				++modified;
		}
		else if( selectedindex == 4 )
		{
			game->VSync = !game->VSync;
			buttons[4].SetText(game->VSync ? "on" : "off");

			if( game->VSync == current.vsync )
				--modified;
			else
				++modified;
		}
		else if( selectedindex == NUM_ROWS )
		{
			selectedindex = NUM_ROWS + 1;
			SelectedIndexChanged(NUM_ROWS);
		}
		else if( modified && selectedindex == NUM_ROWS + 1)
		{
			selectedindex = NUM_ROWS;
			SelectedIndexChanged(NUM_ROWS + 1);
		}
		break;

	case VK_RIGHT:
	case 0x44:
		if( selectedindex == 0 )
		{
			++resolution;

			if( resolution == resolutions.end() )
				resolution = resolutions.begin();

			buttons[0].SetText(*resolution);

			if( (*resolution) == current.resolution )
				--modified;
			else
				++modified;
		}
		else if( selectedindex == 2 )
		{
			GameVariables::EnableShaders = !GameVariables::EnableShaders;
			buttons[2].SetText(GameVariables::EnableShaders ? "on" : "off");

			if( GameVariables::EnableShaders == current.shaders )
				--modified;
			else
				++modified;
		}
		else if( selectedindex == 3 )
		{
			game->FullScreen = !game->FullScreen;
			buttons[3].SetText(game->FullScreen ? "on" : "off");

			if( game->FullScreen == current.fullscreen )
				--modified;
			else
				++modified;
		}
		else if( selectedindex == 4 )
		{
			game->VSync = !game->VSync;
			buttons[4].SetText(game->VSync ? "on" : "off");

			if( game->VSync == current.vsync )
				--modified;
			else
				++modified;
		}
		else if( selectedindex == NUM_ROWS )
		{
			selectedindex = NUM_ROWS + 1;
			SelectedIndexChanged(NUM_ROWS);
		}
		else if( modified && selectedindex == NUM_ROWS + 1)
		{
			selectedindex = NUM_ROWS;
			SelectedIndexChanged(NUM_ROWS + 1);
		}
		break;

	default:
		break;
	}
}
//=============================================================================================================
void OptionsMenu::onmouseup(const DummyFramework::smousestate& mstate)
{
	if( state == TransitionIn || state == TransitionOut )
		return;

	if( selectedindex < NUM_ROWS )
	{
		if( mstate.button & DummyFramework::MB_Left )
		{
			DummyFramework::CApplication::KeyBoardState.key = VK_RIGHT;
			onkeyup(DummyFramework::CApplication::KeyBoardState);
			DummyFramework::CApplication::KeyBoardState.key = 0;
		}
		else if( mstate.button & DummyFramework::MB_Right )
		{
			DummyFramework::CApplication::KeyBoardState.key = VK_LEFT;
			onkeyup(DummyFramework::CApplication::KeyBoardState);
			DummyFramework::CApplication::KeyBoardState.key = 0;
		}
	}
	else if( selectedindex < NUM_BUTTONS )
	{
		DummyFramework::CApplication::KeyBoardState.key = VK_RETURN;
		onkeyup(DummyFramework::CApplication::KeyBoardState);
		DummyFramework::CApplication::KeyBoardState.key = 0;
	}
}
//=============================================================================================================
void OptionsMenu::onmousemove(const DummyFramework::smousestate& mstate)
{
	if( state == TransitionOut )
		return;

	size_t current = NUM_BUTTONS;

	// intentionally don't stop (consider low framerate)
	for( int i = 0; i < NUM_BUTTONS; ++i )
	{
		if( buttons[i].MouseOver(mstate.x, mstate.y) )
			current = i;
	}

	if( current < NUM_BUTTONS && selectedindex != current )
	{
		std::swap(selectedindex, current);
		SelectedIndexChanged(current);
	}
}
//=============================================================================================================
void OptionsMenu::onresetdevice()
{
	D3DXVECTOR2 size;

	title.Position.x = GameVariables::ScreenWidth * 0.5f;
	title.Position.y = GameVariables::ScreenHeight * 0.08f * 1.6f;
	title.GetSize(size);

	buttons[NUM_ROWS].Alignment			= DT_RIGHT|DT_VCENTER;
	buttons[NUM_ROWS].Position.x		= title.Position.x - GameVariables::ScreenHeight * 0.04f;
	buttons[NUM_ROWS + 1].Alignment		= DT_LEFT|DT_VCENTER;
	buttons[NUM_ROWS + 1].Position.x	= title.Position.x + GameVariables::ScreenHeight * 0.04f;
	buttons[NUM_ROWS].Position.y		= buttons[NUM_ROWS + 1].Position.y = GameVariables::ScreenHeight - title.Position.y * 1.2f;

	float spacing = GameVariables::ScreenHeight * 0.08f;
	float height = (NUM_ROWS - 1) * spacing;

	for( size_t i = 0; i < NUM_ROWS; ++i )
	{
		labels[i].Position.x = (GameVariables::ScreenWidth - Background->Size.x) * 0.5f + spacing * 2;
		buttons[i].Position.x = (GameVariables::ScreenWidth + Background->Size.x) * 0.5f - spacing * 2;

		labels[i].Position.y = buttons[i].Position.y =
			(GameVariables::ScreenHeight - height) * 0.5f + i * (spacing);
	}

	CForm::onresetdevice();
}
//=============================================================================================================
