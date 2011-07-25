//=============================================================================================================
#include "worm.h"
#include "avatar.h"
#include "explosion.h"

#include <dummyframework/base/CHelper.h>

Worm::segment::segment()
{
	health = 2;
	alive = true;
}
//=============================================================================================================
Worm::collision::collision()
	: hitinfo()
{
	segment = 0xffffffff;
}
//*************************************************************************************************************
Worm::Worm(size_t length)
	: Enemy()
{
	segments.resize(length);
	precision = 10;
	
	frequency = (2 * D3DX_PI) / (GameVariables::ScreenHeight * 1.3333f);
	phase = D3DX_PI * 0.5f;
	scale = 0.7f;

	Value = 5;
	Direction = 1.0f;
}
//=============================================================================================================
void Worm::GetNext(D3DXVECTOR2& out, const D3DXVECTOR2& curr, float dist)
{
	float step = dist / (float)precision;
	out = curr;

	for( size_t i = 0; i < precision; ++i )
	{
		direction.x = 1.0f;
		direction.y = cos(frequency * out.x + phase) * amplitude * frequency;

		D3DXVec2Normalize(&direction, &direction);
		out.x += direction.x * step;
	}

	out.y = sin(frequency * out.x + phase) * amplitude;
}
//=============================================================================================================
bool Worm::Visible()
{
	if( !alive || segments.size() == 0 )
		return false;

    return segments[segments.size() - 1].position.current().x >= -Image->Size.x;
}
//=============================================================================================================
bool Worm::Collide(const DummyFramework::CSprite9& other)
{
	if( !alive )
		return false;

	D3DXVECTOR2 size = Image->Size;
	collision c;
	bool collide = false;

	for( size_t i = 0; i < segments.size(); ++i )
	{
		segment& s = segments[i];

		// csak élö szelvényekre nézünk ütközést
		if( s.alive )
		{
			s.position.smooth(GameVariables::GetAlpha());
			Image->Position = s.position.value;

			if( (collide = DummyFramework::CSprite9::Collide(c.hitpos, *Image, other)) )
			{
				c.segment = i;
				collisions.push_back(c);
				break;
			}
		}

		// az elsö után kisebbek jönnek
		Image->Size = size * scale;
	}
	
	Image->Size = size;
	return collide;
}
//=============================================================================================================
void Worm::Hit(unsigned short damage)
{
	if( !alive )
		return;

	for( collisionlist::iterator it = collisions.begin(); it != collisions.end(); ++it )
	{
		segment& s = segments[it->segment];

		if( !it->applied && s.alive )
		{
			it->applied = true;

			if( damage >= s.health )
			{
				Explosion ex;
				ex.Value = Value;

				if( it->segment > 0 )
				{
					ex.Scale = min(scale + 0.1f, 1.0f);
					ex.Position = s.position.current() + Image->Size * scale * 0.5f;
				}
				else
				{
					ex.Scale = 1.0f;
					ex.Position = s.position.current() + Image->Size * 0.5f;
				}

				explode(ex);
				s.alive = false;
			}
			else
			{
				it->position = it->hitpos - Hurt->Size * 0.5f - s.position.current();
				it->ticks = 0;
				it->alpha = 0;

				s.health -= damage;
			}
		}
	}
}
//=============================================================================================================
void Worm::SetPosition(const D3DXVECTOR2& newpos)
{
	D3DXVECTOR2 pos(newpos.x, newpos.y);
	D3DXVECTOR2 size(Image->Size);

	float dist = Image->Size.x * 0.7f;
	float start = (GameVariables::ScreenHeight - GameVariables::PlayFieldHeight + Image->Size.y) * 0.5f;
	
	amplitude = (GameVariables::PlayFieldHeight - Image->Size.y) * 0.5f * Direction;
	pos.y = sin(frequency * pos.x + phase) * amplitude;

	for( size_t i = 0; i < segments.size(); ++i )
	{
		segments[i].position = D3DXVECTOR2(
			pos.x - size.x * 0.5f,
			pos.y + abs(amplitude) + start - size.y * 0.5f);

		GetNext(pos, pos, dist);

		dist = Image->Size.x * scale * 0.7f;
		size = Image->Size * scale;
	}
}
//=============================================================================================================
void Worm::Update(const Avatar& av)
{
	if( !alive || segments.size() == 0 )
		return;

	D3DXVECTOR2 newpos, curr;
	D3DXVECTOR2 size(Image->Size);

	float dist = Image->Size.x * 0.7f;
	float start = (GameVariables::ScreenHeight - GameVariables::PlayFieldHeight + Image->Size.y) * 0.5f;

	amplitude = (GameVariables::PlayFieldHeight - Image->Size.y) * 0.5f * Direction;
	curr = segments[0].position.current();

	GetNext(newpos, curr, -Speed);
	newpos.y += abs(amplitude) + start - size.y * 0.5f;

	segments[0].position.previous() = curr;
	segments[0].position.current() = newpos;
	size = Image->Size * scale;

	for( size_t i = 1; i < segments.size(); ++i )
	{
		curr = segments[i - 1].position.current();

		GetNext(newpos, curr, dist);
		newpos.y += abs(amplitude) + start - size.y * 0.5f;

		segments[i].position.previous() = segments[i].position.current();
		segments[i].position.current() = newpos;

		dist = Image->Size.x * scale * 0.7f;
	}
	
	// találat effektek
	collisionlist::iterator next;
	collisionlist::iterator it = collisions.begin();

	while( it != collisions.end() )
	{
		if( it->ticks <= it->alpha.count() + 1 )
		{
			it->alpha.step();
			++it->ticks;

			++it;
		}
		else
		{
			next = it;
			++next;

			collisions.erase(it);
			it = next;
		}
	}
}
//=============================================================================================================
size_t Worm::Write(size_t start, quadbuffer& quad)
{
	if( !alive )
		return 0;

	float texcoords[4];
	D3DXCOLOR color(1, 1, 1, 1);
	D3DXVECTOR2 size(Image->Size);
	size_t count = 0;

	for( size_t i = 0; i < segments.size(); ++i )
	{
		segment& s = segments[i];

		if( (i % 3) == 0 )
			DummyFramework::CHelper::MirrorY(texcoords, Image->Texcoords);
		else if( (i % 3) == 1 )
			DummyFramework::CHelper::MirrorX(texcoords, Image->Texcoords);
		else
			DummyFramework::CHelper::MirrorXY(texcoords, Image->Texcoords);

		if( s.alive )
		{
			s.position.smooth(GameVariables::GetAlpha());

			quad.WriteQuad(start + count, segments[i].position.value, size, texcoords, 0xffffffff, (i % 3) > 1);
			++count;
		}

		size = Image->Size * scale;
	}
	
	// találat effektek
	for( collisionlist::iterator it = collisions.begin(); it != collisions.end(); ++it )
	{
		if( it->ticks <= it->alpha.count() + 1 )
		{
			segment& s = segments[it->segment];

			it->alpha.smooth(GameVariables::GetAlpha());
			color.a = it->alpha.value;

			quad.WriteQuad(start + count, s.position.value + it->position, Hurt->Size, Hurt->Texcoords, (DWORD)color);
			++count;
		}
	}

    return count;
}
//=============================================================================================================
