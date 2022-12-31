#ifndef INTERMIDIATE_CODE_HPP
#define INTERMIDIATE_CODE_HPP

#include <string>
#include <cstdint>
#include <array>

namespace compilerLogic {
  enum class EInstruction: size_t {
    //Do NOT assign values explicitely
    GET,
    PUT,
    LOAD,
    STORE,
    ADD,
    SUB,
    SET,
    INSTR_COUNT // Number of enum items
  };

  enum class EParameterType {
    VALUE,  // Value
    VAR_ID, // Maps to memory location
    LABEL_ID, // Jump target
  };

  inline std::string instructionString(EInstruction instr) {
    //TODO: Wyłuskaj 7 z enuma
    static std::array<std::string, 7> strArray = {
      "GET",
      "PUT",
      "LOAD",
      "STORE", 

    } 
  }

  struct IntermidiateCode {
    EInstruction instrName;
    EParameterType parameterType;
    int64_t value;
    inline IntermidiateCode(EInstruction instrName,
      EParameterType parameterType, int64_t value) : instrName{instrName},
                                                      parameterType{parameterType},
                                                      value{value} {};
  };


}

#endif //INTER_CODE_HPP