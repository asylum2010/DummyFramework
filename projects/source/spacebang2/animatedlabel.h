//=============================================================================================================
#ifndef _ANIMATEDLABEL_H_
#define _ANIMATEDLABEL_H_

#include <dummyframework/menu/CLabel.h>

/**
 * \brief An animable label
 */
class AnimatedLabel : public DummyFramework::CLabel
{
protected:
    int ticks;
    DummyFramework::syncedanimator<float, 7> alpha;

public:
    size_t IdleDelay;   /*!< \brief How int should it be visible */

    AnimatedLabel();
    ~AnimatedLabel();

	//! Change the label's state
    void SetState(unsigned int newstate);

	//! Update the label
    void Update();

	//! Render the label
    void Draw();
};

#endif
//=============================================================================================================
