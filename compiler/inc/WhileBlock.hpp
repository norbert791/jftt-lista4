#ifndef WHILE_BLOCK_HPP
#define WHILE_BLOCK_HPP

#include "ConditionalBlock.hpp"

namespace compilerLogic {
  enum class EWhileType {
    NORMAL,
    UNTIL
  };
  
  class WhileBlock : public ConditionalBlock {
    public:
      inline WhileBlock(std::vector<std::shared_ptr<Identifier>> ids, EWhileType type) : type{type} {
        this->availableIdentifiers = ids;
      }
      virtual std::vector<IntermidiateCode> parseIntermidiate() override;
      virtual void addCommand(std::shared_ptr<Command> command) override;
      virtual void addBlock(std::shared_ptr<Block> block) override;
      virtual void addCondition(Condition cond);
      virtual ~WhileBlock() = default;
    private:
      EWhileType type;
      Condition cond{};
      std::vector<std::shared_ptr<ParsableToIntermidiate>> commands;
  };
}



#endif //WHILE_BLOCK_HPP