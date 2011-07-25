//=============================================================================================================
#include "bomb.h"
#include "avatar.h"
#include "explosion.h"

Bomb::Bomb()
    : Enemy()
{
    flash = false;
    state = Idle;
    ticks = rand() % 20;

    Health = 5;
	Value = 15;
}
//=============================================================================================================
bool Bomb::Collide(const DummyFramework::CSprite9& other)
{
	if( !alive )
		return false;

	hitinfo info;

    // az avatár tul közel van -> ö is felrobban
    if( ticks > 33 && dist < maxdist )
	{
		info.hitpos = position.current() + Image->Size * 0.5f;
		hits.push_back(info);
        return true;
	}

    position.smooth(GameVariables::GetAlpha());
	Image->Position = position.value;
	
	bool collide = false;

	if( collide = DummyFramework::CSprite9::Collide(info.hitpos, *Image, other) )
		hits.push_back(info);

    return collide;
}
//=============================================================================================================
void Bomb::Update(const Avatar& av)
{
	if( !alive )
		return;

    D3DXVECTOR2 dir(position.current() - av.Position.current());

    dist = D3DXVec2Length(&dir);
    maxdist = GameVariables::CorrelateH(400.0f);

    position.extend(direction * Speed);
    ++ticks;

	// aktiválodott-e
    if( state == Armed )
    {
        if( ticks % 2 == 0 )
            flash = !flash;

		// magátol robban de az avatár nincs a közelében
        if( ticks > 35 )
        {
            ticks = 35;
            Value = 0;

			hitinfo info;

			info.hitpos = position.current() + Image->Size * 0.5f;
			hits.push_back(info);

            Hit(Health);
			alive = false;
        }
    }
    else
    {
		// lassan villog
        if( ticks % 20 == 0 )
        {
            flash = !flash;
            ticks = 0;
        }

		// ha közel került az avatár akkor aktiválodik
        if( dist < maxdist )
		{
            state = Armed;
			ticks = 0;
		}
    }

	Enemy::UpdateHits();
}
//=============================================================================================================
size_t Bomb::Write(size_t start, quadbuffer& quad)
{
	if( !alive )
		return 0;

    size_t count = 2;
    position.smooth(GameVariables::GetAlpha());

    quad.WriteQuad(start, position.value, Image->Size, Image->Texcoords, 0xffffffff);
    quad.WriteQuad(start + 1, position.value, Highlight->Size, Highlight->Texcoords, (flash ? 0xffffffff : 0x00ffffff));

	count += Enemy::WriteHits(start + count, quad);
    return count;
}
//=============================================================================================================
