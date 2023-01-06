#include <Procedure.hpp>
#include <memory>
#include <IntermidiateCode.hpp>

namespace compilerLogic {
  void Procedure::addCommand(std::shared_ptr<Command> command) {
    this->commands.push_back(command);
  }

  void Procedure::addBlock(std::shared_ptr<Block> block) {
    this->commands.push_back(block);
  }

  std::shared_ptr<ProcedureCall> Procedure::getCall(
    std::vector<std::shared_ptr<Variable>> args) {
    auto numOfArgs = args.size();
    auto numOfParams = this->parameters.size();
    if (numOfArgs != numOfParams) {
      throw std::logic_error("Incorrect number of arguments in call to: " + 
        this->getName() + " procedure. Expected: " + std::to_string(numOfParams) + ". " + 
        "Provided: " + std::to_string(numOfArgs) + ".");
    }
    auto result = std::make_shared<ProcedureCall>(
                                                  args,
                                                  this->parameters,
                                                  this->returnAdress,
                                                  this->entryLabel
                                                );
    return result;
  }

  std::vector<IntermidiateCode> Procedure::parseIntermidiate() {
    std::vector<IntermidiateCode> body{};
    //Add entry jumpLabel
    body.push_back(this->entryLabel);
    //Generate instruction body
    for (auto elem : this->commands) {
      auto temp = elem->parseIntermidiate();
      body.insert(body.end(), temp.begin(), temp.end());
    }
    //Add return jump
    IntermidiateCode jumpBack{
                                EInstruction::JUMPI,
                                EParameterType::VAR_ID,
                                this->returnAdress->getId()
                              };
    body.push_back(jumpBack);
    
    return body;
  }

  std::vector<IntermidiateCode> ProcedureCall::parseIntermidiate() {
    std::vector<IntermidiateCode> result;
    for (size_t i = 0; i < this->arguments.size(); i++) {
      auto arg = arguments.at(i);
      auto param = parameters.at(i);
      //Set accumulator to address of variable.
      if (arg->getType() == EVariableType::REFERENCE) {
        //Load value of reference (which is addres of certain variable)
        result.push_back({EInstruction::LOAD, EParameterType::VAR_ID, arg->getId()});
      } else {
        //Set acc to mem location of variable
        result.push_back({EInstruction::SET, EParameterType::REF_ID, arg->getId()});
      }
      //Store into parameter variable value of accumulator
      result.push_back({EInstruction::STORE, EParameterType::VAR_ID, param->getId()});
    }
    //Add return label
    IntermidiateCode returnLabel {
      EInstruction::LABEL,
      EParameterType::LABEL_ID,
      IntermidiateCode::availableLabelId++
    };
    //Store return label
    result.push_back({EInstruction::SET, EParameterType::LABEL_ID, returnLabel.value});
    result.push_back({
                      EInstruction::STORE,
                      EParameterType::VAR_ID,
                      this->returnAddress->getId()
                    });
    //Jump to procedure
    result.push_back({EInstruction::JUMP, EParameterType::LABEL_ID, this->entryLabel.value});
    //Add returnLabel After jump
    result.push_back(returnLabel);
    return result;
  }
}