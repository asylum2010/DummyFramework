//=============================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "background.h"
#include "avatar.h"
#include "script.h"
#include "explosion.h"
#include "powerup.h"
#include "chatbubble.h"
#include "hud.h"
#include "enemy.h"
#include "projectile.h"

#include <dummyframework/menu/CForm.h>

#define INITIAL_DELAY    2
#define DEFAULT_DELAY    5
#define DEFAULT_SHOOT    8
#define DEFAULT_CANNONS  1
#define DEFAULT_INVERT   30
#define MAX_SPRITES      200
//#define DEBUG_ENTITIES

/**
 * \brief Base class for all game logic
 */
class Game : public DummyFramework::CForm
{
    friend class Script;

    /**
     * \brief Variables that need to be saved at the beginning of a level
     */
    struct savedvariables
    {
        unsigned long score;
        unsigned short health;
        unsigned short integrity;
        unsigned short maxhealth;
        unsigned short maxintegrity;
    };

    typedef std::list<Projectile> projectilelist;
    typedef std::list<Explosion> explosionlist;
    typedef std::list<Powerup> poweruplist;
	typedef std::set<Enemy*> enemyset;
    typedef std::set<short> powerupset;

private:
    DummyFramework::syncedanimator<float, MENU_TRANSITION * 2> alpha;

    Background          background;           /*!< \brief Layered background */
    Avatar              avatar;               /*!< \brief The main character */
    Hud                 hud;                  /*!< \brief Health, shield, etc. */
    Script              script;               /*!< \brief Script interpreter */
    ChatBubble          avatarbubble;         /*!< \brief Chat bubble for avatar */

    IDirect3DSurface9*  rendersurface;        /*!< \brief Surface of the rendertarget */
    IDirect3DSurface9*  oldsurface;           /*!< \brief Surface of the backbuffer */
    IDirect3DTexture9*  rendertarget;
    IDirect3DTexture9*  wave;
    IDirect3DTexture9*  atlastex1;
    ID3DXEffect*        distort;              /*!< \brief Spacetime distortion effect */

    textureatlas        atlas1;
    projectilelist      avatarprojectiles;    /*!< \brief Projectiles fired by the player */
    projectilelist      enemyprojectiles;     /*!< \brief Projectiles fired by enemies */
    enemyset            enemies;
    explosionlist       explosions;
    poweruplist         powerups;
    quadbuffer          quads;                /*!< \brief Dynamic vertex buffer */
    savedvariables      savedvars;
    unsigned long       id;                   /*!< \brief ID of the rendertarget texture */
    unsigned char       invert, invertmode;
    long                execdelay;            /*!< \brief Time until next command */
    int                 ticks;                /*!< \brief Counter */
    size_t              execticks;            /*!< \brief Counter for script execution */
    float               texcoords[4];
    
	//! Adds an enemy to the game field
    void AddEnemy(long type, long height, long width, long track, long speed);

	//! Adds a powerup
    void AddPowerup(const D3DXVECTOR2 pos);

	//! Deletes all enemies, powerups etc.
    void Clear();

	//! If shaders are supported draw a water-like effect
    void DrawWithShadersInverted(float syncalpha);

	//! Draw normally
    void DrawWithoutShaders(float syncalpha);

	//! Draw without water effect
    void DrawWithoutShadersInverted(float syncalpha);

	//! Deactivate the current powerup
    void UnapplyPowerup();

	//! Check for collisions between entities
    void UpdateCollisions(float syncalpha);

	//! Resets the variables
    void RestartLevel();

public:
	/**
	 * \brief Extended game state
	 */
    enum gamestateex
    {
        TransitionHalf = Inactive + 1,
        Animation = Inactive + 2,
        Play = Inactive + 3,
        Invert = Inactive + 4,
        FlyOff = Inactive + 5,
        FadeToMainMenu = Inactive + 6,
        FadeToRestart = Inactive + 7,
        FadeToNext = Inactive + 8
    };

	/**
	 * \brief Action set
	 */
    enum formaction
    {
        PauseMenu,
        GameOver,
        MainMenu,
        LevelComplete,
        NextLevel,
        Restart
    };

    Game();
    ~Game();
    
	//! Initializes the game
    bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);

	//! Load textures, shaders etc.
    bool LoadContent();

	//! Render the game
    void Draw();

	//! Resize sprites when the display mode changed
    void ResizeSprites();

	//! Reset the game logic
    void Reset(const std::string& stagefile);

	//! Set a new gamestate
    void SetState(unsigned long newstate);

    //! Menu logic update
    void Update();

    //! Game logic update
    void InternalUpdate();

	//! Remove states related to keyboard
	inline void FlushControlStates() {
		avatar.FlushControlStates();
	}

eventhandlers:
    void avatar_shoot();
    void avatar_dead();
    void avatar_explode();

    void enemy_explode(Explosion& ex);
    void enemy_shoot1(Projectile& p);
    void enemy_shoot2(Projectile& p);
    void enemy_dead(const Enemy& sender);

    void rapidfire_apply();
    void twincannons_apply();
    void extrahealth_apply();
    void extrashield_apply();

    void onlostdevice();
    void onresetdevice();
    void onfocusgained();
    void onfocuslost();
    void onkeyup(const DummyFramework::skeyboardstate& kstate);
};

#endif
//=============================================================================================================


