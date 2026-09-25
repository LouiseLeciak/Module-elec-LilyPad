#include "bmp.h"

#include "uart.h"
#include "utils.h"

uint8_t bmp_parse_header(uint8_t* buf, bmp_header* header) {
  *header = *(bmp_header*)(buf);

  uart_printstr("File signature: ");
  uart_printhex_32(header->signature);
  uart_printstr("\r\n");
  // if (header->signature != 0x4D42) {
  //   uart_printstr("Bitmap file got invalid signature.\r\n");
  //   return 1;
  // }
  // if (header->bpp != 24) {
  //   uart_printstr("Bit per pixel: ");
  //   uart_printhex(header->bpp);
  //   uart_printstr("\r\n");
  //   uart_printstr("Bitmap file got invalid bits-per-pixel value.\r\n");
  //   return 2;
  // }
  // if (header->compression != 0) {
  //   uart_printstr("Bitmap file got invalid compression level.\r\n");
  //   return 3;
  // }
  return 0;
}
