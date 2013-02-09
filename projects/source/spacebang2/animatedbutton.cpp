//=============================================================================================================
#include "gameelement.h"
#include "animatedbutton.h"

AnimatedButton::AnimatedButton()
	: CButton()
{
	alpha.set(12, 0.0f, 0.0f);
	alpha.direction = 1;
	alpha = 0;
	alpha.continous = true;
}
//=============================================================================================================
bool AnimatedButton::LoadContent()
{
	bool success = CButton::LoadContent();

	onresetdevice();
	return success;
}
//=============================================================================================================
bool AnimatedButton::MouseOver(short mx, short my)
{
	return
		(mx >= bounds.x && mx <= bounds.z &&
		my >= bounds.y && my <= bounds.w);
}
//=============================================================================================================
void AnimatedButton::UpdateBounds()
{
	if( spritefont )
	{
		D3DXVECTOR2 size;

		spritefont->MeasureText(size, GroupID, textid);

		if( Alignment & DT_RIGHT )
		{
			bounds.x = Position.x - size.x;
			bounds.z = Position.x;
		}
		else if( Alignment & DT_CENTER )
		{
			bounds.x = Position.x - size.x * 0.5f;
			bounds.z = Position.x + size.x * 0.5f;
		}
		else
		{
			bounds.x = Position.x;
			bounds.z = Position.x + size.x;
		}

		if( Alignment & DT_BOTTOM )
		{
			bounds.y = Position.y - size.y;
			bounds.w = Position.y;
		}
		else if( Alignment & DT_VCENTER )
		{
			bounds.y = Position.y - size.y * 0.5f;
			bounds.w = Position.y + size.y * 0.5f;
		}
		else
		{
			bounds.y = Position.y;
			bounds.w = Position.y + size.y;
		}
	}
}
//=============================================================================================================
void AnimatedButton::SetState(unsigned int newstate)
{
	switch( newstate )
	{
	case Idle:
	case Inactive:
	case Hidden:
		alpha.direction = 1;
		alpha = 0;
		break;

	default:
		break;
	}

	CButton::SetState(newstate);
}
//=============================================================================================================
void AnimatedButton::Update()
{
	alpha.step();
	CButton::Update();
}
//=============================================================================================================
void AnimatedButton::Draw()
{
	if( state != Hidden )
	{
		alpha.smooth((float)game->Sync.Alpha());

		D3DXCOLOR color = Color + alpha.value * (ActiveColor - Color);
		float offset = scale * 0.125f * alpha.value;

		spritefont->SetTextScale(GroupID, textid, scale + offset);
		spritefont->Set(GroupID, textid, text, Position, Alignment, (DWORD)color);
	}
}
//=============================================================================================================
void AnimatedButton::onresetdevice()
{
	scale = spritefont->GetTextScale(GroupID, textid);
	UpdateBounds();
}
//=============================================================================================================
