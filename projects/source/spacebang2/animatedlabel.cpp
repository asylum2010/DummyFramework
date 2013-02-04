//=============================================================================================================
#include "animatedlabel.h"
#include "gameelement.h"

AnimatedLabel::AnimatedLabel()
    : CLabel()
{
    IdleDelay = 10;
    ticks = 0;

    alpha.direction = -1;
    alpha.set(0.0f, 1.0f);
    alpha = 0;
}
//=============================================================================================================
AnimatedLabel::~AnimatedLabel()
{
}
//=============================================================================================================
void AnimatedLabel::SetState(unsigned int newstate)
{
    switch( newstate )
    {
    case Idle:
        ticks = 0;
        break;

    case TransitionIn:
        alpha.direction = 1;
        alpha = 0;
        ticks = 0;
        break;

    case TransitionOut:
        alpha.direction = -1;
        alpha = alpha.count() - 1;
        ticks = 0;
        break;
    }

    CLabel::SetState(newstate);
}
//=============================================================================================================
void AnimatedLabel::Update()
{
    switch( state )
    {
    case Idle:
        if( ticks >= (int)IdleDelay )
        {
            SetState(TransitionOut);
        }
        else
            ++ticks;
        break;

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
            SetState(Hidden);
        }
        else
        {
            ++ticks;
            alpha.step();
        }
        break;
    }

    CLabel::Update();
}
//=============================================================================================================
void AnimatedLabel::Draw()
{
    if( state != Hidden )
    {
        alpha.smooth(GameVariables::GetAlpha());
        float offset = GameVariables::CorrelateH(200.0f);

        Position.x = GameVariables::ScreenWidth * 0.5f - alpha.direction * (1.0f - alpha.value) * offset;
        Color.a = alpha.value;
    }

    CLabel::Draw();
}
//=============================================================================================================
