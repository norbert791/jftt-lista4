#include <Block.hpp>

namespace compilerLogic {
  std::vector<std::shared_ptr<Identifier>> Block::getAvailableIdentifiers() {
    return this->availableIdentifiers;
  }
}