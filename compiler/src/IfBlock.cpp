#include <IfBlock.hpp>

namespace compilerLogic {
  void IfBlock::addCommand(std::shared_ptr<Command> command) {
    this->currentSegment.push_back(command);
  }

  void IfBlock::addBlock(std::shared_ptr<Block> block) {
    this->currentSegment.push_back(block);
  }

  std::vector<IntermidiateCode> IfBlock::parseIntermidiate() {
    
  }
}