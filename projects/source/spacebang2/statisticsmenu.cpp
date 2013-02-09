//=============================================================================================================
#include <dummyframework/base/CHelper.h>
#include "statisticsmenu.h"

#define ENTRY(i, k, v) \
	entries[i].key.SetText(k);\
	entries[i].value.SetText(v);\
	entries[i].key.Alignment = DT_LEFT|DT_VCENTER;\
	entries[i].value.Alignment = DT_RIGHT|DT_VCENTER;

StatisticsMenu::StatisticsMenu()
	: CForm()
{
	Background = NULL;

	alpha.direction = -1;
	alpha.set(MENU_TRANSITION, 0, 1.0f);
	alpha = 0;

	for( size_t i = 0; i < NUM_ROWS; ++i )
	{
		AddControl(entries[i].key);
		AddControl(entries[i].value);
	}

	AddControl(cont);
	AddControl(title);
}
//=============================================================================================================
void StatisticsMenu::Draw()
{
	alpha.smooth((float)game->Sync.Alpha());

	D3DXMATRIX transform(Background->Size.x, 0, 0, 0, 0, Background->Size.y, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	D3DXCOLOR color(1, 1, 1, alpha.value);

	transform._41 = (GameVariables::ScreenWidth - Background->Size.x) * 0.5f - 0.5f;
	transform._42 = (GameVariables::ScreenHeight - Background->Size.y) * 0.5f - 0.5f;

	game->StateManager->SetTexture(0, Background->Texture);
	game->DrawImage(transform, Background->Texcoords, color);

	for( size_t i = 0; i < NUM_ROWS; ++i )
	{
		entries[i].key.Color.a = entries[i].value.Color.a = alpha.value;
	}

	cont.Color.a = title.Color.a = alpha.value;

	CForm::Draw();
	spritefont->Draw(GroupID);
}
//=============================================================================================================
bool StatisticsMenu::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
	ENTRY(0, "Enemies killed", "0");
	ENTRY(1, "Powerups acquired", "0");
	ENTRY(2, "Deaths", "0");
	ENTRY(3, "Score", "0");
	ENTRY(4, "Total score", "0");

	title.SetText("Stage complete");
	cont.SetText("Continue");

	return CForm::Initialize(mygame, font);
}
//=============================================================================================================
bool StatisticsMenu::LoadContent()
{
	CForm::LoadContent();
	onresetdevice();

	return true;
}
//=============================================================================================================
void StatisticsMenu::SetState(unsigned int newstate)
{
	switch( newstate )
	{
	case TransitionIn:
		alpha = 0;
		ticks = 0;
		alpha.direction = 1;

		for( size_t i = 0; i < NUM_ROWS; ++i )
		{
			entries[i].key.SetState(Idle);
			entries[i].value.SetState(Idle);
		}

		title.SetState(Idle);
		cont.SetState(Inactive);
		break;

	case TransitionOut:
		alpha = alpha.count() - 1;
		ticks = 0;
		alpha.direction = -1;

		cont.SetState(Inactive);
		break;

	default:
		break;
	}

	CControl::SetState(newstate);
}
//=============================================================================================================
void StatisticsMenu::Update()
{
	switch( state )
	{
	case TransitionIn:
		if( ticks > alpha.count() + 1 )
		{
			SetState(Idle);
			cont.SetState(Idle);
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
			selected(action);
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
void StatisticsMenu::onfocusgained()
{
	CForm::onfocusgained();
	SetState(TransitionIn);

	entries[0].value.SetText(DummyFramework::CHelper::DiscreteToString(GameVariables::EnemiesKilled));
	entries[1].value.SetText(DummyFramework::CHelper::DiscreteToString(GameVariables::PowerupsAcquired));
	entries[2].value.SetText(DummyFramework::CHelper::DiscreteToString(GameVariables::Deaths));
	entries[3].value.SetText(DummyFramework::CHelper::DiscreteToString(GameVariables::Score));
	entries[4].value.SetText(DummyFramework::CHelper::DiscreteToString(GameVariables::TotalScore));
}
//=============================================================================================================
void StatisticsMenu::onfocuslost()
{
	CForm::onfocuslost();
	SetState(Hidden);

	for( size_t i = 0; i < NUM_ROWS; ++i )
	{
		entries[i].key.SetState(Hidden);
		entries[i].value.SetState(Hidden);
	}

	title.SetState(Hidden);
	cont.SetState(Hidden);
}
//=============================================================================================================
void StatisticsMenu::onresetdevice()
{
	D3DXVECTOR2 size;

	title.Position.x = GameVariables::ScreenWidth * 0.5f;
	title.Position.y = GameVariables::ScreenHeight * 0.08f * 1.6f;
	title.GetSize(size);

	cont.Position.x = title.Position.x;
	cont.Position.y = GameVariables::ScreenHeight - title.Position.y * 1.2f;

	float spacing = GameVariables::ScreenHeight * 0.08f;
	float height = (NUM_ROWS - 1) * spacing;

	for( size_t i = 0; i < NUM_ROWS; ++i )
	{
		entries[i].key.Position.x = (GameVariables::ScreenWidth - Background->Size.x) * 0.5f + spacing * 2;
		entries[i].value.Position.x = (GameVariables::ScreenWidth + Background->Size.x) * 0.5f - spacing * 2;

		entries[i].key.Position.y = entries[i].value.Position.y =
			(GameVariables::ScreenHeight - height) * 0.5f + i * (spacing);
	}

	CForm::onresetdevice();
}
//=============================================================================================================
void StatisticsMenu::onkeyup(const DummyFramework::skeyboardstate& kstate)
{
	if( state == TransitionIn || state == TransitionOut )
		return;

	switch( kstate.key )
	{
	case VK_RETURN:
	case VK_SPACE:
		action = Continue;
		SetState(TransitionOut);
		break;

	default:
		break;
	}
}
//=============================================================================================================
void StatisticsMenu::onmouseup(const DummyFramework::smousestate& mstate)
{
	if( state == TransitionIn || state == TransitionOut )
		return;

	if( cont.MouseOver(mstate.x, mstate.y) )
	{
		action = Continue;
		SetState(TransitionOut);
	}
}
//=============================================================================================================
