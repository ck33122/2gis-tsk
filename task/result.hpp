#pragma once
#include <string_view>
#include <type_traits>
#include <cstdlib>
#include <memory>

using Error = std::string_view;

template <typename TValue>
class Result {
  bool error_happend;
  union {
    Error  error;
    TValue result;
  };

public:
  Result(Error error)
    : error_happend(true)
    , error(error) {}

  Result(TValue&& result)
    : error_happend(false)
    , result(std::forward<TValue>(result)) {}

  ~Result() {
    if (!error_happend) {
      if constexpr (!std::is_trivially_destructible_v<TValue>) {
        result.~TValue();
      }
    }
  }

  bool    has_error() { return error_happend; }
  Error&  get_error() { return error_happend ? error : (error = Error{}); }
  TValue& get_result() {
    if (!error_happend)
      return result;
    abort();
  }
};