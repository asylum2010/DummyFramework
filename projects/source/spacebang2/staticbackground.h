//=============================================================================================================
#ifndef _STATICBACKGROUND_H_
#define _STATICBACKGROUND_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CSprite9.h>
#include "gameelement.h"

/**
 * \brief A simple texture that can fade in and out
 *
 * Stays in the background when activated.
 */
class StaticBackground : public DummyFramework::CForm
{
private:
    DummyFramework::syncedanimator<float, MENU_TRANSITION> alpha;
    int ticks;

public:
    enum formaction
    {
        Visible,
        Invisible
    };

    DummyFramework::CSprite9* Background;

    StaticBackground();
    ~StaticBackground() {}

    void Draw();
    void SetState(unsigned int newstate);
    void Update();

_DUMMY_EVENTHANDLERS:
    void onfocusgained();
};

#endif
//=============================================================================================================
 
