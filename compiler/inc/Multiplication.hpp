#ifndef MULTIPLICATION_HPP
#define MULTIPLICATION_HPP

/*
PROCEDURE mult(a,b,c) IS
VAR acc, b_, a_ BEGIN
  acc := 0
  b_ := b;
  a_ := a;
  WHILE b_ > 0 DO
    mem_0 := b_;
    half;
    mem_0 := mem_0 + mem_0
    if mem_0 < b_ THEN
      acc := acc + a_;
    ENDIF
    a_ := a_ + a_;
    mem_0 := b_;
    half;
    b_ := = mem_0;
  ENDWHILE
    c := a
END
*/

#include "Procedure.hpp"

namespace compilerLogic {
  class Multiplication : public Block, public Identifier {
    public:
      Multiplication(int64_t& id);
      std::vector<IntermidiateCode> parseIntermidiate();
      /**
       * c := a * b
      */
      std::shared_ptr<ProcedureCall> getCall(
                                            std::shared_ptr<Variable> a,
                                            std::shared_ptr<Variable> b,
                                            std::shared_ptr<Variable> c
                                            );
      inline virtual std::string getName() {return this->name;};
      inline virtual int64_t getId() {return this->id;};
      inline virtual void addCommand(std::shared_ptr<Command> command) {};
      inline virtual void addBlock(std::shared_ptr<Block> block) {};
      inline virtual ~Multiplication() = default;
    private:
      std::vector<IntermidiateCode> intermidiateCode;
      std::shared_ptr<Variable> left= nullptr; //Ref a
      std::shared_ptr<Variable> right = nullptr; //Ref b
      std::shared_ptr<Variable> target = nullptr; //Ref c
      std::shared_ptr<Variable> leftCopy = nullptr; //Var a_
      std::shared_ptr<Variable> rightCopy = nullptr; //Var b_
      std::shared_ptr<Variable> acc = nullptr;
      std::shared_ptr<Variable> temp = nullptr;
      std::shared_ptr<Variable> returnJump = nullptr;
      IntermidiateCode entryLabel;
      int64_t id;
      std::string name;
      void generateIntermidiateCode();
  };
}


#endif //MULTIPLICATION_HPP
