#include "GlobalBlock.hpp"

#include <stdexcept>

namespace compilerLogic {
  void GlobalBlock::addBlock(std::shared_ptr<Block> block) {
    this->blocks.push_back(block);
  }

  void GlobalBlock::addCommand(std::shared_ptr<Command> command) {
    throw std::logic_error("Command may not be added to global block");
  }

  std::vector<IntermidiateCode> GlobalBlock::parseIntermidiate() {
    std::vector<IntermidiateCode> result{};
    
    for (auto subBlock : this->blocks) {
      auto temp = subBlock->parseIntermidiate();
      result.insert(result.end(), temp.begin(), temp.end());
    }

    return result;
  }
}