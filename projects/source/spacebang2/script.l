%option noyywrap
%option yylineno
%option never-interactive

%{

#include "parser.hpp"
#include "script.h"

#define YY_DECL int yyflex YY_PROTO(( void ))

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4102)
#	pragma warning (disable : 4273)
#	pragma warning (disable : 4065)
#	pragma warning (disable : 4267)
#	pragma warning (disable : 4244)
#	pragma warning (disable : 4996)
#endif

%}

WHITESPACE		[ \t\n\r]
IDSTART			[a-zA-Z_]
IDCHAR			[a-zA-Z_0-9]
NUMBER			[0-9]|[1-9]([0-9]+)

%x str

%%

{WHITESPACE}+			{ scriptdebug("WHITESPACE"); return SPACE; }
{NUMBER}				{ scriptdebug("NUMBER"); return NUMBER; }
"//"(.*)				{ scriptdebug("COMMENT"); return COMMENT; }
"("						{ scriptdebug("LRB"); return LRB; }
")"						{ scriptdebug("RRB"); return RRB; }
"{"						{ scriptdebug("LB"); return LB; }
"}"						{ scriptdebug("RB"); return RB; }
"["						{ scriptdebug("LSB"); return LSB; }
"]"						{ scriptdebug("RSB"); return RSB; }
";"						{ scriptdebug("SEMICOLON"); return SEMICOLON; }
"="						{ scriptdebug("EQ"); return EQ; }
"\""					{ scriptdebug("QUOTE"); BEGIN(str);  return QUOTE; }
"%"						{ scriptdebug("PERCENT"); return PERCENT; }
","						{ scriptdebug("COMMA"); return COMMA; }
{IDSTART}({IDCHAR}*)	{ scriptdebug("IDENTIFIER"); return IDENTIFIER; }

<str>"\""				{ scriptdebug("QUOTE"); BEGIN(INITIAL); return QUOTE; }
<str>[^\"]+				{ scriptdebug("STRING"); return STRING; }

<<EOF>>					{ return 0; }
.						{ std::cout << "ln " << yylineno << ": lexical error\n"; return 0; }

%%

#ifdef _MSC_VER
#	pragma warning(pop)
#endif
