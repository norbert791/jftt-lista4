#include <Multiplication.hpp>


namespace compilerLogic {
  Multiplication::Multiplication(int64_t& id) : id{id++},
                                                name{"@mult"},
                                                entryLabel {
                                                            EInstruction::LABEL,
                                                            EParameterType::LABEL_ID,
                                                            IntermidiateCode::availableLabelId++
                                                           } {
    this->left = std::make_shared<Variable>(id++, "@left", EVariableType::REFERENCE);
    this->right = std::make_shared<Variable>(id++, "@right", EVariableType::REFERENCE);
    this->leftCopy = std::make_shared<Variable>(id++, "@leftCopy", EVariableType::VALUE);
    this->rightCopy = std::make_shared<Variable>(id++, "@rightCopy", EVariableType::VALUE);
    this->acc = std::make_shared<Variable>(id++, "@acc", EVariableType::VALUE);
    this->temp = std::make_shared<Variable>(id++, "@temp", EVariableType::VALUE);
    this->target = std::make_shared<Variable>(id++, "@target", EVariableType::VALUE);
    this->returnJump = std::make_shared<Variable>(id++, "@returnJump", EVariableType::VALUE);
    generateIntermidiateCode();
  }


  void Multiplication::generateIntermidiateCode() {
    IntermidiateCode whileEntry =
      {EInstruction::LABEL, EParameterType::LABEL_ID, IntermidiateCode::availableLabelId++};
    IntermidiateCode whileEnd =
      {EInstruction::LABEL, EParameterType::LABEL_ID, IntermidiateCode::availableLabelId++};
    IntermidiateCode ifFalse =
      {EInstruction::LABEL, EParameterType::LABEL_ID, IntermidiateCode::availableLabelId++};
    intermidiateCode = {
      entryLabel,
      {EInstruction::SET, EParameterType::VALUE, 0},
      {EInstruction::STORE, EParameterType::VAR_ID, this->acc->getId()},
      {EInstruction::LOADI, EParameterType::VAR_ID, this->left->getId()},
      {EInstruction::STORE, EParameterType::VAR_ID, this->leftCopy->getId()},
      {EInstruction::LOADI, EParameterType::VAR_ID, this->right->getId()},
      {EInstruction::STORE, EParameterType::VAR_ID, this->rightCopy->getId()},
      {EInstruction::LABEL, EParameterType::LABEL_ID, IntermidiateCode::availableLabelId++},
      whileEntry,
      {EInstruction::LOAD, EParameterType::VAR_ID, this->rightCopy->getId()},
      {EInstruction::JZERO, EParameterType::LABEL_ID, whileEnd.value},
      {EInstruction::HALF, EParameterType::VALUE, -1},
      {EInstruction::ADD, EParameterType::VALUE, 0},
      {EInstruction::STORE, EParameterType::VAR_ID, this->temp->getId()},
      {EInstruction::LOAD, EParameterType::VAR_ID, this->rightCopy->getId()},
      {EInstruction::SUB, EParameterType::VAR_ID, this->temp->getId()},
      {EInstruction::JZERO, EParameterType::LABEL_ID, ifFalse.value},
      {EInstruction::LOAD, EParameterType::VAR_ID, this->acc->getId()},
      {EInstruction::ADD, EParameterType::VAR_ID, this->leftCopy->getId()},
      {EInstruction::STORE, EParameterType::VAR_ID, this->acc->getId()},
      ifFalse,
      {EInstruction::LOAD, EParameterType::VAR_ID, this->leftCopy->getId()},
      {EInstruction::ADD, EParameterType::VAR_ID, this->leftCopy->getId()},
      {EInstruction::STORE, EParameterType::VAR_ID, this->leftCopy->getId()},
      {EInstruction::LOAD, EParameterType::VAR_ID, this->rightCopy->getId()},
      {EInstruction::HALF, EParameterType::VALUE, -1},
      {EInstruction::STORE, EParameterType::VAR_ID, rightCopy->getId()},
      {EInstruction::JUMP, EParameterType::LABEL_ID, whileEntry.value},
      whileEnd,
      {EInstruction::LOAD, EParameterType::VAR_ID, acc->getId()},
      {EInstruction::STOREI, EParameterType::VAR_ID, target->getId()},
      {EInstruction::JUMPI, EParameterType::VAR_ID, returnJump->getId()},
    };
  }

  std::vector<IntermidiateCode> Multiplication::parseIntermidiate() {
    return this->intermidiateCode;
  }

  std::shared_ptr<ProcedureCall> Multiplication::getCall(
    std::shared_ptr<Variable> a,
    std::shared_ptr<Variable> b,
    std::shared_ptr<Variable> c) {

    std::vector<std::shared_ptr<Variable>> params{left, right, target};
    std::vector<std::shared_ptr<Variable>> args{a, b, c};

    return std::make_shared<ProcedureCall>(args, params, this->returnJump, this->entryLabel);
  }


}