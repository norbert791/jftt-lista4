#include <stdexcept>
#include "Assignment.hpp"

namespace compilerLogic{
  std::vector<IntermidiateCode> Assignment::parseIntermidiate() {
    std::vector<IntermidiateCode> result{};

    auto choseLOAD = [](std::shared_ptr<Variable> var) -> IntermidiateCode {
      if (var->getType() == EVariableType::VALUE) {
        return {EInstruction::LOAD, EParameterType::VAR_ID, var->getId()};
      } else {
        return {EInstruction::LOADI, EParameterType::VAR_ID, var->getId()};
      }
    };

    //If necessary, convert instruction from value base to address based
    auto addISuffix = [](EInstruction type, std::shared_ptr<Variable> var)-> EInstruction {
      if (var->getType() == EVariableType::VALUE) {
        return type;
      }
      switch (type)
      {
      case EInstruction::ADD: return EInstruction::ADDI;
      case EInstruction::SUB: return EInstruction::SUBI;
      case EInstruction::STORE: return EInstruction::STOREI;
      case EInstruction::LOAD: return EInstruction::LOADI;
      default:
        throw std::domain_error("Critical error in Assignment class");
      }
    };

    //Pick LOADI or LOAD depending on var type
    auto myLoad = [addISuffix](std::shared_ptr<Variable> var) -> IntermidiateCode {
      auto type = addISuffix(EInstruction::LOAD, var);
      return {type, EParameterType::VAR_ID, var->getId()};
    };

    auto myStore = [addISuffix](std::shared_ptr<Variable> var) -> IntermidiateCode {
      auto type = addISuffix(EInstruction::STORE, var);
      return {type, EParameterType::VAR_ID, var->getId()};
    };

    auto myAdd = [addISuffix](std::shared_ptr<Variable> var) -> IntermidiateCode {
      auto type = addISuffix(EInstruction::ADD, var);
      return {type, EParameterType::VAR_ID, var->getId()};
    };

    auto mySub = [addISuffix](std::shared_ptr<Variable> var) -> IntermidiateCode {
      auto type = addISuffix(EInstruction::SUB, var);
      return {type, EParameterType::VAR_ID, var->getId()};
    };

    switch (this->operation)
    {

      case EOperator::SUM:
        result.push_back(myLoad(this->left));
        result.push_back(myAdd(this->right));
        result.push_back(myStore(this->target));
        break;
      case EOperator::SUB:
        result.push_back(myLoad(this->left));
        result.push_back(mySub(this->right));
        result.push_back(myStore(this->target));
        break;
      case EOperator::NONE:
        result.push_back(myLoad(this->left));
        result.push_back(myStore(this->target));
        break;
      default:
        throw std::logic_error("Unsoported operation");
      /*TODO: Code from before procedures were added
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
      */
    }
    return result;
  }
  
}