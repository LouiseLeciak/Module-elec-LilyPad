#include "st7796.h"
#include "st7796_cmds.h"

void spi_master_transmit(const char data);

// TODO: Maybe leverage a ST7796 struct to fill in the width and height of the
// screen so we can check if col_start/col_end are [0,<SCREEN WIDTH>[
void st7796_caset(const uint16_t col_start, const uint16_t col_end) {
  ST7796_DC_COMMAND();
  spi_master_transmit(CASET);

  ST7796_DC_DATA();
  spi_master_transmit(col_start >> 8);
  spi_master_transmit(col_start & 0xFF);
  spi_master_transmit(col_end >> 8);
  spi_master_transmit(col_end & 0xFF);
}

// TODO: Maybe leverage a ST7796 struct to fill in the width and height of the
// screen so we can check if row_start/row_end are [0,<SCREEN HEIGHT>[
void st7796_raset(const uint16_t row_start, const uint16_t row_end) {
  ST7796_DC_COMMAND();
  spi_master_transmit(RASET);

  ST7796_DC_DATA();
  spi_master_transmit(row_start >> 8);
  spi_master_transmit(row_start & 0xFF);
  spi_master_transmit(row_end >> 8);
  spi_master_transmit(row_end & 0xFF);
}

void st7796_ramwr(void) {
  ST7796_DC_COMMAND();
  spi_master_transmit(RAMWR);
}
