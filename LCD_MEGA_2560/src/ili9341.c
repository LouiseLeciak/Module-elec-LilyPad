#include "ili9341.h"

#include <stdint.h>
#include <util/delay.h>

#include "ili9341_cmds.h"
#include "small_font.h"
#include "spi_lib.h"
#include "uart_lib.h"

void
ili9341_cmd (uint8_t cmd)
{
  ILI9341_DC_LOW ();
  spi_master_transmit (cmd);
}

void
ili9341_data (uint8_t *data, size_t siz)
{
  ILI9341_DC_HIGH ();
  for (size_t i = 0; i < siz; i++)
    spi_master_transmit (data[i]);
}

void
ili9341_init (void)
{
  ILI9341_SS_LOW ();  // Selects the slave
  ILI9341_RST_LOW (); // Pulls the RESET pin to LOW
  _delay_ms (100);
  ILI9341_RST_HIGH ();
  _delay_ms (100);

  ili9341_software_reset ();
  ili9341_sleep_out ();
  ili9341_pixel_format_set (0x55);

  ili9341_cmd (DISPLAY_ON);
  ILI9341_SS_HIGH ();
}

void
ili9341_set_window (const uint16_t start_x, const uint16_t end_x,
                    const uint16_t start_y, const uint16_t end_y)
{
  ili9341_col_address_set (start_x, end_x);
  ili9341_page_address_set (start_y, end_y);
}

void
ili9341_draw_pixel (const uint16_t pos_x, const uint16_t pos_y,
                    const t_rgb colour)
{
  // if (pos_x >= ILI9341_MAX_WIDTH || pos_y >= ILI9341_MAX_HEIGHT)
  //   {
  //     uart_printstr ("Trying to draw pixel out of bounds\r\n");
  //     return;
  //   }

  ILI9341_SS_LOW ();
  ili9341_set_window (pos_x, pos_y, pos_x + 1, pos_y + 1);
  ili9341_memory_write ();
  ILI9341_DC_HIGH ();

  uint16_t colour565 = pack_rgb565 (colour);
  spi_master_transmit (colour565 >> 8);
  spi_master_transmit (colour565 & 0xFF);

  ILI9341_SS_HIGH ();
}

void
fill_screen (uint16_t colour)
{
  ILI9341_SS_LOW ();
  ili9341_set_window (0, ILI9341_MAX_WIDTH, 0, ILI9341_MAX_HEIGHT);

  ili9341_memory_write ();
  ILI9341_DC_HIGH ();
  for (uint32_t i = 0;
       i < (uint32_t)ILI9341_MAX_HEIGHT * (uint32_t)ILI9341_MAX_WIDTH; i++)
    {
      spi_master_transmit (colour >> 8);
      spi_master_transmit (colour & 0xFF);
    }
  ILI9341_SS_HIGH (); // Deselects the slave
}

uint16_t
pack_rgb565 (const t_rgb colour)
{
  // For the red, we need to isolate the 5 most significant bits:
  //  1 1 1 1 | 1 0 0 0 (the `1` indicate the most significant bits)
  //  As the first half is full of 1, we know our mask starts with 0xF
  //  Then, only a 1 for 2^3=8, so the end of the mask is 8
  //  The mask thus is 0xF8

  // For the green, we need to isolate the 6 most significant bits:
  //  1 1 1 1 | 1 1 0 0 (the `1` indicate the most significant bits)
  //  As the first half is full of 1, we know our mask starts with 0xF
  //  Then, only a 1 for 2^3=8 and 2^2=4, so the end of the mask is 12 in
  //  decimal and C in hex The mask thus is 0xFC

  // For the blue we just bitshift 3 times to the right as it would be
  // essentially the same as applying 0xF8 on it.

  // In the end, this is the structure of the data to send :
  // R  R  R  R  R  G  G G G G G B B B B B
  // 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
  return (((colour.red & 0xF8) << 8) | ((colour.green & 0xFC) << 3)
          | (colour.blue >> 3));
}

uint16_t
hue_to_rgb565 (float hue)
{
  float c = 1.0; // Max brightness
  float x = c * (1.0 - fabs (fmod (hue / 60.0, 2) - 1.0));
  float m = 0.0;

  float r = 0, g = 0, b = 0;
  if (hue >= 0 && hue < 60)
    {
      r = c;
      g = x;
      b = 0;
    }
  else if (hue >= 60 && hue < 120)
    {
      r = x;
      g = c;
      b = 0;
    }
  else if (hue >= 120 && hue < 180)
    {
      r = 0;
      g = c;
      b = x;
    }
  else if (hue >= 180 && hue < 240)
    {
      r = 0;
      g = x;
      b = c;
    }
  else if (hue >= 240 && hue < 300)
    {
      r = x;
      g = 0;
      b = c;
    }
  else
    {
      r = c;
      g = 0;
      b = x;
    }

  // Convert to 8-bit RGB
  uint8_t R8 = (uint8_t)((r + m) * 255.0);
  uint8_t G8 = (uint8_t)((g + m) * 255.0);
  uint8_t B8 = (uint8_t)((b + m) * 255.0);

  t_rgb tmp = { R8, G8, B8 };
  return pack_rgb565 (tmp);
}
