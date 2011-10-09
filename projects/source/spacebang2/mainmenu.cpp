//=============================================================================================================
#include "mainmenu.h"

MainMenu::MainMenu()
    : CForm()
{
    Background = NULL;
    selectedindex = 0;

    alpha.direction = -1;
    alpha = 0;
    alpha.set(0, 1.0f);

    for( size_t i = 0; i < NUM_BUTTONS; ++i )
        AddControl(buttons[i]);
    
    AddControl(title);
}
//=============================================================================================================
void MainMenu::Draw()
{
    alpha.smooth((float)game->Sync.Alpha());

    D3DXMATRIX transform(Background->Size.x, 0, 0, 0, 0, Background->Size.y, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    D3DXCOLOR color(1, 1, 1, alpha.value);

    transform._41 = (GameVariables::ScreenWidth - Background->Size.x) * 0.5f - 0.5f;
    transform._42 = (GameVariables::ScreenHeight - Background->Size.y) * 0.5f - 0.5f;

    game->StateManager->SetTexture(0, Background->Texture);
    game->DrawImage(transform, Background->Texcoords, color);

    buttons[0].Color.a = buttons[1].Color.a =
        buttons[2].Color.a = buttons[3].Color.a = title.Color.a = alpha.value;
    
    CForm::Draw();
    spritefont->Draw(GroupID);
}
//=============================================================================================================
bool MainMenu::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
    onresetdevice();

    buttons[0].Text = "New game";
    buttons[1].Text = "Load chapter";
    buttons[2].Text = "Options";
    buttons[3].Text = "Exit";
    title.Text = "Main menu";

    return CForm::Initialize(mygame, font);
}
//=============================================================================================================
void MainMenu::SetState(unsigned long newstate)
{
    switch( newstate )
    {
    case TransitionIn:
        alpha = 0;
        ticks = 0;
        alpha.direction = 1;

        for( size_t i = 0; i < NUM_BUTTONS; ++i )
            buttons[i].SetState(Inactive);

        title.SetState(Idle);
        break;

    case TransitionOut:
        alpha = alpha.count() - 1;
        ticks = 0;
        alpha.direction = -1;

        for( size_t i = 0; i < NUM_BUTTONS; ++i )
            buttons[i].SetState(Inactive);
        break;
    
    default:
        break;
    }

    CControl::SetState(newstate);
}
//=============================================================================================================
void MainMenu::Update()
{
    switch( state )
    {
    case TransitionIn:
        if( ticks > alpha.count() + 1 )
        {
            SetState(Idle);
            SelectedIndexChanged(2);
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
            selected(action);

            if( action != Quit )
                focuslost(*this);
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
void MainMenu::SelectedIndexChanged(size_t prev)
{
    buttons[prev].SetState(Inactive);
    buttons[selectedindex].SetState(Idle);

    if( selectedindex == 0 )
        action = NewGame;
    else if( selectedindex == 1 )
        action = LoadGame;
    else if( selectedindex == 2 )
        action = Options;
    else if( selectedindex == 3 )
        action = Quit;
}
//=============================================================================================================
void MainMenu::onfocusgained()
{
    CForm::onfocusgained();
    selectedindex = 0;
    SetState(TransitionIn);
}
//=============================================================================================================
void MainMenu::onfocuslost()
{
    CForm::onfocuslost();
    SetState(Hidden);

    for( size_t i = 0; i < NUM_BUTTONS; ++i )
        buttons[i].SetState(Hidden);

    title.SetState(Hidden);
}
//=============================================================================================================
void MainMenu::onkeyup(const DummyFramework::skeyboardstate& kstate)
{
    switch( kstate.key )
    {
    case VK_RETURN:
    case VK_SPACE:
        SetState(TransitionOut);
        break;

    case VK_DOWN:
    case 0x53:
        {
            size_t prev = selectedindex;
            selectedindex = (selectedindex + 1) % NUM_BUTTONS;
            SelectedIndexChanged(prev);
        }
        break;

    case VK_UP:
    case 0x57:
        {
            size_t prev = selectedindex;
            selectedindex = (NUM_BUTTONS + selectedindex - 1) % NUM_BUTTONS;
            SelectedIndexChanged(prev);
        }
        break;

    default:
        break;
    }
}
//=============================================================================================================
void MainMenu::onresetdevice()
{
    float spacing = GameVariables::ScreenHeight * 0.08f;

    buttons[1].Position.x = GameVariables::ScreenWidth * 0.5f;
    buttons[1].Position.y = (GameVariables::ScreenHeight - spacing) * 0.5f;
    buttons[0].Position.x = buttons[1].Position.x;
    buttons[0].Position.y = buttons[1].Position.y - spacing;
    buttons[2].Position.x = buttons[1].Position.x;
    buttons[2].Position.y = buttons[1].Position.y + spacing;
    buttons[3].Position.x = buttons[1].Position.x;
    buttons[3].Position.y = buttons[1].Position.y + 2 * spacing;

    title.Position.x = buttons[1].Position.x;
    title.Position.y = spacing * 1.6f;

    CForm::onresetdevice();
}
//=============================================================================================================

