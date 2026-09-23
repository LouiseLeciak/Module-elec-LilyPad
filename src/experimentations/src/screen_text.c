/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen_text.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:21:04 by nige42            #+#    #+#             */
/*   Updated: 2026/09/15 11:45:27 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "screen_text.h"
#include "pinout.h"
#include "spi.h"
#include "small_font.h"
#include <avr/pgmspace.h>



void ili9341_cmd(uint8_t cmd) {
    DC_LOW();
    CS_MAIN_LOW();
    spi_txrx(cmd);
    CS_MAIN_HIGH();
}

void ili9341_data(uint8_t data) {
    DC_HIGH();
    CS_MAIN_LOW();
    spi_txrx(data);
    CS_MAIN_HIGH();
}


void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    ili9341_cmd(0x2A);
    ili9341_data(x0 >> 8);
    ili9341_data(x0 & 0xFF);
    ili9341_data(x1 >> 8);
    ili9341_data(x1 & 0xFF);
    ili9341_cmd(0x2B);
    ili9341_data(y0 >> 8);
    ili9341_data(y0 & 0xFF);
    ili9341_data(y1 >> 8);
    ili9341_data(y1 & 0xFF);
    ili9341_cmd(0x2C);
}


uint8_t ft_text_len(const char *str) {
     uint8_t len = 0;
    while (str && *str) {
        len++;
        str++;
    }
    return (len);    
}



void draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    
    uint8_t r = ((color >> 11) & 0x1F) << 3; // 5 bits -> 8 bits (bits hauts)
    uint8_t g = ((color >> 5) & 0x3F) << 2;  // 6 bits -> 8 bits (bits hauts)
    uint8_t b = (color & 0x1F) << 3;   
    set_window(x, y, x, y);
    DC_HIGH();
    CS_MAIN_LOW();
    spi_txrx(r);
    spi_txrx(g);
    spi_txrx(b);
    CS_MAIN_HIGH();
}


void draw_char_small(uint16_t x, uint16_t y, uint8_t c, uint16_t fg, uint16_t bg, uint8_t ScaleFactor) {
    if (c < 32 || c > 126) c = '?';

    const uint8_t *char_in_array = font5x7[c - 32];   // pointer to the 5-byte char_in_array

    for (int8_t col = 0; col < 5; col++) {
        uint8_t columnsBits = pgm_read_byte(&char_in_array[col]);   // get char array colums bits

        for (int8_t row = 0; row < 7; row++) {
            uint16_t color = (columnsBits & (1 << row)) ? fg : bg;

            for (int8_t i = 0; i < ScaleFactor; i++) {
                for (int8_t z = 0; z < ScaleFactor; z++) {
                    // if (!(columnsBits & (1 << row))) continue;   //OPTION add if you want to have transparentbackground skip pixels

                    draw_pixel((x + (col * ScaleFactor) + z), (y + (row * ScaleFactor) + i), color);
                }
            }
        }
    }
}


void draw_string(uint16_t x, uint16_t y, const char *str, uint16_t fg, uint16_t bg, uint8_t ScaleFactor, uint8_t HGap) {

    uint8_t len = ft_text_len(str);
    uint8_t i = 0;
    while (i < len) {
            draw_char_small(x, y, str[i], fg, bg, ScaleFactor);
            x += (5 * ScaleFactor) + HGap;     // 5px glyph + gap
        i++;
    }
}

