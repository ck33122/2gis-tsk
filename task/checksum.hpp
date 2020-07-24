#pragma once
#include "result.hpp"

// little endian
Result<uint32_t> checksum(std::istream& input);
