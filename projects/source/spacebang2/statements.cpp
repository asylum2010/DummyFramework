//=============================================================================================================
#include "script.h"
#include "game.h"

void Script::stm_AnimationBegin(char* params)
{
	game->SetState(Game::Animation);
}
//=============================================================================================================
void Script::stm_GameBegin(char* params)
{
	game->SetState(Game::Play);
}
//=============================================================================================================
void Script::stm_Message(char* params)
{
	int ptr, length;

	get_param(ptr, params, 0);
	get_param(length, params, 1);

	std::string* str = reinterpret_cast<std::string*>(ptr);

	game->avatarbubble.Text = *str;
	game->avatarbubble.IdleDelay = GameVariables::ReadLength(*str);
	game->avatarbubble.SetState(DummyFramework::CControl::TransitionIn);
	game->execdelay = (int)((length == 0 ? (game->avatarbubble.IdleDelay + 10) : length));
}
//=============================================================================================================
void Script::stm_Enemy(char* params)
{
	int type, height, width, track, speed;

	get_param(type, params, 0);
	get_param(height, params, 1);
	get_param(width, params, 2);
	get_param(track, params, 3);
	get_param(speed, params, 4);

	game->AddEnemy(type, height, width, track, speed);
}
//=============================================================================================================
void Script::stm_Wait(char* params)
{
	int cond;

	get_param(cond, params, 0);
	game->execdelay = cond;
}
//=============================================================================================================
void Script::stm_Invert(char* params)
{
	int cond;
	get_param(cond, params, 0);

	if( cond != 0 )
	{
		game->invertmode = (unsigned char)cond;
		game->SetState(Game::Invert);
	}
	else
		game->invert = 0;
}
//=============================================================================================================
void Script::stm_Tutorial(char* params)
{
	int ptr;

	get_param(ptr, params, 0);
	std::string* str = reinterpret_cast<std::string*>(ptr);

	game->hud.Tutorial(*str);
	game->execdelay = (int)GameVariables::ReadLength(*str) + 14;
}
//=============================================================================================================
void Script::stm_Title(char* params)
{
	int ptr;

	get_param(ptr, params, 0);
	std::string* str = reinterpret_cast<std::string*>(ptr);

	game->hud.Title(*str);
	game->execdelay = (int)GameVariables::ReadLength(*str) + 25;
}
//=============================================================================================================
