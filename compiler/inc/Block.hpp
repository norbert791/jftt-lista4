#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <memory>
#include "IntermidiateCode.hpp"
#include "Variable.hpp"
#include "Identifier.hpp"
#include "Command.hpp"

namespace compilerLogic {

  class Block {
    public:
      virtual IntermidiateCode parseIntermediate() = 0;
      virtual void addAvailableIdentifier(std::shared_ptr<Identifier> var) = 0;
      virtual void addCommand() = 0;
      virtual void addBlock() = 0;
      virtual ~Block() = default;
  };
}



#endif //BLOCK_HPP