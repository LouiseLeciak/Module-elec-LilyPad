#ifndef MAIN_SCREEN_TEXT_H
#define MAIN_SCREEN_TEXT_H

#include "structs.h"

#define FONT_RES 8

void main_screen_draw_string(position pos, const char *str, const rgb fg,
                             const rgb bg, const uint8_t scale);

static uint16_t compute_max_char_per_line(const position pos,
                                          const uint8_t scale);
static uint8_t compute_nb_lines(const position pos, const char *str,
                                const uint8_t scale);
static uint16_t draw_n_char(position pos, const char *str,
                            const uint16_t *packed_colors, const uint8_t scale,
                            const uint16_t n);

#endif // !MAIN_SCREEN_TEXT_H
