#include "GlobalBlock.hpp"

#include <stdexcept>
#include <memory>
#include <sstream>
#include <iostream>

namespace compilerLogic {
  void GlobalBlock::addBlock(std::shared_ptr<Block> block) {
    this->blocks.push_back(block);
  }

  void GlobalBlock::addCommand(std::shared_ptr<Command> command) {
    throw std::logic_error("Command may not be added to global block");
  }

  std::vector<IntermidiateCode> GlobalBlock::parseIntermidiate() {
    std::vector<IntermidiateCode> result{};
    //Initialize literals
    for (auto it : literals) {
      std::stringstream stream;
      stream << it->getName().substr(1);
      int64_t temp;
      stream >> temp;
      result.push_back({EInstruction::SET, EParameterType::VALUE, temp});
      result.push_back({EInstruction::STORE, EParameterType::VAR_ID, it->getId()});
    }
    //Add main
    auto mainCode = this->mainBlock->parseIntermidiate();
    result.insert(result.end(), mainCode.begin(), mainCode.end());
    //Add other blocks
    for (auto subBlock : this->blocks) {
      auto temp = subBlock->parseIntermidiate();
      result.insert(result.end(), temp.begin(), temp.end());
    }

    return result;
  }

  std::shared_ptr<Variable> GlobalBlock::getLiteral(std::string str, int64_t& id) {
    auto it = std::find_if(this->literals.begin(), this->literals.end(), [&str](auto x){return x->getName() == str;});
    std::shared_ptr<Variable> result = nullptr;
    if (it == this->literals.end()) {
      result = std::make_shared<Variable>(id++, str);
      result->initialize();
      this->literals.push_back(result);
    } else {
      result = *it;
    }

    return result;
  }
}
