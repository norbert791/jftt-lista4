#ifndef INTERMIDIATE_CODE_HPP
#define INTERMIDIATE_CODE_HPP

#include <string>
#include <cstdint>
#include <array>
#include <type_traits>

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
    INSTR_COUNT // Number of enum items, must be last one
  };

  enum class EParameterType {
    VALUE,  // Value
    VAR_ID, // Maps to memory location
    LABEL_ID, // Jump target
  };

  inline std::string instructionNameString(EInstruction instr) {
    //TODO: Wyłuskaj 7 z enuma
    constexpr size_t strArraySize = static_cast<std::underlying_type_t<
                        EInstruction>>(EInstruction::INSTR_COUNT);
    static const std::array<std::string, strArraySize> strArray = {
      "GET",
      "PUT",
      "LOAD",
      "STORE", 
      "ADD",
      "SUB",
      "SET",
    };

    const size_t index = static_cast<std::underlying_type_t<
                        EInstruction>>(instr);
    return strArray[index];
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

  class ParsableToIntermidiate {
    public:
      virtual std::vector<IntermidiateCode> parseIntermidiate() = 0;
  };

}

#endif //INTER_CODE_HPP