#include <stdexcept>
#include <iostream>
#include "IntermidiateParser.hpp"

namespace compilerLogic {
  std::vector<std::string> IntermidiateParser::parseIntermidiateCode(std::vector<IntermidiateCode> code) {
    std::vector<std::string> result{};
    //Map labels to instructions pointer by them
    {
      int64_t instrNumber = 0;

      for (auto instr : code) {
        if (instr.instrName == EInstruction::LABEL) {
          this->labelMap.insert(std::make_pair(instr.value, instrNumber));
        } else {
          instrNumber++;
        }
      }
    }

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
        case EParameterType::LABEL_ID:
          if (instr.instrName == EInstruction::LABEL) {
            continue;
          } else {
            param = this->labelMap.at(instr.value);
          }
          break;
        case EParameterType::VALUE:
          param = instr.value;
          break;
        default:
          throw std::logic_error("Param type not implemented");
      }
      newInstr += " " + std::to_string(param);
      result.push_back(newInstr);
    }

    result.push_back("HALT");
    return result;
  }

  void IntermidiateParser::showIntermidiateCode(std::vector<IntermidiateCode> code) {

    for (auto instr : code) {
      if (instr.instrName == EInstruction::LABEL) {
        std::cout<<"LABEL";
      } else {
        std::cout<<instructionNameString(instr.instrName);
      }
      std::cout<<" "<<instr.value<<std::endl;
    }
  }
}