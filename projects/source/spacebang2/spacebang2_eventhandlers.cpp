//=============================================================================================================
#include "spacebang2.h"

void SpaceBang2::logo_focuslost(DummyFramework::CControl& sender)
{
	Content.UnloadTexture(logobg.Texture);
	formmanager.RemoveForm(logo);

	logobg.Texture = NULL;
}
//=============================================================================================================
void SpaceBang2::menu_selected(size_t action)
{
	switch( action )
	{
	case MainMenu::NewGame:
		currentlevel = 0;
		break;

	case MainMenu::LoadGame:
		break;

	case MainMenu::Options:
		break;

	case MainMenu::Quit:
		Application.Close();
		break;

	default:
		break;
	}
}
//=============================================================================================================
void SpaceBang2::loading_startload()
{
	Sync.Timer().Stop();
	
	if( !formmanager.AddForm(game) )
	{
		Application.Close();
		return;
	}

	formmanager.Connect(loading, LoadingMenu::Loaded, game);
	formmanager.Connect(game, Game::PauseMenu, pausemenu);
	formmanager.Connect(game, Game::MainMenu, background);
	formmanager.Connect(game, Game::Restart, game);
	formmanager.Connect(game, Game::GameOver, gameover);
	formmanager.Connect(pausemenu, PauseMenu::Continue, game);
	formmanager.Connect(pausemenu, PauseMenu::MainMenu, game);
	formmanager.Connect(pausemenu, PauseMenu::Restart, game);
	formmanager.Connect(gameover, GameOverMenu::MainMenu, game);
	formmanager.Connect(gameover, GameOverMenu::Restart, game);

	formmanager.Connect(statistics, StatisticsMenu::Continue, game);
	formmanager.Connect(game, Game::LevelComplete, statistics);
	formmanager.Connect(game, Game::NextLevel, loading);

	game.Reset(games[currentgame].levels[currentlevel].file);
	Sync.Timer().Start();
}
//=============================================================================================================
void SpaceBang2::chapter_selected(size_t action)
{
	if( action == ChapterMenu::LoadGame )
		currentlevel = loadgame.CurrentLevel;
}
//=============================================================================================================
void SpaceBang2::pause_selected(size_t action)
{
	// PauseMenu::Continue esetén inaktiv az állapota
	switch( action )
	{
	case PauseMenu::MainMenu:
		game.SetState(Game::FadeToMainMenu);
		break;

	case PauseMenu::Restart:
		game.SetState(Game::FadeToRestart);
		break;

	default:
		break;
	}
}
//=============================================================================================================
void SpaceBang2::gameover_selected(size_t action)
{
	switch( action )
	{
	case GameOverMenu::MainMenu:
		game.SetState(Game::FadeToMainMenu);
		break;

	case GameOverMenu::Restart:
		game.SetState(Game::FadeToRestart);
		break;

	default:
		break;
	}
}
//=============================================================================================================
void SpaceBang2::statistics_selected(size_t action)
{
	++currentlevel;

	if( currentlevel >= games[currentgame].levels.size() )
		game.SetState(Game::FadeToMainMenu); // TODO: fadetoendcredits
	else
		game.SetState(Game::FadeToNext);
}
//=============================================================================================================
void SpaceBang2::options_settingschanged()
{
	SaveSettings(Application.ExecutablePath + "settings.ini");
}
//=============================================================================================================
void SpaceBang2::onresetdevice()
{
	GameVariables::ScreenWidth = (float)PresentationParameters.BackBufferWidth;
	GameVariables::ScreenHeight = (float)PresentationParameters.BackBufferHeight;

	CGame9::onresetdevice();
	ResizeSprites();
}
//=============================================================================================================
void SpaceBang2::onactivate()
{
	CGame9::onactivate();

	if( game.HasFocus() )
		GameVariables::Sync.Timer().Start();

	game.FlushControlStates();
}
//=============================================================================================================
void SpaceBang2::ondeactivate()
{
	CGame9::ondeactivate();
	GameVariables::Sync.Timer().Stop();
}
//=============================================================================================================
