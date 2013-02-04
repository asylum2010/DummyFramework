//=============================================================================================================
#ifndef _STATISTICSMENU_H_
#define _STATISTICSMENU_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/base/d3d9/CSprite9.h>

#include "animatedbutton.h"
#include "gameelement.h"

#define NUM_ROWS 5

/**
 * \brief Displays some statistics
 */
class StatisticsMenu : public DummyFramework::CForm
{
    struct entry
    {
        DummyFramework::CLabel key;
        DummyFramework::CLabel value;
    };

private:
    DummyFramework::syncedanimator<float, MENU_TRANSITION> alpha;
    DummyFramework::CLabel title;

    AnimatedButton  cont;
    entry           entries[NUM_ROWS];
    int             ticks;

public:
    enum formaction
    {
        Continue
    };

    DummyFramework::CSprite9* Background;

    StatisticsMenu();
    ~StatisticsMenu() {}

    bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);
    bool LoadContent();

    void Draw();
    void SetState(unsigned int newstate);
    void Update();

_DUMMY_EVENTS:
    DummyFramework::signal1<size_t> selected;

_DUMMY_EVENTHANDLERS:
    void onfocusgained();
    void onfocuslost();
    void onkeyup(const DummyFramework::skeyboardstate& kstate);
    void onresetdevice();
};

#endif
//=============================================================================================================
 
