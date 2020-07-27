#pragma once
#include "circular_char_buffer.hpp"
#include <string_view>
#include <limits>

class CountWordsInBufferRabinKarp {
  using integer      = int64_t;
  using integer_down = int32_t;

  static constexpr const integer base  = 1 << (sizeof(char) * 8);                      // radix (typically size of alphabet, i.e. for char is 256)
  static constexpr const integer prime = std::numeric_limits<integer_down>::max() - 1; // prime number where base*prime shoud fit in computer word

  std::string_view target;
  size_t           pos         = 0;
  size_t           count       = 0;
  integer          target_hash = 0;
  integer          text_hash   = 0;
  integer          first_coeff = 1; // = (base ^ (target.length - 1)) % q

public:
  inline CountWordsInBufferRabinKarp(std::string_view target)
    : target(target) {}

  inline void process(const CircularCharBuffer& text) {
    if (pos == 0) {
      if (text.size() < target.size()) {
        return; // text is not big enough to compute
      }
      precalculate_hashes(text);
      if (target_hash == text_hash && text.substring_equals(pos, target)) {
        count++;
      }
      pos++;
    }

    for (; pos <= text.size() - target.size(); pos++) {
      recalc_hash_for_current_position(text);
      if (target_hash == text_hash && text.substring_equals(pos, target)) {
        count++;
      }
    }
  }

  inline size_t result() { return count; }

private:
  inline void precalculate_hashes(const CircularCharBuffer& text) {
    for (size_t i = 0; i < target.size() - 1; i++) {
      first_coeff = (base * first_coeff) % prime;
    }
    for (size_t i = 0; i < target.size(); i++) {
      target_hash = (base * target_hash + target[i]) % prime;
      text_hash   = (base * text_hash + text[i]) % prime;
    }
  }

  inline void recalc_hash_for_current_position(const CircularCharBuffer& text) {
    text_hash -= text[pos - 1] * first_coeff;
    text_hash = (base * text_hash + text[pos + target.size() - 1]) % prime;
  }
};
