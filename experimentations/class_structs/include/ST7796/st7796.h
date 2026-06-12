#ifndef ST7796_H
#define ST7796_H

#include <avr/io.h>
#include <stdint.h>

#include "pins.h"

// WARNING: IT IS YOUR RESPONSIBILITY TO START A TRANSACTION AND END IT !!!
// THOSE FUNCTIONS ONLY IMPLEMENT THE DRIVER'S COMMAND, THEY DON'T KNOW THEY'RE
// USED THROUGH SPI, SO BEFORE CALLING ANY OF THEM, START A SPI TRANSACTION !!!
// THANK YOU :)
// Think of it like malloc()/free() : As soon as you start a transaction, you
// should think about when it ends. If the SPI bus is hanging, chances are you
// left a transaction open.

#define ST7796_DC_COMMAND() (PORTB &= ~(ST7796_DC))
#define ST7796_DC_DATA() (PORTB |= ST7796_DC)
#define ST7796_RST_LOW() (PORTB &= ~(ST7796_RST))
#define ST7796_RST_HIGH() (PORTB |= ST7796_RST)
#define ST7796_SS_LOW() (PORTB &= ~(ST7796_SS))
#define ST7796_SS_HIGH() (PORTB |= ST7796_SS)

typedef struct s_window {
  uint16_t _start_x;
  uint16_t _end_x;
  uint16_t _start_y;
  uint16_t _end_y;
} window;

typedef struct s_position {
  uint16_t _pos_x;
  uint16_t _pos_y;
} position;

typedef struct s_rgb {
  uint16_t red;
  uint16_t green;
  uint16_t blue;
} t_rgb;

typedef struct s_st7796 {

} st7796;

// --- High-level commands ---------------------------------------------------
// --------- Drawing commands ------------------------------------------------
void draw_pixel(const position pos, const t_rgb rgb);
void draw_rectangle(const window win, const t_rgb rgb);
// --------- Utilitaries commands --------------------------------------------
uint16_t pack_rgb565(const t_rgb colour);
void set_window(const window win);

#endif // !ST7796_H
