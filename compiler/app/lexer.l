%{
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "parser.h"

//TODO: Dodać komentarze

int yylex();
%}

%option noyywrap

%%
[_a-z]+                   {
                            myToken tok;
                            tok.str = std::string(yytext);
                            yylval = tok;
                            return identifier;
                          }
[0-9]+                    {
                            int64_t temp = 0; 
                            int r = sscanf(yytext, "%" SCNd64, &temp);
                            if (r != 1) {return ERROR;}
                            yylval.val = r;
                            return num;
                          }
PROCEDURE|IS|VAR          {ECHO;}
END|PROGRAM|IF|THEN|ELSE  {ECHO;}
ENDIF|ENDWHILE|WHILE|DO   {ECHO;}
REPEAT|UNTIL|READ|WRITE   {ECHO;}
BEGIN                     {return MY_BEGIN;}
(:=)|(<=)|(>=)            {ECHO;}
[;+\-*/%(),=\>\<]          {ECHO;}
[ \t\n]                   {}
%%
