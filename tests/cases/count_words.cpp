#include "test_framework.hpp"
#include "count_words.hpp"
#include "count_words_rabin_karp.hpp"
#include "count_words_naive.hpp"
#include <sstream>

static std::vector<Result<size_t>> run_count_words(std::string_view text, std::string_view target) {
  auto results = std::vector<Result<size_t>>();
  {
    auto ss = std::stringstream{};
    ss << text;
    results.emplace_back(count_words<CountWordsInBufferNaive>(ss, target));
  }
  {
    auto ss = std::stringstream{};
    ss << text;
    results.emplace_back(count_words<CountWordsInBufferRabinKarp>(ss, target));
  }
  return results;
}

TEST_CASE(count_words_returns_0_if_input_empty, {
  for (auto result : run_count_words({""}, {"word"})) {
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == 0);
  }
});

TEST_CASE(count_words_returns_count_on_some_input, {
  for (auto result : run_count_words({"abcdabcbababadbacbabcafdbabab"}, {"ab"})) {
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == 7);
  }
});
