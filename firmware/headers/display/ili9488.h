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

void main_screen_init();

void main_screen_ramwr(void);

void ili9488_fill_screen(uint16_t color565);

/**
 * @brief Sets a window for the ST7796 driver. Is usually called before
 * st7796_ramwr().
 *
 * @param win The window to be transmitted to the driver so it can be used then.
 */
void main_screen_set_window(const window win);

/** @} */

#endif  // !ILI9488_H
