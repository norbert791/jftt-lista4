#ifndef INTERMIDIATE_CODE_HPP
#define INTERMIDIATE_CODE_HPP

#include <string>
#include <cstdint>
#include <array>
#include <vector>
#include <memory>
#include <type_traits>
#include <Variable.hpp>

/**
 * Regarding intermidiate language semantics:
 * 
 * LOAD, STORE + VAR_ID => load, store to variable of given id
 * LOAD, STORE + REF_ID => convert to LOADI, STOREI + REF_ID
 * LOADI, STOREI + REF_ID/VAL_ID => LOADI, STOREI to var of given id
 * ADD, SUB + VAR_ID => add, sub with var of given id
 * ADD, SUB + REF_ID => convert to ADDI, SUBI + REF_ID
 * LOADI, STOREI + REF_ID/VAL_ID => LOADI, STOREI to var of given id
 * JUMPI + REF_ID/VAR_ID => JUMPI using var of given id
 * JUMP + LABEL_ID => Jump to lavel of given id
 * SET + VALUE => SET using passed value 
 * SET + REF_ID => SET using ADDRESS of given variable
 * 
 * Any other combinations of EInstruction and EParameterType
 * are undefined behaviour.
*/

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
    JUMP,
    JPOS,
    JZERO,
    JUMPI,
    ADDI,
    SUBI,
    LOADI,
    STOREI,
    HALT,
    HALF,
    INSTR_COUNT, // Number of enum items, must be the last one
    LABEL // Label is not part of the assembler so it can be after INSTR_COUNT
  };

  /**
   * Instruction not ending with 'I' (e.g ADD) should not have type REF_ID.
   * If they do, then it is hint to parser that respective instruction with 'I'
   * should be substitutet (e.g ADD -> ADDI). This implicit convertion is meant
   * to simplify arithmetic operations code.
  */
  enum class EParameterType {
    VALUE,  // Value
    VAR_ID, // Maps to memory location. It stores arithmetic value.
    REF_ID, // Maps to memory location. It stores address value.
    LABEL_ID, // Jump target
  };

  inline std::string instructionNameString(EInstruction instr) {
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
      "JUMP",
      "JPOS",
      "JZERO",
      "JUMPI",
      "ADDI",
      "SUBI",
      "LOADI",
      "STOREI",
      "HALT",
      "HALF"
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
    static int64_t availableLabelId;
  };

  IntermidiateCode pickRefVar(EInstruction type, std::shared_ptr<Variable> var);

  class ParsableToIntermidiate {
    public:
      virtual std::vector<IntermidiateCode> parseIntermidiate() = 0;
  };

}

#endif //INTER_CODE_HPP