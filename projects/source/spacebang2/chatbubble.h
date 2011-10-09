//=============================================================================================================
#ifndef _CHATBUBBLE_H_
#define _CHATBUBBLE_H_

#include <dummyframework/menu/CLabel.h>
#include <dummyframework/base/d3d9/CSprite9.h>
#include "gameelement.h"

/**
 * \brief Chat bubble for conversations
 */
class ChatBubble : public DummyFramework::CLabel
{
protected:
    int ticks;
    DummyFramework::syncedanimator<float, 7> alpha;

public:
    DummyFramework::CSprite9* Corner;
    DummyFramework::CSprite9* Side;
    DummyFramework::CSprite9* Inside;
    size_t IdleDelay;                   /*!< \brief How long it should be visible */

    ChatBubble();
    ~ChatBubble() {}

	//! Render the bubble
    void Draw();

	//! Change the bubble's state
    void SetState(unsigned long newstate);

	//! Update the bubble
    void Update();

	//! Write into dynamic buffer
    size_t Write(size_t start, quadbuffer& quad);
};

#endif
//=============================================================================================================
