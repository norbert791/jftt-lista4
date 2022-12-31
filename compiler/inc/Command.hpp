#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>
#include "IntermidiateCode.hpp"

namespace compilerLogic {
  class Command {
    public:
      virtual std::vector<IntermidiateCode> parseInter() = 0;
      virtual ~Command() = default;
  };
}

#endif //COMMAND_HPP