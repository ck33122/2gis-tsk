#pragma once
#include "result.hpp"
#include "circular_char_buffer.hpp"
#include <istream>

template <typename TCountWordsInBuffer>
Result<size_t> count_words(std::istream& input, std::string_view target) {
  char data[4096];
  auto buffer  = CircularCharBuffer{target.size() + sizeof(data)}; // TODO up to power of two
  auto counter = TCountWordsInBuffer{target};

  while (input.good() && !input.eof()) {
    input.read(data, sizeof(data));
    auto readed = input.gcount();
    if (!readed) {
      break;
    }

    buffer.read(data, readed);
    counter.process(buffer);
  }

  if (input.bad()) {
    return make_error<size_t>("Read/writing error on i/o operation");
  }
  bool fail_caused_by_smaller_input = input.eof() && input.gcount() < sizeof(data);
  if (input.fail() && !fail_caused_by_smaller_input) {
    return make_error<size_t>("Logical error on i/o operation");
  }

  return make_result(std::move(counter.result()));
}
