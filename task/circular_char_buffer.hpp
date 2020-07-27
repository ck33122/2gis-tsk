#pragma once
#include <cstdlib>
#include <cstring>
#include <string_view>
#include <algorithm>

class CircularCharBuffer {
  size_t buffer_size, virtual_size;
  char*  buffer;

public:
  inline CircularCharBuffer(size_t buffer_size)
    : buffer_size(buffer_size)
    , virtual_size(0)
    , buffer(reinterpret_cast<char*>(malloc(buffer_size))) {}

  inline ~CircularCharBuffer() { free(buffer); }

  inline size_t      size() const { return virtual_size; }
  inline char&       operator[](size_t position) { return buffer[position % buffer_size]; }
  inline const char& operator[](size_t position) const { return buffer[position % buffer_size]; }

  inline void read(char* data, size_t data_size) {
    if (data_size > buffer_size) {
      data += data_size - buffer_size;
      data_size = buffer_size;
    }
    auto first_part_size  = virtual_size % buffer_size;
    auto second_part_size = buffer_size - first_part_size;
    if (second_part_size >= data_size) {
      memcpy(buffer + first_part_size, data, data_size);
    } else {
      memcpy(buffer + first_part_size, data, second_part_size);
      memcpy(buffer, data + second_part_size, data_size - second_part_size);
    }
    virtual_size += data_size;
  }

  inline bool substring_equals(size_t from, std::string_view pattern) const {
    if (buffer_size < pattern.size()) {
      return false;
    }
    auto first_part_size  = from % buffer_size;
    auto second_part_size = buffer_size - first_part_size;
    if (second_part_size >= pattern.size()) {
      return std::string_view{buffer + first_part_size, pattern.size()} == pattern;
    } else {
      auto second_part = std::string_view{buffer + first_part_size, second_part_size};
      auto first_part  = std::string_view{buffer, pattern.size() - second_part_size};
      return second_part == pattern.substr(0, second_part_size) &&
             first_part == pattern.substr(second_part_size);
    }
  }
};
