#include "main_screen.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>

#include "fonts/mads.h"
#include "pins.h"

// WARNING: Placeholder until we implement SPI.
void spi_master_transmit(uint8_t data);

// NOTE: I am puzzled about whether SPI transaction start/end should be set
// here or whether it should be even higher level. I'd wager it should be
// higher level even though instinctively I would want to put them here but
// what if we wanted to run several high/low level commands ? Doing the
// stqrt/end transaction here would just bounce the Slave Select SPI line.
// Still, I left the calls commented just in case, but I am pretty positive we
// will end up managing the transaction start/end by ourselves at a higher
// level. (apetitco)

// --- HIGH LEVEL COMMANDS -----------------------------------------------------
// ------ Setup commands -------------------------------------------------------
void main_screen_init(void) {
  MAIN_SCREEN_RST_LOW();
  _delay_ms(100);
  MAIN_SCREEN_RST_HIGH();
  _delay_ms(100);
  main_screen_swreset();
  _delay_ms(5);
  main_screen_slpout();
  _delay_ms(5);
  main_screen_madctl(0x48);  // Not really elegant for now*.
  main_screen_colmod(BIT_16);
  main_screen_dispon();

  // *: The only bits that are different from its standard value are bit 3 and
  // bit 7.
  // Bit 3: Set to 1 so as to send the data in BGR as the LCD panel we
  // have (HSD-9190J-B3) seems to be wired with Red and Blue inverted.
  // Bit 7:
  // Set to 1 so as to have coorinate 0 be left-hand side of the screen instead
  // of right-hand side.
}

// ------ Drawing commands ------------------------------------------------
void main_screen_draw_pixel(const position pos, const rgb rgb) {
  window win = {{pos._pos_x, pos._pos_y}, {pos._pos_x + 1, pos._pos_y + 1}};
  main_screen_set_window(win);
  main_screen_ramwr();
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(pack_rgb565(rgb));
}

void main_screen_draw_rectangle(const window win, const rgb rgb) {
  uint16_t color = pack_rgb565(rgb);

  main_screen_set_window(win);
  main_screen_ramwr();
  MAIN_SCREEN_DC_DATA();
  for (uint32_t i = 0; i < (win._end._pos_x - win._start._pos_x + 1) *
                               (win._end._pos_y - win._start._pos_y + 1);
       i++) {
    spi_master_transmit(color >> 8);
    spi_master_transmit(color & 0xFF);
  }
}

void main_screen_draw_string(position pos,
                             const char* str,
                             const rgb fg,
                             const rgb bg,
                             const uint8_t scale) {
  uint16_t len = strlen(str);  // WARNING: Must make a homemade version.
  uint8_t kerning = 1 * scale;
  uint16_t total_width = len * ((8 * scale) + kerning);

  window win = {pos,
                {pos._pos_x + (8 * scale) - 1, pos._pos_y + total_width - 1}};

  main_screen_set_window(win);

  uint16_t packed_fg = pack_rgb565(fg);
  uint16_t packed_bg = pack_rgb565(bg);

  main_screen_ramwr();
  MAIN_SCREEN_DC_DATA();

  for (uint8_t row = 0; row < 8; row++) {
    for (uint8_t scale_y = 0; scale_y < scale; scale_y++) {
      for (uint16_t i = 0; i < len; i++) {
        char c = str[i];

        if (c < ' ' || c > 'Z')
          c = ' ';

        uint16_t font_index = c - ' ';
        uint8_t row_data = mads8x8[font_index][row];
        for (uint8_t col = 0; col < 8; col++) {
          uint16_t color =
              ((~row_data) & (0x01 << col)) ? packed_fg : packed_bg;
          for (uint8_t scale_x = 0; scale_x < scale; scale_x++) {
            spi_master_transmit(color >> 8);
            spi_master_transmit(color & 0xFF);
          }  // End of scale_x loop
        }  // End of col loop
        for (uint8_t k = 0; k < kerning; k++) {
          spi_master_transmit(packed_bg >> 8);
          spi_master_transmit(packed_bg & 0xFF);
        }  // End of kerning loop
      }  // End of srt[i] loop
    }  // End of scale_y loop
  }  // End of row loop
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
  return (((colour.red & 0xF8) << 8) | ((colour.green & 0xFC) << 3) |
          (colour.blue >> 3));
}

void main_screen_set_window(const window win) {
  main_screen_caset(win._start._pos_y, win._end._pos_y);
  main_screen_raset(win._start._pos_x, win._end._pos_x);
}

// --- LOW LEVEL COMMANDS ------------------------------------------------------
void main_screen_swreset(void) {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(SWRESET);
}

void main_screen_slpin(void) {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(SLPIN);
  _delay_ms(5);  // See 9.2.12 (p.159), Restrictions, paragraph 2
}

void main_screen_slpout(void) {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(SLPOUT);
  _delay_ms(120);  // See 9.2.13 (p.161), Restrictions, paragraph 3
}

void main_screen_dispon(void) {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(DISPON);
}

// TODO: Maybe leverage a MAIN_SCREEN struct to fill in the width and height of
// the screen so we can check if col_start/col_end are [0,<SCREEN WIDTH>[
void main_screen_caset(const uint16_t col_start, const uint16_t col_end) {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(CASET);

  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(col_start >> 8);
  spi_master_transmit(col_start & 0xFF);
  spi_master_transmit(col_end >> 8);
  spi_master_transmit(col_end & 0xFF);
}

// TODO: Maybe leverage a MAIN_SCREEN struct to fill in the width and height of
// the screen so we can check if row_start/row_end are [0,<SCREEN HEIGHT>[
void main_screen_raset(const uint16_t row_start, const uint16_t row_end) {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(RASET);

  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(row_start >> 8);
  spi_master_transmit(row_start & 0xFF);
  spi_master_transmit(row_end >> 8);
  spi_master_transmit(row_end & 0xFF);
}

void main_screen_ramwr(void) {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(RAMWR);
}

// For a table of the different parameters for this command, refer to table at
// p.183
void main_screen_madctl(const uint8_t arg) {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(MADCTL);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(arg);
}

void main_screen_colmod(const colmod_arg arg) {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(COLMOD);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(arg);
}
