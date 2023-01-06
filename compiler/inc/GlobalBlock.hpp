#ifndef GLOBAL_BLOCK_HPP
#define GLOBAL_BLOCK_HPP

#include <vector> //todo: replace with hash_set
#include <string>
#include "Block.hpp"
#include "MainBlock.hpp"

namespace compilerLogic {
  class GlobalBlock : public Block {
    public:
      GlobalBlock() = default;
      virtual std::vector<IntermidiateCode> parseIntermidiate() override final;
      virtual void addCommand(std::shared_ptr<Command> command) override final;
      virtual void addBlock(std::shared_ptr<Block> block) override final;
      inline virtual void addMain(std::shared_ptr<MainBlock> block) {this->mainBlock = block;}
      virtual ~GlobalBlock() = default;
      std::shared_ptr<Variable> getLiteral(std::string str, int64_t& id);
    private:
      std::vector<std::shared_ptr<Variable>> literals{};
      std::vector<std::shared_ptr<Block>> blocks{};
      std::shared_ptr<MainBlock> mainBlock = nullptr;
  };
}

#endif //GLOBAL_BLOCK_HPP
