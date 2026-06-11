#include "ili9341_cmds.h"

#include "ili9341.h"
#include "uart_lib.h"

void
ili9341_software_reset (void)
{
  ili9341_cmd (SOFTWARE_RESET);
}

void
ili9341_sleep_out (void)
{
  ili9341_cmd (SLEEP_OUT);
}

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

// NOTE: Page 134 of the datasheet explains the arguments you can pass as
// data to the Pixel Format Set command. Control in the datasheet p.134
// Here is a representation of the bits for the parameters:
// 7  6    5    4   3  2    1    0
// 0 DPI2 DPI1 DPI0 0 DBI2 DBI1 DBI0
void
ili9341_pixel_format_set (uint8_t data)
{
  // NOTE: We use the MCU interface, which is the DBI, so only the parameters
  // for the DBI are relevant.

  // Possible parameters for the DBI:
  // DBI2 DBI1 DBI0
  //  1    0    1  = 16 bits per pixel
  //  1    1    0  = 18 bits per pixel

  // 0x07 is a mask for the first 3 bits, corresponding to the DBI part of the
  // parameters.
  if (((data & 0x07) == 0))
    {
      uart_printstr (
          "Invalid DBI argument for command 'Pixel Format Set'\r\n");
    }
  // 0x70 is a mask for the last 3 bits, corresponding to the DPI part of the
  // parameters.
  if (((data & 0x70)))
    {
      uart_printstr (
          "Invalid DPI argument for command 'Pixel Format Set'\r\n");
    }
  ili9341_cmd (PIXEL_FORMAT_SET);
  ili9341_data (&data, 1);
}
