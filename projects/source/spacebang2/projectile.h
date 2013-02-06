//=============================================================================================================
#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "gameelement.h"
#include <dummyframework/base/d3d9/CSprite9.h>

/**
 * \brief Represents a projectile
 */
class Projectile
{
public:
	DummyFramework::CSprite9*	Image;
	D3DXVECTOR2					Direction;
	syncedvector				Position;
	float						Speed;

	Projectile();
	Projectile(const Projectile& other);
};

#endif
//=============================================================================================================
