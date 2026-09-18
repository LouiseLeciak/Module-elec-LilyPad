#include <util/delay.h>

#include "GC9A01.h"
#include "main_screen.h"
#include "pinout.h"
#include "screen_text.h"
#include "sd.h"
#include "spi.h"
#include "state_machine.h"
#include "uart.h"

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

  sd_init();
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

  // GC9A01_fillScreen_eyes(GC9A01A_COLOR_GREEN);
  // GC9A01_fillScreen(GC9A01A_COLOR_BLUE, RIGHT_EYE);
  // GC9A01_fillScreen(GC9A01A_COLOR_PINK, LEFT_EYE);

  ili9488_fill_screen(GC9A01A_COLOR_RED);

  draw_pixel(250, 320, GC9A01A_COLOR_GREEN);

  draw_string(30, 120, "Hello World !", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_RED,
              1, 1);
  draw_string(30, 160, "Hello World !", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_RED,
              2, 2);  // MAX H CHARS
  draw_string(30, 220, "Hello World !", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_RED,
              3, 3);  // MAX H CHARS
  draw_string(30, 260, "Hello World !", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_RED,
              4, 4);  // MAX H CHARS

  while (1) {
    // GC9A01_blink(Eye_look_Right, 1);
    // GC9A01_blink(Eye_Front, 1);
    for (uint8_t i = 32; i < 127; i++) {
      draw_char_small(120, 180, i, GC9A01A_COLOR_GREEN, GC9A01A_COLOR_RED, 5);
      // _delay_ms(300);
    }

    ;

    // testPINS();
  }
}
