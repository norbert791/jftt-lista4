#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <memory>
#include <algorithm>
#include <stdexcept>
#include "IntermidiateCode.hpp"
#include "Variable.hpp"
#include "Identifier.hpp"
#include "Command.hpp"

namespace compilerLogic {

  class Block : public ParsableToIntermidiate {
    public:
      virtual std::vector<IntermidiateCode> parseIntermidiate() override = 0;
      inline void addAvailableIdentifier(std::shared_ptr<Identifier> var) {
        this->availableIdentifiers.push_back(var);
      }

      inline std::shared_ptr<Identifier> findIdentifier(std::string identName) {
        auto it = std::find_if(
                                availableIdentifiers.begin(),
                                availableIdentifiers.end(),
                                [&identName](const std::shared_ptr<Identifier> x)
                                {return x->getName() == identName;}
                              );

        if (it == availableIdentifiers.end()) {
          throw std::out_of_range("Identifier " + identName +  " not found");
        }
        return *it;
      }

      inline bool verifyIdentifier(std::string identName) {
        auto it = std::find_if(
                                availableIdentifiers.begin(),
                                availableIdentifiers.end(),
                                [&identName](const std::shared_ptr<Identifier> x)
                                {return x->getName() == identName;}
                              );

        return it != availableIdentifiers.end();
      }
      

      virtual void addCommand(std::shared_ptr<Command> command) = 0;
      virtual void addBlock(std::shared_ptr<Block> block) = 0;
      virtual std::vector<std::shared_ptr<Identifier>> getAvailableIdentifiers();
      virtual ~Block() = default;
    protected:
      std::vector<std::shared_ptr<Identifier>> availableIdentifiers{};
  };
}



#endif //BLOCK_HPP