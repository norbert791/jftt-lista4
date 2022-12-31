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
      Assignment(EOperator operation, Variable target, Variable left, Variable right) :
        target{target}, left{left}, right{right}, operation{operation} {};
      virtual std::vector<IntermidiateCode> parseInter() override final;
    private:
      Variable target;
      Variable left;
      Variable right;
      EOperator operation;
  };
}


#endif //ASSIGNMENT_HPP