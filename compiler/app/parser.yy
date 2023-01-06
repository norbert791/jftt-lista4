%{
//Standard headers
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include <stack>
#include <memory>
#include <iostream>
#include <fstream>
//Proj headers
#include <GlobalBlock.hpp>
#include <MainBlock.hpp>
#include <Block.hpp>
#include <Command.hpp>
#include <Assignment.hpp>
#include <IntermidiateParser.hpp>
#include <IntermidiateCode.hpp>
#include <IOCommand.hpp>
#include <IfBlock.hpp>
#include <Condition.hpp>
#include <WhileBlock.hpp>
#include <Procedure.hpp>

#define YYERROR_VERBOSE 1

extern int yylineno;
extern int yylex();
extern void yyerror (char const *);
extern FILE* yyin;

static compilerLogic::Expression exprVar;
static std::shared_ptr<compilerLogic::GlobalBlock> globalBlock = nullptr;
static std::stack<std::shared_ptr<compilerLogic::Block>> scope{};
static std::array<std::string, 2> varStack = {"", ""};
static std::vector<std::shared_ptr<compilerLogic::Variable>> procedureArgs{};
static size_t varStackIndex = 0;
static int64_t id = 1;

static inline std::shared_ptr<compilerLogic::Variable> getValue(std::string varName) {
  if (varName[0] == '#') {
    return globalBlock->getLiteral(varName, id);
  } else {
    auto temp = scope.top()->findIdentifier(varName);
    return std::dynamic_pointer_cast<compilerLogic::Variable>(temp);
  }
}

static void expressionBuilderWrapper(std::string symbol, std::string var1, std::string var2) {
  try {
    std::shared_ptr<compilerLogic::Variable> left = nullptr, right = nullptr;
    left = getValue(var1);
    if (symbol != "none"){
      right = getValue(var2);
    }
    exprVar = compilerLogic::expressionBuilder(symbol, left, right);
  } catch(const std::out_of_range& e) {
    std::cerr<<e.what()<<std::endl;
    exit(EXIT_FAILURE);
  } catch(const std::logic_error& e) {
    std::cerr<<"Symbol " + symbol + " is not a variable (line: " + std::to_string(yylineno) + " )"<<std::endl;
    exit(EXIT_FAILURE);
  }
}

static void addIf() {
  auto temp = std::make_shared<compilerLogic::IfBlock>(scope.top()->getAvailableIdentifiers());
  scope.top()->addBlock(temp);
  scope.push(temp);
}

static void addElse() {
  std::shared_ptr<compilerLogic::IfBlock> casted =
    std::dynamic_pointer_cast<compilerLogic::IfBlock>(scope.top());
  casted->addElseSegment();
}

static void addCondition(compilerLogic::EComperator condType) {
  auto temp = scope.top();
  std::shared_ptr<compilerLogic::ConditionalBlock> casted =
    std::dynamic_pointer_cast<compilerLogic::ConditionalBlock>(scope.top());
  auto left = getValue(varStack.at(0));
  auto right = getValue(varStack.at(1));
  varStackIndex = 0;
  compilerLogic::Condition cond{condType, left, right};
  casted->addCondition(cond);
}

static void addWhile(compilerLogic::EWhileType type) {
  auto loop = std::make_shared<compilerLogic::WhileBlock>(scope.top()->getAvailableIdentifiers(), type);
  scope.top()->addBlock(loop);
  scope.push(loop);
}

static void addCallArg(std::string varName) {
  auto variable = scope.top()->findIdentifier(varName);
  auto castedVar = std::dynamic_pointer_cast<compilerLogic::Variable>(variable);
  procedureArgs.push_back(castedVar);
}

static void addCall(std::string funName) {
  try {
    auto fun = globalBlock->findIdentifier(funName);
    auto castedFun = std::dynamic_pointer_cast<compilerLogic::Procedure>(fun);
    auto callCommand = castedFun->getCall(procedureArgs);
    scope.top()->addCommand(callCommand);
    procedureArgs.clear();
  } catch (std::logic_error& e) {
    std::cerr<<"Call error\n";
    std::cerr<<e.what()<<std::endl;
    exit(EXIT_FAILURE);
  }
}

static inline void addProcedure() {
  scope.push(std::make_shared<compilerLogic::Procedure>(
    globalBlock->getAvailableIdentifiers(), id));
  globalBlock->addBlock(scope.top());
}

static inline void addParameter(std::string parameterName) {
  try {
    auto fun = scope.top();
    auto castedFun = std::dynamic_pointer_cast<compilerLogic::Procedure>(fun);
    auto paramRef = std::make_shared<compilerLogic::Variable>(
      id++,
      parameterName,
      compilerLogic::EVariableType::REFERENCE
    );
    castedFun->addParameter(paramRef);
  } catch(std::logic_error& e) {
    std::cerr<<e.what()<<std::endl;
    exit(EXIT_FAILURE);
  }
}

static inline void addMain() {
  auto temp = std::make_shared<compilerLogic::MainBlock>(
    globalBlock->getAvailableIdentifiers());
  scope.push(temp);
  globalBlock->addMain(temp);
}

static inline void setProcName(std::string name) {
  auto fun = scope.top();
    auto castedFun = std::dynamic_pointer_cast<compilerLogic::Procedure>(fun);
  castedFun->setName(name);
  if (globalBlock->verifyIdentifier(name)) {
    std::cerr<<("Function redeclaration: " + name)<<std::endl;
    exit(EXIT_FAILURE);
  } else {
    globalBlock->addAvailableIdentifier(castedFun);
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

procedures: procedures PROCEDURE {addProcedure();} proc_def {}
| {}

proc_def: proc_head IS VAR declarations MY_BEGIN commands END {scope.pop();}
| proc_head IS MY_BEGIN commands END {scope.pop();}

main: PROGRAM IS {addMain();} VAR declarations MY_BEGIN commands END {scope.pop();}
| PROGRAM IS {addMain();} MY_BEGIN commands END {scope.pop();}

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
                                                      std::cerr<<"Symbol " + $1.str + " is not a variable (line: " + std::to_string(yylineno) + " )\n";
                                                      exit(EXIT_FAILURE);
                                                    }
                                                  }
 | IF {addIf();} if_scope {}
 | WHILE {addWhile(compilerLogic::EWhileType::NORMAL);} condition DO commands ENDWHILE        {scope.pop();}
 | REPEAT {addWhile(compilerLogic::EWhileType::UNTIL);} commands UNTIL condition ";"          {scope.pop();}
 | proc_call ";"                                  {}
 | READ identifier ";"                            {
                                                    try {
                                                      auto temp = getValue($2.str);
                                                      scope.top()->addCommand(
                                                        std::make_shared<compilerLogic::IOCommand>(
                                                        compilerLogic::EIOType::INPUT, temp));
                                                    } catch(const std::logic_error& r) {
                                                      std::cerr<<"Symbol " + $2.str + " is not a variable (line: " + std::to_string(yylineno) + " )\n";
                                                      exit(EXIT_FAILURE);
                                                    }
                                                  }
 | WRITE value ";"                                {
                                                    try {
                                                      auto temp = getValue(varStack.at(0));
                                                      varStackIndex = 0;
                                                      scope.top()->addCommand(
                                                        std::make_shared<compilerLogic::IOCommand>(
                                                        compilerLogic::EIOType::OUTPUT, temp));
                                                    } catch(const std::logic_error& r) {
                                                      std::cerr<<"Symbol " + varStack.at(0) + " is not a variable (line: " + std::to_string(yylineno) + " )"<<std::endl;
                                                      exit(EXIT_FAILURE);
                                                    }
                                                  }
 if_scope: condition THEN commands ELSE {addElse();} commands ENDIF {scope.pop();}
 | condition THEN commands ENDIF                                    {scope.pop();}
 proc_call: identifier "(" funArguments ")" {addCall($1.str);}
 proc_head: identifier "(" funParams ")" {setProcName($1.str);}

funParams: funParams "," identifier {addParameter($3.str);}
| identifier {addParameter($1.str);}

 funArguments: funArguments "," identifier {addCallArg($3.str);}
 | identifier                              {addCallArg($1.str);}

 declarations: declarations "," identifier {
                                            if (scope.top()->verifyIdentifier($3.str)) {
                                              std::cerr<<("Variable redeclaration: " + $3.str)<<std::endl;
                                            } else {
                                              auto newVar = std::make_shared<compilerLogic::Variable>(id++, $3.str);
                                              scope.top()->addAvailableIdentifier(newVar);
                                            }
                                          }
 | identifier {
                if (scope.top()->verifyIdentifier($1.str)) {
                  std::cerr<<("Variable redeclaration: " + $1.str)<<std::endl;
                } else {
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

 condition: value "=" value {addCondition(compilerLogic::EComperator::EQUAL);}
 | value "!=" value         {addCondition(compilerLogic::EComperator::DIFFERENT);}
 | value ">" value          {addCondition(compilerLogic::EComperator::GREATER);}
 | value "<" value          {addCondition(compilerLogic::EComperator::LESSER);}
 | value ">=" value         {addCondition(compilerLogic::EComperator::GEQ);}
 | value "<=" value         {addCondition(compilerLogic::EComperator::LEQ);}

 value: num   {varStack.at(varStackIndex++) = "#" + std::to_string($1.val);}
 | identifier {varStack.at(varStackIndex++) = $1.str;}
%%

void yyerror(const char msg[]) {
  fprintf(stderr,"error: %s in line %d\n", msg, yylineno - 1);
  fflush(stdin);
  std::exit(EXIT_FAILURE);
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
  interParser.showIntermidiateCode(result);
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