#ifndef PROCEDURE_HPP
#define PROCEDURE_HPP

#include <iostream>
#include "Identifier.hpp"
#include "IntermidiateCode.hpp"
#include "Block.hpp"

namespace compilerLogic {
  class ProcedureCall : public Command {
    public:
      inline ProcedureCall(std::vector<std::shared_ptr<Variable>> arguments,
                           std::vector<std::shared_ptr<Variable>> parameters,
                           std::shared_ptr<Variable> returnAddress,
                           IntermidiateCode entryLabel)
                           : 
                           arguments{arguments}, parameters{parameters},
                           returnAddress{returnAddress}, entryLabel{entryLabel} {};
      virtual std::vector<IntermidiateCode> parseIntermidiate();
      virtual ~ProcedureCall() = default;
    private:
      std::vector<std::shared_ptr<Variable>> arguments{};
      std::vector<std::shared_ptr<Variable>> parameters{};
      //Store return address here
      std::shared_ptr<Variable> returnAddress;
      //label to procedure start
      IntermidiateCode entryLabel;
  };

  class Procedure : public Identifier, public Block {
    public:
      Procedure(std::vector<std::shared_ptr<Identifier>> availableIdentifiers,
                int64_t& id) : id{id++} {
       this->availableIdentifiers = availableIdentifiers;
       returnAdress = std::make_shared<Variable>(id++, "returnFrom" + name);
       this->availableIdentifiers.push_back(returnAdress);
      }
      virtual std::vector<IntermidiateCode> parseIntermidiate() override;
      virtual void addCommand(std::shared_ptr<Command> command) override final;
      virtual void addBlock(std::shared_ptr<Block> block) override final;
      virtual std::shared_ptr<ProcedureCall> getCall(std::vector<std::shared_ptr<Variable>> args);
      virtual std::vector<std::shared_ptr<Identifier>>
              getAvailableIdentifiers() override {return this->availableIdentifiers;}
      inline virtual std::string getName() override {return this->name;}
      inline virtual int64_t getId() override {return this->id;}
      inline virtual void addParameter(std::shared_ptr<Variable> reference) {
        this->availableIdentifiers.push_back(reference);
        this->parameters.push_back(reference);
      };
      inline virtual void setName(std::string name) {this->name = name;}
    private:
      std::vector<std::shared_ptr<Variable>> parameters = {};
      std::vector<std::shared_ptr<ParsableToIntermidiate>> commands = {};
      std::shared_ptr<Variable> returnAdress = nullptr;
      IntermidiateCode entryLabel{
                                  EInstruction::LABEL,
                                  EParameterType::LABEL_ID,
                                  IntermidiateCode::availableLabelId++
                                 };
      int64_t id;
      int64_t EntryLabelId = this->entryLabel.value;
      std::string name;
  };
}

#endif //PROCEDURE_HPP