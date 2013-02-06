//=============================================================================================================
#include "staticlogo.h"

StaticLogo::StaticLogo()
	: CForm()
{
	Background = NULL;
	IdleDelay = 20;

	alpha.direction = -1;
	alpha.set(MENU_TRANSITION, 0, 1.0f);
	alpha = 0;

	ticks = 0;
}
//=============================================================================================================
void StaticLogo::Draw()
{
	alpha.smooth((float)game->Sync.Alpha());

	D3DXMATRIX transform(Background->Size.x, 0, 0, 0, 0, Background->Size.y, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	D3DXCOLOR color(1, 1, 1, alpha.value);

	transform._41 = (GameVariables::ScreenWidth - Background->Size.x) * 0.5f - 0.5f;
	transform._42 = (GameVariables::ScreenHeight - Background->Size.y) * 0.5f - 0.5f;

	game->StateManager->SetTexture(0, Background->Texture);
	game->DrawImage(transform, Background->Texcoords, color);

	CForm::Draw();
}
//=============================================================================================================
void StaticLogo::SetState(unsigned int newstate)
{
	switch( newstate )
	{
	case TransitionIn:
		alpha = 0;
		ticks = 0;
		alpha.direction = 1;
		break;

	case TransitionOut:
		alpha = alpha.count() - 1;
		ticks = 0;
		alpha.direction = -1;
		break;

	default:
		break;
	}

	CForm::SetState(newstate);
}
//=============================================================================================================
void StaticLogo::Update()
{
	switch( state )
	{
	case Idle:
		if( ticks > (int)IdleDelay + 1 )
		{
			ticks = 0;
			SetState(TransitionOut);
		}
		else
			++ticks;
		break;

	case TransitionIn:
		if( ticks > alpha.count() + 1 )
		{
			ticks = 0;
			SetState(Idle);
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
			action = Exit;
			focuslost(*this);
			SetState(Hidden);
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
void StaticLogo::onfocusgained()
{
	CForm::onfocusgained();
	SetState(TransitionIn);
}
//=============================================================================================================
