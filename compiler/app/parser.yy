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
#include <Command.hpp>
#include <Assignment.hpp>
#include <IntermidiateParser.hpp>
#include <IntermidiateCode.hpp>
#include <fstream>

extern int yylex();
extern void yyerror (char const *);
extern FILE* yyin;

static compilerLogic::Expression exprVar;
static std::shared_ptr<compilerLogic::GlobalBlock> globalBlock = nullptr;
static std::stack<std::shared_ptr<compilerLogic::Block>> scope{};
static std::array<std::string, 2> varStack = {"", ""};
static size_t varStackIndex = 0;
static int64_t id = 1;

static void expressionBuilderWrapper(std::string symbol, std::string var1, std::string var2) {
  try {
    std::shared_ptr<compilerLogic::Identifier> left = nullptr, right = nullptr;
    if (var1[0] == '#') {
      left = globalBlock->getLiteral(var1, id);
    } else {
      left = scope.top()->findIdentifier(varStack.at(0));
    }
    if (symbol != "none"){
        if (var2[0] == '#') {
        right = globalBlock->getLiteral(var2, id);
      } else {
        right = scope.top()->findIdentifier(varStack.at(1));
      }
    }
    std::shared_ptr<compilerLogic::Variable> leftConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(left);
    std::shared_ptr<compilerLogic::Variable> rightConverted = std::dynamic_pointer_cast<compilerLogic::Variable>(right);
    exprVar = compilerLogic::expressionBuilder(symbol, leftConverted, rightConverted);
    std::cout<<"Expression built: "<<std::endl;
  } catch(const std::out_of_range& e) {
    std::cerr<<e.what()<<std::endl;
  } catch(const std::logic_error& e) {
    std::cerr<<"Lorem ipsum"<<std::endl;
  }
}
%}

%code requires{
#include <utilities.h>
}

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

main: PROGRAM IS {scope.push(std::make_shared<compilerLogic::MainBlock>()); globalBlock->addBlock(scope.top());} VAR declarations MY_BEGIN commands END
| PROGRAM IS MY_BEGIN commands END {scope.pop();}

 commands: commands command
 | command

 command: identifier ":=" expression ";"          {
                                                    try {
                                                      std::string temp{$1.str};
                                                      auto temp2 = scope.top()->findIdentifier(temp);
                                                      std::shared_ptr<compilerLogic::Variable> temp2Converted =
                                                        std::dynamic_pointer_cast<compilerLogic::Variable>(temp2);
                                                      
                                                      scope.top()->addCommand(std::make_shared<compilerLogic::Assignment>(temp2Converted, exprVar));
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

 declarations: declarations "," identifier {
                                            if (scope.top()->verifyIdentifier($3.str)) {
                                              std::cerr<<("Variable redeclaration: " + $3.str)<<std::endl;
                                            } else {
                                              std::cout<<("Identifier pushed: " + $3.str)<<std::endl; 
                                              auto newVar = std::make_shared<compilerLogic::Variable>(id++, $3.str);
                                              scope.top()->addAvailableIdentifier(newVar);
                                            }
                                          }
 | identifier {
                if (scope.top()->verifyIdentifier($1.str)) {
                  std::cerr<<("Variable redeclaration: " + $1.str)<<std::endl;
                } else {
                  std::cout<<("Identifier pushed: " + $1.str)<<std::endl;
                  auto newVar = std::make_shared<compilerLogic::Variable>(id++, $1.str);
                  scope.top()->addAvailableIdentifier(newVar);
                }
              }

 expression: value {
                    expressionBuilderWrapper("none", varStack.at(0), varStack.at(1));
                    varStackIndex = 0;
                   }
 | value "+" value {
                    expressionBuilderWrapper("+", varStack.at(0), varStack.at(1));
                    varStackIndex = 0;
                   }
 | value "-" value {
                    expressionBuilderWrapper("-", varStack.at(0), varStack.at(1));
                    varStackIndex = 0;
                   }
 | value "*" value {
                    expressionBuilderWrapper("*", varStack.at(0), varStack.at(1));
                    varStackIndex = 0;
                   }
 | value "/" value {
                    expressionBuilderWrapper("/", varStack.at(0), varStack.at(1));
                    varStackIndex = 0;
                   }
 | value "%" value {
                    expressionBuilderWrapper("%", varStack.at(0), varStack.at(1));
                    varStackIndex = 0;
                   }

 condition: value "=" value {}
 | value "!=" value         {}
 | value ">" value          {}
 | value "<" value          {}
 | value ">=" value         {}
 | value "<=" value         {}

 value: num   {varStack.at(varStackIndex++) = "#" + std::to_string($1.val); std::cout<<"Literal: "<<$1.val<<std::endl;}
 | identifier {varStack.at(varStackIndex++) = $1.str;}
%%

void yyerror(const char msg[]) {
  fprintf(stderr, "%s\n", msg);
}

int main(int argc, char* argv[]) {
  globalBlock = std::make_shared<compilerLogic::GlobalBlock>();
  scope.push(globalBlock);
  if (argc < 3) {std::cerr<<"Not enough args. Call with input name and output name";}
  FILE* input_file = fopen(argv[1], "r");
  if (input_file == NULL) {
    std::cerr<<"Could not open input file: " + std::string(argv[1])<<std::endl;
    return EXIT_FAILURE;
  }
  yyin = input_file;
  yyparse();
  std::vector<compilerLogic::IntermidiateCode> result =
                             globalBlock->parseIntermidiate();
  std::cout<<("Inter length: " + std::to_string(result.size()))<<std::endl;
  compilerLogic::IntermidiateParser interParser{};
  auto parsed = interParser.parseIntermidiateCode(result);
  std::ofstream outputFile{argv[2]};
  if (!outputFile.good()) {
    std::cerr<<"Could not open output file"<<std::endl;
    return EXIT_FAILURE;
  }
  for (auto line : parsed) {
    outputFile<<line<<"\n";
  }
  outputFile.close();
  fclose(input_file);
}