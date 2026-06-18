#include <stdint.h>
#include <string.h>

#include "fonts/mads.h"
#include "main_screen.h"
#include "main_screen_text.h"
#include "pins.h"
#include "spi.h"

void main_screen_draw_string(position pos, const char *str, const rgb fg,
                             const rgb bg, const uint8_t scale) {

  uint16_t packed_colors[2] = {pack_rgb565(fg), pack_rgb565(bg)};
  uint16_t n = compute_max_char_per_line(pos, scale);
  uint8_t lines = compute_nb_lines(pos, str, scale);
  const char *runner = str;

  for (uint8_t current_line = 0; current_line < lines; current_line++) {
    uint16_t printed = draw_n_char(pos, runner, packed_colors, scale, n);
    pos._pos_x += FONT_RES * scale;
    runner += printed;
  }
}

static uint16_t compute_max_char_per_line(const position pos,
                                          const uint8_t scale) {
  return (MAIN_SCREEN_WIDTH - pos._pos_y) / (scale * FONT_RES);
}

static uint8_t compute_nb_lines(const position pos, const char *str,
                                const uint8_t scale) {
  const uint16_t max_char_per_line = compute_max_char_per_line(pos, scale);
  const uint16_t len = strlen(str);
  if (max_char_per_line == 0)
    return (0);

  uint16_t nb_lines = len / max_char_per_line;

  if (len % max_char_per_line != 0) // Catches any "spillover" lines
    nb_lines += 1;

  return (nb_lines);
}

static uint8_t find_glyph(const char c) {
  uint8_t ret = c;
  if (ret < ' ' || ret > 'Z')
    ret = ' ';
  return (ret - ' ');
}

static void draw_char(const char c, const uint8_t row,
                      const uint16_t *packed_colors, const uint8_t scale) {
  uint8_t font_index = find_glyph(c);
  uint8_t data = mads8x8[font_index][row];

  for (uint8_t col = 0; col < FONT_RES; col++) {
    uint16_t color =
        ((~data) & (0x01 << col)) ? packed_colors[0] : packed_colors[1];
    for (uint8_t scale_col = 0; scale_col < scale; scale_col++) {
      spi_master_transmit(color >> 8);
      spi_master_transmit(color & 0xFF);
    } // !scale_col loop
  } // !col loop
}

static uint16_t draw_n_char(position pos, const char *str,
                            const uint16_t *packed_colors, const uint8_t scale,
                            const uint16_t n) {
  uint16_t ret = strnlen(str, n);

  const window win = {pos,
                      {pos._pos_x + (FONT_RES * scale) - 1,
                       pos._pos_y + (ret * FONT_RES * scale) - 1}};

  main_screen_set_window(win);
  main_screen_ramwr();
  MAIN_SCREEN_DC_DATA();
  for (uint16_t row = 0; row < FONT_RES; row++) {
    for (uint8_t scale_row = 0; scale_row < scale; scale_row++) {
      for (uint16_t i = 0; i < ret; i++) {
        draw_char(str[i], row, packed_colors, scale);
      } //! i loop
    } // !scale_row loop
  } // !row loop
  return (ret);
}
