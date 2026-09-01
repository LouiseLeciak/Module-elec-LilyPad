#ifndef PINOUT_H
#define PINOUT_H

// I2C GENERAL
#define SCL (1 << PD0)
#define SDA (1 << PD1)

// SPI GENERAL
#define CS (1 << PB0)
#define SCK (1 << PB1)
#define MOSI (1 << PB2)
#define MISO (1 << PB3)

// SLAVE SELECTS (SPI)
#define MAIN_SCREEN_CS (1 << PH0)
#define LEFT_EYE_CS (1 << PE5)
#define RIGHT_EYE_CS (1 << PE7)
#define SD_CS (1 << PH5)

// RESETS (SPI)
#define EYES_RST (1 << PE6)
#define MAIN_SCREEN_RST (1 << PH1)

// SCREENS SPI GENERAL
#define SCREENS_DC (1 << PH6)

// MAIN SCREEN SPI
#define MAIN_SCREEN_BL (1 << PH2)

// KEYBOARD
#define KB_R1 (1 << PJ2)
#define KB_R2 (1 << PJ1)
#define KB_R3 (1 << PJ0)
#define KB_R4 (1 << PC7)

#define KB_C1 (1 << PA3)
#define KB_C2 (1 << PA4)
#define KB_C3 (1 << PA5)
#define KB_C4 (1 << PA6)
#define KB_C5 (1 << PA7)
#define KB_C6 (1 << PG2)
#define KB_C7 (1 << PJ6)
#define KB_C8 (1 << PJ5)
#define KB_C9 (1 << PJ4)
#define KB_C10 (1 << PJ3)

// ROTARY ENCODER
#define SDL_SW1 (1 << PC1)
#define SDL_SW2 (1 << PC2)
#define SDL_SW3 (1 << PC3)

#endif /* ifndef PINOUT_H */
