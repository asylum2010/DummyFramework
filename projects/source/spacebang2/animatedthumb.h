//=============================================================================================================
#ifndef _ANIMATEDTHUMB_H_
#define _ANIMATEDTHUMB_H_

#include "animatedbutton.h"

/**
 * \brief An animable thumbnail
 */
class AnimatedThumb : public AnimatedButton
{
public:
    float Texcoords[4];  /*!< \brief Texture coordinates in atlas */
    D3DXVECTOR2 Size;
    DummyFramework::CSprite9* Image;

    AnimatedThumb();
    ~AnimatedThumb() {}

	//! Render the thumbnail
    void Draw();

eventhandlers:
    void onresetdevice();
};

#endif
//=============================================================================================================

