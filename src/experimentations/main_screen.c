#include "main_screen.h"

#include <util/delay.h>

#include "ili9488.h"
#include "spi.h"
#include "structs.h"

static inline void dc_cmd(void) { DC_PORT &= ~(DC_PIN); }
static inline void dc_data(void) { DC_PORT |= (DC_PIN); }
static inline void cs_low(void) { CS_PORT &= ~(CS_PIN); }
static inline void cs_high(void) { CS_PORT |= (CS_PIN); }

// NOTE: I am puzzled about whether SPI transaction start/end should be set
// here or whether it should be even higher level. I'd wager it should be
// higher level even though instinctively I would want to put them here but
// what if we wanted to run several high/low level commands ? Doing the
// stqrt/end transaction here would just bounce the Slave Select SPI line.
// Still, I left the calls commented just in case, but I am pretty positive we
// will end up managing the transaction start/end by ourselves at a higher
// level. (apetitco)

// --- HIGH LEVEL COMMANDS -----------------------------------------------------

// Setup commands
void main_screen_init() {
  CS_DDR |= (CS_PIN);
  PORTH |= (CS_PIN);

  DC_DDR |= (DC_PIN);
  DC_PORT |= (DC_PIN);

  RST_DDR |= (RST_PIN);
  RST_PORT |= (RST_PIN);

  BL_DDR |= (BL_PIN);
  BL_PORT |= (BL_PIN);

  ili9488_reset();
  ili9488_init_driver();
}

// ------ Drawing commands ------------------------------------------------
void ili9488_fill_screen(uint16_t color565) {
  // Convertit le RGB565 en RGB666 (18-bit), format attendu par
  // l'ILI9486 sur son interface SPI : chaque composante sur 6 bits
  // utiles, alignée dans les bits hauts d'un octet.
  uint8_t r = ((color565 >> 11) & 0x1F) << 3; // 5 bits -> 8 bits (bits hauts)
  uint8_t g = ((color565 >> 5) & 0x3F) << 2;  // 6 bits -> 8 bits (bits hauts)
  uint8_t b = (color565 & 0x1F) << 3;         // 5 bits -> 8 bits (bits hauts)

  // Column Address Set (CASET, 0x2A)
  cs_low();
  dc_cmd();
  spi_txrx(0x2A);
  dc_data();
  spi_txrx(0x00);
  spi_txrx(0x00);
  spi_txrx(0x01);
  spi_txrx(0x3F);
  cs_high();

  // Page/Row Address Set (PASET, 0x2B)
  cs_low();
  dc_cmd();
  spi_txrx(0x2B);
  dc_data();
  spi_txrx(0x00);
  spi_txrx(0x00);
  spi_txrx(0x01);
  spi_txrx(0xDF);
  cs_high();

  // Memory Write (RAMWR, 0x2C) — 3 octets par pixel maintenant
  cs_low();
  dc_cmd();
  spi_txrx(0x2C);
  dc_data();
  uint32_t n_pixels = 480UL * 320UL;
  for (uint32_t i = 0; i < n_pixels; i++) {
    spi_txrx(r);
    spi_txrx(g);
    spi_txrx(b);
  }
  cs_high();
}

void main_screen_draw_pixel(const position pos, const rgb rgb) {
  window win = {{pos._pos_x, pos._pos_y}, {pos._pos_x + 1, pos._pos_y + 1}};
  main_screen_set_window(win);
  main_screen_ramwr();
  dc_data();
  spi_txrx(pack_rgb565(rgb));
}

void main_screen_draw_rectangle(const window win, const rgb rgb) {
  // uint16_t color = pack_rgb565(rgb);

  main_screen_set_window(win);
  main_screen_ramwr();
  dc_data();
  for (uint32_t i = 0; i < (win._end._pos_x - win._start._pos_x + 1) *
                               (win._end._pos_y - win._start._pos_y + 1);
       i++) {
    spi_txrx(rgb._red & 0xFC);
    spi_txrx(rgb._green & 0xFC);
    spi_txrx(rgb._blue & 0xFC);
  }
}

// ------ Utilitaries commands --------------------------------------------
// NOTE: Interesting bit on 16-bit pixel SPI transmission at MAIN_SCREEN's
// datasheet p.88
uint16_t pack_rgb565(const rgb colour) {
  // For the red, we need to isolate the 5 most significant bits:
  //  1 1 1 1 | 1 0 0 0 (the `1` indicate the most significant bits)
  //  As the first half is full of 1, we know our mask starts with 0xF
  //  Then, only a 1 for 2^3=8, so the end of the mask is 8
  //  The mask thus is 0xF8

  // For the green, we need to isolate the 6 most significant bits:
  //  1 1 1 1 | 1 1 0 0 (the `1` indicate the most significant bits)
  //  As the first half is full of 1, we know our mask starts with 0xF
  //  Then, only a 1 for 2^3=8 and 2^2=4, so the end of the mask is 12 in
  //  decimal and C in hex The mask thus is 0xFC

  // For the blue we just bitshift 3 times to the right as it would be
  // essentially the same as applying 0xF8 on it.

  // In the end, this is the structure of the data to send :
  // R  R  R  R  R  G  G G G G G B B B B B
  // 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
  return (((colour._red & 0xF8) << 8) | ((colour._green & 0xFC) << 3) |
          (colour._blue >> 3));
}

uint16_t pack_rgb666(const rgb colour) {
  // For the red, we need to isolate the 5 most significant bits:
  //  1 1 1 1 | 1 0 0 0 (the `1` indicate the most significant bits)
  //  As the first half is full of 1, we know our mask starts with 0xF
  //  Then, only a 1 for 2^3=8, so the end of the mask is 8
  //  The mask thus is 0xF8

  // For the green, we need to isolate the 6 most significant bits:
  //  1 1 1 1 | 1 1 0 0 (the `1` indicate the most significant bits)
  //  As the first half is full of 1, we know our mask starts with 0xF
  //  Then, only a 1 for 2^3=8 and 2^2=4, so the end of the mask is 12 in
  //  decimal and C in hex The mask thus is 0xFC

  // For the blue we just bitshift 3 times to the right as it would be
  // essentially the same as applying 0xF8 on it.

  // In the end, this is the structure of the data to send :
  // R  R  R  R  R  G  G G G G G B B B B B
  // 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
  return (((colour._red & 0xF8) << 8) | ((colour._green & 0xFC) << 3) |
          (colour._blue >> 3));
}

void main_screen_set_window(const window win) {
  main_screen_caset(win._start._pos_y, win._end._pos_y);
  main_screen_raset(win._start._pos_x, win._end._pos_x);
}

// --- LOW LEVEL COMMANDS
// ------------------------------------------------------
void main_screen_swreset(void) {
  dc_data();
  spi_txrx(SWRESET);
}

void main_screen_slpin(void) {
  dc_data();
  spi_txrx(SLPIN);
  _delay_ms(5); // See 9.2.12 (p.159), Restrictions, paragraph 2
}

void main_screen_slpout(void) {
  dc_data();
  spi_txrx(SLPOUT);
  _delay_ms(120); // See 9.2.13 (p.161), Restrictions, paragraph 3
}

void main_screen_dispon(void) {
  dc_data();
  spi_txrx(DISPON);
}

// TODO: Maybe leverage a MAIN_SCREEN struct to fill in the width and height
// of the screen so we can check if col_start/col_end are [0,<SCREEN WIDTH>[
void main_screen_caset(const uint16_t col_start, const uint16_t col_end) {
  dc_data();
  spi_txrx(CASET);

  dc_data();
  spi_txrx(col_start >> 8);
  spi_txrx(col_start & 0xFF);
  spi_txrx(col_end >> 8);
  spi_txrx(col_end & 0xFF);
}

// TODO: Maybe leverage a MAIN_SCREEN struct to fill in the width and height
// of the screen so we can check if row_start/row_end are [0,<SCREEN HEIGHT>[
void main_screen_raset(const uint16_t row_start, const uint16_t row_end) {
  dc_data();
  spi_txrx(RASET);

  dc_data();
  spi_txrx(row_start >> 8);
  spi_txrx(row_start & 0xFF);
  spi_txrx(row_end >> 8);
  spi_txrx(row_end & 0xFF);
}

void main_screen_ramwr(void) {
  dc_data();
  spi_txrx(RAMWR);
}

// For a table of the different parameters for this command, refer to table at
// p.183
void main_screen_madctl(const uint8_t arg) {
  dc_data();
  spi_txrx(MADCTL);
  dc_data();
  spi_txrx(arg);
}

void main_screen_colmod(const colmod_arg arg) {
  dc_data();
  spi_txrx(COLMOD);
  dc_data();
  spi_txrx(arg);
}
