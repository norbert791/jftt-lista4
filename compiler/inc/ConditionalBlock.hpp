#ifndef CONDITIONAL_BLOCK_HPP
#define CONDITIONAL_BLOCK_HPP

#include "Block.hpp"
#include "Condition.hpp"

namespace compilerLogic {
  class ConditionalBlock : public Block {
    public:
      virtual std::vector<IntermidiateCode> parseIntermidiate() override = 0;
      virtual void addCommand(std::shared_ptr<Command> command) override = 0;
      virtual void addBlock(std::shared_ptr<Block> block) override = 0;
      virtual void addCondition(Condition cond) = 0;
      virtual ~ConditionalBlock() = default;
  };
}

#endif //CONDITIONAL_BLOCK_HPP