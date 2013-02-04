//=============================================================================================================
#ifndef _RAZOR_H_
#define _RAZOR_H_

#include "enemy.h"

/**
 * \brief An almost intelligent enemy
 */
class Razor : public Enemy
{
private:
	D3DXVECTOR2 locations[5];  /*!< \brief Locations where the razor can go */
	D3DXVECTOR2 positions[4];  /*!< \brief Corners of the sprite */
	unsigned short state;
	syncedfloat angle;
	size_t ticks;
	
public:
	enum razorstate
	{
		Waiting,
		Floating,
		Idle
	};

	Razor();

	void Hit(unsigned short damage);
	void SetPosition(const D3DXVECTOR2& newpos);
	void Update(const Avatar& av);

	size_t Write(size_t start, quadbuffer& quad);
};

#endif
//=============================================================================================================
