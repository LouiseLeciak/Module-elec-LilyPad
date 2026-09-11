/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GC9A01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:07:34 by nige42            #+#    #+#             */
/*   Updated: 2026/09/11 11:53:22 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "pinout.h"
#include "GC9A01.h"

void GC9A01_init(uint8_t screen) {
    // 1. Hardware Reset
    RST_HIGH();
    _delay_ms(10);
    RST_LOW();
    _delay_ms(20);
    RST_HIGH();
    _delay_ms(150);

    // 2. Software Reset & Wake-up
    GC9A01_cmd(0x01, screen);
    _delay_ms(150);

    GC9A01_cmd(0x11, screen); // Exit Sleep first so charge pumps activate
    _delay_ms(120);

    // 3. Unlock internal command registers
    GC9A01_cmd(0xFE, screen);
    GC9A01_cmd(0xEF, screen);

    GC9A01_cmd(0xEB, screen); GC9A01_data(0x14, screen);
    GC9A01_cmd(0x84, screen); GC9A01_data(0x40, screen);
    GC9A01_cmd(0x85, screen); GC9A01_data(0xFF, screen);
    GC9A01_cmd(0x86, screen); GC9A01_data(0xFF, screen);
    GC9A01_cmd(0x87, screen); GC9A01_data(0xFF, screen);
    GC9A01_cmd(0x88, screen); GC9A01_data(0x0A, screen);
    GC9A01_cmd(0x89, screen); GC9A01_data(0x21, screen);
    GC9A01_cmd(0x8A, screen); GC9A01_data(0x00, screen);
    GC9A01_cmd(0x8B, screen); GC9A01_data(0x80, screen);
    GC9A01_cmd(0x8C, screen); GC9A01_data(0x01, screen);
    GC9A01_cmd(0x8D, screen); GC9A01_data(0x01, screen);
    GC9A01_cmd(0x8E, screen); GC9A01_data(0xFF, screen);
    GC9A01_cmd(0x8F, screen); GC9A01_data(0xFF, screen);

    // Display Function Control: Scan direction
    GC9A01_cmd(0xB6, screen);
    GC9A01_data(0x00, screen);
    GC9A01_data(0x00, screen);

    // Memory Access Control
    GC9A01_cmd(0x36, screen);
    GC9A01_data(0x08, screen); // BGR order

    // Pixel Format: 16-bit RGB565
    GC9A01_cmd(0x3A, screen);
    GC9A01_data(0x05, screen);

    // VREG / Power Setup
    GC9A01_cmd(0x90, screen);
    GC9A01_data(0x08, screen);
    GC9A01_data(0x08, screen);
    GC9A01_data(0x08, screen);
    GC9A01_data(0x08, screen);

    GC9A01_cmd(0xBD, screen); GC9A01_data(0x06, screen);
    GC9A01_cmd(0xBC, screen); GC9A01_data(0x00, screen);

    GC9A01_cmd(0xFF, screen);
    GC9A01_data(0x60, screen);
    GC9A01_data(0x01, screen);
    GC9A01_data(0x04, screen);

    GC9A01_cmd(0xC3, screen); GC9A01_data(0x13, screen);
    GC9A01_cmd(0xC4, screen); GC9A01_data(0x13, screen);
    GC9A01_cmd(0xC9, screen); GC9A01_data(0x22, screen);

    GC9A01_cmd(0xBE, screen); GC9A01_data(0x11, screen);

    GC9A01_cmd(0xE1, screen);
    GC9A01_data(0x10, screen);
    GC9A01_data(0x0E, screen);

    GC9A01_cmd(0xDF, screen);
    GC9A01_data(0x21, screen);
    GC9A01_data(0x0C, screen);
    GC9A01_data(0x02, screen);

    // Gamma Curves
    GC9A01_cmd(0xF0, screen);
    GC9A01_data(0x45, screen); GC9A01_data(0x09, screen);
    GC9A01_data(0x08, screen); GC9A01_data(0x08, screen);
    GC9A01_data(0x26, screen); GC9A01_data(0x2A, screen);

    GC9A01_cmd(0xF1, screen);
    GC9A01_data(0x43, screen); GC9A01_data(0x70, screen);
    GC9A01_data(0x72, screen); GC9A01_data(0x36, screen);
    GC9A01_data(0x37, screen); GC9A01_data(0x6F, screen);

    GC9A01_cmd(0xF2, screen);
    GC9A01_data(0x45, screen); GC9A01_data(0x09, screen);
    GC9A01_data(0x08, screen); GC9A01_data(0x08, screen);
    GC9A01_data(0x26, screen); GC9A01_data(0x2A, screen);

    GC9A01_cmd(0xF3, screen);
    GC9A01_data(0x43, screen); GC9A01_data(0x70, screen);
    GC9A01_data(0x72, screen); GC9A01_data(0x36, screen);
    GC9A01_data(0x37, screen); GC9A01_data(0x6F, screen);

    GC9A01_cmd(0xED, screen);
    GC9A01_data(0x1B, screen);
    GC9A01_data(0x0B, screen);

    GC9A01_cmd(0xAE, screen); GC9A01_data(0x77, screen);
    GC9A01_cmd(0xCD, screen); GC9A01_data(0x63, screen);

    GC9A01_cmd(0xE8, screen); GC9A01_data(0x34, screen);

    // Source Timing Configuration
    GC9A01_cmd(0x62, screen);
    GC9A01_data(0x18, screen); GC9A01_data(0x0D, screen);
    GC9A01_data(0x71, screen); GC9A01_data(0xED, screen);
    GC9A01_data(0x70, screen); GC9A01_data(0x70, screen);
    GC9A01_data(0x18, screen); GC9A01_data(0x0F, screen);
    GC9A01_data(0x71, screen); GC9A01_data(0xEF, screen);
    GC9A01_data(0x70, screen); GC9A01_data(0x70, screen);

    GC9A01_cmd(0x63, screen);
    GC9A01_data(0x18, screen); GC9A01_data(0x11, screen);
    GC9A01_data(0x71, screen); GC9A01_data(0xF1, screen);
    GC9A01_data(0x70, screen); GC9A01_data(0x70, screen);
    GC9A01_data(0x18, screen); GC9A01_data(0x13, screen);
    GC9A01_data(0x71, screen); GC9A01_data(0xF3, screen);
    GC9A01_data(0x70, screen); GC9A01_data(0x70, screen);

    GC9A01_cmd(0x64, screen);
    GC9A01_data(0x28, screen); GC9A01_data(0x29, screen);
    GC9A01_data(0xF1, screen); GC9A01_data(0x01, screen);
    GC9A01_data(0xF1, screen); GC9A01_data(0x00, screen);
    GC9A01_data(0x07, screen);

    GC9A01_cmd(0x66, screen);
    GC9A01_data(0x3C, screen); GC9A01_data(0x00, screen);
    GC9A01_data(0xCD, screen); GC9A01_data(0x67, screen);
    GC9A01_data(0x45, screen); GC9A01_data(0x45, screen);
    GC9A01_data(0x10, screen); GC9A01_data(0x00, screen);
    GC9A01_data(0x00, screen); GC9A01_data(0x00, screen);

    GC9A01_cmd(0x67, screen);
    GC9A01_data(0x00, screen); GC9A01_data(0x3C, screen);
    GC9A01_data(0x00, screen); GC9A01_data(0x00, screen);
    GC9A01_data(0x00, screen); GC9A01_data(0x01, screen);
    GC9A01_data(0x54, screen); GC9A01_data(0x10, screen);
    GC9A01_data(0x32, screen); GC9A01_data(0x98, screen);

    GC9A01_cmd(0x74, screen);
    GC9A01_data(0x10, screen); GC9A01_data(0x85, screen);
    GC9A01_data(0x80, screen); GC9A01_data(0x00, screen);
    GC9A01_data(0x00, screen); GC9A01_data(0x4E, screen);
    GC9A01_data(0x00, screen);

    GC9A01_cmd(0x98, screen);
    GC9A01_data(0x3E, screen);
    GC9A01_data(0x07, screen);

    // Inversion and Tearing
    GC9A01_cmd(0x35, screen); // TEON
    GC9A01_cmd(0x21, screen); // INVON

    // 4. Re-lock inner registers
    GC9A01_cmd(0xFE, screen);
    GC9A01_cmd(0xEF, screen);

    // 5. Display ON
    GC9A01_cmd(0x29, screen);
    _delay_ms(120);
}


void GC9A01_cmd(uint8_t cmd, uint8_t screen) {
    DC_LOW();    
    if (screen == LEFT_EYE)  {
        CS_LEFT_EYE_LOW();
        spi_master_transmit(cmd);
        CS_LEFT_EYE_HIGH();
        return;
    } else if (screen == RIGHT_EYE)  {
        CS_RIGHT_EYE_LOW();
        spi_master_transmit(cmd);
        CS_RIGHT_EYE_HIGH();
        return;
    }
}

void GC9A01_data(uint8_t data, uint8_t screen){
    DC_HIGH();

    if (screen == LEFT_EYE)  {

        CS_LEFT_EYE_LOW();
        spi_master_transmit(data);
        CS_LEFT_EYE_HIGH();
        return;
    } else if (screen == RIGHT_EYE)  {

        CS_RIGHT_EYE_LOW();
        spi_master_transmit(data);
        CS_RIGHT_EYE_HIGH();
        return;
    }
}

void GC9A01_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t screen) {
    // Column Address Set
    GC9A01_cmd(0x2A, screen);
    GC9A01_data(0x00, screen);
    GC9A01_data(x0, screen);
    GC9A01_data(0x00, screen);
    GC9A01_data(x1, screen);

    // Row Address Set
    GC9A01_cmd(0x2B, screen);
    GC9A01_data(0x00, screen);
    GC9A01_data(y0, screen);
    GC9A01_data(0x00, screen);
    GC9A01_data(y1, screen);

    // Memory Write (start RAM write)
    GC9A01_cmd(0x2C, screen);
}

void GC9A01_fillScreen(uint16_t color, uint8_t screen) {

    GC9A01_setAddrWindow(0, 0, GC9A01_WIDTH - 1, GC9A01_HEIGHT - 1, screen);
    GC9A01_pushColor(color, (uint32_t)GC9A01_WIDTH * GC9A01_HEIGHT, screen);
}

// pixel push
void GC9A01_pushColor(uint16_t color, uint32_t count, uint8_t screen) {
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    
    if (screen == LEFT_EYE) {
        DC_HIGH();
        CS_LEFT_EYE_LOW();
        while (count--) {
            spi_master_transmit(hi);
            spi_master_transmit(lo);
        }
        CS_LEFT_EYE_HIGH();
        return;

    } else if (screen == RIGHT_EYE) {
        DC_HIGH();
  
        CS_RIGHT_EYE_LOW();
        CS_LEFT_EYE_HIGH();
        _delay_ms(150);
        while (count--) {
            spi_master_transmit(hi);
            spi_master_transmit(lo);
        }
        CS_RIGHT_EYE_HIGH();
        return;
    }
}

