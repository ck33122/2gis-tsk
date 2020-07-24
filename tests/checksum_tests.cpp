#include "unit_test_framework.hpp"
#include "checksum.hpp"
#include <sstream>

TEST_CASE(checksum_of_empty_stream_should_be_0, {
  std::stringstream ss;
  auto              result = checksum(ss);
  REQUIRE(!result.has_error());
  REQUIRE(result.get_value() == 0);
});

TEST_CASE(checksum_of_32_bit_value_should_be_that_value, {
  std::stringstream ss;
  ss << (char) 0x01 << (char) 0x02 << (char) 0x03 << (char) 0x04;
  auto result = checksum(ss);
  REQUIRE(!result.has_error());
  // printf("0x%08x\n", result.get_value());
  REQUIRE(result.get_value() == 0x04030201);
});
