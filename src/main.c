#include <util/delay.h>

#include "GC9A01.h"
#include "main_screen.h"
#include "pinout.h"
#include "spi.h"
#include "state_machine.h"
#include "storage/fatfs.h"
#include "storage/sd_streaming.h"
#include "structs.h"
#include "uart.h"
#include "utils.h"

// #define CS_MAIN_LOW()  PORTH &= ~(1 << PH0);
// #define CS_MAIN_HIGH() PORTH |=(1 << PH0);
#define MAX_PIXEL_WIDTH 240
#define MAX_PIXEL_HIGH 320

t_state current_state = INIT;

void eyes_init() {
  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
  GC9A01_init(LEFT_EYE);
  GC9A01_init(RIGHT_EYE);
}

void screens_init() {
  DDRH |= (SCREENS_DC);
  main_screen_init();
  eyes_init();
}

void keyboard_init(void) {
  DDRA |= (KB_C1 | KB_C2 | KB_C3 | KB_C4 | KB_C5);
  DDRC |= (KB_R4);
  DDRG |= (KB_C6);
  DDRJ |= (KB_R1 | KB_R2 | KB_R3 | KB_C7 | KB_C8 | KB_C9 | KB_C10);

  PORTA &= ~(KB_C1 | KB_C2 | KB_C3 | KB_C4 | KB_C5);
  PORTC &= ~(KB_R4);
  PORTG &= ~(KB_C6);
  PORTJ &= ~(KB_R1 | KB_R2 | KB_R3 | KB_C7 | KB_C8 | KB_C9 | KB_C10);
}

void rotary_encoder_init(void) { DDRC |= (SDL_SW1 | SDL_SW2 | SDL_SW3); }

void init(void) {
  spi_master_init();
  uart_init(MYUBRR);

  _delay_ms(150);
  disk_initialize(0);  // SD Initialisation
  if (parse_mbr() == RES_OK) {
    if (parse_vbr() == RES_OK) {
      scan_root_dir();
    } else {
      uart_printstr("Failed to parse VBR\r\n");
    }
  } else {
    uart_printstr("Failed to parse MBR\r\n");
  }
  // Set SPI clock to highest speed after SD initialisation
  SPSR |= (1 << SPI2X);
  SPCR &= ~(1 << SPR0);
  SPCR &= ~(1 << SPR1);

  screens_init();

  // keyboard_init();
}

// ─── Text rendering
// ────────────────────────────────────────────────────────── ─── Text
// rendering ──────────────────────────────────────────────────────────

int main(void) {
  init();

  scan_root_dir();  // Populates image_lut with the BMP files on the card

  // dump_mbr();
  // dump_partition_entry(PART_ENTRY_NO_1);
  // uart_printstr("Image 0 is named: ");
  // uart_printstr(image_lut[0].name);
  // uart_printstr("\r\nAddress: ");
  // uart_printhex_32(image_lut[0].address);
  // uart_printstr("\r\n");
  // for (uint8_t i = 0; i < 2; i++) {
  // uart_printstr("Trying to print: ");
  // uart_printstr(image_lut[0].name);
  // uart_printstr(", located at: ");
  // uart_printhex_32(image_lut[0].address);
  // uart_printstr("\r\n");
  // sd_stream_bmp_to_screen(cluster_to_lba(image_lut[0].address));
  // uart_printstr("Finished !");
  // uart_printstr("\r\n");

  // _delay_ms(1000);

  uart_printstr("Trying to print: ");
  uart_printstr(image_lut[1].name);
  uart_printstr(", located at: ");
  uart_printhex_32(image_lut[1].address);
  uart_printstr("\r\n");
  sd_stream_bmp_to_screen(cluster_to_lba(image_lut[1].address));
  uart_printstr("Finished !");
  uart_printstr("\r\n");
  // }

  // BYTE sector[512] = {0};
  // DRESULT res = disk_read(0, sector, 0, 1);
  //
  // if (res == RES_OK) {
  //   dump_buf(sector, 512);
  // } else {
  //   uart_printstr("disk_read failed\r\n");
  // }

  // GC9A01_fillScreen_eyes(GC9A01A_COLOR_GREEN);
  // GC9A01_fillScreen(GC9A01A_COLOR_BLUE, RIGHT_EYE);
  // GC9A01_fillScreen(GC9A01A_COLOR_PINK, LEFT_EYE);

  // ili9488_fill_screen(GC9A01A_COLOR_RED);

  // draw_pixel(250, 320, GC9A01A_COLOR_GREEN);
  //
  // draw_string(30, 120, "Hello World !", GC9A01A_COLOR_GREEN,
  // GC9A01A_COLOR_RED,
  //             1, 1);
  // draw_string(30, 160, "Hello World !", GC9A01A_COLOR_GREEN,
  // GC9A01A_COLOR_RED,
  //             2, 2);  // MAX H CHARS
  // draw_string(30, 220, "Hello World !", GC9A01A_COLOR_GREEN,
  // GC9A01A_COLOR_RED,
  //             3, 3);  // MAX H CHARS
  // draw_string(30, 260, "Hello World !", GC9A01A_COLOR_GREEN,
  // GC9A01A_COLOR_RED,
  //             4, 4);  // MAX H CHARS

  while (1) {
    // GC9A01_blink(Eye_look_Right, 1);
    // GC9A01_blink(Eye_Front, 1);
    // for (uint8_t i = 32; i < 127; i++) {
    //   draw_char_small(120, 180, i, GC9A01A_COLOR_GREEN, GC9A01A_COLOR_RED,
    //   5);
    // _delay_ms(300);
    // }

    // testPINS();
  }
}
