/**
 * @file ili9488.h
 * @brief ILI9488 driver.
 * @author lleciak, mpetitco, nrobinso
 */

#ifndef ILI9488_H
#define ILI9488_H

#include <avr/io.h>

#include "structs.h"

#define MAIN_SCREEN_WIDTH 320
#define MAIN_SCREEN_HEIGHT 480

/**
 * @defgroup Display_ILI9488
 * @brief    API for the ILI9488 driver.
 * @{
 */

void ili9488_init();

void ili9488_ramwr(void);

void ili9488_fill_screen(uint16_t color565);
// --------- Utilitaries commands --------------------------------------------
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
void ili9488_set_window(const window win);

/** @} */

#endif  // !ILI9488_H
