//=============================================================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "gameelement.h"
#include <dummyframework/base/d3d9/CSprite9.h>

/**
 * \brief Class for explosions
 *
 * Represents a procedurally animated explosion sprite.
 */
class Explosion : public GameElement
{
	functor sine2x
	{
		inline static float y(float x) {
			return sin(2 * x) + 0.1f;
		}
	};

private:
	size_t ticks;
	unsigned int state;

	DummyFramework::syncedanimator<float> alpha;
	DummyFramework::syncedanimator<float, sine2x> scale;

public:
	enum explosionstate
	{
		Growing,
		Idle,
		Fading,
		Faded
	};

	DummyFramework::CSprite9*	Image;		/*!< \brief Explosion graphic */
	syncedvector				Position;
	float						Speed;		/*!< \brief Movement speed */
	float						Scale;		/*!< \brief Scaling */
	unsigned int				Value;		/*!< \brief Some explosions worth points */

	Explosion();
	Explosion(const Explosion& other);
	~Explosion() {}

	//! Fixed timestep update
	void Update();

	//! Write into the dynamic buffer
	size_t Write(size_t start, quadbuffer& quad);
	
	//! Returns the explosion's state
	inline unsigned int GetState() const {
		return state;
	}
};

#endif
//=============================================================================================================
