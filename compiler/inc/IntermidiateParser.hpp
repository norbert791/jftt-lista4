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
    private:
      int64_t mem_pool = 1; //The smallest unallocated mem adress. Starts with 1 since 0 <-- acc
      std::map<int64_t, int64_t> addressMap{}; //Map var ids to real mem locations
  };
}

#endif //INTERMIDIATE_PARSER_HPP