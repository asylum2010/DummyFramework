//=============================================================================================================
#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/menu/CButton.h>
#include <dummyframework/base/d3d9/CSprite9.h>

#include "gamevariables.h"

class PauseMenu : public DummyFramework::CForm
{
private:
    DummyFramework::syncedanimator<float, MENU_TRANSITION> alpha;
    DummyFramework::CLabel title;
    DummyFramework::CButton  buttons[2];

    size_t selectedindex;
    int ticks;

    void SelectedIndexChanged(size_t prev);

public:
    enum formaction
    {
        Continue,
        MainMenu
    };

    DummyFramework::CSprite9* Background;

    PauseMenu();
    ~PauseMenu() {}

    bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);

    void Draw();
    void SetState(unsigned long newstate);
    void Update();

events:
    DummyFramework::signal1<size_t> selected;

eventhandlers:
    void onfocusgained();
    void onfocuslost();
    void onkeyup(const DummyFramework::skeyboardstate& kstate);
    void onresetdevice();
};

#endif
//=============================================================================================================
 
 
