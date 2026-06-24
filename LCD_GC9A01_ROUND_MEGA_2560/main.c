/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/06/24 17:50:19 by nige42           ###   ########.fr       */
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

#define CS_LOW()   PORTB &= ~(1 << PB4)
#define CS_HIGH()  PORTB |=  (1 << PB4)
#define DC_LOW()   PORTB &= ~(1 << PB5)
#define DC_HIGH()  PORTB |=  (1 << PB5)
#define RST_LOW()  PORTB &= ~(1 << PB7)
#define RST_HIGH() PORTB |=  (1 << PB7)

// ─── Colors (RGB565) ───────────────────────────────────<──────────────────────

#define COLOR_BLACK  0x0000
#define COLOR_WHITE  0xFFFF
#define COLOR_BLUE    0xF800
#define COLOR_GREEN  0x07E0
#define COLOR_RED   0x001F
#define COLOR_YELLOW 0xFFE0


// --- FONT LCD SIZE 240 x 320 - 3,2 TFT GC9A01

  #define MAX_PIXEL_WIDTH 240
#define MAX_PIXEL_HIGH 320




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

// void gc9a01a_tearing_on() {
//     GC9A01_cmd(0x35);
//     GC9A01_data(0x00);   // V-blank only
// }

// void gc9a01a_tearing_off() {
//     GC9A01_cmd(0x34);
// }
void GC9A01_init(void) {
    RST_LOW();
    _delay_ms(50);
    RST_HIGH();
    _delay_ms(50);



    
    GC9A01_cmd(0xFE);
    GC9A01_cmd(0xEF);

    GC9A01_cmd(0x3A); GC9A01_data(0x05);   // RGB565 16 bits by pixel 
    GC9A01_cmd(0x36); GC9A01_data(0x00);   // MADCTL

    GC9A01_cmd(0xB6);                      // Display function control
    GC9A01_data(0x00);
    GC9A01_data(0x20);

    // -------- Source Timing Adjust (corrected for VER1.0 PCB) --------
    GC9A01_cmd(0x62);
    GC9A01_data(0x18); GC9A01_data(0x0D); GC9A01_data(0x71); GC9A01_data(0xED);
    GC9A01_data(0x70); GC9A01_data(0x70); GC9A01_data(0x18); GC9A01_data(0x0F);
    GC9A01_data(0x71); GC9A01_data(0xEF); GC9A01_data(0x70); GC9A01_data(0x70);

    GC9A01_cmd(0x63);
    GC9A01_data(0x18); GC9A01_data(0x11); GC9A01_data(0x71); GC9A01_data(0xF1);
    GC9A01_data(0x70); GC9A01_data(0x70); GC9A01_data(0x18); GC9A01_data(0x13);
    GC9A01_data(0x71); GC9A01_data(0xF3); GC9A01_data(0x70); GC9A01_data(0x70);

    GC9A01_cmd(0x64);
    GC9A01_data(0x28); GC9A01_data(0x29); GC9A01_data(0xF1);
    GC9A01_data(0x01); GC9A01_data(0x8F); GC9A01_data(0x00); GC9A01_data(0x00);

    GC9A01_cmd(0x66);
    GC9A01_data(0x3C); GC9A01_data(0x00); GC9A01_data(0xCD); GC9A01_data(0x67);
    GC9A01_data(0x45); GC9A01_data(0x45); GC9A01_data(0x10);
    GC9A01_data(0x00); GC9A01_data(0x00); GC9A01_data(0x00);

    GC9A01_cmd(0x67);
    GC9A01_data(0x00); GC9A01_data(0x3C); GC9A01_data(0x00); GC9A01_data(0x00);
    GC9A01_data(0x00); GC9A01_data(0x01); GC9A01_data(0x54);
    GC9A01_data(0x10); GC9A01_data(0x32); GC9A01_data(0x98);

    GC9A01_cmd(0x74);
    GC9A01_data(0x10); GC9A01_data(0x85); GC9A01_data(0x80);
    GC9A01_data(0x00); GC9A01_data(0x00); GC9A01_data(0x4E); GC9A01_data(0x00);
    // // -----------------------------------------------------------------


    

    GC9A01_cmd(0x21);  // inversion ON
    
    GC9A01_cmd(0x11);  // sleep out
    _delay_ms(120);
    
    GC9A01_cmd(0x29);  // display ON
    _delay_ms(20);
}




void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    GC9A01_cmd(0x2A);
    GC9A01_data(x0 >> 8); GC9A01_data(x0 & 0xFF);
    GC9A01_data(x1 >> 8); GC9A01_data(x1 & 0xFF);
    GC9A01_cmd(0x2B);
    GC9A01_data(y0 >> 8); GC9A01_data(y0 & 0xFF);
    GC9A01_data(y1 >> 8); GC9A01_data(y1 & 0xFF);
    GC9A01_cmd(0x2C);
}

void draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    set_window(x, y, x, y);
    DC_HIGH();
    CS_LOW();
    spi_send(color >> 8);
    spi_send(color & 0xFF);
    CS_HIGH();
}

void draw_color_screen(uint16_t color) {
    set_window(0, 0, MAX_PIXEL_WIDTH - 1, MAX_PIXEL_HIGH - 1);
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    DC_HIGH();
    CS_LOW();
    for (uint32_t i = 0; i < (uint32_t)MAX_PIXEL_WIDTH * MAX_PIXEL_HIGH; i++) {
        
        spi_send(hi);
        spi_send(lo);
    }
    CS_HIGH();

}

void setup() {
    
    uart_init();
    spi_init();
    GC9A01_init();

// wdt_enable(WDTO_2S);


}

int main() {
    //    wdt_reset();
    
    // read reset cause
    
    uart_init();
   
    setup();
    draw_color_screen(COLOR_BLUE);

    // gc9a01a_tearing_off();
    while (1) {
        ;

        // draw_color_screen(COLOR_BLACK);
    }
}