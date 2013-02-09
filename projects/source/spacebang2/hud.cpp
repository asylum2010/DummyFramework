//=============================================================================================================
#include <dummyframework/base/CHelper.h>
#include <dummyframework/base/d3d9/CSpriteFont9.h>
#include "hud.h"

Hud::Hud()
	: CForm()
{
	Health			= 5;
	Shield			= 5;
	Score			= 0;
	Side = Panel	= Middle = NULL;

	alpha.set(5, 0.0f, 1.0f);

	AddControl(score);
	AddControl(message);
	AddControl(warning);
}
//=============================================================================================================
bool Hud::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
	score.Alignment = DT_LEFT|DT_VCENTER;
	score.SetText("Your score: " + DummyFramework::CHelper::DiscreteToString(Score));
	score.Color = 0xffffffdd;

	message.Alignment = DT_CENTER|DT_TOP;
	message.SetText("Hello");
	message.Color = 0xffffffdd;

	warning.Alignment = DT_CENTER|DT_VCENTER;
	warning.SetText("Hello");
	warning.Color = 0xffffaaaa;

	onresetdevice();
	return CForm::Initialize(mygame, font);
}
//=============================================================================================================
void Hud::Announce(const std::string& str)
{
	message.Color = 0xffffffdd;
	message.SetText(str);
	message.IdleDelay = GameVariables::ReadLength(str);
	message.SetState(TransitionIn);
}
//=============================================================================================================
void Hud::Tutorial(const std::string& str)
{
	message.Color = 0xffaaffaa;
	message.SetText(str);
	message.IdleDelay = GameVariables::ReadLength(str);
	message.SetState(TransitionIn);
}
//=============================================================================================================
void Hud::Warn(const std::string& str, size_t delay)
{
	warning.Color = 0xffffaaaa;
	warning.SetText(str);
	warning.IdleDelay = delay;
	warning.SetState(TransitionIn);
}
//=============================================================================================================
void Hud::Title(const std::string& str)
{
	warning.Color = 0xffFFDFC6;
	warning.SetText(str);
	warning.IdleDelay = GameVariables::ReadLength(str) + 10;
	warning.SetState(TransitionIn);
}
//=============================================================================================================
void Hud::Reset()
{
	SetState(Idle);
	warning.SetState(Hidden);
	message.SetState(Hidden);

	alpha.direction = -1;
	alpha = 0;
	ticks = 0;
}
//=============================================================================================================
void Hud::SetState(unsigned int newstate)
{
	switch( newstate )
	{
	case Idle:
		score.SetState(Idle);
		break;

	case TransitionIn:
		if( alpha.direction != 1 )
		{
			alpha = 0;
			ticks = 0;
			alpha.direction = 1;
			score.SetState(Idle);
		}
		break;

	case TransitionOut:
		if( alpha.direction == 1 )
		{
			alpha.direction = -1;
			alpha = alpha.count() - 1;
		}
		else
		{
			alpha.direction = 1;
			alpha = 0;
		}

		ticks = 0;
		break;

	case Hidden:
		score.SetState(Hidden);
		break;

	case TransitionHalf:
		if( alpha.direction != -1 )
		{
			alpha = alpha.count() - 1;
			ticks = 0;
			alpha.direction = -1;
		}
		break;

	default:
		break;
	}

	DummyFramework::CForm::SetState(newstate);
}
//=============================================================================================================
void Hud::Update()
{
	switch( state )
	{
	case Idle:
		break;

	case TransitionIn:
		if( ticks > alpha.count() + 1 )
			SetState(Idle);
		else
		{
			++ticks;
			alpha.step();
		}
		break;

	case TransitionHalf:
		if( ticks > alpha.count() + 1 )
			SetState(Idle);
		else
		{
			++ticks;
			alpha.step();
		}
		break;

	case TransitionOut:
		if( ticks > alpha.count() + 1 )
			SetState(Hidden);
		else
		{
			++ticks;
			alpha.step();
		}
		break;

	default:
		break;
	}

	DummyFramework::CForm::Update();
}
//=============================================================================================================
size_t Hud::Write(size_t start, quadbuffer& quad)
{
	if( state == Hidden )
		return 0;

	size_t count = 0;
	D3DXVECTOR2 barpos, position(0, 0);
	D3DXCOLOR color(0xffffffff);

	alpha.smooth(GameVariables::GetAlpha());

	barpos.x = 0.01f * GameVariables::ScreenHeight;
	barpos.y = Panel->Size.y * 0.5f - Side->Size.y;

	healthcolor = (Health < 1 ? 0xffff0000 : 0xff66ff66);
	shieldcolor = (Shield < 1 ? 0xffff0000 : 0xff59fff9);
	healthcolor.a = shieldcolor.a = alpha.value;
	color.a = 1.0f - alpha.value;

	// fekete csikok animáciohoz
	if( state != TransitionOut || (alpha.direction == 1 && state == TransitionOut) )
	{
		position.y = -Panel->Size.y * alpha.value;
		quad.WriteQuad(start, position, Panel->Size, Panel->Texcoords, (DWORD)color);

		position.y = GameVariables::ScreenHeight - Panel->Size.y * (1.0f - alpha.value);
		quad.WriteQuad(start + 1, position, Panel->Size, Panel->Texcoords, (DWORD)color);

		count += 2;
	}

	if( state != TransitionOut || (alpha.direction == -1 && state == TransitionOut) )
	{
		barpos.x = floor(barpos.x);
		barpos.y = floor(barpos.y);

		// élet két vége
		DummyFramework::CHelper::MirrorX(texcoords, Side->Texcoords);

		position.x = barpos.x;
		position.y = -Panel->Size.y + (barpos.y + Panel->Size.y) * alpha.value;
		quad.WriteQuad(start + count, position, Side->Size, Side->Texcoords, (DWORD)healthcolor);

		position.x = barpos.x + Side->Size.x + (float)Health * Middle->Size.x;
		quad.WriteQuad(start + count + 1, position, Side->Size, texcoords, (DWORD)healthcolor);

		// pajzs két vége
		position.x = barpos.x;
		position.y += Side->Size.y;
		quad.WriteQuad(start + count + 2, position, Side->Size, Side->Texcoords, (DWORD)shieldcolor);

		position.x = barpos.x + Side->Size.x + (float)Shield * Middle->Size.x;
		quad.WriteQuad(start + count + 3, position, Side->Size, texcoords, (DWORD)shieldcolor);

		// pajzs belsö része
		position.x = barpos.x + Side->Size.x;
		quad.WriteQuad(start + count + 4, position, D3DXVECTOR2(Middle->Size.x * Shield, Middle->Size.y), Middle->Texcoords, (DWORD)shieldcolor);

		// élet belsö része
		position.x = barpos.x + Side->Size.x;
		position.y -= Side->Size.y;
		quad.WriteQuad(start + count + 5, position, D3DXVECTOR2(Middle->Size.x * Health, Middle->Size.y), Middle->Texcoords, (DWORD)healthcolor);

		// pont
		score.Position.x = barpos.x;
		score.Position.y = GameVariables::ScreenHeight - Panel->Size.y * (alpha.value - 0.5f);
		score.Color.a = alpha.value;
		score.SetText("Your score: " + DummyFramework::CHelper::DiscreteToString(Score));

		count += 6;
	}

	return count;
}
//=============================================================================================================
void Hud::onresetdevice()
{
	message.Position.y = GameVariables::ScreenHeight * 0.25f;
	warning.Position.y = GameVariables::ScreenHeight * 0.2f;

	CForm::onresetdevice();
}
//=============================================================================================================
