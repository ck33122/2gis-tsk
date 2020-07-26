#include "test_framework.hpp"
#include "count_words.hpp"
#include <sstream>

TEST_CASE(buffer_occurrences_returns_count_on_highly_partial_content, {
  auto occ = BufferOccurrences{std::string_view{"hello"}};
  REQUIRE(occ.get_count() == 0);
  occ.read(std::string_view{"asdh"});
  REQUIRE(occ.get_count() == 0);
  occ.read(std::string_view{"el"});
  REQUIRE(occ.get_count() == 0);
  occ.read(std::string_view{"l"});
  REQUIRE(occ.get_count() == 0);
  occ.read(std::string_view{"o"});
  REQUIRE(occ.get_count() == 1);
  occ.read(std::string_view{"ooo0010)"});
  REQUIRE(occ.get_count() == 1);
});

TEST_CASE(buffer_occurrences_returns_count_on_partial_repetetive_pattern, {
  auto occ = BufferOccurrences{std::string_view{"abab"}};
  REQUIRE(occ.get_count() == 0);
  occ.read(std::string_view{"aaba"});
  REQUIRE(occ.get_count() == 0);
  occ.read(std::string_view{"ba"});
  REQUIRE(occ.get_count() == 1);
  occ.read(std::string_view{"bababaabab"});
  REQUIRE(occ.get_count() == 3);
  occ.read(std::string_view{"ab"});
  REQUIRE(occ.get_count() == 3);
  occ.read(std::string_view{"ab"});
  REQUIRE(occ.get_count() == 4);
});

TEST_CASE(count_words_returns_0_if_input_empty, {
  auto ss     = std::stringstream{};
  auto result = count_words(ss, std::string_view{"word"});
  REQUIRE(!result.has_error());
  REQUIRE(result.get_value() == 0);
});

TEST_CASE(count_words_returns_count_on_some_input, {
  auto ss = std::stringstream{};
  ss << "abcdabcbababadbacbabcafdbabab";
  auto result = count_words(ss, std::string_view{"ab"});
  REQUIRE(!result.has_error());
  REQUIRE(result.get_value() == 7);
});

TEST_CASE(count_words_returns_error_on_empty_target, {
  auto ss = std::stringstream{};
  ss << "abcdabcbababadbacbabcafdbabab";
  auto result = count_words(ss, std::string_view{""});
  REQUIRE(result.has_error());
});
