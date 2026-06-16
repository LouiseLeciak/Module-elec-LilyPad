#ifndef PINS_H
#define PINS_H

// MCU SPI pins
#define MCU_SCK (1 << PB1)
#define MCU_MOSI (1 << PB2)
#define MCU_MISO (1 << PB3)

// Main screen (ST7796) pins
// WARNING: The pins used here are for prototyping purposes, they do not
// represent the pins as per described in the Lily's Pad specifications pinout.
#define MAIN_SCREEN_SS (1 << PB0)
#define MAIN_SCREEN_DC (1 << PB5)
#define MAIN_SCREEN_RST (1 << PB6)

#endif // !PINS_H
