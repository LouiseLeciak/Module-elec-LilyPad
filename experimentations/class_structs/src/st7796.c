#include "ST7796/st7796.h"

#include "ST7796/st7796_cmds.h"
#include <stdint.h>

// NOTE: I am puzzled about whether SPI transaction start/end should be set here
// or whether it should be even higher level. I'd wager it should be higher
// level even though instinctively I would want to put them here but what if we
// wanted to run several high/low level commands ? Doing the stqrt/end
// transaction here would just bounce the Slave Select SPI line.
// Still, I left the calls commented just in case, but I am pretty positive we
// will end up managing the transaction start/end by ourselves at a higher
// level. (apetitco)

void spi_master_transmit(const char data);

// --------- Drawing commands ------------------------------------------------
void draw_pixel(const position pos, const t_rgb rgb) {
  // ST7796_SS_LOW();

  window win = {pos._pos_x, pos._pos_x + 1, pos._pos_y, pos._pos_y + 1};
  set_window(win);
  st7796_ramwr();
  ST7796_DC_DATA();
  spi_master_transmit(pack_rgb565(rgb));

  // ST7796_SS_HIGH();
}

void draw_rectangle(const window win, const t_rgb rgb) {
  uint16_t color = pack_rgb565(rgb);

  // ST7796_SS_LOW();

  set_window(win);
  st7796_ramwr();
  ST7796_DC_DATA();
  for (uint32_t i = 0;
       i < (win._end_x - win._start_x) * (win._end_y - win._start_y); i++) {
    spi_master_transmit(color);
  }
  // ST7796_SS_HIGH();
}

// --------- Utilitaries commands --------------------------------------------
uint16_t pack_rgb565(const t_rgb colour) {
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

void set_window(const window win) {
  // ST7796_SS_LOW();
  st7796_caset(win._start_x, win._end_x);
  st7796_raset(win._start_y, win._end_y);
  // ST7796_SS_HIGH();
}
