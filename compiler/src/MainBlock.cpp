#include "MainBlock.hpp"

#include <stdexcept>

namespace compilerLogic {

  void MainBlock::addBlock(std::shared_ptr<Block> block) {
    this->stuctures.push_back(block);
  }

  void MainBlock::addCommand(std::shared_ptr<Command> command) {
    this->stuctures.push_back(command);
  }

  std::vector<IntermidiateCode> MainBlock::parseIntermidiate() {
    std::vector<IntermidiateCode> body{};

    for (auto subBlock : this->stuctures) {
      auto temp = subBlock->parseIntermidiate();
      body.insert(body.end(), temp.begin(), temp.end());
    }

    return body;
  }
}