//=============================================================================================================
#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include <dummyframework/base/d3d9/CSprite9.h>

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <map>

#define scripterror(e, x) { if( !(x) ) { std::cout << "syntax error: " << e << "\n"; } }
#define scriptnerror(e, x) { if( (x) ) { std::cout << "syntax error: " << e << "\n"; } }
#define scriptdebug(x) //{ std::cout << "debug: " << x << ".\n"; }

#define set_opcode(oc)         *(nextstm + 0) = oc
#define set_pcount(pc)         *(nextstm + 1) = pc
#define set_param(i, p)        memcpy(nextstm + 2 + i * sizeof(long), &p, sizeof(long));
#define get_param(d, p, n)     memcpy(&d, p + n * sizeof(long), sizeof(long));

#define STM_ANIMATIONBEGIN  0
#define STM_GAMEBEGIN       1
#define STM_MESSAGE         2
#define STM_ENEMY           3
#define STM_WAIT            4
#define STM_INVERT          5
#define STM_TUTORIAL        6
#define STM_TITLE           7

#define TYPE_STRING         0
#define TYPE_NUMBER         1
#define TYPE_PERCENT        2
#define TYPE_IDENTIFIER     3
#define PROGRAM_SIZE        4096

class Game;

/**
 * \brief Description for parameters and attributes
 */
struct parameter_desc
{
    char type;
    std::string* name;
    std::string* value;

    parameter_desc() : type(0), name(0), value(0) {}
};

/**
 * \brief Block types
 */
enum blocktype
{
	settingsblock,
	animationblock,
	gameblock
};

typedef std::list<parameter_desc*> parameterlist;

/**
 * \brief Interpreter for level scripts
 */
class Script
{
    friend int yyparse();
    friend int yylex();

private:
    typedef void (Script::*stmfunc)(char*);
    typedef std::list<std::string*> garbagelist;
    typedef std::map<std::string, DummyFramework::CSprite9> texturemap;

    Game*          game;
    garbagelist    garbage;
    texturemap     loadedtextures;
    texturemap     needtodelete;
    stmfunc        statements[10];
    char*          program;
    char*          nextstm;
    long           stmno;
    long           stmcount;
    size_t         layer;
	blocktype      block;

	//! Translates a statement to byte code
    void ProcessStatement(std::string* name, parameterlist* params);

    void stm_AnimationBegin(char* params);
    void stm_GameBegin(char* params);
    void stm_Message(char* params);
    void stm_Enemy(char* params);
    void stm_Wait(char* params);
    void stm_Invert(char* params);
    void stm_Tutorial(char* params);
    void stm_Title(char* params);

public:
    Script();
    ~Script();

	//! Compiles a script
    bool Compile(const std::string& file, Game& mygame);

	//! Execute next statement
    bool ExecuteNext();

	//! Delete strings etc.
    void DeleteGarbage();

	//! Unload resources
    void Flush();

	//! Restart the script
    void Reset();
};

#endif
//=============================================================================================================
