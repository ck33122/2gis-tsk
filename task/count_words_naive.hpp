#pragma once
#include "circular_char_buffer.hpp"
#include <string_view>

class CountWordsInBufferNaive {
  std::string_view target;
  size_t           pos   = 0;
  size_t           count = 0;

public:
  inline CountWordsInBufferNaive(std::string_view target)
    : target(target) {}

  inline void process(const CircularCharBuffer& text) {
    for (; pos + target.size() <= text.size(); pos++) {
      if (text.substring_equals(pos, target)) {
        count++;
      }
    }
  }

  inline size_t result() { return count; }
};
