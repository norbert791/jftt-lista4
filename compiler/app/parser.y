%{
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>

extern int yylex();
extern void yyerror (char const *);

%}

%union {
  int64_t n;
}

%token <n> num;
%token PROCEDURE "PROCEDURE";
%token IS "IS";
%token VAR "VAR";
%token MY_BEGIN "BEGIN";
%token END "END";
%token PROGRAM "PROGRAM";
%token IF "IF";
%token THEN "THEN";
%token ELSE "ELSE";
%token ENDIF "ENDIF";
%token ENDWHILE "ENDWHILE";
%token WHILE "WHILE";
%token DO "DO";
%token REPEAT "REPEAT";
%token UNTIL "UNTIL";
%token READ "READ";
%token WRITE "WRITE";
%token <std::string> identifier; //change
%token ASSIGN ":=";
%token EQ "=";
%token NEQ "!=";
%token GEQ ">=";
%token LEQ "<=";
%token GE ">";
%token LE "<";
%token PLUS "+";
%token SUB "-";
%token MULT "*";
%token DIV "/";
%token MOD "%";
%token COMMA ",";
%token LBRACKET "(";
%token RBRACKET ")";
%token SEMICOLON ";";
%token ERROR;

%left "-" "+"
%left "*" "/"
%left "%"
%type <n> expression

%%
program_all: procedures main

procedures: procedures PROCEDURE proc_head IS VAR declarations MY_BEGIN commands END
| procedures PROCEDURE proc_head IS MY_BEGIN commands END
|

main: PROGRAM IS VAR declarations MY_BEGIN commands END
| PROGRAM IS MY_BEGIN commands END

 commands: commands command
 | command

 command: identifier ":=" expression ";"
 | IF condition THEN commands ELSE commands ENDIF
 | IF condition THEN commands ENDIF
 | WHILE condition DO commands ENDWHILE
 | REPEAT commands UNTIL condition ";"
 | proc_call ";"
 | READ identifier ";"
 | WRITE value ";"

 proc_call: identifier "(" declarations ")"
 proc_head: identifier "(" declarations ")"

 declarations: declarations "," identifier
 | identifier

 expression: value
 | value "+" value
 | value "-" value
 | value "*" value
 | value "/" value
 | value "%" value

 condition: value "=" value
 | value "!=" value
 | value ">" value
 | value "<" value
 | value ">=" value
 | value "<=" value

 value: num
 | identifier
%%

void yyerror(const char msg[]) {
  fprintf(stderr, "%s\n", msg);
}

int main(void) {
  yyparse();
}