/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/06/26 14:44:22 by nige42           ###   ########.fr       */
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
#include <avr/wdt.h>


#include <avr/io.h>
#include <util/delay.h>
#include "spi_lib.h"
#include "uart_lib.h"
#include "tools.h"

// ─── Pin macros ───────────────────────────────────────────────────────────────

#define CS_LOW(eye)   CS_##eye##_EYE_LOW()
#define CS_HIGH(eye)  CS_##eye##_EYE_HIGH()

#define CS_LEFT_EYE_LOW()   PORTB &= ~(1 << PB4)
#define CS_LEFT_EYE_HIGH()  PORTB |=  (1 << PB4)
#define DC_LOW()   PORTB &= ~(1 << PB5)
#define DC_HIGH()  PORTB |=  (1 << PB5)
#define RST_LOW()  PORTB &= ~(1 << PB7)
#define RST_HIGH() PORTB |=  (1 << PB7)

// ─── Colors (RGB-565) ───────────────────────────────────<──────────────────────

#define GC9A01A_COLOR_BLACK    0x0000
#define GC9A01A_COLOR_WHITE    0xFFFF
#define GC9A01A_COLOR_RED      0xF800
#define GC9A01A_COLOR_GREEN    0x07E0
#define GC9A01A_COLOR_BLUE     0x001F
#define GC9A01A_COLOR_YELLOW   0xFFE0
#define GC9A01A_COLOR_MAGENTA  0xF81F
#define GC9A01A_COLOR_CYAN     0x07FF
#define GC9A01A_COLOR_ORANGE   0xFD20
#define GC9A01A_COLOR_PURPLE   0x8010
#define GC9A01A_COLOR_PINK     0xFC18
#define GC9A01A_COLOR_LIME     0xBFE0
#define GC9A01A_COLOR_NAVY     0x000F
#define GC9A01A_COLOR_MAROON   0x7800
#define GC9A01A_COLOR_OLIVE    0x7BE0
#define GC9A01A_COLOR_TEAL     0x03EF
#define GC9A01A_COLOR_SILVER   0xC618
#define GC9A01A_COLOR_GRAY     0x7BEF
#define GC9A01A_COLOR_DARKGRAY 0x4208
#define GC9A01A_COLOR_BROWN    0xA145


// --- FONT LCD SIZE 240 x 320 - 3,2 TFT GC9A01

#define MAX_PIXEL_WIDTH 240
#define MAX_PIXEL_HIGH 320

#define LEFT_EYE 1
#define RIGHT_EYE 2


void GC9A01_cmd(uint8_t cmd, uint8_t screen) {
    DC_LOW();    
    if (screen == LEFT_EYE)  {
        CS_LEFT_EYE_LOW();
        spi_send(cmd);
        CS_LEFT_EYE_HIGH();
    }
}

void GC9A01_data(uint8_t data) {
    DC_HIGH();
    CS_LEFT_EYE_LOW();
    spi_send(data);
    CS_LEFT_EYE_HIGH();
}


void GC9A01_init(uint8_t screen) {

    // Hardware reset
    RST_LOW();
    _delay_ms(50);
    RST_HIGH();
    _delay_ms(50);

    // Unlock extended command set
    GC9A01_cmd(0xFE, screen);
    GC9A01_cmd(0xEF, screen);

    // Pixel format: RGB565 (16 bits/pixel)
    GC9A01_cmd(0x3A, screen); GC9A01_data(0x05);

    // Memory Access Control: default scan direction
    GC9A01_cmd(0x36, screen); GC9A01_data(0x08);

    // Display Function Control
    GC9A01_cmd(0xB6, screen); GC9A01_data(0x00); GC9A01_data(0x20);

    // --- Analog / Source timing (panel-specific, do not change) ---
    GC9A01_cmd(0x62, screen);
    GC9A01_data(0x18); GC9A01_data(0x0D); GC9A01_data(0x71); GC9A01_data(0xED);
    GC9A01_data(0x70); GC9A01_data(0x70); GC9A01_data(0x18); GC9A01_data(0x0F);
    GC9A01_data(0x71); GC9A01_data(0xEF); GC9A01_data(0x70); GC9A01_data(0x70);

    GC9A01_cmd(0x63, screen);
    GC9A01_data(0x18); GC9A01_data(0x11); GC9A01_data(0x71); GC9A01_data(0xF1);
    GC9A01_data(0x70); GC9A01_data(0x70); GC9A01_data(0x18); GC9A01_data(0x13);
    GC9A01_data(0x71); GC9A01_data(0xF3); GC9A01_data(0x70); GC9A01_data(0x70);

    // Gamma voltage — omit if causes flicker on some panels
    GC9A01_cmd(0x64, screen);
    GC9A01_data(0x28); GC9A01_data(0x29); GC9A01_data(0xF1);
    GC9A01_data(0x01); GC9A01_data(0xF0); GC9A01_data(0x00); GC9A01_data(0x00);

    GC9A01_cmd(0x66, screen);
    GC9A01_data(0x3C); GC9A01_data(0x00); GC9A01_data(0xCD); GC9A01_data(0x67);
    GC9A01_data(0x45); GC9A01_data(0x45); GC9A01_data(0x10);
    GC9A01_data(0x00); GC9A01_data(0x00); GC9A01_data(0x00);

    GC9A01_cmd(0x67, screen);
    GC9A01_data(0x00); GC9A01_data(0x3C); GC9A01_data(0x00); GC9A01_data(0x00);
    GC9A01_data(0x00); GC9A01_data(0x01); GC9A01_data(0x54);
    GC9A01_data(0x10); GC9A01_data(0x32); GC9A01_data(0x98);

    GC9A01_cmd(0x74, screen);
    GC9A01_data(0x10); GC9A01_data(0x85); GC9A01_data(0x80);
    GC9A01_data(0x00); GC9A01_data(0x00); GC9A01_data(0x4E); GC9A01_data(0x00);

    GC9A01_cmd(0x98, screen); GC9A01_data(0x3E); GC9A01_data(0x07);
    // --- End analog / source timing ---

    // Frame rate: fastest (0x00 = slowest, 0x1F = fastest)
    GC9A01_cmd(0xC6, screen); GC9A01_data(0x01);

    // Gate control
    GC9A01_cmd(0xB7, screen); GC9A01_data(0x35);

    // VCOM voltage
    GC9A01_cmd(0xBB, screen); GC9A01_data(0x06);

    // LCM control
    GC9A01_cmd(0xC0, screen); GC9A01_data(0x6C);

    // VRH: GVDD voltage level
    GC9A01_cmd(0xC3, screen); GC9A01_data(0x0B);

    // VDV: VCOM offset voltage
    GC9A01_cmd(0xC4, screen); GC9A01_data(0x0F);

    // Power control
    GC9A01_cmd(0xD0, screen); GC9A01_data(0xA4); GC9A01_data(0xA1);

    // Display inversion ON (required for correct colors on this panel)
    GC9A01_cmd(0x21, screen);

    // Sleep out — must wait 120ms before display on
    GC9A01_cmd(0x11, screen);
    _delay_ms(120);

    // Display on
    GC9A01_cmd(0x29, screen);
    _delay_ms(20);
}

void set_window(uint8_t screen, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    GC9A01_cmd(0x2A, screen);
    GC9A01_data(x0 >> 8); GC9A01_data(x0 & 0xFF);
    GC9A01_data(x1 >> 8); GC9A01_data(x1 & 0xFF);
    GC9A01_cmd(0x2B, screen);
    GC9A01_data(y0 >> 8); GC9A01_data(y0 & 0xFF);
    GC9A01_data(y1 >> 8); GC9A01_data(y1 & 0xFF);
    GC9A01_cmd(0x2C, screen);
}

void draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    set_window(LEFT_EYE, x, y, x, y);
    DC_HIGH();
    CS_LEFT_EYE_LOW();
    spi_send(color >> 8);
    spi_send(color & 0xFF);
    CS_LEFT_EYE_HIGH();
}

void GC9A01_draw_color_screen(uint8_t screen, uint16_t color) {
    if (screen == LEFT_EYE) {
        set_window(LEFT_EYE, 0, 0, MAX_PIXEL_WIDTH - 1, MAX_PIXEL_HIGH - 1);
        uint8_t hi = color >> 8;
        uint8_t lo = color & 0xFF;
        DC_HIGH();
        CS_LEFT_EYE_LOW();
        for (uint32_t i = 0; i < (uint32_t)MAX_PIXEL_WIDTH * MAX_PIXEL_HIGH; i++) {
        
            spi_send(hi);
            spi_send(lo);
        }
        CS_LEFT_EYE_HIGH();
    }

}

void setup() {
    
    uart_init();
    spi_init();
    GC9A01_init(LEFT_EYE);

// wdt_enable(WDTO_2S);


}

int main() {
    //    wdt_reset();
    
    // read reset cause
    
    uart_init();
   
    setup();
    GC9A01_draw_color_screen(LEFT_EYE, GC9A01A_COLOR_GREEN);

    // gc9a01a_tearing_off();
    while (1) {
        ;

        // draw_color_screen(COLOR_BLACK);
        // _delay_ms(500);
        GC9A01_draw_color_screen(LEFT_EYE, GC9A01A_COLOR_BLUE);
        _delay_ms(5000);
        // draw_color_screen(COLOR_BLACK);
    }
}