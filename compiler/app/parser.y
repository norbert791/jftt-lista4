%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern int yylex();
extern void yyerror (char const *);

%}

%union {
  long long n;
}

%token <n> NUM;
%token ERROR;
%token EOL;
%left '-' '+'
%left '*' '/'
%precedence NEG
%left '^'     

%type <n> expression

%%
start:
  %empty
| start line
;

line:
  EOL
| expression EOL {
              printf ("\nwynik: %lld\n", $1);
            }
| error EOL {yyclearin;}
;

expression:
| '(' expression ')' { $$ = $2; }
| expression '+' expression { $$ = $1 + $3;}
| expression '-' expression { $$ = $1 - $3;}
| expression '*' expression { $$ = $1 * $3;}
| expression '/' expression {
                if ($3 == 0) {exit(EXIT_FAILURE);}
                $$ = $1 / $3;
                }
| '-' expression     %prec NEG { $$ = -$2;}
;
%%

void yyerror(const char msg[]) {
  fprintf(stderr, "%s\n", msg);
}

int main(void) {
  yyparse();
}