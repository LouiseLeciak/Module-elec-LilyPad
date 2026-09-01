#ifndef PINOUT_H
#define PINOUT_H

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

#endif /* ifndef PINOUT_H */
