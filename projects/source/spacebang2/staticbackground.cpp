//=============================================================================================================
#include "staticbackground.h"

StaticBackground::StaticBackground()
    : CForm()
{
    Background = NULL;

    alpha.direction = -1;
    alpha = 0;
    alpha.set(0, 1.0f);
    ticks = 0;
}
//=============================================================================================================
void StaticBackground::Draw()
{
    alpha.smooth((float)game->Sync.Alpha());
    D3DXMATRIX transform(Background->Size.x, 0, 0, 0, 0, Background->Size.y, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    D3DXCOLOR color(1, 1, 1, alpha.value);
    
    transform._41 = (GameVariables::ScreenWidth - Background->Size.x) * 0.5f - 0.5f;
    transform._42 = (GameVariables::ScreenHeight - Background->Size.y) * 0.5f - 0.5f;

    game->StateManager->SetTexture(0, Background->Texture);
    game->DrawImage(transform, Background->Texcoords, color);

    CForm::Draw();
}
//=============================================================================================================
void StaticBackground::SetState(unsigned long newstate)
{
    switch( newstate )
    {
    case TransitionIn:
        alpha = 0;
        ticks = 0;
        alpha.direction = 1;
        break;

    case TransitionOut:
        alpha = alpha.count() - 1;
        ticks = 0;
        alpha.direction = -1;
        break;

    default:
        break;
    }

    CForm::SetState(newstate);
}
//=============================================================================================================
void StaticBackground::Update()
{
    switch( state )
    {
    case TransitionIn:
        if( ticks > alpha.count() + 1 )
        {
            action = Visible;
            focuslost(*this);
            SetState(Inactive);
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
            action = Invisible;
            focuslost(*this);
            SetState(Hidden);
        }
        else
        {
            ++ticks;
            alpha.step();
        }
        break;
                
    default:
        break;
    }

    CForm::Update();
}
//=============================================================================================================
void StaticBackground::onfocusgained()
{
    CForm::onfocusgained();

    if( state == Hidden )
        SetState(TransitionIn);
    else if( state == Inactive )
        SetState(TransitionOut);
}
//=============================================================================================================
