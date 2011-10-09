//=============================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "gameelement.h"

#include <dummyframework/base/d3d9/CSprite9.h>
#include <vector>

class Avatar;
class Explosion;
class Projectile;

/**
 * \brief Base class for enemies
 *
 * Each enemy type has different tactics defined by their Update() method.
 * Their behavior depends on the state of the avatar (position, powerups etc.).
 * This basic enemy type shoots when the avatar is in its line of sight.
 */
class Enemy : public GameElement
{
protected:
	struct sinexpi
    {
        inline static float y(float x) {
            return (float)sin(x * D3DX_PI) * 1.2f;
        }
    };

	typedef DummyFramework::syncedanimator<float, 5, sinexpi> hitalpha;

	/**
	 * \brief Information about projectile hits
	 *
	 * From our view the avatar is also a projectile.
	 */
	struct hitinfo
	{
		D3DXVECTOR2  hitpos;         /*!< \brief Collision point */
		D3DXVECTOR2  position;       /*!< \brief Current position */
		hitalpha     alpha;          /*!< \brief Visibility */
		int          ticks;          /*!< \brief Counter */
		bool         applied;        /*!< \brief Did-we apply it? */

		hitinfo();
	};
	
	typedef std::list<hitinfo> hitlist;

	D3DXCOLOR       color;           /*!< \brief Color to use in any method */
    D3DXVECTOR2     direction;       /*!< \brief Moving direction */
	hitlist         hits;            /*!< \brief List of projectile hits */
    syncedvector    position;        /*!< \brief The enemy's position */
    unsigned short  cooldown;        /*!< \brief  Weapon cooldown */
	bool            alive;           /*!< \brief Indiactes whether the enemy is alive */

	//! Update projectile hits
	void UpdateHits();

	//! Renders hits
	size_t WriteHits(size_t start, quadbuffer& quad);

public:
    DummyFramework::CSprite9*  Image;             /*!< \brief Enemy graphic */
    DummyFramework::CSprite9*  Hurt;              /*!< \brief Hit graphic */
    D3DXVECTOR2                ProjectileOrigin;  /*!< \brief Starting position (%) */
    float                      Speed;             /*!< \brief Move speed */
    float                      ProjectileSpeed;   /*!< \brief Speed of projectiles */
    unsigned long              Value;             /*!< \brief How much points the enemy worth */
    unsigned short             Health;            /*!< \brief How many hits it can take */

    Enemy();
    virtual ~Enemy() {}

	//! Checks if the enemy overlaps with the given sprite
    virtual bool Collide(const DummyFramework::CSprite9& other);

	//! Determines whether the enemy is on the screen
    virtual bool Visible();
	
	//! Applies damage based on hit count
    virtual void Hit(unsigned short damage);

	//! Positions the enemy to the given coordinates
    virtual void SetPosition(const D3DXVECTOR2& newpos);

    //! Fixed timestep AI, with respect to the avatar's position
    virtual void Update(const Avatar& av);
    
    //! Write the sprite into the buffer
    virtual size_t Write(size_t start, quadbuffer& quad);

events:
    DummyFramework::unisignal1<Projectile&> shoot;
    DummyFramework::unisignal1<Explosion&> explode;
    DummyFramework::unisignal1<const Enemy&> dead;
};

/**
 * \brief An enemy which can change its position
 *
 * Not yet implemented.
 */
class Elite : public Enemy
{
};

/**
 * \brief A floating island which cannot be killed
 */
class Island : public Enemy
{
public:
	Island() : Enemy() { Health = 65000; }

	//! Has no effect on the island
	void Hit(unsigned short damage) {
		hits.clear();
	}
};

#endif
//=============================================================================================================
