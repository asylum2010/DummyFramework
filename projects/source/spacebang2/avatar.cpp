//=============================================================================================================
#include <dummyframework/base/CHelper.h>
#include "avatar.h"

Avatar::Avatar()
	: GameElement()
{
	Image				= NULL;
	WeaponCooldown		= 10;
	Cannons				= 1;
	Speed				= 45;
	Acceleration		= 20;
	Resistance			= 6;
	ProjectileSpeed		= 100.0f;
	ProjectileOrigin	= D3DXVECTOR2(0.95f, 0.7f);
	InitialPosition		= D3DXVECTOR2(0, 0);
	MaxHealth			= MaxIntegrity = 3;
	Health				= 3; 

	power.set(5, 0.0f, 0.0f);
	shield.set(5, 0.0f, 0.0f);

	Reset();
	memset(bindings, 0, 10 * sizeof(char));
}
//=============================================================================================================
void Avatar::Hit(D3DXVECTOR2 pos)
{
	// ellenörizzük, hogy van-e még pajzs
	if( Integrity > 0 )
	{
		shieldticks = 0;
		shield = 0;
		--Integrity;
	}
	else
		SetState(Exploded);
}
//=============================================================================================================
void Avatar::Reset()
{
	state				= Idle;
	cooldown			= 0;
	velocity			= D3DXVECTOR2(0, 0);
	direction			= D3DXVECTOR2(0, 0);
	disablecontrols		= false;
	drawornot			= true;
	flashticks			= 0;
	empowerticks		= power.count() + 2;
	shieldticks			= shield.count() + 2;

	Position			= InitialPosition;
	Integrity			= MaxIntegrity;

	power.direction = 1;
	power = 0;

	shield.direction = 1;
	shield = 0;
}
//=============================================================================================================
void Avatar::SetState(unsigned int newstate)
{
	switch( newstate )
	{
	case Empowered:
		state |= Empowered;
		empowerticks = 0;
		power = 0;
		break;

	case Exploded:
		DisableControls(true);

		velocity = D3DXVECTOR2(0, 0);
		shieldticks = shield.count() + 2;
		empowerticks = power.count() + 2;
		flashticks = 0;

		// ha nincs életünk, akkor csak robbanunk
		if( Health == 0 )
		{
			state = Exploded;
			explode();
		}
		// ha van akkor robbanunk és halhatatlanok leszünk
		else
		{
			state = Exploded|Invincible;
			Integrity = MaxIntegrity;

			--Health;
			explode();
		}

		break;

	default:
		break;
	}
}
//=============================================================================================================
void Avatar::Update()
{
	// robbanáskor mi történjen
	if( state & Exploded )
	{
		// lehetne az életet is nézni, de akkor eggyel elöbb hal
		bool alive = ((state & Invincible) == Invincible);

		// lejárt-e a várakozási idö
		if( flashticks > (unsigned)(alive ? 5 : 20) )
		{
			// nincs több élet, meghalt
			if( !alive )
			{
				dead();
			}
			// van még élet, egy kis idöre halhatatlan lesz
			else
			{
				flashticks = 0;
				power = 0;
				empowerticks = 0;
				Position = InitialPosition;

				DisableControls(false);
				state = Invincible;
			}
		}
		else
			++flashticks;
	}
	else
	{
		// halhatatlan modban vagyunk
		if( state & Invincible )
		{
			// lejárt-e az idö
			if( flashticks > 25 )
			{
				flashticks = 0;
				state &= (~Invincible);
			}
			else
			{
				++flashticks;

				// villogtatjuk
				if( flashticks % 4 == 0 )
					drawornot = !drawornot;
			}
		}

		// felvett egy powerupot
		if( state & Empowered )
		{
			if( empowerticks > (size_t)power.count() + 1 )
				state &= (~Empowered);
			else
			{
				++empowerticks;
				power.step();
			}
		}

		// találat érte, de a pajzs felfogta
		if( shieldticks <= shield.count() + 1 )
		{
			shield.step();
			++shieldticks;
		}
	}

	// minden mennyiséget a felbontáshoz kell igazitani
	float maxvel = GameVariables::CorrelateH(Speed);
	float accel = GameVariables::CorrelateH(Acceleration);
	float resist = GameVariables::CorrelateH(Resistance);
	float length;

	direction.x = direction.y = 0;

	if( state & Forward )
		direction.x = (state & Backward ? 0.0f : 1.0f);
	else if( state & Backward )
		direction.x = -1;

	if( state & Up )
		direction.y = (state & Down ? 0.0f : -1.0f);
	else if( state & Down )
		direction.y = 1;

	D3DXVec2Normalize(&direction, &direction);
	velocity += direction * accel;

	D3DXVec2Normalize(&resistance, &velocity);
	length = D3DXVec2Length(&velocity);

	// max sebesség
	if( length > maxvel )
		velocity = resistance * maxvel;

	// légellenállás
	if( length > resist )
		velocity -= resistance * resist;
	else
		velocity.x = velocity.y = 0;

	// fix idöközönként lehet löni
	if( state & Shoot )
	{
		if( cooldown == 0 )
		{
			shoot();
			cooldown = WeaponCooldown;
		}
	}

	Position.extend(velocity);

	// ne menjen ki a képernyöröl
	if( DummyFramework::CHelper::Clamp2<float>(Position.current.x, Bounds.x, Bounds.z - Image->Size.x) )
		velocity.x = 0;

	if( DummyFramework::CHelper::Clamp2<float>(Position.current.y, Bounds.y, Bounds.w - Image->Size.y) )
		velocity.y = 0;

	if( cooldown > 0 )
		--cooldown;
}
//=============================================================================================================
size_t Avatar::Write(size_t start, quadbuffer& quad)
{
	size_t count = 0;
	float syncalpha = GameVariables::GetAlpha();

	if( drawornot )
	{
		if( !(state & Exploded) )
		{
			Position.smooth(syncalpha);
			quad.WriteQuad(start, Position.value, Image->Size, Image->Texcoords);
			++count;
		}

		if( state & Empowered )
		{
			power.smooth(syncalpha);

			D3DXCOLOR color(1, 1, 1, power.value);
			D3DXVECTOR2 pos(Position.value + (Image->Size - Lights->Size) * 0.5f);

			quad.WriteQuad(start + count, pos, Lights->Size, Lights->Texcoords, (DWORD)color);
			++count;
		}

		if( shieldticks <= shield.count() + 1 )
		{
			shield.smooth(syncalpha);

			D3DXCOLOR color(1, 1, 1, shield.value);
			D3DXVECTOR2 pos(Position.value + (Image->Size - Shield->Size) * 0.5f);

			quad.WriteQuad(start + count, pos, Shield->Size, Shield->Texcoords, (DWORD)color);
			++count;
		}
	}

	return count;
}
//=============================================================================================================
void Avatar::onkeydown(const DummyFramework::skeyboardstate& kbstate)
{
	if( disablecontrols )
		return;

	if( kbstate.keytable[bindings[0][0]] & 0x80 || kbstate.keytable[bindings[0][1]] & 0x80 )
		state |= Forward;

	if( kbstate.keytable[bindings[1][0]] & 0x80 || kbstate.keytable[bindings[1][1]] & 0x80 )
		state |= Backward;

	if( kbstate.keytable[bindings[2][0]] & 0x80 || kbstate.keytable[bindings[2][1]] & 0x80 )
		state |= Up;

	if( kbstate.keytable[bindings[3][0]] & 0x80 || kbstate.keytable[bindings[3][1]] & 0x80 )
		state |= Down;

	if( kbstate.keytable[bindings[4][0]] & 0x80 || kbstate.keytable[bindings[4][1]] & 0x80 )
		state |= Shoot;
}
//=============================================================================================================
void Avatar::onkeyup(const DummyFramework::skeyboardstate& kbstate)
{
	if( disablecontrols )
		return;

	if( kbstate.keytable[bindings[0][0]] == 0 && kbstate.keytable[bindings[0][1]] == 0 )
		state &= (~Forward);

	if( kbstate.keytable[bindings[1][0]] == 0 && kbstate.keytable[bindings[1][1]] == 0 )
		state &= (~Backward);

	if( kbstate.keytable[bindings[2][0]] == 0 && kbstate.keytable[bindings[2][1]] == 0 )
		state &= (~Up);

	if( kbstate.keytable[bindings[3][0]] == 0 && kbstate.keytable[bindings[3][1]] == 0 )
		state &= (~Down);

	if( kbstate.keytable[bindings[4][0]] == 0 && kbstate.keytable[bindings[4][1]] == 0 )
		state &= (~Shoot);
}
//=============================================================================================================
