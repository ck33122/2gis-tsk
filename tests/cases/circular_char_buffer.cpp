#include "test_framework.hpp"
#include "circular_char_buffer.hpp"
#include <sstream>

TEST_CASE(circular_char_buffer_initializes_with_0_size, {
  auto buf = CircularCharBuffer{10};
  REQUIRE(buf.size() == 0);
});

TEST_CASE(circular_char_buffer_groth_with_read, {
  auto buf = CircularCharBuffer{10};

  char data1[] = {'a', 'b', 'c', 'd', 'e'};
  buf.read(data1, sizeof(data1));
  REQUIRE(buf.size() == sizeof(data1));
  for (size_t i = 0; i < sizeof(data1); i++) {
    REQUIRE(buf[i] == data1[i]);
  }

  char data2[] = {'f', 'g', 'h'};
  buf.read(data2, sizeof(data2));
  REQUIRE(buf.size() == sizeof(data1) + sizeof(data2));
  for (size_t i = 0; i < sizeof(data1); i++) {
    REQUIRE(buf[i] == data1[i]);
  }
  for (size_t i = 0; i < sizeof(data2); i++) {
    REQUIRE(buf[sizeof(data1) + i] == data2[i]);
  }
});

TEST_CASE(circular_char_buffer_overread_causes_data_rewrite, {
  auto buf = CircularCharBuffer{3};

  char data1[] = {'a', 'b'};
  buf.read(data1, sizeof(data1));
  REQUIRE(buf.size() == sizeof(data1));

  char data2[] = {'c', 'd'};
  buf.read(data2, sizeof(data2));
  REQUIRE(buf.size() == sizeof(data1) + sizeof(data2));

  REQUIRE(buf[1] == 'b');
  REQUIRE(buf[2] == 'c');
  REQUIRE(buf[3] == 'd');
});
