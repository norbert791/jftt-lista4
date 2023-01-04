#include <iostream>
#include <IfBlock.hpp>

namespace compilerLogic {
  void IfBlock::addCommand(std::shared_ptr<Command> command) {
    this->currentSegment->push_back(command);
  }

  void IfBlock::addBlock(std::shared_ptr<Block> block) {
    this->currentSegment->push_back(block);
  }

  void IfBlock::addCondition(Condition cond) {
    this->cond = cond;
  }

  std::vector<IntermidiateCode> IfBlock::parseIntermidiate() {
    std::vector<IntermidiateCode> result{};
    IntermidiateCode trueJumpLabel {
                                      EInstruction::LABEL,
                                      EParameterType::LABEL_ID,
                                      IntermidiateCode::availableLabelId++
                                   };
    IntermidiateCode falseJumpLabel {
                                      EInstruction::LABEL,
                                      EParameterType::LABEL_ID,
                                      IntermidiateCode::availableLabelId++
                                    };

    switch (this->cond.compType)
    { //IF L >= R
      case EComperator::GEQ:
        //If 0 >= R - L then jump true
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::SUB, EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::JZERO, EParameterType::LABEL_ID, trueJumpLabel.value});
        result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      //If L <= R
      case EComperator::LEQ:
        //If L - R <= 0 then jump true
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::SUB, EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::JZERO, EParameterType::LABEL_ID, trueJumpLabel.value});
        result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      //If L > R
      case EComperator::GREATER:
        //If L - R > 0 then jump true
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, trueJumpLabel.value});
        result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      //If L < R
      case EComperator::LESSER:
      //If 0 < R - L then jump true
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, trueJumpLabel.value});
        result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      //If L == R
      case EComperator::EQUAL:
        //If L >= R and L <= R <==> If L < R or L > R then false
        //If 0 < R - L then jmp false
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, falseJumpLabel.value});
        //If L - R > 0 then jump false
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      case EComperator::DIFFERENT:
        //If L < R or L > R then true
        //If 0 < R - L then jmp false
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, trueJumpLabel.value});
        //If L - R > 0 then jump false
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, trueJumpLabel.value});
        result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      default:
        break;

    }

    //Generate if true code
    std::vector<IntermidiateCode> trueCode{};
    trueCode.push_back(trueJumpLabel);

    for (auto parsable : this->ifTrue) {
      auto temp = parsable->parseIntermidiate();
      trueCode.insert(trueCode.end(), temp.begin(), temp.end());
    }

    result.insert(result.end(), trueCode.begin(), trueCode.end());

    bool elsePresent = (this->currentSegment) == &(this->ifFalse);
    if (elsePresent) {
      std::vector<IntermidiateCode> falseCode{};
      falseCode.push_back(falseJumpLabel);
      for (auto parsable : this->ifFalse) {
        auto temp = parsable->parseIntermidiate();
        falseCode.insert(falseCode.end(), temp.begin(), temp.end());
      }
      //Add escape jump for if true block
      IntermidiateCode escapeJumpLabel {
                                    EInstruction::LABEL,
                                    EParameterType::LABEL_ID,
                                    IntermidiateCode::availableLabelId++
                                  };
      result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, escapeJumpLabel.value});
      falseCode.push_back(escapeJumpLabel);
      result.insert(result.end(), falseCode.begin(), falseCode.end());
    } else {
      result.push_back(falseJumpLabel);
    }

    return result;
  }
}