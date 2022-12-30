#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include "Variable.hpp"
#include "Command.hpp"

namespace compilerLogic {
  enum class EOperator {
    SUM,
    SUB,
    PROD,
    DIV,
    MOD,
  };
  
  class Assignment : Command {
    public:
      Assignment(EOperator operation, Variable left, Variable irght) :
        left{left}, right{right}, operation{operation} {};
      virtual IntermidiateCode parseInter() override final;
    private:
      Variable left;
      Variable right;
      EOperator operation;
  };
}


#endif //ASSIGNMENT_HPP