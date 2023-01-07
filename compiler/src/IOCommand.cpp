#include "IntermidiateCode.hpp"
#include "IOCommand.hpp"

namespace compilerLogic{
  std::vector<IntermidiateCode> IOCommand::parseIntermidiate() {
    std::vector<IntermidiateCode> result;
    /*
      Perform different IO operation based of IO type.
      Also if variable is a reference, then it has to be uploaded to
      acc (mem 0) before printing.
    */
    if (this->type == EIOType::INPUT) {
      if (this->var->getType() == EVariableType::VALUE) {
        result.push_back({EInstruction::GET, EParameterType::VAR_ID, this->var->getId()});
      } else {
        result.push_back({EInstruction::GET, EParameterType::VALUE, 0});
        result.push_back({EInstruction::STOREI, EParameterType::VAR_ID, this->var->getId()});
      }
    } else {
      if (this->var->getType() == EVariableType::VALUE) {
        result.push_back({EInstruction::PUT, EParameterType::VAR_ID, this->var->getId()});
      } else {
        result.push_back({EInstruction::LOADI, EParameterType::VAR_ID, this->var->getId()});
        result.push_back({EInstruction::PUT, EParameterType::VALUE, 0});
      }
    }

    return result;
  }
}