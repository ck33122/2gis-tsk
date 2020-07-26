#pragma once
#include <string_view>
#include <type_traits>
#include <cstdlib>
#include <memory>

struct Error {
  std::string_view reason;
};

template <typename TValue>
class Result {
  bool error_happend;
  union {
    Error  error;
    TValue value;
  };

public:
  Result(Error error)
    : error_happend(true)
    , error(error) {}

  Result(TValue&& value)
    : error_happend(false)
    , value(std::forward<TValue>(value)) {}

  ~Result() {
    if (!error_happend) {
      if constexpr (!std::is_trivially_destructible_v<TValue>) {
        value.~TValue();
      }
    }
  }

  bool    has_error() { return error_happend; }
  Error&  get_error() { return error_happend ? error : (error = Error{}); }
  TValue& get_value() {
    if (!error_happend)
      return value;
    abort();
  }
};

template <>
class Result<void> {
  bool  error_happend;
  Error error;

public:
  Result()
    : error_happend(false) {}

  Result(Error error)
    : error_happend(true)
    , error(error) {}

  bool   has_error() { return error_happend; }
  Error& get_error() { return error_happend ? error : (error = Error{}); }
};

template <typename TValue>
auto make_result(TValue&& value) {
  return Result<std::remove_reference_t<TValue>>{std::forward<TValue>(value)};
}

template <typename TValue>
auto make_error(std::string_view reason) {
  return Result<TValue>{Error{reason}};
}
