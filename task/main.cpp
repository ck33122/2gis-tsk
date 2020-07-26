#include "checksum.hpp"
#include "count_words.hpp"
#include "arguments_parser.hpp"
#include <iostream>
#include <fstream>

struct Arguments {
  bool             show_help;
  std::string_view module, target, file;
};

static int run_module(const Arguments& args, std::ifstream& stream) {
  if (args.module == "words") {
    if (!args.target.size()) {
      std::cout << "you shoud choose target. consider to use -h for help\n";
      return 1;
    }
    auto result = count_words(stream, args.target);
    if (result.has_error()) {
      std::cout << result.get_error().reason << '\n';
      return 1;
    }
    std::cout << result.get_value() << '\n';
  } else if (args.module == "checksum") {
    auto result = checksum(stream);
    if (result.has_error()) {
      std::cout << result.get_error().reason << '\n';
      return 1;
    }
    std::cout << std::hex << result.get_value() << '\n';
  } else {
    std::cout << "you shoud choose module. consider to use -h for help\n";
    return 1;
  }
  return 0;
}

int main(int argc, char** argv) {
  Arguments args;
  auto      parse_result =
    ArgumentsParser{}
      .bind('h', args.show_help)
      .bind('m', args.module)
      .bind('v', args.target)
      .bind('f', args.file)
      .parse(argc, argv);

  if (parse_result.has_error()) {
    std::cout << parse_result.get_error().reason;
    return 1;
  }

  if (args.show_help) {
    std::cout << "test task for 2gis\n"
              << "usage: task [options]\n"
              << "options:\n"
              << "  -h              show help\n"
              << "  -m <module>     module to run. one of:\n"
              << "    words    - prints <target> count in <file>\n"
              << "    checksum - prints 32-bit checksum of <file>\n"
              << "  -f <file>       file to read\n"
              << "  -v <target>     search target (in module -m words)\n";
    return 0;
  }

  if (!args.file.size()) {
    std::cout << "you shoud choose file. consider to use -h for help\n";
    return 1;
  }

  auto stream        = std::ifstream(&args.file.front(), std::ifstream::in);
  int  module_result = run_module(args, stream);
  stream.close();

  return module_result;
}