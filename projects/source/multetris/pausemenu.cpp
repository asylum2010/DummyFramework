//=============================================================================================================
#include "pausemenu.h"

PauseMenu::PauseMenu()
    : CForm()
{
	Background = NULL;
	selectedindex = 0;

	alpha.direction = -1;
	alpha = 0;
	alpha.set(0, 1.0f);

	AddControl(buttons[0]);
	AddControl(buttons[1]);
	AddControl(title);
}
//=============================================================================================================
void PauseMenu::Draw()
{
	D3DXMATRIX transform(Background->Size.x, 0, 0, 0, 0, Background->Size.y, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	D3DXCOLOR color(0xffffffff);

	alpha.smooth((float)game->Sync.Alpha());
	color.a = alpha.value;

	transform._41 = (GameVariables::ScreenWidth - Background->Size.x) * 0.5f - 0.5f;
	transform._42 = (GameVariables::ScreenHeight - Background->Size.y) * 0.5f - 0.5f;

	game->StateManager->SetTexture(0, Background->Texture);
	game->DrawImage(transform, Background->Texcoords, color);

	buttons[0].Color.a = buttons[1].Color.a =
		title.Color.a = alpha.value;

	CForm::Draw();
	spritefont->Draw(GroupID);
}
//=============================================================================================================
bool PauseMenu::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
	onresetdevice();

	buttons[0].Text = "Continue";
	buttons[1].Text = "Exit to main menu";
	title.Text = "Game paused";

	return CForm::Initialize(mygame, font);
}
//=============================================================================================================
void PauseMenu::SetState(unsigned int newstate)
{
	switch( newstate )
	{
	case TransitionIn:
		alpha = 0;
		ticks = 0;
		alpha.direction = 1;

		buttons[0].SetState(Inactive);
		buttons[1].SetState(Inactive);
		title.SetState(Idle);
		break;

	case TransitionOut:
		alpha = alpha.count() - 1;
		ticks = 0;
		alpha.direction = -1;

		buttons[0].SetState(Inactive);
		buttons[1].SetState(Inactive);
		break;

	default:
		break;
	}

	CControl::SetState(newstate);
}
//=============================================================================================================
void PauseMenu::Update()
{
	alpha.step();

	switch( state )
	{
	case TransitionIn:
		if( ticks >= alpha.count() )
		{
			SetState(Idle);
			SelectedIndexChanged(1);
		}
		else
			++ticks;
		break;

	case TransitionOut:
		if( ticks >= alpha.count() )
		{
			selected(action);
			focuslost(*this);
		}
		else
			++ticks;
		break;

	default:
		break;
	}

	CForm::Update();
}
//=============================================================================================================
void PauseMenu::SelectedIndexChanged(size_t prev)
{
	buttons[prev].SetState(Inactive);
	buttons[selectedindex].SetState(Idle);

	if( selectedindex == 0 )
		action = Continue;
	else if( selectedindex == 1 )
		action = MainMenu;
}
//=============================================================================================================
void PauseMenu::onfocusgained()
{
	CForm::onfocusgained();
	selectedindex = 0;
	SetState(TransitionIn);
}
//=============================================================================================================
void PauseMenu::onfocuslost()
{
	CForm::onfocuslost();
	SetState(Hidden);

	buttons[0].SetState(Hidden);
	buttons[1].SetState(Hidden);
	title.SetState(Hidden);
}
//=============================================================================================================
void PauseMenu::onkeyup(const DummyFramework::skeyboardstate& kstate)
{
	switch( kstate.key )
	{
	case VK_RETURN:
	case VK_SPACE:
		SetState(TransitionOut);
		break;

	case VK_DOWN:
	case 0x53:
		{
			size_t prev = selectedindex;
			selectedindex = (selectedindex + 1) % 2;
			SelectedIndexChanged(prev);
		}
		break;

	case VK_UP:
	case 0x57:
		{
			size_t prev = selectedindex;
			selectedindex = (2 + selectedindex - 1) % 2;
			SelectedIndexChanged(prev);
		}
		break;

	default:
		break;
	}
}
//=============================================================================================================
void PauseMenu::onresetdevice()
{
	float spacing = GameVariables::ScreenHeight * 0.065f;

	buttons[0].Position.x = GameVariables::ScreenWidth * 0.5f;
	buttons[0].Position.y = (GameVariables::ScreenHeight - spacing) * 0.5f;
	buttons[1].Position.x = buttons[0].Position.x;
	buttons[1].Position.y = buttons[0].Position.y + spacing;

	title.Position.x = buttons[1].Position.x;
	title.Position.y = GameVariables::ScreenHeight * 0.15625f;

	CForm::onresetdevice();
}
//=============================================================================================================
