//=============================================================================================================
#ifndef _POWERUP_H_
#define _POWERUP_H_

#include "gameelement.h"
#include <dummyframework/base/d3d9/CSprite9.h>

/**
 * \brief Improves avatar abilities
 *
 * Theres no need for inheritance since the game handles powerups,
 * so it can use the signal/slot system.
 */
class Powerup : public GameElement
{
	struct sinexpi_pi
	{
		inline static float y(float x) {
			return (float)sin((x + 0.5f) * D3DX_PI);
		}
	};

private:
	DummyFramework::syncedanimator<float, sinexpi_pi> offset;
	size_t ticks;

public:
	DummyFramework::CSprite9* Image;
	syncedvector Position;
	float Speed;

	Powerup();
	~Powerup() {}

	//! Returns the powerup's position with its current offset
	void GetPositionWithOffset(D3DXVECTOR2& out);

	//! Fixed timestep update
	void Update();

	//! Render
	size_t Write(size_t start, quadbuffer& quad);

	//! Set the powerup's position
	inline void SetPosition(const D3DXVECTOR2& newpos) {
		Position = D3DXVECTOR2(
			newpos.x - Image->Size.x * 0.5f,
			newpos.y - Image->Size.y * 0.5f);
	}

_DUMMY_EVENTS:
	DummyFramework::unisignal0 apply;
};

#endif
//=============================================================================================================
