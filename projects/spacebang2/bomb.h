//=============================================================================================================
#ifndef _BOMB_H_
#define _BOMB_H_

#include "enemy.h"

/**
 * \brief A bomb which explodes on approach
 *
 * If the avatar is close enough to the bomb it will also explode.
 */
class Bomb : public Enemy
{
    enum bombstate
    {
        Idle,
        Armed
    };

private:
    size_t          ticks;                /*!< \brief How much time until epxlosion */
    float           dist;                 /*!< \brief Distance between bomb and avatar */
    float           maxdist;              /*!< \brief Danger radius */
    unsigned short  state;                /*!< \brief Internal state */
    bool            flash;                /*!< \brief Is the LED red or dark */

public:
    Bomb();

    DummyFramework::CSprite9* Highlight;  /*!< \brief LED on the bomb */

	//! Returns true if the sprites overlap, or the sprite is in range of the bomb
    bool Collide(const DummyFramework::CSprite9& other);

	//! Updates the bomb
    void Update(const Avatar& av);

	//! Writes the bomb into the dynamic buffer
    size_t Write(size_t start, quadbuffer& quad);
};

#endif
//=============================================================================================================
