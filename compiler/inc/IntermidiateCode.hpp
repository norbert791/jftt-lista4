#ifndef INTERMIDIATE_CODE_HPP
#define INTERMIDIATE_CODE_HPP

#include <cstdint>

namespace compilerLogic {
  enum class EInstruction {
    GET,
    PUT,
    LOAD,
    STORE,
    ADD,
    SUB,
    SET,
  };

  enum class EParameterType {
    VALUE, 
    VAR_ID,
    LABEL_ID, //Jump target
  };

  struct IntermidiateCode {
    EInstruction instrName;
    EParameterType parameterType;
    uint64_t value;
  };


}

#endif //INTER_CODE_HPP