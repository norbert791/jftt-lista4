#include <memory>
#include <Variable.hpp>
#include <stdexcept>
#include <IntermidiateCode.hpp>

namespace compilerLogic {
  int64_t IntermidiateCode::availableLabelId = 1;


  IntermidiateCode pickRefVar(EInstruction type, std::shared_ptr<Variable> var) {
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
        throw std::domain_error("Critical error in pickRefFun class");
      }
      return EInstruction::HALT;
    };

    return {addISuffix(type, var), EParameterType::VAR_ID, var->getId()};
  }
}

