//=============================================================================================================
#include "projectile.h"

Projectile::Projectile()
{
	Image = 0;
}

Projectile::Projectile(const Projectile& other)
{
	Image = other.Image;
	Direction = other.Direction;
	Position = other.Position;
	Speed = other.Speed;
}
