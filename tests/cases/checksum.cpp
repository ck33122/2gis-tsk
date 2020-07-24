#include "test_framework.hpp"
#include "checksum.hpp"
#include <sstream>

void write_bytes(std::stringstream& ss, std::initializer_list<int> values) {
  for (auto& value : values) {
    ss << (char) value;
  }
}

TEST_CASE(checksum_of_empty_stream_should_be_0, {
  std::stringstream ss;
  auto              result = checksum(ss);
  REQUIRE(!result.has_error());
  REQUIRE(result.get_value() == 0);
});

TEST_CASE(checksum_of_32_bit_value_should_be_that_value, {
  std::stringstream ss;
  write_bytes(ss, {0x01, 0x02, 0x03, 0x04});
  auto result = checksum(ss);
  REQUIRE(!result.has_error());
  // printf("0x%08x\n", result.get_value());
  REQUIRE(result.get_value() == 0x04030201);
});

TEST_CASE(checksum_of_8_bit_value_should_be_as_in_little_endian, {
  std::stringstream ss;
  write_bytes(ss, {0x01});
  auto result = checksum(ss);
  REQUIRE(!result.has_error());
  // printf("0x%08x\n", result.get_value());
  REQUIRE(result.get_value() == 0x01);
});

TEST_CASE(checksum_of_32_bits_and_8_bit_value_should_be_sum_as_in_little_endian, {
  std::stringstream ss;
  write_bytes(ss, {0x01, 0x02, 0x03, 0x04});
  write_bytes(ss, {0x03});
  auto result = checksum(ss);
  REQUIRE(!result.has_error());
  // printf("0x%08x\n", result.get_value());
  REQUIRE(result.get_value() == 0x04030204);
});

TEST_CASE(checksum_of_3_words_should_be_sum_as_in_little_endian, {
  std::stringstream ss;
  write_bytes(ss, {0x01, 0x02, 0x03, 0x04});
  write_bytes(ss, {0x03, 0x04, 0x05, 0x06});
  write_bytes(ss, {0x07, 0x08, 0x09, 0xA0});
  auto result = checksum(ss);
  REQUIRE(!result.has_error());
  // printf("0x%08x\n", result.get_value());
  REQUIRE(result.get_value() == 0xAA110E0B);
});
