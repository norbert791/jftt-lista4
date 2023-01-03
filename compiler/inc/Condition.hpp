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

  struct Condition {
    inline Condition(EComperator compType,
                     std::shared_ptr<Variable> left,
                     std::shared_ptr<Variable> right
              ) : compType{compType}, left{left}, right{right}{}
    EComperator compType;
    std::shared_ptr<Variable> left = nullptr;
    std::shared_ptr<Variable> right = nullptr;
  };
}

#endif //CONDITION_HPP
