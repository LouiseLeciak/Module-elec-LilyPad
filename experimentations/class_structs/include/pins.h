#ifndef PINS_H
#define PINS_H

// The value of those defines represent the ATmega2560 pin on which they are
// plugged to.
#define ST7796_SCK (1 << PB1)
#define ST7796_MOSI (1 << PB2)
#define ST7796_MISO (1 << PB3)
#define ST7796_SS (1 << PB0)
#define ST7796_DC (1 << PB5)
#define ST7796_RST (1 << PB6)

#endif // !PINS_H
