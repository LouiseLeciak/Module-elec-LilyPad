#include "storage/sd_streaming.h"

#include "display/bmp.h"
#include "display/ili9488.h"
#include "display/main_screen.h"
#include "spi.h"
#include "storage/sd.h"

static uint8_t buf_header[512] = {0};
static uint8_t buf_img[512] = {0};

void sd_stream_bmp_to_screen(uint32_t start_sector) {
  sd_read_single_block(start_sector >> 24, start_sector >> 16,
                       start_sector >> 8, start_sector, buf_header);
  bmp_header header = {0};
  if (bmp_parse_header(buf_header, &header) != 0) {
    return;  // ERROR
  }
  uint32_t abs_height = (header.height < 0 ? -header.height : header.height);
  window win = {{0, 0}, {abs_height - 1, header.width - 1}};
  // window win = {{0, 0}, {header.width - 1, abs_height - 1}};
  MAIN_SCREEN_CS_LOW();
  main_screen_set_window(win);
  main_screen_ramwr();
  MAIN_SCREEN_CS_HIGH();

  uint32_t pixel_sector = start_sector + (header.pixel_data_offset / 512);
  uint16_t buffer_idx = header.pixel_data_offset % 512;

  uint32_t byte_count = (uint32_t)abs_height * header.width * 3;
  uint32_t bytes_processed = 0;

  uint8_t rgb[3];
  uint8_t rgb_idx = 0;

  while (bytes_processed < byte_count) {
    sd_read_single_block(pixel_sector >> 24, pixel_sector >> 16,
                         pixel_sector >> 8, pixel_sector, buf_img);
    pixel_sector++;
    SD_CS_HIGH();
    MAIN_SCREEN_CS_LOW();

    for (; buffer_idx < 512 && bytes_processed < byte_count; buffer_idx++) {
      rgb[rgb_idx++] = buf_img[buffer_idx];

      if (rgb_idx == 3) {
        spi_txrx(rgb[0] & 0xFC);  // B
        spi_txrx(rgb[1] & 0xFC);  // G
        spi_txrx(rgb[2] & 0xFC);  // R
        rgb_idx = 0;
      }
      bytes_processed++;
    }

    MAIN_SCREEN_CS_HIGH();
    SD_CS_LOW();
    buffer_idx = 0;
  }
}
