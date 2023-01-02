%{
//Standard headers
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include <stack>
#include <memory>
#include <iostream>
//Proj headers
#include <GlobalBlock.hpp>
#include <MainBlock.hpp>
#include <Block.hpp>
#include <Assignment.hpp>

struct myToken {
  std::string str;
  int64_t val;
};
extern int yylex();
extern void yyerror (char const *);

static compilerLogic::Expression exprVar;
static std::shared_ptr<compilerLogic::GlobalBlock> globalBlock = nullptr;
static std::stack<std::shared_ptr<compilerLogic::Block>> scope{};
static std::array<std::string, 2> varStack = {"", ""};
static size_t varStackIndex = 0;
static int64_t id = 1;
%}

%define api.value.type {myToken}

%token num;
%token identifier;
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
%token WRITE "WRITE";
%token ERROR;

%left "-" "+"
%left "*" "/"
%left "%"

%%
program_all: procedures main

procedures: procedures PROCEDURE proc_head IS VAR declarations MY_BEGIN commands END
| procedures PROCEDURE proc_head IS MY_BEGIN commands END
|

main: PROGRAM IS {scope.push(std::make_shared<compilerLogic::MainBlock>());} VAR declarations MY_BEGIN commands END
| PROGRAM IS MY_BEGIN commands END 

 commands: commands command
 | command

 command: identifier ":=" expression ";"          {
                                                    try {
                                                      std::string temp{$1.str};
                                                      auto temp2 = scope.top()->findIdentifier(temp);
                                                      scope.top()->addCommand(std::make_shared<compilerLogic::Command>(new compilerLogic::Assignment(temp2, exprVar)));
                                                    } catch (const std::logic_error& e) {
                                                      std::cerr<<"Lorem ipsum"<<std::endl;
                                                    }
                                                  }
 | IF condition THEN commands ELSE commands ENDIF {}
 | IF condition THEN commands ENDIF               {}
 | WHILE condition DO commands ENDWHILE           {}
 | REPEAT commands UNTIL condition ";"            {}
 | proc_call ";"                                  {}
 | READ identifier ";"                            {}
 | WRITE value ";"                                {}

 proc_call: identifier "(" declarations ")"
 proc_head: identifier "(" declarations ")"

 declarations: declarations "," identifier
 | identifier {
                if (scope.top()->findIdentifier($1)) {
                  std::cerr<<"Variable redeclaration"<<std::endl;
                } else {
                  auto newVar = std::make_shared<compilerLogic::Variable>(id++, $1.str);
                }
              }

 expression: value {}
 | value "+" value {
                      try {
                        auto left = scope.top()->findIdentifier(varStack.at(0));
                        auto right = scope.top()->findIdentifier(varStack.at(1));
                        std::shared_ptr<compilerLogic::Variable> leftConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(left);
                        std::shared_ptr<compilerLogic::Variable> rightConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(right);
                        exprVar = compilerLogic::expressionBuilder("+", leftConverted, rightConverted);
                        varStackIndex = 0;
                      } catch(const std::logic_error& e) {
                        std::cerr<<"Lorem ipsum"<<std::endl;
                      }
                   }
 | value "-" value {
                      try {
                        auto left = scope.top()->findIdentifier(varStack.at(0));
                        auto right = scope.top()->findIdentifier(varStack.at(1));
                        std::shared_ptr<compilerLogic::Variable> leftConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(left);
                        std::shared_ptr<compilerLogic::Variable> rightConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(right);
                        exprVar = compilerLogic::expressionBuilder("-", leftConverted, rightConverted);
                        varStackIndex = 0;
                      } catch(const std::logic_error& e) {
                        std::cerr<<"Lorem ipsum"<<std::endl;
                      }
                   }
 | value "*" value {
                      try {
                        auto left = scope.top()->findIdentifier(varStack.at(0));
                        auto right = scope.top()->findIdentifier(varStack.at(1));
                        std::shared_ptr<compilerLogic::Variable> leftConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(left);
                        std::shared_ptr<compilerLogic::Variable> rightConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(right);
                        exprVar = compilerLogic::expressionBuilder("*", leftConverted, rightConverted);
                        varStackIndex = 0;
                      } catch(const std::logic_error& e) {
                        std::cerr<<"Lorem ipsum"<<std::endl;
                      }
                   }
 | value "/" value {
                      try {
                        auto left = scope.top()->findIdentifier(varStack.at(0));
                        auto right = scope.top()->findIdentifier(varStack.at(1));
                        std::shared_ptr<compilerLogic::Variable> leftConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(left);
                        std::shared_ptr<compilerLogic::Variable> rightConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(right);
                        exprVar = compilerLogic::expressionBuilder("/", leftConverted, rightConverted);
                        varStackIndex = 0;
                      } catch(const std::logic_error& e) {
                        std::cerr<<"Lorem ipsum"<<std::endl;
                      }
                   }
 | value "%" value {
                      try {
                        auto left = scope.top()->findIdentifier(varStack.at(0));
                        auto right = scope.top()->findIdentifier(varStack.at(1));
                        std::shared_ptr<compilerLogic::Variable> leftConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(left);
                        std::shared_ptr<compilerLogic::Variable> rightConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(right);
                        exprVar = compilerLogic::expressionBuilder("%", leftConverted, rightConverted);
                        varStackIndex = 0;
                      } catch(const std::logic_error& e) {
                        std::cerr<<"Lorem ipsum"<<std::endl;
                      }
                   }

 condition: value "=" value {}
 | value "!=" value         {}
 | value ">" value          {}
 | value "<" value          {}
 | value ">=" value         {}
 | value "<=" value         {}

 value: num {std::to_string($1.val);}
 | identifier {return $1.str;}
%%

void yyerror(const char msg[]) {
  fprintf(stderr, "%s\n", msg);
}

int main(void) {
  globalBlock = std::make_shared<compilerLogic::GlobalBlock>();
  scope.push(globalBlock);
  yyparse();
}