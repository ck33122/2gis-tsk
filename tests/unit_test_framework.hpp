#pragma once
#include <vector>
#include <iostream>
#include <functional>
#include <string_view>
#include <algorithm>

enum class TestStatus {
  Ok,
  Failed
};

struct Test {
  std::function<TestStatus()> functor;
  const char*                 name;
};

#define TEST_CASE(test_name, ...) \
  struct test_name {              \
    test_name() {                 \
      tests.push_back(            \
        {[]() -> TestStatus {     \
           __VA_ARGS__;           \
           return TestStatus::Ok; \
         },                       \
         #test_name});            \
    }                             \
  } static_init_##test_name;

#define REQUIRE_(expression, line, file)                                                    \
  if (!(expression)) {                                                                      \
    std::cout << "requirement (" #expression ") failed on " << file << ":" << line << "\n"; \
    return TestStatus::Failed;                                                              \
  }

#define REQUIRE(expression) REQUIRE_(expression, __LINE__, __FILE__)

#ifndef TEST_MAIN
extern std::vector<Test> tests;
#else
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

#endif