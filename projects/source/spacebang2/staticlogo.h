//=============================================================================================================
#ifndef _STATICLOGO_H_
#define _STATICLOGO_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CSprite9.h>
#include "gameelement.h"

/**
 * \brief A simple texture that can fade in and out
 */
class StaticLogo : public DummyFramework::CForm
{
private:
    DummyFramework::syncedanimator<float, MENU_TRANSITION> alpha;
    int ticks;

public:
	size_t IdleDelay;

    enum formaction
    {
        Exit
    };

    DummyFramework::CSprite9* Background;

    StaticLogo();
    ~StaticLogo() {}

    void Draw();
    void SetState(unsigned int newstate);
    void Update();

_DUMMY_EVENTHANDLERS:
    void onfocusgained();
};

#endif
//=============================================================================================================
 
