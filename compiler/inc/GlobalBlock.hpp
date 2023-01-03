#ifndef GLOBAL_BLOCK_HPP
#define GLOBAL_BLOCK_HPP

#include <vector> //todo: replace with hash_set
#include <string>
#include "Block.hpp"

namespace compilerLogic {
  class GlobalBlock : public Block {
    public:
      GlobalBlock() = default;
      virtual std::vector<IntermidiateCode> parseIntermidiate() override final;
      virtual void addCommand(std::shared_ptr<Command> command) override final;
      virtual void addBlock(std::shared_ptr<Block> block) override final;
      virtual ~GlobalBlock() = default;
      std::shared_ptr<Variable> getLiteral(std::string str, int64_t& id);

    private:
      std::vector<std::shared_ptr<Identifier>> availableIdentifiers{};
      std::vector<std::shared_ptr<Variable>> literals;
      std::vector<std::shared_ptr<Block>> blocks{};
  };
}

#endif //GLOBAL_BLOCK_HPP