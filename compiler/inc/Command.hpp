#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "IntermidiateCode.hpp"

namespace compilerLogic {
  class Command {
    public:
      virtual IntermidiateCode parseInter() = 0;
      virtual ~Command() = default;
  };
}

#endif //COMMAND_HPP