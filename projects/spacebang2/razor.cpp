//=============================================================================================================
#include "razor.h"
#include "avatar.h"
#include "explosion.h"
#include "projectile.h"

#include <dummyframework/base/CHelper.h>

Razor::Razor()
	: Enemy()
{
	Health = 10;
	Value = 30;

	state = Floating;
	angle = 0;

	locations[0] = D3DXVECTOR2(GameVariables::ScreenWidth * 0.25f, GameVariables::ScreenHeight * 0.75f);
	locations[1] = D3DXVECTOR2(GameVariables::ScreenWidth * 0.25f, GameVariables::ScreenHeight * 0.25f);
	locations[2] = D3DXVECTOR2(GameVariables::ScreenWidth * 0.75f, GameVariables::ScreenHeight * 0.25f);
	locations[3] = D3DXVECTOR2(GameVariables::ScreenWidth * 0.75f, GameVariables::ScreenHeight * 0.75f);
	locations[4] = D3DXVECTOR2(GameVariables::ScreenWidth * 0.5f, GameVariables::ScreenHeight * 0.5f);
}
//=============================================================================================================
void Razor::Hit(unsigned short damage)
{
	// ez nem engedi, hogy ütközéstöl haljon meg
	if( Health <= 1 || damage < Health )
		Enemy::Hit(damage);
}
//=============================================================================================================
void Razor::SetPosition(const D3DXVECTOR2& newpos)
{
	Enemy::SetPosition(newpos);

	size_t next = rand() % 4;
	std::swap(locations[next], locations[4]);
}
//=============================================================================================================
void Razor::Update(const Avatar& av)
{
	if( !alive )
		return;

	Projectile p;
	D3DXVECTOR2 shootdir;
	const D3DXVECTOR2& goal = locations[4];

	direction = (goal - Image->Size * 0.5f - position.current());
	shootdir = av.Position.current() - position.current();

	float dist = D3DXVec2Length(&direction);

	D3DXVec2Normalize(&direction, &direction);
	D3DXVec2Normalize(&shootdir, &shootdir);

	switch( state )
	{
	case Floating:
		if( dist < Speed )
		{
			ticks = 0;
			state = Idle;
			position.extend(direction * dist);
		}
		else
			position.extend(direction * Speed);
		break;

	case Idle:
		++ticks;
		position = position.current();

		if( ticks % 12 == 0 )
		{
			ticks = 0;
			state = Waiting;

			size_t next = rand() % 4;
			std::swap(locations[next], locations[4]);
		}
		else if( ticks % 3 == 0 )
		{
			D3DXVECTOR2 pos(
				position.current().x + Image->Size.x * 0.5f,
				position.current().y + Image->Size.y * 0.5f);

			p.Direction.x = shootdir.x;
			p.Direction.y = shootdir.y;
			p.Speed = 50.0f;
			p.Position = pos;

			shoot(p);

			float s = sin(D3DX_PI / 4);
			float c = cos(D3DX_PI / 4);

			p.Direction.x = shootdir.x * c - shootdir.y * s;
			p.Direction.y = shootdir.x * s + shootdir.y * c;
			p.Speed = 50.0f;
			p.Position = pos;

			shoot(p);

			p.Direction.x = shootdir.x * c + shootdir.y * s;
			p.Direction.y = shootdir.y * c - shootdir.x * s;
			p.Speed = 50.0f;
			p.Position = pos;

			shoot(p);
		}
		break;

	case Waiting:
		++ticks;

		if( ticks % 15 == 0 )
			state = Floating;
		break;

	default:
		break;
	}

	// zseniális csalás
	angle.current() = fmodf(angle.current() + 0.2f, D3DX_PI * 2);
	angle.previous() = angle.current() - 0.2f;

	Enemy::UpdateHits();
}
//=============================================================================================================
size_t Razor::Write(size_t start, quadbuffer& quad)
{
	if( !alive )
		return 0;

	size_t count = 1;

    position.smooth(GameVariables::GetAlpha());
    angle.smooth(GameVariables::GetAlpha());

	float a = 0.5f * (1.0f - Image->Size.x) - 0.5f;
	float b = 0.5f * (1.0f - Image->Size.y) - 0.5f;
	float c = cos(angle.value);
	float s = sin(angle.value);

	positions[0].x = a * c + b * s + position.value.x + Image->Size.x * 0.5f;
	positions[0].y = b * c - a * s + position.value.y + Image->Size.y * 0.5f;

	positions[1].x = positions[0].x + Image->Size.x * c;
	positions[1].y = positions[0].y + Image->Size.x * -s;

	positions[2].x = positions[1].x + Image->Size.y * s;
	positions[2].y = positions[1].y + Image->Size.y * c;

	positions[3].x = positions[0].x + Image->Size.y * s;
	positions[3].y = positions[0].y + Image->Size.y * c;
	
    quad.WriteQuad(start, positions, Image->Texcoords, 0xffffffff);
	count += WriteHits(start + count, quad);

    return count;
}
//=============================================================================================================
