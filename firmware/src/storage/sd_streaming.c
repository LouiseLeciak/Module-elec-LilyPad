#include "storage/sd_streaming.h"

#include "display/bmp.h"
#include "display/main_screen.h"
#include "pinout.h"
#include "storage/fatfs.h"
#include "storage/sd.h"
#include "structs.h"
#include "system/globals.h"
#include "system/spi.h"
#include "system/uart.h"
#include "utils/utils.h"

#define LUT_INVALID_CLUSTER 0

static uint32_t lut_fetch(const char* filename)
{
  for (uint8_t i = 0; i < IMG_LUT_MAX_SIZE && image_lut[i].address != 0; i++)
  {
    if (!ft_strncmp(filename, image_lut[i].name, FILE_NAME_SIZE))
    {
#ifdef DEBUG
      uart_printstr("Queried file name: ");
      uart_printstr(filename);
      uart_printstr("--\r\nMATCH avec cette image la:\r\nName: ");
      uart_printstr(image_lut[i].name);
      uart_printstr("--\r\nAddress: ");
      uart_printhex_32(image_lut[i].address);
      uart_printstr("\r\n");
#endif /* ifdef DEBUG */
      return (image_lut[i].address);
    }
  }
  return (LUT_INVALID_CLUSTER);
}

static void push_pixels_to_screen(const uint8_t* buf, uint16_t from,
                                  uint8_t* rgb, uint8_t* rgb_idx,
                                  uint32_t* bytes_processed,
                                  uint32_t byte_count)
{
  SD_CS_HIGH();
  MAIN_SCREEN_CS_LOW();

  for (uint16_t i = from; i < BUF_SIZE && *bytes_processed < byte_count; i++)
  {
    rgb[(*rgb_idx)++] = buf[i];
    if (*rgb_idx == 3)
    {
      spi_txrx(rgb[2] & 0xFC);  // R
      spi_txrx(rgb[1] & 0xFC);  // G
      spi_txrx(rgb[0] & 0xFC);  // B
      *rgb_idx = 0;
    }
    (*bytes_processed)++;
  }
  MAIN_SCREEN_CS_HIGH();
  SD_CS_LOW();
}

static uint8_t stream_data_from_sd(uint32_t first_block)
{
  uint8_t read_buf[BUF_SIZE];
  uint8_t rgb[3];
  uint8_t rgb_idx = 0;
  uint32_t bytes_processed = 0;

  sd_resp start_resp = sd_read_multiple_block_start(
      first_block >> 24, first_block >> 16, first_block >> 8, first_block);
  if (start_resp.r1 != 0x00)
  {
    uart_printstr("SD: CMD18 failed\r\n");
    SD_CS_HIGH();
    return (1);
  }

  if (sd_read_multiple_block_next(read_buf))
  {
    uart_printstr("Error while printing image\r\n");
    sd_read_multiple_block_stop();
    return (1);
  }
  bmp_header header = {0};
  if (bmp_parse_header(read_buf, &header) != 0)
  {
    uart_printstr("Error while printing image\r\n");
    sd_read_multiple_block_stop();
    return (1);
  }

  if (header.height == INT32_MIN)
  {
    uart_printstr("BMP: invalid height\r\n");
    sd_read_multiple_block_stop();
    return (1);
  }

  uint32_t abs_height = (header.height < 0 ? (uint32_t)(-header.height)
                                           : (uint32_t)header.height);
  if (header.width == 0 || abs_height == 0 ||
      header.width > MAIN_SCREEN_WIDTH || abs_height > MAIN_SCREEN_HEIGHT)
  {
    uart_printstr("BMP: dimensions out of range\r\n");
    sd_read_multiple_block_stop();
    return (1);
  }

  window win = {{0, 0}, {abs_height - 1, header.width - 1}};
  uint32_t byte_count = (uint32_t)abs_height * header.width * 3;

#ifdef DEBUG
  uart_printstr("Starting printing image\r\n");
#endif /* ifdef DEBUG */

  SD_CS_HIGH();
  MAIN_SCREEN_CS_LOW();
  main_screen_set_window(win);
  main_screen_ramwr();
  MAIN_SCREEN_CS_HIGH();
  SD_CS_LOW();

  uint16_t first_pixel_idx = header.pixel_data_offset % 512;
  push_pixels_to_screen(read_buf, first_pixel_idx, rgb, &rgb_idx,
                        &bytes_processed, byte_count);

  while (bytes_processed < byte_count)
  {
    if (sd_read_multiple_block_next(read_buf))
    {
      uart_printstr("Error while printing image\r\n");
      sd_read_multiple_block_stop();
      return (1);
    }
    push_pixels_to_screen(read_buf, 0, rgb, &rgb_idx, &bytes_processed,
                          byte_count);
  }
#ifdef DEBUG
  uart_printstr("Finished printing image\r\n");
#endif /* ifdef DEBUG */

  sd_read_multiple_block_stop();
  return (0);
}

uint8_t display_sd_image(const char* filename, const device_screen target)
{
  // TODO: Add variations to be able to print
  (void)target;
  uint32_t img_address = lut_fetch(filename);
  if (img_address == LUT_INVALID_CLUSTER)
  {
    uart_printstr("No image found for: ");
    uart_printstr(filename);
    uart_printstr("\r\n");
    return (1);
  }

  if (stream_data_from_sd(cluster_to_lba(img_address)) != 0)
  {
    return (1);
  }
  return (0);
}
