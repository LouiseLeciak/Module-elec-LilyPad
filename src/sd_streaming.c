#include "sd_streaming.h"

#include <stdint.h>

#include "bmp.h"
#include "ili9488.h"
#include "main_screen.h"
#include "pinout.h"
#include "sd.h"
#include "spi.h"

uint8_t buf_header[512] = {0};
uint8_t buf_img[512] = {0};

void sd_stream_bmp_to_screen(uint32_t start_sector) {
  sd_read_single_block(start_sector >> 24, start_sector >> 16,
                       start_sector >> 8, start_sector, buf_header);
  bmp_header header = {0};
  if (bmp_parse_header(buf_header, &header) != 0) {
    return;  // ERROR
  }
  uint32_t abs_height = (header.height < 0 ? -header.height : header.height);
  window win = {{0, 0}, {header.width, abs_height}};
  main_screen_set_window(win);
  main_screen_ramwr();

  uint32_t pixel_sector = start_sector + (header.pixel_data_offset / 512);
  sd_read_multiple_block_start(pixel_sector >> 24, pixel_sector >> 16,
                               pixel_sector >> 8, pixel_sector);

  uint32_t total_bytes = (uint32_t)abs_height * header.width * 3;
  uint32_t total_sectors = (total_bytes + 511) / 512;
  for (uint32_t i = 0; i < total_sectors; i++) {
    sd_read_multiple_block_next(buf_img);
    SD_CS_HIGH();
    MAIN_SCREEN_CS_LOW();
    for (uint16_t j = 0; j + 2 < 512; j += 3) {
      uint8_t b = buf_img[j];
      uint8_t g = buf_img[j + 1];
      uint8_t r = buf_img[j + 2];
      spi_txrx(r & 0xFC);
      spi_txrx(g & 0xFC);
      spi_txrx(b & 0xFC);
    }
    MAIN_SCREEN_CS_HIGH();
    SD_CS_LOW();
  }

  sd_read_multiple_block_stop();
}
