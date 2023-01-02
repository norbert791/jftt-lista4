#include<stdexcept>
#include "IntermidiateParser.hpp"

namespace compilerLogic {
  std::vector<std::string> IntermidiateParser::parseIntermidiateCode(std::vector<IntermidiateCode> code) {
    std::vector<std::string> result{code.size()};

    for (auto instr : code) {
      std::string newInstr = instructionNameString(instr.instrName);
      int64_t param = -1;
      switch (instr.parameterType) {
        case EParameterType::VAR_ID: {
            const auto element = this->addressMap.find(instr.value);
            if (element == addressMap.end()) {
              addressMap.insert(std::make_pair(instr.value, this->mem_pool));
              param = mem_pool;
              this->mem_pool++;
            } else {
              param = element->second;
            }
            break;
        }
        case EParameterType::VALUE:
          param = instr.value;
          break;
        default:
          throw std::logic_error("Param type not implemented");
      }
      newInstr += " " + std::to_string(param);
      result.emplace_back(newInstr);
    }

    return result;
  }
}