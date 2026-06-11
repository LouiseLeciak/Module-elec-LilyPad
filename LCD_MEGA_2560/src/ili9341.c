#include "ili9341.h"

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "spi_lib.h"

void
ili9341_onoff_sequence ()
{
}

void
ili9341_cmd (uint8_t cmd)
{
  ILI9341_DC_LOW ();
  ILI9341_SS_LOW ();
  spi_master_transmit (cmd);
  ILI9341_SS_HIGH ();
}

void
ili9341_data (uint8_t data)
{
  ILI9341_DC_HIGH ();
  ILI9341_SS_LOW ();
  spi_master_transmit (data);
  ILI9341_SS_HIGH ();
}

void
ili9341_init (void)
{
  ILI9341_RST_LOW ();
  _delay_ms (100);
  ILI9341_RST_HIGH ();
  _delay_ms (100);
  ili9341_cmd (SOFTWARE_RESET);
  _delay_ms (150);
  ili9341_cmd (SLEEP_OUT);
  _delay_ms (150);
  ili9341_cmd (PIXEL_FORMAT_SET);

  // NOTE: I am confused because 0x55 is Write Content Adaptive Brightness
  // Control in the datasheet p.85
  ili9341_data (0x55); // 16-bit color (RGB565)

  ili9341_cmd (DISPLAY_ON);
}

void
set_window (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  ili9341_cmd (0x2A);
  ili9341_data (x0 >> 8);
  ili9341_data (x0 & 0xFF);
  ili9341_data (x1 >> 8);
  ili9341_data (x1 & 0xFF);
  ili9341_cmd (0x2B);
  ili9341_data (y0 >> 8);
  ili9341_data (y0 & 0xFF);
  ili9341_data (y1 >> 8);
  ili9341_data (y1 & 0xFF);
  ili9341_cmd (0x2C);
}

void
fill_screen (uint16_t colour)
{
  ILI9341_SS_LOW (); // Selects the slave
  ILI9341_DC_LOW (); // DC LOW = CMD
  spi_master_transmit (COL_ADDRESS_SET);
  ILI9341_DC_HIGH (); // DC HIGH = DATA
  spi_master_transmit (0x00);
  spi_master_transmit (0x00);
  spi_master_transmit (0x00);
  spi_master_transmit (0xEF);
  ILI9341_SS_HIGH ();

  ILI9341_SS_LOW (); // Selects the slave
  ILI9341_DC_LOW (); // DC LOW = CMD
  spi_master_transmit (PAGE_ADDRESS_SET);
  ILI9341_DC_HIGH (); // DC HIGH = DATA
  spi_master_transmit (0x00);
  spi_master_transmit (0x00);
  spi_master_transmit (0x01);
  spi_master_transmit (0x3F);
  ILI9341_SS_HIGH (); // Deselects the slave

  ILI9341_SS_LOW (); // Selects the slave
  ILI9341_DC_LOW (); // DC LOW = CMD
  spi_master_transmit (MEMORY_WRITE);
  ILI9341_DC_HIGH (); // DC HIGH = DATA

  for (uint32_t i = 0;
       i < (uint32_t)ILI9341_MAX_HEIGHT * (uint32_t)ILI9341_MAX_WIDTH; i++)
    {
      spi_master_transmit (colour >> 8);
      spi_master_transmit (colour & 0xFF);
    }
  ILI9341_SS_HIGH (); // Deselects the slave
}
