//=============================================================================================================
#include "gameelement.h"
#include "animatedbutton.h"

AnimatedButton::AnimatedButton()
    : CButton()
{
    alpha.set(0.0f, 0.0f);
    alpha.direction = 1;
    alpha = 0;
    alpha.continous = true;
}
//=============================================================================================================
bool AnimatedButton::LoadContent()
{
    bool success = CButton::LoadContent();

    onresetdevice();
    return success;
}
//=============================================================================================================
void AnimatedButton::SetState(unsigned long newstate)
{
    switch( newstate )
    {
    case Idle:
    case Inactive:
    case Hidden:
        alpha.direction = 1;
        alpha = 0;
        break;
    
    default:
        break;
    }

    CButton::SetState(newstate);
}
//=============================================================================================================
void AnimatedButton::Update()
{
    alpha.step();
    CButton::Update();
}
//=============================================================================================================
void AnimatedButton::Draw()
{
    if( state != Hidden )
    {
        alpha.smooth((float)game->Sync.Alpha());
        D3DXCOLOR color = Color + alpha.value * (ActiveColor - Color);
        float offset = scale * 0.125f * alpha.value;

        spritefont->SetTextScale(GroupID, textid, scale + offset);
        spritefont->Set(GroupID, textid, Text, Position, Alignment, (DWORD)color);
    }
}
//=============================================================================================================
void AnimatedButton::onresetdevice()
{
	scale = spritefont->GetTextScale(GroupID, textid);
}
//=============================================================================================================

