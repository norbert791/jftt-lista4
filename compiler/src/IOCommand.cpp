#include "IntermidiateCode.hpp"
#include "IOCommand.hpp"

namespace compilerLogic{
  std::vector<IntermidiateCode> IOCommand::parseIntermidiate() {
    std::vector<IntermidiateCode> result;
    if (this->type == EIOType::INPUT) {
      result.push_back({EInstruction::GET, EParameterType::VAR_ID, this->var->getId()});
    } else {
      result.push_back({EInstruction::PUT, EParameterType::VAR_ID, this->var->getId()});
    }

    return result;
  }
}