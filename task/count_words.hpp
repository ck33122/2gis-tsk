#pragma once
#include "result.hpp"
#include <istream>

Result<uint64_t> count_words(std::istream& input, std::string_view target);
