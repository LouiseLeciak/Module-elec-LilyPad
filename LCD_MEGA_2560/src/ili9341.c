#include "ili9341.h"

#include <util/delay.h>

#include "ili9341_cmds.h"

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
  // _delay_ms (150);
  ili9341_cmd (SLEEP_OUT);
  // _delay_ms (150);
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
