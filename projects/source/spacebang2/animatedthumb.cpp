//=============================================================================================================
#include "gameelement.h"
#include "animatedthumb.h"

AnimatedThumb::AnimatedThumb()
	: AnimatedButton()
{
	Size.x = Size.y = 0;
	Alignment = DT_CENTER|DT_TOP; // for text
}
//=============================================================================================================
void AnimatedThumb::UpdateBounds()
{
	if( Alignment & DT_RIGHT )
	{
		bounds.x = Position.x - Size.x;
		bounds.z = Position.x;
	}
	else if( Alignment & DT_CENTER )
	{
		bounds.x = Position.x - Size.x * 0.5f;
		bounds.z = Position.x + Size.x * 0.5f;
	}
	else
	{
		bounds.x = Position.x;
		bounds.z = Position.x + Size.x;
	}

	// always VCENTER
	bounds.y = Position.y - Size.y * 0.5f;
	bounds.w = Position.y + Size.y * 0.5f;
}
//=============================================================================================================
void AnimatedThumb::Draw()
{
	if( state != Hidden )
	{
		alpha.smooth((float)game->Sync.Alpha());

		float tmp = Position.y;
		float offset = GameVariables::CorrelateH(20) * alpha.value;

		D3DXVECTOR2 s(Size.x + offset, Size.y + offset);
		D3DXVECTOR2 sc(0.5f, 0.5f);
		D3DXVECTOR2 tr(Position.x, Position.y);

		D3DXMATRIX transform(
			s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, 1, 0,
			-sc.x * s.x + sc.x + tr.x - 0.5f,
			-sc.y * s.y + sc.y + tr.y - 0.5f, 0, 1);

		game->DrawImage(transform, Texcoords, (DWORD)Color);

		Position.y += Size.y * 0.5f;
		AnimatedButton::Draw();
		Position.y = tmp;
	}
}
//=============================================================================================================
void AnimatedThumb::onresetdevice()
{
	AnimatedButton::onresetdevice();
	scale = GameVariables::CorrelateH(0.5f);
}
//=============================================================================================================
