/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/06/24 10:23:30 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// GC9A01 - spi_lib.h
// Draws "s" using a 5x7 bitmap font over hardware SPI
//
// Pin Map: ON ARDUINO MEGA.        _____________________________________________________
// 1 - SCK  -> D52 (PB3).         |. BREADBOARD. |   Position of Wires to MCU  2560      |
// 2 - MOSI -> D51 (PB2)          |              |    1    2    3    4     5             |
// 3 - DC   ->  D11 (PB6)         |     LCD      |  -[R1]-[R1]-[R1]-[R1]-[R1]-- center   |
// 4 - RST  ->  D12 (PB7)         |              |  -[R2]-[R2]-[R2]-[R2]-[R2]--          |
// 5 - CS   -> D10 (PB5)          |              |            GND                        |
//                                 ------------------------------------------------------
// Power pins:
//   VCC  -> 3.3V -> BOTTOM LINE + BREAD BOARD
//   VCC  -> 5V -> TOP LINE + BREAD BOARD
//   GND  -> GND


#include <avr/io.h>
#include <util/delay.h>
#include "spi_lib.h"
#include "uart_lib.h"
#include "small_font.h"
#include "tools.h"

// ─── Pin macros ───────────────────────────────────────────────────────────────

#define CS_LOW()   PORTB &= ~(1 << PB4)
#define CS_HIGH()  PORTB |=  (1 << PB4)
#define DC_LOW()   PORTB &= ~(1 << PB5)   // command mode
#define DC_HIGH()  PORTB |=  (1 << PB5)   // data mode
#define RST_LOW()  PORTB &= ~(1 << PB6)
#define RST_HIGH() PORTB |=  (1 << PB6)

// ─── Colors (RGB565) ─────────────────────────────────────────────────────────

#define COLOR_BLACK  0x0000
#define COLOR_WHITE  0xFFFF
#define COLOR_BLUE    0xF800
#define COLOR_GREEN  0x07E0
#define COLOR_RED   0x001F
#define COLOR_YELLOW 0xFFE0

// --- FONT SIZE ---------------------------------------------------------------

#define SMALL 1
// --- FONT LCD (ORDERED) SIZE 320 x 480 - 4 TFT GC9A01


// --- FONT LCD SIZE 240 x 320 - 3,2 TFT GC9A01

#define SMALL_MAX_CHAR_WIDTH 40
#define MAX_PIXEL_WIDTH 240
#define MAX_PIXEL_HIGH 240

volatile uint8_t scale;
volatile char hex[4];               // global for function toHex()
volatile char nbr_in_a_string[33];  // global variable for function nbr_to_str()

// ─── GC9A01 low-level ───────────────────────────────────────────────────────

void GC9A01_cmd(uint8_t cmd) {
    DC_LOW();
    CS_LOW();
    spi_send(cmd);
    CS_HIGH();
}

void GC9A01_data(uint8_t data) {
    DC_HIGH();
    CS_LOW();
    spi_send(data);
    CS_HIGH();
}

void GC9A01_init(void) {
    RST_LOW();  _delay_ms(100);
    RST_HIGH(); _delay_ms(100);
    GC9A01_cmd(0x01);          // software reset
    _delay_ms(150);
    GC9A01_cmd(0x11);          // sleep out
    _delay_ms(150);
    GC9A01_cmd(0x3A);          // pixel format
    GC9A01_data(0x55);         // 16-bit color (RGB565)
    GC9A01_cmd(0x29);          // display on
}

// ─── Drawing primitives ──────────────────────────────────────────────────────

void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    GC9A01_cmd(0x2A);
    GC9A01_data(x0 >> 8);
    GC9A01_data(x0 & 0xFF);
    GC9A01_data(x1 >> 8);
    GC9A01_data(x1 & 0xFF);
    GC9A01_cmd(0x2B);
    GC9A01_data(y0 >> 8);
    GC9A01_data(y0 & 0xFF);
    GC9A01_data(y1 >> 8);
    GC9A01_data(y1 & 0xFF);
    GC9A01_cmd(0x2C);
}


uint8_t ft_main_strlen(const char *str) {
     uint8_t len = 0;
    while (str && *str) {
        len++;
        str++;
    }
    return (len);    
}


void draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    set_window(x, y, (MAX_PIXEL_WIDTH) - 1, MAX_PIXEL_HIGH - 1);
    DC_HIGH();
    CS_LOW();
    spi_send(color >> 8);
    spi_send(color & 0xFF);
    CS_HIGH();
}

// ─── 10x14 Font ────────────────────────────────────────────────────────────────
// Each char = 5 bytes (one per column) x 4
// CH0 | CH1
// ---------
// CH3 | CH4
static const uint8_t font10x14[95][4][5] = {    
    // [0] = {
    //     { 0x40, 0x60, 0x30, 0x30, 0x30 }, // CH0
    //     { 0x30, 0x30, 0x30, 0x30, 0x30 }, // CH1
    //     { 0x61, 0x63, 0x66, 0x66, 0x66 }, // CH2
    //     { 0x66, 0x66, 0x66, 0x3E, 0x18 },  // CH3
    // } // 115 's'

    ['s' - 32] = {
        { 0x00, 0x30, 0x30, 0x30, 0x30 }, // CH1  (was CH0 reversed)
        { 0x30, 0x30, 0x30, 0x60, 0x40 }, // CH0  (was CH1 reversed)
        { 0x18, 0x3E, 0x66, 0x66, 0x66 }, // CH3  (was CH2 reversed)
        { 0x66, 0x66, 0x66, 0x63, 0x01 }, // CH2  (was CH3 reversed)
    } // 115 's'

};


void draw_color_screen(uint16_t color) {
    set_window(0, 0, MAX_PIXEL_WIDTH - 1, MAX_PIXEL_HIGH - 1);

    DC_HIGH();
    CS_LOW();

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    for (uint32_t i = 0; i < (uint32_t)MAX_PIXEL_WIDTH * MAX_PIXEL_HIGH; i++) {
        spi_send(hi);
        spi_send(lo);
    }

    CS_HIGH();
}



// ─── Text rendering ──────────────────────────────────────────────────────────

void draw_char(uint16_t x, uint16_t y, char c, uint16_t fg, uint16_t bg) {
    if (c < 32 || c > 126) c = '?';
    // const uint8_t *glyph = font5x7[(uint8_t)c - 32];
   
   
    const uint8_t (*glyph)[5] = font10x14[(uint8_t)c - 32];

    uint16_t xTemp = x;
    uint16_t yTemp = y;
    

    for (int8_t ch = 0; ch < 4; ch++) {

        if (ch == 0) {
            x = xTemp, y=yTemp;
            // continue;
        }
        if (ch == 1) {
            x += (5 * scale), y = yTemp;
            // continue;
        }
        if (ch == 2) {
            x = xTemp, y = yTemp + (7 * scale);
            // continue;
        }
        if (ch == 3) {
            x += (5 * scale), y = yTemp + (7 * scale);
            // continue;
        }
        for (int8_t col = 0; col < 5; col++) {
            for (int8_t row = 0; row < 7; row++) {
                uint16_t color;
                if (glyph[ch][col] & (1 << row)) {
                    color = fg;
                } else {
                    color = bg;
                    continue;
                }
                for (uint8_t i = 0; i < scale; i++) {
                    for (uint8_t z = 0; z < scale; z++) {
                        draw_pixel((x + (col * scale) + z), (y + (row * scale)) + i, color);
                    } 
                }            
            }
        }
    }
}



// ─── Text rendering ──────────────────────────────────────────────────────────

void draw_char_small(uint16_t x, uint16_t y, uint8_t c, uint16_t fg, uint16_t bg) {
    if (c < 32 || c > 126) c = '?';

    // const volatile uint8_t *glyph = font5x7[c - 32];
    const volatile uint8_t *glyph = font5x7[(uint8_t)c - 32];

//    const uint8_t *glyph = font5x7[33];

        for (int8_t col = 0; col < 5; col++) {
            for (int8_t row = 0; row < 7; row++) {
                uint16_t color;

                if (glyph[col] &(1 << row)) {
                    // putnbr(1);
                    color = fg;
                } else {
                    color = bg;
                    // putnbr(0);

                    continue;
                }
                for (int8_t i = 0; i < scale; i++) {
                    for (int8_t z = 0; z < scale; z++) {
                        draw_pixel((x + (col * scale) + z), (y + (row * scale)) + i, color);
                    } 
                }            
            }
            // uart_printstr(" ");
        }
    
}


void draw_string(uint16_t x, uint16_t y, const char *str,
                 uint16_t fg, uint16_t bg) {

    uint8_t len = ft_main_strlen(str);
    
    
    while (len > 0) {
       

        if (scale >= 2) {
            draw_char(x, y, str[len - 1], fg, bg);
            x += 1 + (5 * scale) + 5 * scale;     // 5px glyph + 1px gap
        } else {

            draw_char_small(x, y, str[len - 1], fg, bg);
            x += (5 * scale) + 1;     // 5px glyph + 1px gap

        }
    
        // str++;
        len--;
    }
}

// ─── Arduino entry points ────────────────────────────────────────────────────



void setup() {
    uart_init();
    spi_init();
    GC9A01_init();
}

int main() {
    setup();
    // scale = 2;
    // _delay_ms(500);

    // scale = 2;
    // _delay_ms(500);
    // draw_string(0, 0,  "s", COLOR_WHITE, COLOR_BLACK); // MAX H CHARS
    draw_color_screen(COLOR_BLUE);
    // scale = 1;
    // _delay_ms(500);
    
    // draw_string(0, 120,  "This is a text to READD", COLOR_WHITE, COLOR_BLACK); // MAX H CHARS
    // _delay_ms(1500);



//     draw_string(0, -7,  "ss", COLOR_BLUE, COLOR_BLACK); // MAX H CHARS


//     scale = 4;
//     draw_string(0, -14,  "ss", COLOR_WHITE, COLOR_BLACK); // MAX H CHARS
//     _delay_ms(500);
//     draw_string(0, -14,  "ss", COLOR_BLUE, COLOR_BLACK); // MAX H CHARS


//   scale = 6;
//     draw_string(0, -21,  "ss", COLOR_WHITE, COLOR_BLACK); // MAX H CHARS
//     _delay_ms(500);
//     draw_string(0, -21,  "ss", COLOR_BLUE, COLOR_BLACK); // MAX H CHARS

//       scale = 8;
//     draw_string(0, -28,  "ss", COLOR_WHITE, COLOR_BLACK); // MAX H CHARS
//     _delay_ms(500);
//     draw_string(0, -28,  "ss", COLOR_BLUE, COLOR_BLACK); // MAX H CHARS

    // for (int x = 0; x < 80; x++) {
    //     draw_pixel((50),(40+x),COLOR_RED);

    // }

   while(1) { 
 
        ;
    }
    // nothing

    return (0);
}