#include <avr/io.h>
#include <util/delay.h>

#include "ili9341.h"
#include "spi_lib.h"

#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_BLUE 0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_RED 0x001F
#define COLOR_YELLOW 0xFFE0

void
setup (void)
{
  spi_master_init ();
  ili9341_init ();
}

int
main (void)
{
  setup ();
  fill_screen (COLOR_GREEN);

  while (1)
    ;

  return (0);
}
