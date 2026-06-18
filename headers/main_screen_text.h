#ifndef MAIN_SCREEN_TEXT_H
#define MAIN_SCREEN_TEXT_H

#include "structs.h"

#define FONT_RES 8

/**
 * @brief Draws the provided string on the main screen (ST7796), function
 * includes text wrapping in case the string can't fit on one line only.
 *
 * @param pos Origin position of the string on the screen.
 * @param str String that will be drawn.
 * @param fg Foreground (text) colour.
 * @param bg Background colour.
 * @param scale Scale for the font.
 */
void main_screen_draw_string(position pos, const char *str, const rgb fg,
                             const rgb bg, const uint8_t scale);

/**
 * @brief Determines the maximum number of characters that can be displayed on a
 * line, starting from pos.
 *
 * @param pos Origin position of the string on the screen.
 * @param scale Scale for the font.
 * @return The number of characters that can be displayed on the screen.
 */
static uint16_t compute_max_char_per_line(const position pos,
                                          const uint8_t scale);

/**
 * @brief Finds the index
 *
 * @param c The font the function will try to find the index for.
 * @return The index of the glyph if it belongs to the font, ' '
 */
static uint8_t find_glyph(const char c);

/**
 * @brief Draws a row of a single char.
 *
 * @param c The character to draw.
 * @param row The row of the character to draw.
 * @param packed_colors Index 0 is foreground (text) colour, index 1 is
 * background colour.
 * @param scale Scale for the font.
 */
static void draw_char_row(const char c, const uint8_t row,
                          const uint16_t *packed_colors, const uint8_t scale);

/**
 * @brief Computes the number of lines needed to draw a string on the screen.
 *
 * @param pos Origin position of the string.
 * @param str String to draw.
 * @param scale Scale for the font.
 * @return Returns the number of lines needed ot draw the string on the screen.
 */
static uint8_t compute_nb_lines(const position pos, const char *str,
                                const uint8_t scale);

/**
 * @brief Draws up to n characters on the screen.
 *
 * @param pos Origin position of the string.
 * @param str The string of characters to display.
 * @param packed_colors Index 0 is foreground (text) colour, index 1 is
 * background colour.
 * @param scale Scale for the font.
 * @param n Max number of characters to draw.
 * @return Returns the number of characters drawn.
 */
static uint16_t draw_n_char(position pos, const char *str,
                            const uint16_t *packed_colors, const uint8_t scale,
                            const uint16_t n);

#endif // !MAIN_SCREEN_TEXT_H
