#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <memory>
#include "InterCode.hpp"
#include "Variable.hpp"

namespace compilerLogic {

  class Block {
    public:
      virtual IntermidiateCode parseIntermediate() = 0;
      void addAvailableIdentifier(std::shared_ptr<Identifier> var);
      virtual void addCommand() = 0;
      virtual void addBlock() = 0;
  };
}



#endif //BLOCK_HPP