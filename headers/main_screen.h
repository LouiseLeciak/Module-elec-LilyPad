#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <stdint.h>

#include "structs.h"

// NOTE: It is assumed the screen is using a ST7796 as driver.

#define MAIN_SCREEN_WIDTH 320
#define MAIN_SCREEN_HEIGHT 480

// --- High-level commands ---------------------------------------------------
// --------- Drawing commands ------------------------------------------------
/**
 * @brief Draws the provided string inside the driver's RAM.
 *
 * @param pos Position for the start of the text.
 * @param str The string of text to displa.
 * @param fg Text's colour.
 * @param bg Background's colour.
 * @param scale Text scaling (default 1).
 */
void main_screen_draw_string(position pos, const char *str, const rgb fg,
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
void main_screen_set_window(const window win);

// --- Low-level commands ----------------------------------------------------

/**
 * @brief This command causes the commands and parameters to their S/W Reset
 * default values. After sending this command, it is necessary to wait 5ms
 * before sending another command.
 * If a software reset is sent during sleep in
 * mode, it will be necessary to wait 120ms before sending sleep out command.
 * Software Reset command can't be sent during sleep out sequence.
 * SWRESET (0x01) p.141
 */
void main_screen_swreset(void);

/**
 * @brief This command causes the LCD module to enter the minimum power
 * consumption mode.
 * SLPIN (0x10) p.159
 */
void main_screen_slpin(void);

/**
 * @brief This command turns off sleep mode.
 * SLPOUT (0x11) p.160
 */
void main_screen_slpout(void);

/**
 * @brief This command turns off sleep mode. It will be necessary to wait 5ms
 * before sending any other command.
 * It will be necessary to wait 120ms before sending a sleep in (SLPIN) command
 * to the driver.
 * SLPOUT (0x11) p.160
 */
void main_screen_slpout(void);

/**
 * @brief Enables the display
 * DISPON (0X29) p.169
 */
void main_screen_dispon(void);

/**
 * @brief Sets the column address.
 * CASET (0x2A) p.170
 *
 * @param col_start Start of the column (between 0 and the width of the screen
 * minus one).
 * @param col_end End of the column (between 0 and the width of the screen
 * minus one).
 */
void main_screen_caset(const uint16_t col_start, const uint16_t col_end);

/**
 * @brief Sets the row address.
 * RASET (0x2B) p.172
 *
 * @param row_start Start of the row (between 0 and the height of the screen
 * minus one).
 * @param row_end End of the row (between 0 and the height of the screen minus
 * one).
 */
void main_screen_raset(const uint16_t row_start, const uint16_t row_end);

/**
 * @brief This command is used to transfer data from the MCU to frame memory.
 * When this command is set, both the column and row registers are reset to the
 * start column/row start positions. Sending any other command can stop frame
 * write.
 * RAMWR (0x2C) p.173
 */
void main_screen_ramwr(void);

/**
 * @brief This command defines read/write scanning direction of frame memory.
 * MADCTL (0x36) p.183
 *
 * @param arg Argument for the command, according to table p.183.
 */
void main_screen_madctl(const uint8_t arg);

/**
 * @typedef e_colmod_arg
 * @brief Arguments for COLMOD.
 *
 */
typedef enum e_colmod_arg {
  CI_16B = 0x05,  ///< Colour Interface 16bits/pixel colour format.
  CI_18B = 0x06,  ///< Colour Interface 18bits/pixel colour format.
  CI_24B = 0x07,  ///< Colour Interface 24bits/pixel colour format.
  RGB_16B = 0x50, ///< RGB Interface 16bits/pixel colour format.
  RGB_18B = 0x60  ///< RGB Interface 18bits/pixel colour format.
} colmod_arg;

/**
 * @brief Defines the format of RGB picture data, which is to be transferred
 * via the MCU interface. The table of the parameters is listed at p.190
 * COLMOD (0x3A) p.190
 */
void main_screen_colmod(const colmod_arg);

#endif // !MAIN_SCREEN_H
