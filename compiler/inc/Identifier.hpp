#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include <string>

namespace compilerLogic {
  
  class Identifier {
    public:
      virtual std::string getName() = 0;
      virtual size_t getId() = 0;
      virtual ~Identifier() = default;
  };
}

#endif //VARIABLE_HPP