/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GC9A01.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:07:56 by nige42            #+#    #+#             */
/*   Updated: 2026/09/13 09:59:54 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC9A01_H
#define GC9A01_H

#include <avr/io.h>

// ─── GC09A01 Pin control macros
// ─────────────────────────────────────────────────────────

#define CS_LEFT_EYE_LOW() PORTE &= ~LEFT_EYE_CS
#define CS_LEFT_EYE_HIGH() PORTE |= LEFT_EYE_CS
#define CS_RIGHT_EYE_LOW() PORTE &= ~RIGHT_EYE_CS
#define CS_RIGHT_EYE_HIGH() PORTE |= RIGHT_EYE_CS
#define DC_LOW() PORTH &= ~SCREENS_DC
#define DC_HIGH() PORTH |= SCREENS_DC
#define RST_LOW() PORTE &= ~EYES_RST
#define RST_HIGH() PORTE |= EYES_RST

// --- MAX LCD SIZE 240 x 240 - 1,28 inch TFT GC9A01

#define GC9A01_WIDTH 240
#define GC9A01_HEIGHT 240
#define FULLSCREEN 57600       // in Pixels 240 x 240
#define LID_80_PERCENT 48000.  // in Pixels 240 x 200

// --- EYES -- used for the CS line calls
#define LEFT_EYE 1
#define RIGHT_EYE 9

// ─── Colors in Format (RGB-565)
// ──────────────────────────────────────────────────
#define GC9A01A_COLOR_BLACK 0x0000
#define GC9A01A_COLOR_WHITE 0xFFFF
#define GC9A01A_COLOR_RED 0xF800
#define GC9A01A_COLOR_GREEN 0x07E0
#define GC9A01A_COLOR_BLUE 0x001F
#define GC9A01A_COLOR_YELLOW 0xFFE0
#define GC9A01A_COLOR_MAGENTA 0xF81F
#define GC9A01A_COLOR_CYAN 0x07FF
#define GC9A01A_COLOR_ORANGE 0xFD20
#define GC9A01A_COLOR_PURPLE 0x8010
#define GC9A01A_COLOR_PINK 0xFC18
#define GC9A01A_COLOR_LIME 0xBFE0
#define GC9A01A_COLOR_NAVY 0x000F
#define GC9A01A_COLOR_MAROON 0x7800
#define GC9A01A_COLOR_OLIVE 0x7BE0
#define GC9A01A_COLOR_TEAL 0x03EF
#define GC9A01A_COLOR_SILVER 0xC618
#define GC9A01A_COLOR_GRAY 0x7BEF
#define GC9A01A_COLOR_DARKGRAY 0x4208
#define GC9A01A_COLOR_BROWN 0xA145
#define GC9A01A_COLOR_EYE 0XD77A

/* functions that send to a specific eye using the CS pin */

/// @brief Initates each GC0901 LCD with the register settingd
/// @param screen Designates the screens CS to be called Left Eye or Right Eye
void GC9A01_init(uint8_t screen);

/// @brief Sends a Command to the specific GC0901 LCD using the SPI BUS
/// @param cmd  Command in unsigned int  format
/// @param screen Designates the screens CS to be called Left Eye or Right Eye
void GC9A01_cmd(uint8_t cmd, uint8_t screen);

/// @brief Sends a Data to the specific GC0901 LCD using the SPI BUS
/// @param data Data in unsigned int format
/// @param screen Designates the screens CS to be called Left Eye or Right Eye
void GC9A01_data(uint8_t data, uint8_t screen);

/// @brief Sets the size of the GC0901 LCD window to be draw to
/// @param x0 Position x of the point top left
/// @param y0 Position y of the point top left
/// @param x1 Position x of the point bottom right
/// @param y1 Position y of the point bottom right
/// @param screen Designates the screens CS to be called Left Eye or Right Eye
void GC9A01_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
                          uint8_t screen);

/// @brief Sets the drawing area of a specific GC09101 LCD to full screen and
/// fills the screen with the color
/// @param color Color to be drawn to screen format RGB-565
/// @param screen Designates the screens CS to be called Left Eye or Right Eye
/// @attention calls helper functions GC9A01_setAddrWindow(), GC9A01_pushColor()
void GC9A01_fillScreen(uint16_t color, uint8_t screen);

/// @brief Write to a specific GC09A01 LCD a number of pixels of the color to
/// the SPI BUS
/// @param color Color to be drawn to screen format RGB-565
/// @param pixels Number of pixels to be written to ex: 57600 is Full Screen
/// @param screen Designates the screens CS to be called Left Eye or Right Eye
void GC9A01_pushColor(uint16_t color, uint16_t pixels, uint8_t screen);

/* functions that send to BOTH eyes at the same time */

/// @brief Sets the drawing area of BOTH GC09101 LCDs to full screen and fills
/// the screen with the color
/// @param color Color to be drawn to screen format RGB-565
/// @attention calls helper functions GC9A01_setAddrWindow_eyes(),
/// GC9A01_pushColor_eyes()
void GC9A01_fillScreen_eyes(uint16_t color);

/// @brief Write to a BOTH GC09A01 LCDs a number of pixels of the color to the
/// SPI BUS
/// @param color Color to be drawn to screen format RGB-565
/// @param pixels Number of pixels to be written to ex: 57600 is Full Screen
void GC9A01_pushColor_eyes(uint16_t color, uint16_t count);

/// @brief Sends a Command to the BOTH GC0901 LCD using the SPI BUS
/// @param cmd  Command in unsigned int format
void GC9A01_cmd_eyes(uint8_t cmd);

/// @brief Sends a Data to the BOTH GC0901 LCD using the SPI BUS
/// @param data Data in unsigned int format
void GC9A01_data_eyes(uint8_t data);

/// @brief Sets the size of the GC0901 LCD windows "BOTH EYES" to be drawn to
/// @param x0 Position x of the point top left
/// @param y0 Position y of the point top left
/// @param x1 Position x of the point bottom right
/// @param y1 Position y of the point bottom right
void GC9A01_setAddrWindow_eyes(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

/// @brief Sends on the BUS SPI a pixel of a color
/// @param color Color to be drawn to screen format RGB-565
void GC9A01_draw_pixel(uint16_t color);

/// @brief Draws a image file ""
/// @param file  pointer to the bitmap data in flash/PROGMEM (1 bit per pixel,
/// packed into bytes)
/// @param maxNbrLines height of the image in rows
/// @param hSizeBytes width of the image in bytes (so width in pixels =
/// hSizeBytes * 8)
/// @param fg foreground color to use for "off" bits
/// @param bg background color to use for "on" bits
void GC9A01_drawImg_eyes(const uint8_t* file, uint8_t maxNbrLines,
                         uint8_t hSizeBytes, uint16_t fg, uint16_t bg);

/// @brief draws a rectrangle number of pixel in a color
/// @param pixels Number of pixels to be written to ex: 48000 is about 80% of
/// the screen
/// @param color Color to be drawn to screen format RGB-565
void GC9A01_draw_eye_Lids(uint16_t pixels, uint16_t color);

/// @brief Sets the direction of the drawing to start bottom to the top
/// @param void
void GC9A01_eye_Lids_up(void);

/// @brief Sets the direction of the drawing to start top to the bottom
/// @param void
void GC9A01_eye_Lids_down(void);

/// @brief Makes the eyes blink, eye lid up,adding image eye from file
/// @param file pointer to the bitmap data in flash/PROGMEM (1 bit per pixel,
/// packed into bytes)
/// @param nbr  Number of it repeats the action
void GC9A01_blink(const uint8_t* file, int nbr);

// toggle eye with the rotary sw
uint8_t change_eye(uint8_t es);

// moving eyes in main
void eyes_action();

#endif
