#include <iostream>
#include <WhileBlock.hpp>
#include <IntermidiateCode.hpp>

namespace compilerLogic {
  void WhileBlock::addCommand(std::shared_ptr<Command> command) {
    this->commands.push_back(command);
  }

  void WhileBlock::addBlock(std::shared_ptr<Block> block) {
    this->commands.push_back(block);
  }

  void WhileBlock::addCondition(Condition cond) {
    this->cond = cond;
  }

  std::vector<IntermidiateCode> WhileBlock::parseIntermidiate() {
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
    if (this->type == EWhileType::UNTIL) {
      this->cond.compType = negateCondition(this->cond.compType);
    }
    switch (this->cond.compType)
    { //IF L >= R
      case EComperator::GEQ:
        //If 0 >= R - L then jump true
        result.push_back(pickRefVar(EInstruction::LOAD, cond.right));
        result.push_back(pickRefVar(EInstruction::SUB, cond.left));
        //result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.right->getId()});
        //result.push_back({EInstruction::SUB, EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::JZERO, EParameterType::LABEL_ID, trueJumpLabel.value});
        result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      //If L <= R
      case EComperator::LEQ:
        //If L - R <= 0 then jump true
        result.push_back(pickRefVar(EInstruction::LOAD, cond.left));
        result.push_back(pickRefVar(EInstruction::SUB, cond.right));
        //result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.left->getId()});
        //result.push_back({EInstruction::SUB, EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::JZERO, EParameterType::LABEL_ID, trueJumpLabel.value});
        result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      //If L > R
      case EComperator::GREATER:
        //If L - R > 0 then jump true
        result.push_back(pickRefVar(EInstruction::LOAD, cond.left));
        result.push_back(pickRefVar(EInstruction::SUB, cond.right));
        //result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.left->getId()});
        //result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, trueJumpLabel.value});
        result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      //If L < R
      case EComperator::LESSER:
      //If 0 < R - L then jump true
        result.push_back(pickRefVar(EInstruction::LOAD, cond.right));
        result.push_back(pickRefVar(EInstruction::SUB, cond.left));
        //result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.right->getId()});
        //result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, trueJumpLabel.value});
        result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      //If L == R
      case EComperator::EQUAL:
        //If L >= R and L <= R <==> If L < R or L > R then false
        //If 0 < R - L then jmp false
        result.push_back(pickRefVar(EInstruction::LOAD, cond.right));
        result.push_back(pickRefVar(EInstruction::SUB, cond.left));
        //result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.right->getId()});
        //result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, falseJumpLabel.value});
        //If L - R > 0 then jump false
        result.push_back(pickRefVar(EInstruction::LOAD, cond.left));
        result.push_back(pickRefVar(EInstruction::SUB, cond.right));
        //result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.left->getId()});
        //result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      case EComperator::DIFFERENT:
        //If L < R or L > R then true
        //If 0 < R - L then jmp true
        result.push_back(pickRefVar(EInstruction::LOAD, cond.right));
        result.push_back(pickRefVar(EInstruction::SUB, cond.left));
        //result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.right->getId()});
        //result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.left->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, trueJumpLabel.value});
        //If L - R > 0 then jump true
        result.push_back(pickRefVar(EInstruction::LOAD, cond.left));
        result.push_back(pickRefVar(EInstruction::SUB, cond.right));
        //result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, cond.left->getId()});
        //result.push_back({EInstruction::SUB,  EParameterType::VAR_ID, cond.right->getId()});
        result.push_back({EInstruction::JPOS, EParameterType::LABEL_ID, trueJumpLabel.value});
        result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, falseJumpLabel.value});
        break;
      default:
        break;

    }

    IntermidiateCode entryLabel {
                                  EInstruction::LABEL,
                                  EParameterType::LABEL_ID,
                                  IntermidiateCode::availableLabelId++
                                };

    result.insert(result.begin(), entryLabel);
    if (this->type == EWhileType::UNTIL) {
      std::cout<<"dupa\n";
      result.insert(result.begin(), {{EInstruction::JUMP, EParameterType::LABEL_ID, trueJumpLabel.value}});
    }
    //Generate if true code
    std::vector<IntermidiateCode> trueCode{};
    trueCode.push_back(trueJumpLabel);

    for (auto parsable : this->commands) {
      auto temp = parsable->parseIntermidiate();
      trueCode.insert(trueCode.end(), temp.begin(), temp.end());
    }

    result.insert(result.end(), trueCode.begin(), trueCode.end());
    result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, entryLabel.value});
    result.push_back(falseJumpLabel);
    
    
    
    
    
    
    /* else { //this->type == EWhileType::UNTIL
      std::vector<IntermidiateCode> prepended = {{EInstruction::JUMP, EParameterType::LABEL_ID, falseJumpLabel.value}, entryLabel};
      result.insert(result.begin(), prepended.begin(), prepended.end());
      //Generate if true code
      std::vector<IntermidiateCode> trueCode{};
      trueCode.push_back(falseJumpLabel);

      for (auto parsable : this->commands) {
        auto temp = parsable->parseIntermidiate();
        trueCode.insert(trueCode.end(), temp.begin(), temp.end());
      }

      result.insert(result.end(), trueCode.begin(), trueCode.end());
      result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, entryLabel.value});
      result.push_back(trueJumpLabel);
    }*/

    return result;
  }
}