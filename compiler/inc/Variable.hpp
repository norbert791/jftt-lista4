#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>
#include "Identifier.hpp"

namespace compilerLogic {
  class Variable : public Identifier {
    public:
      Variable(size_t id, std::string name) : id{id}, name{name} {};
      inline virtual std::string getName() override final {return name;};
      inline virtual size_t getId() override final {return id;};
    private:
      size_t id;
      std::string name;
      
  };
}

#endif