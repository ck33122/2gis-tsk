#include "unit_test_framework.hpp"
#include "result.hpp"


TEST_CASE(result_should_call_destructor_on_result, {
  struct TestDestructorCall {
    int& num_instances;

    TestDestructorCall(int& num_instances)
      : num_instances(num_instances) { num_instances++; }

    TestDestructorCall(TestDestructorCall&& value)
      : num_instances(value.num_instances) { num_instances++; }

    ~TestDestructorCall() { num_instances--; }
  };

  int num_instances = 0;
  {
    Result<TestDestructorCall> result = {TestDestructorCall{num_instances}};
    REQUIRE(num_instances == 1);
  }
  REQUIRE(num_instances == 0);
});
