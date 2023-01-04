#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include <memory>
#include <iostream>
#include "Variable.hpp"
#include "Command.hpp"

namespace compilerLogic {
  enum class EOperator {
    SUM,
    SUB,
    PROD,
    DIV,
    MOD,
    NONE,
  };
  
  struct Expression {
    EOperator operation;
    std::shared_ptr<Variable> left;
    std::shared_ptr<Variable> right;
  };

  class Assignment : public Command {
    public:
      inline Assignment(EOperator operation, std::shared_ptr<Variable> target, std::shared_ptr<Variable> left, std::shared_ptr<Variable> right) :
        target{target}, left{left}, right{right}, operation{operation} {};
      inline Assignment(EOperator operation, std::shared_ptr<Variable> target, std::shared_ptr<Variable> left) : target{target}, left{left} {};
      inline Assignment(std::shared_ptr<Variable> target, Expression expr) :
        operation{expr.operation},
        left{expr.left},
        right{expr.right},
        target{target} {}
      virtual std::vector<IntermidiateCode> parseIntermidiate() override final;
    private:
      std::shared_ptr<Variable> target = nullptr;
      std::shared_ptr<Variable> left   = nullptr;
      std::shared_ptr<Variable> right  = nullptr;
      int64_t value = -1;
      EOperator operation;
  };

  static inline Expression expressionBuilder(std::string op,
                               std::shared_ptr<Variable> left,
                               std::shared_ptr<Variable> right) {
    Expression result{};
    result.left = left;
    result.right = right;

    switch (op[0]) {
      case '+':
        result.operation = EOperator::SUM;
        break;
      case '-':
        result.operation = EOperator::SUB;
        break;
      case '/':
        result.operation = EOperator::DIV;
        break;
      case '%':
        result.operation = EOperator::MOD;
        break;
      case '*':
        result.operation = EOperator::PROD;
        break;
      case 'n':
        result.operation = EOperator::NONE;
        break;
      default:
        throw std::logic_error("Operator not found");
    }
    return result;
  }

}


#endif //ASSIGNMENT_HPP