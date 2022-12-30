#ifndef PROCEDURE_HPP
#define PROCEDURE_HPP

#include "IntermidiateCode.hpp"
#include "Block.hpp"

namespace compilerLogic {
  class Procedure : public Identifier, public Block {      
  };
}

#endif //PROCEDURE_HPP