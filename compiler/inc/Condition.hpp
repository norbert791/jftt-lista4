#ifndef CONDITION_HPP
#define CONDITION_HPP

#include <memory>
#include <Variable.hpp>

namespace compilerLogic {
  enum class EComperator {
    EQUAL,
    GREATER,
    LESSER,
    GEQ,
    LEQ,
    DIFFERENT,
  };

  static inline EComperator negateCondition(EComperator cond) {
    switch(cond) {
      case EComperator::EQUAL: return EComperator::DIFFERENT;
      case EComperator::DIFFERENT: return EComperator::EQUAL;
      case EComperator::GREATER: return EComperator::LEQ;
      case EComperator::LESSER: return EComperator::GEQ;
      case EComperator::LEQ: return EComperator::GREATER;
      case EComperator::GEQ: return EComperator::LESSER;
      default:
        return EComperator::DIFFERENT;
    }
  }

  struct Condition {
    inline Condition(EComperator compType,
                     std::shared_ptr<Variable> left,
                     std::shared_ptr<Variable> right
              ) : compType{compType}, left{left}, right{right}{}
    inline Condition(){};
    EComperator compType;
    std::shared_ptr<Variable> left = nullptr;
    std::shared_ptr<Variable> right = nullptr;
  };
}

#endif //CONDITION_HPP
