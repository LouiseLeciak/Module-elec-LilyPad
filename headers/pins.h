#ifndef PINS_H
#define PINS_H

#include <avr/io.h>

// MCU SPI pins
#define MCU_SCK (1 << PB1)
#define MCU_MOSI (1 << PB2)
#define MCU_MISO (1 << PB3)
#define MCU_SPI_MASK (MCU_SCK | MCU_MOSI)

// --- MAIN SCREEN (ST7796) ----------------------------------------------------
// ------ PINS -----------------------------------------------------------------
// WARNING: The pins used here are for prototyping purposes, they do not
// represent the pins as per described in the Lily's Pad specifications pinout.
#define MAIN_SCREEN_SS (1 << PB0)
#define MAIN_SCREEN_DC (1 << PB5)
#define MAIN_SCREEN_RST (1 << PB6)
// ------ MACROS ---------------------------------------------------------------
#define MAIN_SCREEN_DC_COMMAND() (PORTB &= ~(MAIN_SCREEN_DC))
#define MAIN_SCREEN_DC_DATA() ((PORTB |= MAIN_SCREEN_DC))
#define MAIN_SCREEN_RST_LOW() (PORTB &= ~(MAIN_SCREEN_RST))
#define MAIN_SCREEN_RST_HIGH() (PORTB |= MAIN_SCREEN_RST)
#define MAIN_SCREEN_SS_LOW() (PORTB &= ~(MAIN_SCREEN_SS))
#define MAIN_SCREEN_SS_HIGH() (PORTB |= MAIN_SCREEN_SS)

// --- SD CARD -----------------------------------------------------------------
#define SD_SS (1 << PB4)
// --- !SD CARD ----------------------------------------------------------------
#define SPI_SS_MASK (MAIN_SCREEN_SS | SD_SS)
// ------ PINS -----------------------------------------------------------------
#define SD_SS (1 << PB4)

#define SS_MASK (MAIN_SCREEN_SS | SD_SS)

#endif  // !PINS_H
