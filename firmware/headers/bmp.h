#ifndef BMP_H
#define BMP_H

// NOTE: Bitmap header description:
// https://en.wikipedia.org/wiki/BMP_file_format#File_header

#include <avr/io.h>

typedef struct __attribute__((packed)) {
  uint16_t signature;
  uint32_t file_size;
  uint32_t reserved;
  uint32_t pixel_data_offset;
  uint32_t dib_header_size;
  uint32_t width;
  int32_t height;
  uint16_t planes;
  uint16_t bpp;
  uint32_t compression;
} bmp_header;

uint8_t bmp_parse_header(uint8_t* buf, bmp_header* header);
void bmp_dump_header(bmp_header* header);

#endif  // !BMP_H
