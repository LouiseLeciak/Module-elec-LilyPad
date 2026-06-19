#include "main_screen.h"
#include "simavr/avr/avr_mcu_section.h"
#include "unity.h"
#include "unity_internals.h"

#include <avr/sleep.h>
#include <stdint.h>

AVR_MCU(F_CPU, "atmega2560");

AVR_MCU_SIMAVR_CONSOLE(&UDR0);

static int uart_putchar(char c, FILE *stream) {
  (void)stream;
  UDR0 = c;
  return 0;
}

static FILE uart_stdout =
    FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void setUp(void) {}

void tearDown(void) {}

void test_pack_rgb_565(void) {
  uint16_t ret = 0;

  const rgb black = {0, 0, 0};
  const rgb white = {255, 255, 255};
  const rgb red = {255, 0, 0};
  const rgb green = {0, 255, 0};
  const rgb blue = {0, 0, 255};
  const rgb yellow = {255, 255, 0};
  const rgb cyan = {0, 255, 255};
  const rgb magenta = {255, 0, 255};

  ret = pack_rgb565(black);
  TEST_ASSERT_EQUAL_MESSAGE(
      0, ret, "Invalid value returned for pack_rgb565(black). Expected: 0");

  ret = pack_rgb565(white);
  TEST_ASSERT_EQUAL_MESSAGE(
      65535, ret,
      "Invalid value returned for pack_rgb565(white). Expected: 65,535");

  ret = pack_rgb565(red);
  TEST_ASSERT_EQUAL_MESSAGE(
      63488, ret,
      "Invalid value returned for pack_rgb565(red). Expected: 63,488");

  ret = pack_rgb565(green);
  TEST_ASSERT_EQUAL_MESSAGE(
      2016, ret,
      "Invalid value returned for pack_rgb565(green). Expected: 2,016");

  ret = pack_rgb565(blue);
  TEST_ASSERT_EQUAL_MESSAGE(
      31, ret, "Invalid value returned for pack_rgb565(blue). Expected: 31");

  ret = pack_rgb565(yellow);
  TEST_ASSERT_EQUAL_MESSAGE(
      65504, ret,
      "Invalid value returned for pack_rgb565(yellow). Expected: 65,504");

  ret = pack_rgb565(cyan);
  TEST_ASSERT_EQUAL_MESSAGE(
      2047, ret,
      "Invalid value returned for pack_rgb565(cyan). Expected: 2,047");

  ret = pack_rgb565(magenta);
  TEST_ASSERT_EQUAL_MESSAGE(
      63519, ret,
      "Invalid value returned for pack_rgb565(magenta). Expected: 63,519");
}

int main(void) {
  stdout = &uart_stdout;

  UNITY_BEGIN();

  RUN_TEST(test_pack_rgb_565);

  sleep_cpu(); // Necessary for avrsim to work

  return UNITY_END();
}
