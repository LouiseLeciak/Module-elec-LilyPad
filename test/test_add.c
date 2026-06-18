#include "blabla.h"
#include "unity.h"
#include "unity_internals.h"

void setUp(void) {}

void tearDown(void) {}

void test_addition(void) {
  int ret = addit(1, 1);

  TEST_ASSERT_EQUAL(2, ret);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_addition);

  return UNITY_END();
}
