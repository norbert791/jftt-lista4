#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>
#include "Identifier.hpp"

namespace compilerLogic {
  enum class EVariableType {
    VALUE, //This variable is used for storing integer values
    REFERENCE //This variable is used for storing other vars' address
  };

  class Variable : public Identifier {
    public:
      inline Variable(int64_t id, std::string name) : id{id}, name{name} {};
      inline Variable(int64_t id, std::string name, EVariableType varType)
        : id{id}, name{name}, varType{varType} {};
      inline virtual std::string getName() override final {return name;};
      inline virtual int64_t getId() override final {return id;};
      inline virtual EVariableType getType() {return varType;}
      inline virtual void setType(EVariableType varType) {this->varType = varType;}
      inline virtual bool isInitialized() {return initialized;}
      inline virtual void initialize() {this->initialized = true;}
    private:
      int64_t id;
      std::string name;
      bool initialized = false;
      EVariableType varType = EVariableType::VALUE;
  };
}

#endif //VARIABLE_HPP