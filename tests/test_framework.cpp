#include "test_framework.hpp"

std::vector<Test> tests;

int main(int argc, char** argv) {
  std::sort(tests.begin(), tests.end(), [](auto a, auto b) {
    return std::string_view(a.name) < std::string_view(b.name);
  });
  auto failed = false;

  for (auto& test : tests) {
    try {
      auto status = test.functor();
      if (status != TestStatus::Ok) {
        std::cout << test.name << ": FAIL\n";
        failed = true;
      } else {
        std::cout << test.name << ": OK\n";
      }
    } catch (std::exception& ex) {
      std::cout << test.name << ": FAIL ON EXCEPTION (" << ex.what() << ")\n";
      failed = true;
    }
  }

  if (failed) {
    std::cout << "run tests failed!\n";
    return 1;
  } else {
    std::cout << "all tests ok!\n";
    return 0;
  }
}
