#ifndef DIV_MOD_HPP
#define DIV_MOD_HPP

#include "Block.hpp"
#include "Identifier.hpp"
#include "Procedure.hpp"

/*
  Pseudokod w Pythonie
  def div(a : int, b : int):
  if b == 0:
    return 0, 0
  a_ = a
  b_ = b
  q = 0
  r = 0
  while a_ >= b_:
    tempB = b_
    tempQ = 1
    while a_ >= tempB:
      tempB = tempB + tempB
      tempQ = tempQ + tempQ
    tempB //= 2
    tempQ //= 2
    q += tempQ
    a_ = a_ - tempB
  r = a_
  return q, r
*/

namespace compilerLogic {
  class DivMod : public Block, public Identifier {
    public:
      DivMod(int64_t& id);
      std::vector<IntermidiateCode> parseIntermidiate();
      /**
       * c := a * b
      */
      std::shared_ptr<ProcedureCall> getDivCall(
                                              std::shared_ptr<Variable> a,
                                              std::shared_ptr<Variable> b,
                                              std::shared_ptr<Variable> c
                                            );
      std::shared_ptr<ProcedureCall> getModCall(
                                              std::shared_ptr<Variable> a,
                                              std::shared_ptr<Variable> b,
                                              std::shared_ptr<Variable> c
                                            );
      inline virtual std::string getName() {return this->name;};
      inline virtual int64_t getId() {return this->id;};
      inline virtual void addCommand(std::shared_ptr<Command> command) {};
      inline virtual void addBlock(std::shared_ptr<Block> block) {};
      inline virtual ~DivMod() = default;
    private:
      std::vector<IntermidiateCode> intermidiateCode;
      std::shared_ptr<Variable> left= nullptr; //Ref a
      std::shared_ptr<Variable> right = nullptr; //Ref b
      std::shared_ptr<Variable> quotient = nullptr; //Ref q
      std::shared_ptr<Variable> remainder = nullptr; //Ref r
      std::shared_ptr<Variable> rightCopy = nullptr; //Var b_
      std::shared_ptr<Variable> tempB = nullptr;
      std::shared_ptr<Variable> tempQ = nullptr;
      std::shared_ptr<Variable> returnJump = nullptr;
      std::shared_ptr<Variable> discarded = nullptr;
      IntermidiateCode entryLabel;
      int64_t id;
      std::string name;
      void generateIntermidiateCode();
  };
}

#endif //DIV_MOD_HPP