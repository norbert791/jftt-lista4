#include <stdexcept>
#include "Assignment.hpp"

namespace compilerLogic{
  std::vector<IntermidiateCode> Assignment::parseIntermidiate() {
    std::vector<IntermidiateCode> result{};

    switch (this->operation)
    {
      case EOperator::SUM:
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, this->left->getId()});
        result.push_back({EInstruction::ADD, EParameterType::VAR_ID, this->right->getId()});
        result.push_back({EInstruction::STORE, EParameterType::VAR_ID, this->target->getId()});
        break;
      case EOperator::SUB:
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, this->left->getId()});
        result.push_back({EInstruction::SUB, EParameterType::VAR_ID, this->right->getId()});
        result.push_back({EInstruction::STORE, EParameterType::VAR_ID, this->target->getId()});
        break;
      case EOperator::NONE:
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, this->left->getId()});
        result.push_back({EInstruction::STORE, EParameterType::VAR_ID, this->target->getId()});
        break;
      default:
        throw std::logic_error("Unsoported operation");
    }
    return result;
  }
  
}