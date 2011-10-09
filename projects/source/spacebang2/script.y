%locations

%{

#include "script.h"

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning (disable : 4102)
#    pragma warning (disable : 4273)
#    pragma warning (disable : 4065)
#    pragma warning (disable : 4267)
#    pragma warning (disable : 4244)
#    pragma warning (disable : 4996)
#endif

int yylex();
void yyerror(const char *s);
Script* interpreter = 0;

%}

%union
{
    std::string*      text_t;
    parameter_desc*   param_t;
    parameterlist*    param_list;
}

%token             QUOTE
%token             COMMENT
%token             SPACE
%token             LRB
%token             RRB
%token             LB
%token             RB
%token             LSB
%token             RSB
%token             SEMICOLON
%token             EQ
%token             PERCENT
%token             COMMA

%token<text_t>     NUMBER
%token<text_t>     IDENTIFIER
%token<text_t>     STRING

%type<param_t>     parameter;
%type<param_t>     attribute;
%type<param_list>  parameter_list;
%type<param_list>  parameter_list2;
%type<param_list>  attributes;
%type<param_list>  attribute_list;

%%

program: misc_list block_list
         {
             scriptdebug("program -> block_list misc_list");
         }
;

misc_list: /* empty */
           | misc_list misc
;

misc: SPACE
      | COMMENT
;

space: /* empty */
       | SPACE
;

block_list: /* empty */
            {
                scriptdebug("block_list -> empty");
            }
          | block misc_list block_list
            {
                scriptdebug("block_list -> block misc_list block_list");
            }
;

block: block_type misc_list LB misc_list statement_list RB
       {
       }
;

block_type: IDENTIFIER SPACE attributes
            {
                interpreter->ProcessStatement($1, $3);
                
                if( $3 )
                {
                    for( parameterlist::iterator it = $3->begin(); it != $3->end(); ++it )
                        delete (*it);
                }
               
                delete $3;
            }
;

attributes: /* empty */
            {
                $$ = NULL;
            }
          | LSB space attribute_list space RSB
            {
                $$ = $3;
            }
;

attribute_list: attribute
                {
                    $$ = new parameterlist();
                    $$->push_back($1);
                }
              | attribute_list space COMMA space attribute
                {
                    $$ = $1;
                    $$->push_back($5);
                }
;

attribute: IDENTIFIER space EQ space NUMBER
           {
               $$ = new parameter_desc();
               
               $$->name = $1;
               $$->value = $5;
               $$->type = TYPE_NUMBER;
           }
         | IDENTIFIER space EQ space STRING
           {
               $$ = new parameter_desc();
               
               $$->name = $1;
               $$->value = $5;
               $$->type = TYPE_STRING;
           }
         | IDENTIFIER space EQ space IDENTIFIER
           {
               $$ = new parameter_desc();
               
               $$->name = $1;
               $$->value = $5;
               $$->type = TYPE_IDENTIFIER;
           }
;

statement_list: /* empty */
                | statement misc_list statement_list
;

statement: IDENTIFIER space LRB space parameter_list RRB space SEMICOLON
           {
               interpreter->ProcessStatement($1, $5);
               
               if( $5 )
               {
                   for( parameterlist::iterator it = $5->begin(); it != $5->end(); ++it )
                       delete (*it);
               }
               
               delete $5;
           }
;

parameter_list: /* empty */
                {
                    $$ = NULL;
                }
              | parameter_list2
                {
                    $$ = $1;
                }
;

parameter_list2: parameter
                 {
                     $$ = new parameterlist();
                     $$->push_back($1);
                 }
               | parameter_list2 space COMMA space parameter
                 {
                     $$ = $1;
                     $$->push_back($5);
                 }
;

parameter: QUOTE STRING QUOTE
           {
               $$ = new parameter_desc();
               
               $$->type = TYPE_STRING;
               $$->value = $2;
           }
         | NUMBER
           {
               $$ = new parameter_desc();
               
               $$->type = TYPE_NUMBER;
               $$->value = $1;
           }
         | NUMBER PERCENT
           {
               $$ = new parameter_desc();
               
               $$->type = TYPE_PERCENT;
               $$->value = $1;
           }
         | IDENTIFIER
           {
               $$ = new parameter_desc();
               
               $$->type = TYPE_IDENTIFIER;
               $$->value = $1;
           }
;



%%

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
