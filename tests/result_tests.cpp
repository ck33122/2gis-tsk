#include "unit_test_framework.hpp"
#include "result.hpp"

struct TestNumInstancesCounter {
  int& num_instances;

  TestNumInstancesCounter(int& num_instances)
    : num_instances(num_instances) { num_instances++; }

  TestNumInstancesCounter(TestNumInstancesCounter&& value)
    : num_instances(value.num_instances) { num_instances++; }

  ~TestNumInstancesCounter() { num_instances--; }
};

TEST_CASE(result_should_call_destructor_on_value_if_exists, {
  int num_instances = 0;
  {
    auto result = make_result(TestNumInstancesCounter{num_instances});
    REQUIRE(num_instances == 1);
  }
  REQUIRE(num_instances == 0);
});


struct TestDestructorCallCounter {
  int& destructor_calls;

  TestDestructorCallCounter(int& destructor_calls)
    : destructor_calls(destructor_calls) {}

  TestDestructorCallCounter(TestDestructorCallCounter&& value)
    : destructor_calls(value.destructor_calls) {}

  ~TestDestructorCallCounter() { destructor_calls++; }
};

TEST_CASE(result_should_not_call_destructor_on_value_if_not_exists, {
  int  destructor_calls = 0;
  auto result           = make_error<TestDestructorCallCounter>("error happend");
  REQUIRE(destructor_calls == 0);
});

TEST_CASE(result_value_available, {
  auto result = make_result(10);
  REQUIRE(!result.has_error());
  REQUIRE(result.get_value() == 10);
});

TEST_CASE(result_value_not_available_if_error, {
  auto result = make_error<int>("some error message");
  REQUIRE(result.has_error());
  REQUIRE(result.get_error().reason == std::string_view{"some error message"});
});

TEST_CASE(result_make_returns_clear_type, {
  auto result = make_result(10);
  REQUIRE(std::is_same_v<decltype(result), Result<int>>);
});

TEST_CASE(result_make_returns_clear_type_on_variable, {
  auto value  = 10;
  auto result = make_result(std::move(value));
  REQUIRE(std::is_same_v<decltype(result), Result<int>>);
});
