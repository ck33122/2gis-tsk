#include "checksum.hpp"
#include <istream>

#if defined(_MSC_VER)
  #include <cstdlib>
  #define byte_swap_32 _byteswap_ulong
#elif defined(__APPLE__)
  #include <libkern/OSByteOrder.h>
  #define byte_swap_32 OSSwapInt32
#else
  #include <byteswap.h>
  #define byte_swap_32 bswap_32
#endif

// then we should bswap (little endian is preffered)
static bool check_big_endian() {
  union {
    uint32_t integer;
    uint8_t  chars[4];
  } value = {0x01020304};
  return value.chars[0] == 1;
}

Result<uint32_t> checksum(std::istream& input) {
  uint32_t result        = 0;
  bool     is_big_endian = check_big_endian();

  while (input.good() && !input.eof()) {
    uint32_t word = 0;
    input.read(reinterpret_cast<char*>(&word), sizeof(uint32_t));
    if (is_big_endian) {
      word = byte_swap_32(word);
    }
    result += word;
  }

  if (input.bad()) {
    return make_error<uint32_t>("Read/writing error on i/o operation");
  }
  bool fail_caused_by_smaller_input = input.eof() && input.gcount() < sizeof(uint32_t);
  if (input.fail() && !fail_caused_by_smaller_input) {
    return make_error<uint32_t>("Logical error on i/o operation");
  }
  return make_result(std::move(result));
}
