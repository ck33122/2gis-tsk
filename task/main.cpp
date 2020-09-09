#include "checksum.hpp"
#include "count_words.hpp"
#include "arguments_parser.hpp"
#include "count_words_naive.hpp"
#include "count_words_rabin_karp.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

struct Arguments {
  bool             show_help = false;
  std::string_view module    = std::string_view{""};
  std::string_view target    = std::string_view{""};
  std::string_view file      = std::string_view{""};
  std::string_view algorithm = std::string_view{"naive"};
};

int main(int argc, char** argv) {
  Arguments args;
  auto      parse_result =
    ArgumentsParser{}
      .bind('h', args.show_help)
      .bind('m', args.module)
      .bind('v', args.target)
      .bind('f', args.file)
      .bind('a', args.algorithm)
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
              << "    words     - prints <target> count in <file>\n"
              << "    checksum  - prints 32-bit checksum of <file>\n"
              << "  -f <file>       file to read\n"
              << "  -v <target>     search <target> in <file> (in module -m words)\n"
              << "  -a <algorithm>  algorithm of search <target> (in module -m words). one of:\n"
              << "    naive     - simple bruteforce substring search (default)\n"
              << "    rk        - rabin-karp substring search\n";
    return 0;
  }

  if (!args.file.size()) {
    std::cout << "you shoud choose file. consider to use -h for help\n";
    return 1;
  }

  if (args.module == "words") {
    if (!args.target.size()) {
      std::cout << "you shoud choose target. consider to use -h for help\n";
      return 1;
    }

    auto stream = std::ifstream{&args.file.front(), std::ios::in};
    auto result = make_result<size_t>(0);

    if (args.algorithm == "naive") {
      result = count_words<CountWordsInBufferNaive>(stream, args.target);
    } else if (args.algorithm == "rk") {
      result = count_words<CountWordsInBufferRabinKarp>(stream, args.target);
    } else {
      std::cout << "unknown algorithm of substring search. consider to use -h for help\n";
      return 1;
    }

    if (result.has_error()) {
      std::cout << result.get_error().reason << '\n';
      return 1;
    }
    std::cout << result.get_value() << '\n';

  } else if (args.module == "checksum") {
    auto stream = std::ifstream{&args.file.front(), std::ios::in | std::ios::binary};
    auto result = checksum(stream);

    if (result.has_error()) {
      std::cout << result.get_error().reason << '\n';
      return 1;
    }

    std::cout << std::setfill('0')
              << std::setw(sizeof(uint32_t) * 2)
              << std::right
              << std::hex
              << result.get_value() << '\n';
  } else {
    std::cout << "you shoud choose module. consider to use -h for help\n";
    return 1;
  }

  return 0;
}
