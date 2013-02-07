//=============================================================================================================
#include "powerup.h"

Powerup::Powerup()
{
	Image = NULL;
	Speed = 20;

	offset.set(10, 1.0f, -1.0f);
	offset.direction = 1;

	offset = 0;
	ticks = 0;
}
//=============================================================================================================
void Powerup::GetPositionWithOffset(D3DXVECTOR2& out)
{
	float syncalpha = GameVariables::GetAlpha();

	Position.smooth(syncalpha);
	offset.smooth(syncalpha);

	out.x = Position.value.x;
	out.y = Position.value.y + offset.value * GameVariables::CorrelateH(60.0f);
}
//=============================================================================================================
void Powerup::Update()
{
	offset.step();
	ticks += offset.direction;

	if( ticks >= (size_t)offset.count() )
	{
		offset.direction = -1;
	}
	else if( ticks == 0 )
	{
		offset.direction = 1;
	}

	Position.extend(D3DXVECTOR2(-Speed, 0));
}
//=============================================================================================================
size_t Powerup::Write(size_t start, quadbuffer& quad)
{
	D3DXVECTOR2 pos;
	GetPositionWithOffset(pos);

	quad.WriteQuad(start, pos, Image->Size, Image->Texcoords);
	return 1;
}
//=============================================================================================================
