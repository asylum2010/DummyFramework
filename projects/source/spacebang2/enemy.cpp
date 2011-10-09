//=============================================================================================================
#include "enemy.h"
#include "avatar.h"
#include "explosion.h"
#include "projectile.h"

Enemy::hitinfo::hitinfo()
{
	alpha.set(0.0f, 0.0f);
	alpha = 0;
	alpha.direction = 1;

	ticks = alpha.count() + 2;
	applied = 0;
}
//*************************************************************************************************************
Enemy::Enemy()
    : GameElement()
{
    direction         = D3DXVECTOR2(-1, 0);
    cooldown          = 0;
    position          = D3DXVECTOR2(0, 0);
	color             = D3DXCOLOR(1, 1, 1, 1);
	alive             = true;

    Image             = NULL;
    Hurt              = NULL;
    Speed             = 10;
    ProjectileSpeed   = 100.0f;
    ProjectileOrigin  = D3DXVECTOR2(0.95f, 0.5f);
    Value             = 10;
    Health            = 2;
}
//=============================================================================================================
bool Enemy::Collide(const DummyFramework::CSprite9& other)
{
	if( !alive )
		return false;

	hitinfo info;
	bool collide = false;

    position.smooth(GameVariables::GetAlpha());
	Image->Position = position.value;

	if( collide = DummyFramework::CSprite9::Collide(info.hitpos, *Image, other) )
		hits.push_back(info);

    return collide;
}
//=============================================================================================================
bool Enemy::Visible()
{
	if( !alive )
		return false;

	// kint van-e a képernyöröl
    return position.current().x >= -Image->Size.x;
}
//=============================================================================================================
void Enemy::Hit(unsigned short damage)
{
	if( !alive )
		return;

	for( hitlist::iterator it = hits.begin(); it != hits.end(); ++it )
	{
		if( !it->applied )
		{
			it->applied = true;

			// ha tul sok a sebzés akkor meghal
			if( damage >= Health )
			{
				alive = false;
				Explosion ex;

				ex.Position = position.current() + Image->Size * 0.5f;
				ex.Scale = 1.0f;

				explode(ex);
				dead(*this);

				break;
			}
			else
			{
				it->hitpos = it->hitpos - position.value;
				it->position = it->hitpos - Hurt->Size * 0.5f;
				it->ticks = 0;
				it->alpha = 0;

				Health-= damage;
			}
		}
	}
		
    //TODO: if( hitpos.x > position.current().x && ... )
}
//=============================================================================================================
void Enemy::SetPosition(const D3DXVECTOR2& newpos)
{
    position = D3DXVECTOR2(
        newpos.x - Image->Size.x * 0.5f,
        newpos.y - Image->Size.y * 0.5f);
}
//=============================================================================================================
void Enemy::Update(const Avatar& av)
{
	if( !alive )
		return;

	Projectile p;
    position.extend(direction * Speed);

    if( cooldown == 0 )
    {
        if( !(av.GetState() & Avatar::Exploded) )
        {
			// ha teljesen a képernyön van már
            if( (position.current().x + Image->Size.x) < GameVariables::ScreenWidth)
            {
                D3DXVECTOR2 avpos = av.Position.current();

				// ha látja az avatárt
                if( avpos.x < position.current().x &&
                    abs(avpos.y - position.current().y) <= Image->Size.y )
                {
					p.Direction.x = -1;
					p.Direction.y = 0;
					p.Speed = Speed;
                    
                    p.Position = D3DXVECTOR2(
						position.previous().x + Image->Size.x * 0.12f,
						position.previous().y + Image->Size.y * 0.7f);

                    shoot(p);
                    cooldown = 60;
                }
            }
        }
    }
    else
        --cooldown;

	UpdateHits();
}
//=============================================================================================================
size_t Enemy::Write(size_t start, quadbuffer& quad)
{
	if( !alive )
		return 0;

    size_t count = 1;

    position.smooth(GameVariables::GetAlpha());
    quad.WriteQuad(start, position.value, Image->Size, Image->Texcoords, 0xffffffff);

	count += WriteHits(start + count, quad);
    return count;
}
//=============================================================================================================
void Enemy::UpdateHits()
{
	hitlist::iterator next;
	hitlist::iterator it = hits.begin();

	while( it != hits.end() )
	{
		if( it->ticks <= it->alpha.count() + 1 )
		{
			it->position = it->hitpos - Hurt->Size * 0.5f;
			it->alpha.step();

			++it->ticks;
			++it;
		}
		else
		{
			next = it;
			++next;

			hits.erase(it);
			it = next;
		}
	}

	dnerror(, "Enemy::UpdateHits(): Too many hits", hits.size() > 20);
}
//=============================================================================================================
size_t Enemy::WriteHits(size_t start, quadbuffer& quad)
{
	if( !alive )
		return 0;

	size_t count = 0;

	for( hitlist::iterator it = hits.begin(); it != hits.end(); ++it )
	{
		if( it->ticks <= it->alpha.count() + 1 )
		{
			it->alpha.smooth(GameVariables::GetAlpha());
			color.a = it->alpha.value;

			quad.WriteQuad(start + count, position.value + it->position, Hurt->Size, Hurt->Texcoords, (DWORD)color);
			++count;
		}
	}

	return count;
}
//=============================================================================================================
