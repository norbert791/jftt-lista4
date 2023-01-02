#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>
#include "IntermidiateCode.hpp"

namespace compilerLogic {
  class Command : public ParsableToIntermidiate {
    public:
      virtual std::vector<IntermidiateCode> parseIntermidiate() = 0;
      virtual ~Command() = default;
  };
}

#endif //COMMAND_HPP