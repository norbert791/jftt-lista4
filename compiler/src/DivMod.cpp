#include <DivMod.hpp>


namespace compilerLogic {
  DivMod::DivMod(int64_t& id) : id{id++},
                                name{"@mult"},
                                entryLabel {
                                            EInstruction::LABEL,
                                            EParameterType::LABEL_ID,
                                            IntermidiateCode::availableLabelId++
                                            } {
    this->left = std::make_shared<Variable>(id++, "@left", EVariableType::REFERENCE);
    this->right = std::make_shared<Variable>(id++, "@right", EVariableType::REFERENCE);
    this->rightCopy = std::make_shared<Variable>(id++, "@rightCopy", EVariableType::VALUE);
    this->tempQ = std::make_shared<Variable>(id++, "@tempQ", EVariableType::VALUE);
    this->tempB = std::make_shared<Variable>(id++, "@tempB", EVariableType::VALUE);
    this->quotient = std::make_shared<Variable>(id++, "@quotient", EVariableType::REFERENCE);
    this->remainder = std::make_shared<Variable>(id++, "@remainder", EVariableType::REFERENCE);
    this->returnJump = std::make_shared<Variable>(id++, "@returnJump", EVariableType::VALUE);
    this->discarded = std::make_shared<Variable>(id++, "@returnJump", EVariableType::VALUE);
    generateIntermidiateCode();
  }

  void DivMod::generateIntermidiateCode() {
    IntermidiateCode outerWhileBegin =
      {EInstruction::LABEL, EParameterType::LABEL_ID, IntermidiateCode::availableLabelId++};
    IntermidiateCode outerWhileEnd =
      {EInstruction::LABEL, EParameterType::LABEL_ID, IntermidiateCode::availableLabelId++};
    IntermidiateCode innerWhileBegin =
      {EInstruction::LABEL, EParameterType::LABEL_ID, IntermidiateCode::availableLabelId++};
    IntermidiateCode innerWhileEnd =
      {EInstruction::LABEL, EParameterType::LABEL_ID, IntermidiateCode::availableLabelId++};
    IntermidiateCode ifFalse =
      {EInstruction::LABEL, EParameterType::LABEL_ID, IntermidiateCode::availableLabelId++};
    intermidiateCode = {
      entryLabel,
      {EInstruction::LOADI, EParameterType::VAR_ID,  this->right->getId()},
      {EInstruction::JPOS, EParameterType::LABEL_ID, ifFalse.value},
      {EInstruction::SET, EParameterType::VALUE, 0},
      {EInstruction::STOREI, EParameterType::VAR_ID, this->quotient->getId()},
      {EInstruction::STOREI, EParameterType::VAR_ID, this->remainder->getId()},
      {EInstruction::JUMPI, EParameterType::VAR_ID,  this->returnJump->getId()},
      ifFalse,
      {EInstruction::STORE, EParameterType::VAR_ID,  this->rightCopy->getId()}, //acc == *right
      {EInstruction::LOADI, EParameterType::VAR_ID,  this->left->getId()},
      {EInstruction::STOREI, EParameterType::VAR_ID,  this->remainder->getId()},
      {EInstruction::SET, EParameterType::VALUE, 0},
      {EInstruction::STOREI, EParameterType::VAR_ID, this->quotient->getId()},
      outerWhileBegin,
      {EInstruction::LOAD, EParameterType::VAR_ID, this->rightCopy->getId()},
      {EInstruction::SUBI, EParameterType::VAR_ID, this->remainder->getId()},
      {EInstruction::JPOS, EParameterType::LABEL_ID, outerWhileEnd.value},
      {EInstruction::LOAD, EParameterType::VAR_ID, this->rightCopy->getId()},
      {EInstruction::STORE, EParameterType::VAR_ID, this->tempB->getId()},
      {EInstruction::SET, EParameterType::VALUE, 1},
      {EInstruction::STORE, EParameterType::VAR_ID, this->tempQ->getId()},
      innerWhileBegin,
      {EInstruction::LOAD, EParameterType::VAR_ID, this->tempB->getId()},
      {EInstruction::SUBI, EParameterType::VAR_ID, this->remainder->getId()},
      {EInstruction::JPOS, EParameterType::LABEL_ID, innerWhileEnd.value},
      {EInstruction::LOAD, EParameterType::VAR_ID, this->tempB->getId()},
      {EInstruction::ADD, EParameterType::VAR_ID, this->tempB->getId()},
      {EInstruction::STORE, EParameterType::VAR_ID, this->tempB->getId()},
      {EInstruction::LOAD, EParameterType::VAR_ID, this->tempQ->getId()},
      {EInstruction::ADD, EParameterType::VAR_ID, this->tempQ->getId()},
      {EInstruction::STORE, EParameterType::VAR_ID, this->tempQ->getId()},
      {EInstruction::JUMP, EParameterType::LABEL_ID, innerWhileBegin.value},
      innerWhileEnd,
      {EInstruction::LOAD, EParameterType::VAR_ID, this->tempB->getId()},
      {EInstruction::HALF, EParameterType::VALUE, -1},
      {EInstruction::STORE, EParameterType::VAR_ID, this->tempB->getId()},
      {EInstruction::LOAD, EParameterType::VAR_ID, this->tempQ->getId()},
      {EInstruction::HALF, EParameterType::VALUE, -1},
      {EInstruction::STORE, EParameterType::VAR_ID, this->tempQ->getId()},
      {EInstruction::LOADI, EParameterType::VAR_ID, this->quotient->getId()},
      {EInstruction::ADD, EParameterType::VAR_ID, this->tempQ->getId()},
      {EInstruction::STOREI, EParameterType::VAR_ID, this->quotient->getId()},
      {EInstruction::LOADI, EParameterType::VAR_ID, this->remainder->getId()},
      {EInstruction::SUB, EParameterType::VAR_ID, this->tempB->getId()},
      {EInstruction::STOREI, EParameterType::VAR_ID, this->remainder->getId()},
      {EInstruction::JUMP, EParameterType::LABEL_ID, outerWhileBegin.value},
      outerWhileEnd,
      {EInstruction::JUMPI, EParameterType::VAR_ID, this->returnJump->getId()}
    };
  }

  std::vector<IntermidiateCode> DivMod::parseIntermidiate() {
    return this->intermidiateCode;
  }

  std::shared_ptr<ProcedureCall> DivMod::getDivCall(
  std::shared_ptr<Variable> a,
  std::shared_ptr<Variable> b,
  std::shared_ptr<Variable> c) {

  std::vector<std::shared_ptr<Variable>> params{left, right, quotient, remainder};
  std::vector<std::shared_ptr<Variable>> args{a, b, c, this->discarded};

  return std::make_shared<ProcedureCall>(args, params, this->returnJump, this->entryLabel);
  }

  std::shared_ptr<ProcedureCall> DivMod::getModCall(
  std::shared_ptr<Variable> a,
  std::shared_ptr<Variable> b,
  std::shared_ptr<Variable> c) {

  std::vector<std::shared_ptr<Variable>> params{left, right, quotient, remainder};
  std::vector<std::shared_ptr<Variable>> args{a, b, this->discarded, c};

  return std::make_shared<ProcedureCall>(args, params, this->returnJump, this->entryLabel);
  }
}