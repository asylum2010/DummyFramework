//=============================================================================================================
#include "GameOverMenu.h"

GameOverMenu::GameOverMenu()
	: CForm()
{
	Background		= NULL;
	selectedindex	= 0;

	alpha.direction	= -1;
	alpha.set(MENU_TRANSITION, 0, 1.0f);
	alpha = 0;

	AddControl(buttons[0]);
	AddControl(buttons[1]);
	AddControl(title);
}
//=============================================================================================================
void GameOverMenu::Draw()
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
bool GameOverMenu::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
	onresetdevice();

	buttons[0].Text = "Restart level";
	buttons[1].Text = "Exit to main menu";
	title.Text = "You have died";

	return CForm::Initialize(mygame, font);
}
//=============================================================================================================
void GameOverMenu::SetState(unsigned int newstate)
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
void GameOverMenu::Update()
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
void GameOverMenu::SelectedIndexChanged(size_t prev)
{
	buttons[prev].SetState(Inactive);
	buttons[selectedindex].SetState(Idle);

	if( selectedindex == 0 )
		action = Restart;
	else
		action = MainMenu;
}
//=============================================================================================================
void GameOverMenu::onfocusgained()
{
	CForm::onfocusgained();

	selectedindex = 0;
	SetState(TransitionIn);
}
//=============================================================================================================
void GameOverMenu::onfocuslost()
{
	CForm::onfocuslost();
	SetState(Hidden);

	buttons[0].SetState(Hidden);
	buttons[1].SetState(Hidden);
	title.SetState(Hidden);
}
//=============================================================================================================
void GameOverMenu::onkeyup(const DummyFramework::skeyboardstate& kstate)
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
void GameOverMenu::onresetdevice()
{
	float spacing = GameVariables::ScreenHeight * 0.08f;

	buttons[1].Position.x = GameVariables::ScreenWidth * 0.5f;
	buttons[1].Position.y = (GameVariables::ScreenHeight + spacing) * 0.5f;
	buttons[0].Position.x = buttons[1].Position.x;
	buttons[0].Position.y = buttons[1].Position.y - spacing;

	title.Position.x = buttons[1].Position.x;
	title.Position.y = spacing * 1.6f;

	CForm::onresetdevice();
}
//=============================================================================================================
