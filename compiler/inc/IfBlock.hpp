#ifndef IF_BLOCK_HPP
#define IF_BLOCK_HPP

#include <vector>
#include <memory>
#include "Block.hpp"
#include "Condition.hpp"

namespace compilerLogic {
  class IfBlock : public Block{
    public:
      virtual std::vector<IntermidiateCode> parseIntermidiate() override;
      virtual void addCommand(std::shared_ptr<Command> command) override;
      virtual void addBlock(std::shared_ptr<Block> block) override;
      virtual ~IfBlock() = default;
      inline void addElseSegment() {}

    private:
      Condition cond;
      std::vector<std::shared_ptr<ParsableToIntermidiate>> ifTrue{};
      std::vector<std::shared_ptr<ParsableToIntermidiate>> ifFalse{};
      std::vector<std::shared_ptr<ParsableToIntermidiate>>& currentSegment = ifTrue;
  }
}

#endif //IF_BLOCK_HPP