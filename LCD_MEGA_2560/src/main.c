#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "ili9341.h"
#include "spi_lib.h"
#include "uart_lib.h"

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
  uart_init ();
}

int
main (void)
{
  t_rgb bg = { 0, 0, 0 };
  // t_rgb pixel = { 227, 150, 223 };
  t_rgb pixel = { 255, 255, 255 };

  setup ();

  fill_screen (pack_rgb565 (bg));
  while (1)
    {

      ili9341_draw_pixel (0, 0, pixel);
      // for (uint16_t i = 0; i < 360; i += 10)
      //   {
      //     uint16_t color = hue_to_rgb565 ((float)i);
      //     fill_screen (color);
      //   };
    }

  return (0);
}
