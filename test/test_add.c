#include "avr/avr_mcu_section.h"
#include "blabla.h"
#include "unity.h"
#include "unity_internals.h"

#include <avr/sleep.h>

AVR_MCU (F_CPU, "atmega2560");

AVR_MCU_SIMAVR_CONSOLE (&UDR0);

static int
uart_putchar (char c, FILE *stream)
{
  (void)stream;
  UDR0 = c;
  return 0;
}

static FILE uart_stdout
    = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

void
setUp (void)
{
}

void
tearDown (void)
{
}

void
test_addition (void)
{
  int ret = addit (1, 1);
  TEST_ASSERT_EQUAL_MESSAGE (2, ret, "Invalid value returned for addit(1,1)");

  ret = addit (2, 2);
  TEST_ASSERT_EQUAL_MESSAGE (0, ret, "Invalid value returned for addit(1,1)");
}

int
main (void)
{
  stdout = &uart_stdout;

  UNITY_BEGIN ();

  RUN_TEST (test_addition);

  sleep_cpu (); // Necessary for avrsim to work

  return UNITY_END ();
}
