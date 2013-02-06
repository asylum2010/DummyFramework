//=============================================================================================================
#include "multetris.h"

void MulTetris::menu_hover(size_t action)
{
	if( action == MainMenu::SinglePlayer )
	{
		currentgame = 0;
		single.SetState(SingleGame::BlinkIn);
		versus.SetState(VersusGame::Hidden);
		quit.SetState(ExitGame::Hidden);
	}
	else if( action == MainMenu::MultiVersus )
	{
		currentgame = 1;
		versus.SetState(VersusGame::BlinkIn);
		single.SetState(SingleGame::Hidden);
		coop.SetState(CoopGame::Hidden);
	}
	else if( action == MainMenu::MultiCoop )
	{
		currentgame = 2;
		coop.SetState(CoopGame::BlinkIn);
		versus.SetState(VersusGame::Hidden);
		quit.SetState(ExitGame::Hidden);
	}
	else
	{
		quit.SetState(ExitGame::BlinkIn);
		coop.SetState(VersusGame::Hidden);
		single.SetState(SingleGame::Hidden);
	}
}
//=============================================================================================================
void MulTetris::menu_selected(size_t action)
{
	switch( action )
	{
	case MainMenu::Quit:
		Application.Close();
		break;

	case MainMenu::SinglePlayer:
		currentgame = 0;
		formmanager.Connect(pausemenu, PauseMenu::Continue, single);
		break;

	case MainMenu::MultiVersus:
		currentgame = 1;
		formmanager.Connect(pausemenu, PauseMenu::Continue, versus);
		break;

	case MainMenu::MultiCoop:
		currentgame = 2;
		formmanager.Connect(pausemenu, PauseMenu::Continue, coop);
		break;

	default:
		break;
	}
}
//=============================================================================================================
void MulTetris::pause_selected(size_t action)
{
	if( action == PauseMenu::MainMenu )
	{
		if( currentgame == 0 )
			single.SetState(SingleGame::BlinkOut);
		else if( currentgame == 1 )
			versus.SetState(VersusGame::BlinkOut);
		else if( currentgame == 2 )
			coop.SetState(CoopGame::BlinkOut);
	}
}
//=============================================================================================================
