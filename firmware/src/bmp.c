#include "bmp.h"

#include "uart.h"
#include "utils.h"

uint8_t bmp_parse_header(uint8_t* buf, bmp_header* header) {
  *header = *(bmp_header*)(buf);

  bmp_dump_header(header);

  uart_printstr("File signature: ");
  uart_printhex_32(header->signature);
  uart_printstr("\r\n");
  if (header->signature != 0x4D42) {
    uart_printstr("Bitmap file got invalid signature.\r\n");
    return 1;
  }
  if (header->bpp != 24) {
    uart_printstr("Bit per pixel: ");
    uart_printhex(header->bpp);
    uart_printstr("\r\n");
    uart_printstr("Bitmap file got invalid bits-per-pixel value.\r\n");
    return 2;
  }
  if (header->compression != 0) {
    uart_printstr("Bitmap file got invalid compression level.\r\n");
    return 3;
  }
  return 0;
}

void bmp_dump_header(bmp_header* header) {
  uart_printstr("--- BMP Header Dump ---\r\n");

  uart_printstr("Signature        : 0x");
  uart_printhex_32(header->signature);
  uart_printstr("\r\n");

  uart_printstr("File size        : 0x");
  uart_printhex_32(header->file_size);
  uart_printstr("\r\n");

  uart_printstr("Reserved         : 0x");
  uart_printhex_32(header->reserved);
  uart_printstr("\r\n");

  uart_printstr("Pixel data offset: 0x");
  uart_printhex_32(header->pixel_data_offset);
  uart_printstr("\r\n");

  uart_printstr("DIB header size  : 0x");
  uart_printhex_32(header->dib_header_size);
  uart_printstr("\r\n");

  uart_printstr("Width            : 0x");
  uart_printhex_32(header->width);
  uart_printstr("\r\n");

  uart_printstr("Height           : 0x");
  uart_printhex_32((uint32_t)header->height);
  uart_printstr("\r\n");

  uart_printstr("Planes           : 0x");
  uart_printhex_32(header->planes);
  uart_printstr("\r\n");

  uart_printstr("Bits per pixel   : 0x");
  uart_printhex_32(header->bpp);
  uart_printstr("\r\n");

  uart_printstr("Compression      : 0x");
  uart_printhex_32(header->compression);
  uart_printstr("\r\n");

  uart_printstr("-----------------------\r\n");
}
