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

#define REQUIRE_(line, file, ...)                                                            \
  if (!(__VA_ARGS__)) {                                                                      \
    std::cout << "requirement (" #__VA_ARGS__ ") failed on " << file << ":" << line << "\n"; \
    return TestStatus::Failed;                                                               \
  }

#define REQUIRE(...) REQUIRE_(__LINE__, __FILE__, __VA_ARGS__)

extern std::vector<Test> tests;
