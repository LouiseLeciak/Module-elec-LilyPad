#include <avr/io.h>

#include "main_screen.h"
#include "pins.h"
#include "spi.h"
#include "structs.h"

static void setup(void) {
  DDRB |= (MAIN_SCREEN_SS | MAIN_SCREEN_DC | MAIN_SCREEN_RST | MCU_SPI_MASK);
  PORTB |= (MAIN_SCREEN_SS | MAIN_SCREEN_DC | MAIN_SCREEN_RST);

  spi_master_init(FOSC_DIV2);
  spi_start_transaction(&PORTB, MAIN_SCREEN_SS);
  main_screen_init();
  spi_end_transaction(&PORTB, MAIN_SCREEN_SS);
}

int main(void) {
  setup();

  window tmp = {{0, 0}, {MAIN_SCREEN_HEIGHT - 1, MAIN_SCREEN_WIDTH - 1}};
  rgb colour = {0, 0, 0};
  spi_start_transaction(&PORTB, MAIN_SCREEN_SS);
  main_screen_draw_rectangle(tmp, colour);
  spi_end_transaction(&PORTB, MAIN_SCREEN_SS);

  rgb fg = {255, 255, 255};
  rgb bg = {0, 0, 0};
  position str_pos = {0, 0};
  spi_start_transaction(&PORTB, MAIN_SCREEN_SS);
  main_screen_draw_string(str_pos, "ABCDEFGHIKLMNOPQRSTUVWXYZYXWVUTSRQPO", fg,
                          bg, 2);
  spi_end_transaction(&PORTB, MAIN_SCREEN_SS);

  while (1) {
    ;
  }

  return (0);
}
