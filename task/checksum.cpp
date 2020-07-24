#include "checksum.hpp"
#include <istream>

// then we should bswap (little endian is preffered)
static bool is_big_endian() {
  constexpr const union {
    uint32_t i;
    uint8_t  c[4];
  } val = {0x01020304};
  return val.c[0] == 1;
}

Result<uint32_t> checksum(std::istream& input) {
  uint32_t result = 0;
  while (input && !input.eof()) {
    uint32_t word = 0;
    input.read(reinterpret_cast<char*>(&word), sizeof(word));
    result += word;
  }
  return make_result(std::move(result));
}
