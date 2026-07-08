#include <avr/io.h>
#include <util/delay.h>

#include "ili9488.h"
#include "main_screen.h"
#include "pins.h"
#include "spi.h"
#include "structs.h"

static void setup(void) {
  DDRB |= (SS_MASK | MAIN_SCREEN_DC | MAIN_SCREEN_RST | MCU_SPI_MASK);
  PORTB |= (SS_MASK | MAIN_SCREEN_DC | MAIN_SCREEN_RST);

  spi_master_init(FOSC_DIV16);
  spi_start_transaction(&PORTB, MAIN_SCREEN_SS);
  ili9488_init_driver();
  // main_screen_init();
  spi_end_transaction(&PORTB, MAIN_SCREEN_SS);
}

int main(void) {
  setup();

  while (1) {
    ;
  }

  return (0);
}
