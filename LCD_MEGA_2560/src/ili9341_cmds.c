#include "ili9341_cmds.h"

#include "ili9341.h"

void

ili9341_col_address_set (const uint16_t col_start, const uint16_t col_end)
{
  // We break the two 16 bits variables into four 8 bits values in order to be
  // able to send them over spi through the ili9342_data function.
  uint8_t data[4]
      = { col_start >> 8, col_start & 0xFF, col_end >> 8, col_end & 0xFF };

  ili9341_cmd (COL_ADDRESS_SET);
  ili9341_data (data, 4);
}

void
ili9341_page_address_set (const uint16_t page_start, const uint16_t page_end)
{
  // We break the two 16 bits variables into four 8 bits values in order to be
  // able to send them over spi through the ili9342_data function.
  uint8_t data[4]
      = { page_start >> 8, page_start & 0xFF, page_end >> 8, page_end & 0xFF };

  ili9341_cmd (PAGE_ADDRESS_SET);
  ili9341_data (data, 4);
}

void
ili9341_memory_write (void)
{
  ILI9341_DC_LOW (); // DC LOW = CMD
  spi_master_transmit (MEMORY_WRITE);
}
