#include "st7796_cmds.h"

#include <util/delay.h>

#include "st7796.h"

void spi_master_transmit (const char data);

void
st7796_swreset (void)
{
  ST7796_DC_COMMAND ();
  spi_master_transmit (SWRESET);
}

void
st7796_slpin (void)
{
  ST7796_DC_COMMAND ();
  spi_master_transmit (SLPIN);
  _delay_ms (5); // See 9.2.12 (p.159), Restrictions, paragraph 2
}

void
st7796_slpout (void)
{
  ST7796_DC_COMMAND ();
  spi_master_transmit (SLPOUT);
  _delay_ms (120); // See 9.2.13 (p.161), Restrictions, paragraph 3
}

// TODO: Maybe leverage a ST7796 struct to fill in the width and height of the
// screen so we can check if col_start/col_end are [0,<SCREEN WIDTH>[
void
st7796_caset (const uint16_t col_start, const uint16_t col_end)
{
  ST7796_DC_COMMAND ();
  spi_master_transmit (CASET);

  ST7796_DC_DATA ();
  spi_master_transmit (col_start >> 8);
  spi_master_transmit (col_start & 0xFF);
  spi_master_transmit (col_end >> 8);
  spi_master_transmit (col_end & 0xFF);
}

// TODO: Maybe leverage a ST7796 struct to fill in the width and height of the
// screen so we can check if row_start/row_end are [0,<SCREEN HEIGHT>[
void
st7796_raset (const uint16_t row_start, const uint16_t row_end)
{
  ST7796_DC_COMMAND ();
  spi_master_transmit (RASET);

  ST7796_DC_DATA ();
  spi_master_transmit (row_start >> 8);
  spi_master_transmit (row_start & 0xFF);
  spi_master_transmit (row_end >> 8);
  spi_master_transmit (row_end & 0xFF);
}

void
st7796_ramwr (void)
{
  ST7796_DC_COMMAND ();
  spi_master_transmit (RAMWR);
}
