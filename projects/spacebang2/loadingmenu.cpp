//=============================================================================================================
#include "loadingmenu.h"

LoadingMenu::LoadingMenu()
    : CForm()
{
    alpha.direction = -1;
    alpha = 0;
    alpha.set(0, 1.0f);
    ticks = 0;
        
    AddControl(title);
}
//=============================================================================================================
void LoadingMenu::Draw()
{
    D3DXCOLOR color(0xffffffff);

    alpha.smooth((float)game->Sync.Alpha());
    color.a = title.Color.a = alpha.value;
    
    CForm::Draw();
    spritefont->Draw(GroupID);
}
//=============================================================================================================
bool LoadingMenu::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
    onresetdevice();
    title.Text = "Loading";

    return CForm::Initialize(mygame, font);
}
//=============================================================================================================
void LoadingMenu::SetState(unsigned long newstate)
{
    switch( newstate )
    {
    case TransitionIn:
        alpha = 0;
        ticks = 0;
        alpha.direction = 1;
        title.SetState(Idle);
        break;

    case Idle:
        startload();
        newstate = OneMoreTick;
        break;

    case TransitionOut:
        alpha = alpha.count() - 1;
        ticks = 0;
        alpha.direction = -1;
        break;
    
    default:
        break;
    }

    CControl::SetState(newstate);
}
//=============================================================================================================
void LoadingMenu::Update()
{
    switch( state )
    {
    case TransitionIn:
        if( ticks > alpha.count() + 1 )
        {
            SetState(Idle);
        }
        else
        {
            ++ticks;
            alpha.step();
        }
        break;

    case TransitionOut:
        if( ticks > alpha.count() + 1 )
        {
            action = Loaded;
            focuslost(*this);
        }
        else
        {
            ++ticks;
            alpha.step();
        }
        break;

    case OneMoreTick:
        SetState(TransitionOut);
        break;
                
    default:
        break;
    }
    
    CForm::Update();
}
//=============================================================================================================
void LoadingMenu::onfocusgained()
{
    CForm::onfocusgained();
    SetState(TransitionIn);
}
//=============================================================================================================
void LoadingMenu::onfocuslost()
{
    CForm::onfocuslost();
    SetState(Hidden);
    title.SetState(Hidden);
}
//=============================================================================================================
void LoadingMenu::onresetdevice()
{
    title.Position.x = GameVariables::ScreenWidth * 0.5f;
    title.Position.y = GameVariables::ScreenHeight * 0.5f;

    CForm::onresetdevice();
}
//=============================================================================================================

