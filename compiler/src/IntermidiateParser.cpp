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
    //Map variables to memory adresses
    {
      for (auto instr : code) {
        if (
            instr.parameterType == EParameterType::VAR_ID && 
            this->addressMap.find(instr.value) == addressMap.end()
           ){
            addressMap.insert(std::make_pair(instr.value, this->mem_pool++));
        }
      }
    }


    for (auto instr : code) {
      std::string newInstr = instructionNameString(instr.instrName);
      int64_t param = -1;
      switch (instr.parameterType) {
        case EParameterType::REF_ID:
          //Only SET requires additional handling
          if (instr.instrName == EInstruction::SET) {
            const auto element = this->addressMap.find(instr.value);
            param = element->second;
            break;
          }
          [[fallthrough]];
        case EParameterType::VAR_ID: {
            const auto element = this->addressMap.find(instr.value);
            param = element->second;
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
      newInstr += instr.instrName == EInstruction::HALT ||
        instr.instrName == EInstruction::HALF ?
        "" : " " + std::to_string(param);
      result.push_back(newInstr);
    }

    std::cout<<"Memory map:\n";
    for (auto p : this->addressMap) {
      std::cout<<p.first<<" -> "<<p.second<<"\n";
    }

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