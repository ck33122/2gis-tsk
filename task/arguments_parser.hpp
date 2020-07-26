#pragma once
#include "result.hpp"
#include <functional>

class ArgumentsParser {
  std::function<Result<void>(char)>                   on_argument;
  std::function<Result<void>(char, std::string_view)> on_argument_with_value;

public:
  ArgumentsParser();

  ArgumentsParser& bind(char flag, bool& value);
  ArgumentsParser& bind(char flag, std::string_view& value);
  Result<void>     parse(int argc, char** argv);
};
