#ifndef ILI9341_H
#define ILI9341_H

#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>

#include "spi_lib.h"

// NOTE: The ILI9314 can use several SPI configurations, we will use the
// 4-Wires one (SCL, SDA, D/C, SC)

#define ILI9341_DC (1 << PB5)
#define ILI9341_RST (1 << PB6)

#define ILI9341_SS_LOW() (PORTB &= ~(SS_LINES))
#define ILI9341_SS_HIGH() (PORTB |= SS_LINES)
#define ILI9341_DC_LOW() PORTB &= ~(ILI9341_DC) // COMMAND mode
#define ILI9341_DC_HIGH() PORTB |= ILI9341_DC   // DATA mode
#define ILI9341_RST_LOW() PORTB &= ~ILI9341_RST
#define ILI9341_RST_HIGH() PORTB |= ILI9341_RST

#define ILI9341_MAX_HEIGHT 320
#define ILI9341_MAX_WIDTH 240

void fill_screen (uint16_t colour);

// void set_window (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

void ili9341_cmd (uint8_t cmd);

void ili9341_data (uint8_t *data, size_t siz);

/**
 * @brief Initialises the ILI9341 driver, following the sequence detailed in
 * the ILI9341 datasheet p.214.
 * NOTE: Refer to case 12.1 or 12.2, as they indicate how to use the RESET
 * (RSX) pin to trigger a hardware reset
 */
void ili9341_init (void);

#endif // !ILI9341_H
