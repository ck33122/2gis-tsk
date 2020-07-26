#include "count_words.hpp"
#include <functional>
#include <algorithm>

template <typename T, typename TFunc>
static std::pair<T, T> min_max(T a, T b, TFunc&& criterion) {
  auto a_by_criterion = criterion(a);
  auto b_by_criterion = criterion(b);
  if (a_by_criterion < b_by_criterion) {
    return std::make_pair(a, b);
  } else {
    return std::make_pair(b, a);
  }
}

void BufferOccurrences::read(std::string_view buffer) {
  if (partial_occurrence) {
    auto partial = target.substr(partial_occurrence);
    auto [a, b]  = min_max(buffer, partial, [](auto s) { return s.size(); });
    if (a == b.substr(0, a.size())) {
      partial_occurrence += a.size();
      buffer = buffer.substr(a.size());
      if (partial_occurrence == target.size()) {
        count++;
        partial_occurrence = 0;
      }
    } else {
      partial_occurrence = 0;
    }
  }

  if (partial_occurrence) {
    return;
  }

  for (size_t first = 0; (first = buffer.find(target[0], first)) != std::string_view::npos;) {
    auto length     = std::min(target.size(), buffer.size() - first);
    auto sub_buffer = buffer.substr(first, length);
    auto sub_target = target.substr(0, length);
    if (sub_buffer == sub_target) {
      if (length != target.size()) {
        partial_occurrence = length;
      } else {
        count++;
      }
      first += target.size();
    } else {
      first++;
    }
  }
}

Result<uint64_t> count_words(std::istream& input, std::string_view target) {
  if (target.size() == 0) {
    return make_error<uint64_t>("bad argument: target shoud have at least 1 character");
  }

  auto occ = BufferOccurrences{target};
  char buffer[4096];

  while (input.good() && !input.eof()) {
    input.read(buffer, sizeof(buffer));
    auto readed = input.gcount();
    if (readed) {
      occ.read(std::string_view{buffer, static_cast<std::string_view::size_type>(readed)});
    } else {
      break;
    }
  }

  if (input.bad()) {
    return make_error<uint64_t>("Read/writing error on i/o operation");
  }
  bool fail_caused_by_smaller_input = input.eof() && input.gcount() < sizeof(buffer);
  if (input.fail() && !fail_caused_by_smaller_input) {
    return make_error<uint64_t>("Logical error on i/o operation");
  }

  return make_result(std::move(occ.get_count()));
}
