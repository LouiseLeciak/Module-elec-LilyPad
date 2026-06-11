#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "ili9341.h"
#include "spi_lib.h"

#define COLOR_BLACK 0x0000
#define COLOR_RED 0x001F
#define COLOR_PINK 0x024F
#define COLOR_GREEN 0x07E0
#define COLOR_BLUE 0xF800
#define COLOR_YELLOW 0xFFE0
#define COLOR_WHITE 0xFFFF

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
  // fill_screen (COLOR_PINK);

  while (1)
    {
      for (uint16_t i = 0; i < (0xFFFF); i++)
        {
          fill_screen (i);
        };
    }

  return (0);
}
