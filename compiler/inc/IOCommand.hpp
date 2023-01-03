#ifndef IOCOMMAND_HPP
#define IOCOMMAND_HPP

#include <memory>
#include "Command.hpp"
#include "Variable.hpp"

namespace compilerLogic {
  enum class EIOType {
    INPUT,
    OUTPUT,
  };

  class IOCommand : public Command {
    public:
      inline IOCommand(EIOType type, std::shared_ptr<compilerLogic::Variable> var) : type{type}, var{var}{};
      EIOType type;
      std::shared_ptr<compilerLogic::Variable> var;
      virtual std::vector<IntermidiateCode> parseIntermidiate();
  };
}



#endif //IOCOMMAND_HPP