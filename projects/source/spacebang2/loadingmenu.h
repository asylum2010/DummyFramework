//=============================================================================================================
#ifndef _LOADINGMENU_H_
#define _LOADINGMENU_H_

#include <dummyframework/menu/CForm.h>
#include <dummyframework/menu/CLabel.h>
#include "gameelement.h"

/**
 * \brief Displays something while loading content
 */
class LoadingMenu : public DummyFramework::CForm
{
private:
    DummyFramework::syncedanimator<float, MENU_TRANSITION> alpha;
    DummyFramework::CLabel title;
    int ticks;
    
public:
    enum stateex
    {
        OneMoreTick = Inactive + 1
    };

    enum formaction
    {
        Loaded
    };

    LoadingMenu();
    ~LoadingMenu() {}

    bool Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font);

    void Draw();
    void SetState(unsigned long newstate);
    void Update();

events:
    DummyFramework::signal0 startload;

eventhandlers:
    void onfocusgained();
    void onfocuslost();
    void onresetdevice();
};

#endif
//=============================================================================================================
 
