#include "st7796.h"

#include <util/delay.h>

#include "main_screen.h"
#include "pins.h"

// ------ Setup commands -------------------------------------------------------
void main_screen_init(void) {
  MAIN_SCREEN_RST_LOW();
  _delay_ms(100);
  MAIN_SCREEN_RST_HIGH();
  _delay_ms(100);
  main_screen_swreset();
  _delay_ms(5);
  main_screen_slpout();
  _delay_ms(5);
  main_screen_madctl(0x48); // Not really elegant for now*.
  main_screen_colmod(CI_16B);
  main_screen_dispon();

  // *: The only bits that are different from its standard value are bit 3 and
  // bit 7.
  // Bit 3: Set to 1 so as to send the data in BGR as the LCD panel we
  // have (HSD-9190J-B3) seems to be wired with Red and Blue inverted.
  // Bit 7:
  // Set to 1 so as to have coorinate 0 be left-hand side of the screen instead
  // of right-hand side.
}
