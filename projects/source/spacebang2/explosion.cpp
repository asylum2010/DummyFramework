//=============================================================================================================
#include "explosion.h"

Explosion::Explosion()
	: GameElement()
{
	ticks = 0;
	state = Growing;

	alpha.direction = 1;
	alpha.set(5, 0.0f, 1.0f);
	alpha = 0;

	scale.direction = 1;
	scale.set(4, 0.0f, 1.0f);
	scale = 0;

	Speed = 0;
	Scale = 1.0f;
	Value = 0;
}
//=============================================================================================================
void Explosion::Update()
{
	++ticks;

	switch( state )
	{
	case Growing:
		scale.step();

		if( ticks > (size_t)scale.count() )
		{
			ticks = 0;
			state = Idle;
		}
		break;

	case Idle:
		if( ticks > 5 )
		{
			state = Fading;
			ticks = 0;
		}
		break;

	case Fading:
		alpha.step();

		if( ticks > (size_t)alpha.count() )
			state = Faded;
		break;

	default:
		break;
	}

	Position.extend(D3DXVECTOR2(-1, 0) * Speed);
}
//=============================================================================================================
size_t Explosion::Write(size_t start, quadbuffer& quad)
{
	float syncalpha = GameVariables::GetAlpha();

	alpha.smooth(syncalpha);
	scale.smooth(syncalpha);
	Position.smooth(syncalpha);

	D3DXCOLOR color(1, 1, 1, 1.0f - alpha.value);
	D3DXVECTOR2 size = Image->Size * Scale;
	D3DXVECTOR2 pos = Position.value + size * 0.5f - (size * scale.value * 0.5f);

	quad.WriteQuad(start, pos, size * scale.value, Image->Texcoords, (DWORD)color);

	return 1;
}
//=============================================================================================================


