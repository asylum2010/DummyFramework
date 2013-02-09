//=============================================================================================================
#ifndef _ANIMATEDTHUMB_H_
#define _ANIMATEDTHUMB_H_

#include "animatedbutton.h"

/**
 * \brief An animable thumbnail
 */
class AnimatedThumb : public AnimatedButton
{
private:
	void UpdateBounds();

public:
	float Texcoords[4];		/*!< \brief Texture coordinates in atlas */
	D3DXVECTOR2 Size;
	DummyFramework::CSprite9* Image;

	AnimatedThumb();
	~AnimatedThumb() {}

	//! Render the thumb
	void Draw();

_DUMMY_EVENTHANDLERS:
	void onresetdevice();
};

#endif
//=============================================================================================================

