//=============================================================================================================
#include <dummyframework/base/CHelper.h>
#include <d3d9.h>

#include "game.h"
#include "script.h"
#include "lexer.cpp"
#include "parser.cpp"

Script::Script()
{
	game = NULL;
	nextstm = NULL;
	program = new char[PROGRAM_SIZE];

	statements[STM_ANIMATIONBEGIN] = &Script::stm_AnimationBegin;
	statements[STM_GAMEBEGIN] = &Script::stm_GameBegin;
	statements[STM_MESSAGE] = &Script::stm_Message;
	statements[STM_ENEMY] = &Script::stm_Enemy;
	statements[STM_WAIT] = &Script::stm_Wait;
	statements[STM_INVERT] = &Script::stm_Invert;
	statements[STM_TUTORIAL] = &Script::stm_Tutorial;
	statements[STM_TITLE] = &Script::stm_Title;
}
//=============================================================================================================
Script::~Script()
{
	if( program )
		delete[] program;

	DeleteGarbage();
}
//=============================================================================================================
void Script::DeleteGarbage()
{
	for( garbagelist::iterator it = garbage.begin(); it != garbage.end(); ++it )
		delete (*it);

	garbage.clear();
}
//=============================================================================================================
void Script::Reset()
{
	nextstm = program;
	stmno = 0;
}
//=============================================================================================================
void Script::Flush()
{
	for( texturemap::iterator it = loadedtextures.begin(); it != loadedtextures.end(); ++it )
	{
		game->game->Content.UnloadTexture(it->second.Texture);
	}

	loadedtextures.clear();
}
//=============================================================================================================
bool Script::Compile(const std::string& file, Game& mygame)
{
	int end, length;

	if( !game )
		game = &mygame;

	layer = 0;
	DeleteGarbage();

#ifdef _MSC_VER
	FILE* f = NULL;
	fopen_s(&f, file.c_str(), "rb");
#else
	FILE* f = fopen(file.c_str(), "rb");
#endif

	dassert(false, "Script::Compile(): Could not open file", f);

	fseek(f, 0, SEEK_END);
	end = ftell(f);

	fseek(f, 0, SEEK_SET);
	length = end - ftell(f);

	char* buffer = (char*)malloc((length + 2) * sizeof(char));
	dassert(false, "Script::Compile(): Could not create buffer", buffer);

	buffer[length + 1] = buffer[length] = 0;
	fread(buffer, sizeof(char), length, f);

	yy_scan_buffer(buffer, length + 2);
	fclose(f);

	stmcount = 0;
	nextstm = program;
	interpreter = this;

	std::cout << "Compiling \'" << file << "\'\n";

	needtodelete = loadedtextures;
	int ret = yyparse();

	yy_delete_buffer(YY_CURRENT_BUFFER);
	free(buffer);

	for( texturemap::iterator it = needtodelete.begin(); it != needtodelete.end(); ++it )
	{
		loadedtextures.erase(it->first);
		game->game->Content.UnloadTexture(it->second.Texture);
	}

	nextstm = program;
	stmno = 0;

	dnassert(false, "Script::Compile(): Parse error", ret != 0);
	return (ret == 0);
}
//=============================================================================================================
bool Script::ExecuteNext()
{
	if( stmno == stmcount )
		return false;

	char opcode = *nextstm;
	char params = *(nextstm + 1);

	stmfunc func = statements[opcode];
	(this->*func)(nextstm + 2);

	nextstm += params * sizeof(int) + 2;
	++stmno;

	return true;
}
//=============================================================================================================
void Script::ProcessStatement(std::string* name, parameterlist* params)
{
	if( (nextstm + 22) >= (program + PROGRAM_SIZE) )
	{
		dsad("Script::ProcessStatement(): No more space for byte code");
		return;
	}

	// meg lehet ezt majd máshogyan is, ezért külön fv

	if( 0 == name->compare("message") )
	{
		scripterror("message(): cannot be used in settings block", block != settingsblock);
		scripterror("message(): takes 1-2 arguments", params);
		scripterror("message(): takes 1-2 arguments", params->size() == 1 || params->size() == 2);

		set_opcode(STM_MESSAGE);
		set_pcount(2);

		parameterlist::iterator it = params->begin();
		scripterror("message(): argument 0 must be string", (*it)->type == TYPE_STRING);

		int ptr = reinterpret_cast<int>((*it)->value);
		int length = 0L;

		set_param(0, ptr);
		++it;

		if( it != params->end() )
		{
			scripterror("message(): argument 1 must be int", (*it)->type == TYPE_NUMBER);
			length = atoi((*it)->value->c_str());
		}

		set_param(1, length);
		nextstm += (2 + 2 * sizeof(int));
	}
	else if( 0 == name->compare("wait") )
	{
		scripterror("wait(): cannot be used in settings block", block != settingsblock);
		scripterror("wait(): takes 1 arguments", params);
		scripterror("wait(): takes 1 arguments", params->size() == 1);

		set_opcode(STM_WAIT);
		set_pcount(1);

		parameterlist::iterator it = params->begin();
		int cond;

		if( (*it)->type == TYPE_NUMBER )
		{
			cond = atoi((*it)->value->c_str());
		}
		else if( (*it)->type == TYPE_IDENTIFIER )
		{
			cond = -1;
		}
		else
			scripterror("wait(): argument 0 must be number or identifier", false);

		set_param(0, cond);
		nextstm += (2 + sizeof(int));
	}
	else if( 0 == name->compare("enemy") )
	{
		scripterror("enemy(): cannot be used in settings block", block != settingsblock);
		scripterror("enemy(): takes 5 arguments", params);
		scripterror("enemy(): takes 5 arguments", params->size() == 5);

		set_opcode(STM_ENEMY);
		set_pcount(5);

		parameterlist::iterator it = params->begin();
		scripterror("enemy(): argument 0 must be identifier", (*it)->type == TYPE_IDENTIFIER);

		int type, height, width, pos, speed;

		if( 0 == (*it)->value->compare("regular") )
			type = 0;
		else if( 0 == (*it)->value->compare("worm") )
			type = 1;
		else if( 0 == (*it)->value->compare("bomb") )
			type = 2;
		else if( 0 == (*it)->value->compare("island") )
			type = 3;
		else if( 0 == (*it)->value->compare("elite") )
			type = 4;
		else if( 0 == (*it)->value->compare("razor") )
			type = 5;
		else
			scripterror("enemy(): argument 0 invalid", false);

		++it;
		scripterror("enemy(): argument 1 must be number", (*it)->type == TYPE_NUMBER);
		height = (int)atoi((*it)->value->c_str());

		++it;
		scripterror("enemy(): argument 2 must be number", (*it)->type == TYPE_NUMBER);
		width = (int)atoi((*it)->value->c_str());

		++it;
		scripterror("enemy(): argument 3 must be identifier", (*it)->type == TYPE_IDENTIFIER);

		if( 0 == (*it)->value->compare("top") )
			pos = 0;
		else if( 0 == (*it)->value->compare("center") )
			pos = 1;
		else if( 0 == (*it)->value->compare("bottom") )
			pos = 2;
		else
			scripterror("enemy(): argument 3 invalid", false);

		++it;
		scripterror("enemy(): argument 4 must be number", (*it)->type == TYPE_NUMBER);
		speed = (int)atoi((*it)->value->c_str());

		set_param(0, type);
		set_param(1, height);
		set_param(2, width);
		set_param(3, pos);
		set_param(4, speed);

		nextstm += (2 + 5 * sizeof(int));
	}
	else if( 0 == name->compare("invert") )
	{
		scripterror("invert(): cannot be used in settings block", block != settingsblock);
		int cond = 0;

		set_opcode(STM_INVERT);
		set_pcount(1);

		if( params )
		{
			scripterror("invert(): takes 1 arguments", params->size() == 1);
			parameterlist::iterator it = params->begin();

			if( (*it)->type == TYPE_IDENTIFIER )
			{
				if( 0 == (*it)->value->compare("x") )
					cond = 1;
				else if( 0 == (*it)->value->compare("y") )
					cond = 2;
				else if( 0 == (*it)->value->compare("xy") )
					cond = 3;
			}
			else
				scripterror("invert(): argument 0 must be identifier", false);
		}

		set_param(0, cond);
		nextstm += (2 + sizeof(int));
	}
	else if( 0 == name->compare("animation") )
	{
		block = animationblock;

		set_opcode(STM_ANIMATIONBEGIN);
		set_pcount(0);

		nextstm += 2;
	}
	else if( 0 == name->compare("game") )
	{
		block = gameblock;

		set_opcode(STM_GAMEBEGIN);
		set_pcount(0);

		nextstm += 2;
	}
	else if( 0 == name->compare("settings") )
	{
		block = settingsblock;
		scripterror("settings[]: 'layers' attribute must be set", params);
		bool haslayer = false;
		GameVariables::EnablePowerups = true;

		for( parameterlist::iterator it = params->begin(); it != params->end(); ++it )
		{
			if( 0 == (*it)->name->compare("layers") )
			{
				size_t count = (size_t)atoi((*it)->value->c_str());
				game->background.Reset(count);

				haslayer = true;
			}
			else if( 0 == (*it)->name->compare("powerups") )
			{
				GameVariables::EnablePowerups = (0 == (*it)->value->compare("yes"));
			}
			else if( 0 == (*it)->name->compare("speed") )
			{
				float speed = (float)atoi((*it)->value->c_str());
				game->background.Speed = GameVariables::CorrelateW(speed);
			}
		}

		scripterror("settings[]: 'layers' attribute must be set", haslayer);
		return;
	}
	else if( 0 == name->compare("layer") )
	{
		scripterror("layer(): can be used only in settings block", block == settingsblock);
		scripterror("layer(): does not take 0 arguments", params);

		parameterlist::reverse_iterator rit = params->rbegin();
		scripterror("layer(): last argument must be percent", (*rit)->type == TYPE_PERCENT);

		float percent = (float)atoi((*rit)->value->c_str()) / 100.0f;
		bool success;
		size_t count = params->size() - 1;
		DummyFramework::CSprite9 sprite;

		game->background.SetLayer(layer, percent, count);

		for( parameterlist::iterator it = params->begin(); it != params->end(); ++it )
		{
			if( (*it)->type == TYPE_STRING )
			{
				std::string path;
				GameVariables::Qualify(path, *((*it)->value));

				texturemap::iterator it = loadedtextures.find(path);

				if( it == loadedtextures.end() )
				{
					success = game->game->Content.LoadSprite(sprite, path, &GameVariables::CorrelateH);
					dassert(, "layer(): Could not load texture", success);
					loadedtextures[path] = sprite;
				}
				else
				{
					sprite = it->second;
					needtodelete.erase(it->first);
				}

				game->background.AddElement(layer, sprite);
			}
			else
				scripterror("layer(): arguments must be string", params);
		}

		++layer;
		return;
	}
	else if( 0 == name->compare("title") )
	{
		scripterror("title(): cannot be used in settings block", block != settingsblock);
		scripterror("title(): takes 1 arguments", params);
		scripterror("title(): takes 1 arguments", params->size() == 1);

		set_opcode(STM_TITLE);
		set_pcount(1);

		parameterlist::iterator it = params->begin();
		scripterror("title(): argument 0 must be string", (*it)->type == TYPE_STRING);

		int ptr = reinterpret_cast<int>((*it)->value);

		set_param(0, ptr);
		nextstm += (2 + sizeof(int));
	}
	else if( 0 == name->compare("tutorial") )
	{
		scripterror("tutorial(): cannot be used in settings block", block != settingsblock);
		scripterror("tutorial(): takes 1 arguments", params);
		scripterror("tutorial(): takes 1 arguments", params->size() == 1);

		set_opcode(STM_TUTORIAL);
		set_pcount(1);

		parameterlist::iterator it = params->begin();
		scripterror("tutorial(): argument 0 must be string", (*it)->type == TYPE_STRING);

		int ptr = reinterpret_cast<int>((*it)->value);

		set_param(0, ptr);
		nextstm += (2 + sizeof(int));
	}
	else
		return;

	++stmcount;
}
//=============================================================================================================
int yylex()
{
	int ret = yyflex();

	switch( ret )
	{
	case NUMBER:
	case IDENTIFIER:
	case STRING:
		yylval.text_t = new std::string(yytext);
		DummyFramework::CHelper::Replace(*yylval.text_t, "\\n", "\n", yytext);
		interpreter->garbage.push_back(yylval.text_t);
		break;

	default:
		break;
	}

	return ret;
}
//=============================================================================================================
void yyerror(const char *s)
{
	std::cout << "ln " << yylloc.first_line << ": " << s << "\n";
}
//=============================================================================================================
