/**
 * @file ili9488.h
 * @brief ILI9488 driver.
 * @author lleciak, mpetitco, nrobinso
 */

#ifndef ILI9488_H
#define ILI9488_H

#define MAIN_SCREEN_WIDTH 320
#define MAIN_SCREEN_HEIGHT 480

#include <avr/io.h>

#include "structs.h"

/**
 * @defgroup Display_ILI9488
 * @brief    API for the ILI9488 driver.
 * @{
 */

void ili9488_reset(void);

void main_screen_init();

void main_screen_ramwr(void);

/**
 * @brief Draws the provided string inside the driver's RAM.
 *
 * @param pos Position for the start of the text.
 * @param str The string of text to displa.
 * @param fg Text's colour.
 * @param bg Background's colour.
 * @param scale Text scaling (default 1).
 */
void main_screen_draw_string(position pos, const char* str, const rgb fg,
                             const rgb bg, const uint8_t scale);

/**
 * @brief Draws a rectangle of the size and position specified in win and of
 * colour rgb.
 *
 * @param win Window for the rectangle, specifies the position and de facto
 * size.
 * @param rgb RGB colour to fill the rectangle with.
 */
void main_screen_draw_rectangle(const window win, const rgb rgb);

void ili9488_fill_screen(uint16_t color565);

/**
 * @brief Packs a RGB colour stored in 3*8 bits in a uint16_t for the ST7796
 * driver to send to the screen. The format is the following:
 * - 5 bits red,
 * - 6 bits green,
 * - 5 bits blue
 *
 * @param colour The colour to be packed.
 * @return The uint16_t with the packed colour.
 */
uint16_t pack_rgb565(const rgb colour);

/**
 * @brief Sets a window for the ST7796 driver. Is usually called before
 * st7796_ramwr().
 *
 * @param win The window to be transmitted to the driver so it can be used then.
 */
void main_screen_set_window(const window win);

/** @} */

#endif  // !ILI9488_H
