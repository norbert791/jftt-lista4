#ifndef INTERMIDIATE_PARSER_HPP
#define INTERMIDIATE_PARSER_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include "IntermidiateCode.hpp"

namespace compilerLogic {
  class IntermidiateParser {
    public:
      std::vector<std::string> parseIntermidiateCode(std::vector<IntermidiateCode>);
      void showIntermidiateCode(std::vector<IntermidiateCode>);
    private:
      //The smallest unallocated mem adress. Starts with 1 since 0 <-- acc
      int64_t mem_pool = 1;
      //Map var ids to real mem locations
      std::map<int64_t, int64_t> addressMap{};
      //Map label ids to the instruction number AFTER them.
      std::map<int64_t, int64_t> labelMap{};
  };
}

#endif //INTERMIDIATE_PARSER_HPP