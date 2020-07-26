#include "arguments_parser.hpp"

static constexpr const char* argument_not_handled =
  "supplied argument or argument format is invalid. consider to use -h for help";

ArgumentsParser::ArgumentsParser() {
  on_argument = [](char c) {
    return Result<void>(Error{argument_not_handled});
  };
  on_argument_with_value = [](char, std::string_view) {
    return Result<void>(Error{argument_not_handled});
  };
}

ArgumentsParser& ArgumentsParser::bind(char flag, bool& value) {
  value = false;
  on_argument =
    [flag, &value, func = std::move(on_argument)](char input_flag) {
      if (input_flag == flag) {
        value = true;
        return Result<void>();
      }
      return func(input_flag);
    };
  return *this;
}

ArgumentsParser& ArgumentsParser::bind(char flag, std::string_view& value) {
  value = std::string_view{""};
  on_argument_with_value =
    [flag, &value, func = std::move(on_argument_with_value)](char input_flag, std::string_view input_value) {
      if (input_flag == flag) {
        value = input_value;
        return Result<void>();
      }
      return func(input_flag, input_value);
    };
  return *this;
}

Result<void> ArgumentsParser::parse(int argc, char** argv) {
  for (uint32_t i = 1; i < argc; i += 2) {
    Result<void> result;
    auto         flag_str = std::string_view{argv[i]};

    if (flag_str.size() != 2 || flag_str[0] != '-') {
      result = Result<void>(Error{"invalid flag"});
    } else {
      auto flag = flag_str[1];

      if (i + 1 == argc) {
        result = on_argument(flag);
      } else {
        auto value = std::string_view{argv[i + 1]};
        if (value.size() == 0) {
          result = Result<void>(Error{"invalid flag value"});
        } else {
          result = on_argument_with_value(flag, value);
        }
      }
    }

    if (result.has_error()) {
      return result;
    }
  }
  return Result<void>();
}
