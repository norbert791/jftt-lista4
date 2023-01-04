%{
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include "parser.h"

//TODO: Dodać komentarze

int yylex();
%}

%option noyywrap yylineno

%%
[_a-z]+                   {
                            struct myToken tok;
                            tok.str = std::string(yytext);
                            yylval = tok;
                            return identifier;
                          }
[0-9]+                    {
                            int64_t temp = 0; 
                            int r = sscanf(yytext, "%" SCNd64, &temp);
                            if (r != 1) {return ERROR;}
                            yylval.val = temp;
                            return num;
                          }
PROCEDURE                 {return PROCEDURE;}
IS                        {return IS;}
VAR                       {return VAR;}
END                       {return END;}
PROGRAM                   {return PROGRAM;}
IF                        {return IF;}
THEN                      {return THEN;}
ELSE                      {return ELSE;}
ENDIF                     {return ENDIF;}
ENDWHILE                  {return ENDWHILE;}
WHILE                     {return WHILE;}
DO                        {return DO;}
REPEAT                    {return REPEAT;}
UNTIL                     {return UNTIL;}
READ                      {return READ;}
WRITE                     {return WRITE;}
BEGIN                     {return MY_BEGIN;}
[;]                       {return SEMICOLON;}
\+                        {return PLUS;}
-                         {return SUB;}
[*]                       {return MULT;}
[/]                       {return DIV;}
[%]                       {return MOD;}
[,]                       {return COMMA;}
[(]                       {return LBRACKET;}
[)]                       {return RBRACKET;}
:=                        {return ASSIGN;}
=                         {return EQ;}
!=                        {return NEQ;}
>=                        {return GEQ;}
[<]=                      {return LEQ;}
>                         {return GE;}
[<]                       {return LE;}
[ \t\n]                   {}
%%
