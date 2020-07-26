#pragma once
#include "result.hpp"
#include <istream>

class BufferOccurrences {
  std::string_view target;
  uint64_t         partial_occurrence = 0;
  uint64_t         count              = 0;

public:
  // remark: target shoud have at least 1 symbol
  inline BufferOccurrences(std::string_view target)
    : target(target) {}

  inline uint64_t get_count() { return count; }
  void            read(std::string_view buffer);
};

Result<uint64_t> count_words(std::istream& input, std::string_view target);
