//=============================================================================================================
#ifndef _WORM_H_
#define _WORM_H_

#include "enemy.h"

/**
 * \brief A worm with sine-like movement
 *
 * Approximates the sine curve with small steps along
 * the derivative.
 */
class Worm : public Enemy
{
	/**
	 * \brief A part of the worm
	 */
	struct segment
	{
		syncedvector position;
		unsigned short health;
		bool alive;

		segment();
	};

	/**
	 * \brief Extended hit information
	 */
	struct collision : hitinfo
	{
		size_t segment;

		collision();
	};

private:
	typedef std::vector<segment> segmentlist;
	typedef std::list<collision> collisionlist;

	segmentlist    segments;
	collisionlist  collisions;
	size_t         precision;
	float          amplitude;
	float          frequency;
	float          phase;
	float          scale;

	//! Get the next position on the sine wave
	void GetNext(D3DXVECTOR2& out, const D3DXVECTOR2& curr, float dist);

public:
	float Direction;    /*!< \brief Indicates the amplitude sign of the wave */

	Worm(size_t length);

	//! Checks if on of the worm's segments overlaps with the given sprite
    bool Collide(const DummyFramework::CSprite9& other);

	//! Determines whether the worm is on the screen
	bool Visible();

	//! Applies damage based on hit count
	void Hit(unsigned short damage);

	//! Positions the worm to the given x coordinate
	void SetPosition(const D3DXVECTOR2& newpos);

	//! Updates the worm
    void Update(const Avatar& av);

	//! Renders the worm
    size_t Write(size_t start, quadbuffer& quad);
};

#endif
//=============================================================================================================
