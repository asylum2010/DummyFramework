//=============================================================================================================
#ifndef _AVATAR_H_
#define _AVATAR_H_

#include "gameelement.h"
#include <dummyframework/base/d3d9/CSprite9.h>

/**
 * \brief The character controlled by the player
 *
 * This class represents a user-controllable sprite, specifically for shooter games.
 */
class Avatar : public GameElement, public DummyFramework::has_slots
{
    enum action
    {
        Idle = 0,
        Up = 1,
        Down = 2,
        Forward = 4,
        Backward = 8,
        Shoot = 16
    };

    struct sinexpi
    {
        inline static float y(float x) {
            return (float)sin(x * D3DX_PI) * 1.2f;
        }
    };

private:
    DummyFramework::syncedanimator<float, 5, sinexpi> power;
    DummyFramework::syncedanimator<float, 5, sinexpi> shield;

    D3DXVECTOR2                velocity;
    D3DXVECTOR2                direction;
    D3DXVECTOR2                resistance;
    size_t                     empowerticks;
    size_t                     flashticks;
    int                        shieldticks;
    unsigned long              state;
    unsigned short             cooldown;          /*!< \brief Weapon cooldown */
    char                       bindings[5][2];    /*!< \brief Key bindings */
    bool                       disablecontrols;
    bool                       drawornot;         /*!< \brief Flashing after explosion */

public:
    enum avatarstate
    {
        Empowered = 32,
        Exploded = 64,
        Invincible = 128
    };

    DummyFramework::CSprite9*  Image;
    DummyFramework::CSprite9*  Lights;
    DummyFramework::CSprite9*  Shield;        
    D3DXVECTOR2                InitialPosition;   /*!< \brief Starting position on the screen */
    D3DXVECTOR2                ProjectileOrigin;  /*!< \brief Starting position of projectiles (%) */
    D3DXVECTOR4                Bounds;            /*!< \brief Movement limits */
    syncedvector               Position;
    unsigned short             WeaponCooldown;    /*!< \brief Delay until next weapon fire */
    unsigned short             Cannons;
    unsigned short             Health;
    unsigned short             Integrity;         /*!< \brief Shield integrity */
    unsigned short             MaxHealth;
    unsigned short             MaxIntegrity;
    unsigned short             MaxLife;
    float                      Speed;
    float                      Acceleration;
    float                      Resistance;
    float                      ProjectileSpeed;   /*!< \brief Speed of projectiles */
    
    Avatar();
    ~Avatar() {}
    
    //! Call this when the avatar is hit
    void Hit(D3DXVECTOR2 pos);

    //! Resets the avatar's properties
    void Reset();

	//! Puts the avatar into a new state
    void SetState(unsigned long newstate);

    //! Fixed timestep update
    void Update();

    //! Writes the sprites used by the avatar into the dynamic buffer
    size_t Write(size_t start, quadbuffer& quad);

    //! Binds the specified key to the action as primary
    inline void BindPrimary(char index, char key) {
        bindings[index][0] = key;
    }

    //! Binds the specified key to the action as secondary
    inline void BindSecondary(char index, char key) {
        bindings[index][1] = key;
    }

    //! Disables user input for the avatar
    inline void DisableControls(bool disable) {
        disablecontrols = disable;
        velocity.x = velocity.y = 0;
		
		FlushControlStates();
    }

	//! Remove states related to keyboard
	inline void FlushControlStates() {
		state = state & (0xffffffe0);
	}
	    
    //! Gets the avatar's current state
    inline unsigned long GetState() const {
        return state;
    }

events:
    DummyFramework::unisignal0 shoot;       /*!< \brief Fires when the user hits the shoot key */
    DummyFramework::unisignal0 dead;        /*!< \brief Fires when the avatar gets a deadly blow */
    DummyFramework::unisignal0 explode;     /*!< \brief Fires when the avatar loses health */

eventhandlers:
    void onkeydown(const DummyFramework::skeyboardstate& kbstate);
    void onkeyup(const DummyFramework::skeyboardstate& kbstate);
};

#endif
//=============================================================================================================
