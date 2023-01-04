#ifndef MAIN_BLOCK_HPP
#define MAIN_BLOCK_HPP

#include "Block.hpp"

namespace compilerLogic {
  class MainBlock : public Block {
    public:
      inline MainBlock(std::vector<std::shared_ptr<Identifier>> availableIdentifiers) {
        this->availableIdentifiers = availableIdentifiers;
      }
      virtual std::vector<IntermidiateCode> parseIntermidiate() override final;
      virtual void addCommand(std::shared_ptr<Command> command) override final;
      virtual void addBlock(std::shared_ptr<Block> block) override final;
      virtual std::vector<std::shared_ptr<Identifier>>
              getAvailableIdentifiers() override {return this->availableIdentifiers;}
      virtual ~MainBlock() = default;
    private:
      std::vector<std::shared_ptr<ParsableToIntermidiate>> stuctures{};
  };
}

#endif //MAIN_BLOCK_HPP