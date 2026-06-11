#include "ili9341.h"

#include <stdint.h>
#include <util/delay.h>

#include "ili9341_cmds.h"
#include "small_font.h"
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
  ili9341_cmd (SOFTWARE_RESET);
  ili9341_cmd (SLEEP_OUT);
  ili9341_cmd (PIXEL_FORMAT_SET);

  // NOTE: I am confused because 0x55 is Write Content Adaptive Brightness
  // Control in the datasheet p.85
  uint8_t tmp = 0x55;
  ili9341_data (&tmp, 1); // 16-bit color (RGB565)

  ili9341_cmd (DISPLAY_ON);
  ILI9341_SS_HIGH ();
}

void
fill_screen (uint16_t colour)
{
  ILI9341_SS_LOW ();
  ili9341_col_address_set (0x0000, 0x00EF);
  ili9341_page_address_set (0x0000, 0x013F);

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

// void
// draw_char_small (uint16_t pos_x, uint16_t pos_y, uint8_t c, uint8_t size,
//                  uint16_t fg, uint16_t bg)
// {
//   const volatile uint8_t *glyph = font5x7[(uint8_t)c - 32];
//
//   if (c < 32 || c > 126)
//     c = '?';
// }
