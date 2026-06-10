#include "ili9341.h"

#include <avr/io.h>
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
