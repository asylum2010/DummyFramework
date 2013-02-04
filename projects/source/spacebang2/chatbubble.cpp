//=============================================================================================================
#include "chatbubble.h"
#include <dummyframework/base/CHelper.h>

ChatBubble::ChatBubble()
    : CLabel()
{
    IdleDelay = 10;
    Corner = Side = Inside = NULL;
    Alignment = DT_CENTER|DT_VCENTER;
    Color = 0xff6772d3;
    ticks = 0;

    alpha.direction = -1;
    alpha.set(0.0f, 1.0f);
    alpha = 0;
}
//=============================================================================================================
void ChatBubble::SetState(unsigned int newstate)
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
void ChatBubble::Update()
{
    switch( state )
    {
    case Idle:
        if( ticks > (int)IdleDelay + 1 )
        {
            SetState(TransitionOut);
        }
        else
        {
            ++ticks;
            alpha.step();
        }
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
void ChatBubble::Draw()
{
    if( state != Hidden )
    {
        D3DXVECTOR2 textsize;
        D3DXVECTOR2 position;

        spritefont->MeasureText(textsize, GroupID, textid);

        if( Alignment & DT_CENTER )
            position.x = Position.x;
        else if( Alignment & DT_RIGHT )
            position.x = Position.x - Corner->Size.x - textsize.x * 0.5f;
        else
            position.x = Position.x + Corner->Size.x + textsize.x * 0.5f;

        if( Alignment & DT_VCENTER )
            position.y = Position.y;
        else if( Alignment & DT_BOTTOM )
            position.y = Position.y - Corner->Size.y - textsize.y * 0.5f;
        else
            position.y = Position.y + Corner->Size.y + textsize.y * 0.5f;

        spritefont->SetTextScale(GroupID, textid, GameVariables::CorrelateH(0.5f));
        spritefont->Set(GroupID, textid, Text, position, DT_CENTER|DT_VCENTER, (DWORD)Color);
    }
}
//=============================================================================================================
size_t ChatBubble::Write(size_t start, quadbuffer& quad)
{
    size_t count = 0;

    if( state != Hidden )
    {
        D3DXCOLOR color(0xffffffff);
        D3DXVECTOR2 textsize, position, size, tmp;
        float texcoords[4];

        Position.x = floor(Position.x);
        Position.y = floor(Position.y);

        alpha.smooth(GameVariables::GetAlpha());
        Color.a = color.a = alpha.value;

        spritefont->MeasureText(textsize, GroupID, textid);

        if( Alignment & DT_CENTER )
            position.x = Position.x - textsize.x * 0.5f - Corner->Size.x;
        else if( Alignment & DT_RIGHT )
            position.x = Position.x - textsize.x - Corner->Size.x * 2;
        else
            position.x = Position.x;

        if( Alignment & DT_VCENTER )
            position.y = Position.y - textsize.y * 0.5f - Corner->Size.y;
        else if( Alignment & DT_BOTTOM )
            position.y = Position.y - textsize.y - Corner->Size.y * 2;
        else
            position.y = Position.y;

        tmp.x = position.x;
        tmp.y = position.y;

        // bal felsö sarok
        quad.WriteQuad(start, position, Corner->Size, Corner->Texcoords, (DWORD)color);

        // belsö
        quad.WriteQuad(start + 1, position + Corner->Size, textsize, Inside->Texcoords, (DWORD)color);

        // jobb felsö sarok
        position.x += Corner->Size.x + textsize.x;
        DummyFramework::CHelper::MirrorX(texcoords, Corner->Texcoords);
        quad.WriteQuad(start + 2, position, Corner->Size, texcoords, (DWORD)color);

        // jobb also sarok
        position.y += Corner->Size.y + textsize.y;
        DummyFramework::CHelper::MirrorXY(texcoords, Corner->Texcoords);
        quad.WriteQuad(start + 3, position, Corner->Size, texcoords, (DWORD)color, true);

        // bal also sarok
        position.x = tmp.x;
        DummyFramework::CHelper::MirrorY(texcoords, Corner->Texcoords);
        quad.WriteQuad(start + 4, position, Corner->Size, texcoords, (DWORD)color);

        // felsö oldal
        position.x += Corner->Size.x;
        position.y = tmp.y;
        size.x = textsize.x;
        size.y = Side->Size.y;
        quad.WriteQuad(start + 5, position, size, Side->Texcoords, (DWORD)color);

        // also oldal
        position.y += Corner->Size.y + textsize.y;
        DummyFramework::CHelper::MirrorY(texcoords, Side->Texcoords);
        quad.WriteQuad(start + 6, position, size, texcoords, (DWORD)color);
        
        // bal oldal
        position.x = tmp.x;
        position.y = tmp.y + Corner->Size.y;
        size.x = Side->Size.y;
        size.y = textsize.y;
        DummyFramework::CHelper::MirrorYX(texcoords, Side->Texcoords);
        quad.WriteQuad(start + 7, position, size, texcoords, (DWORD)color, true);
        
        // jobb oldal
        position.x += Corner->Size.x + textsize.x;
        DummyFramework::CHelper::MirrorXY(texcoords, Side->Texcoords);
        quad.WriteQuad(start + 8, position, size, texcoords, (DWORD)color, true);

        count = 9;
    }

    return count;
}
//=============================================================================================================
