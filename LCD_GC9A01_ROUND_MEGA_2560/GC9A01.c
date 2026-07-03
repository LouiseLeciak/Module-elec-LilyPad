/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GC9A01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 13:53:11 by nige42            #+#    #+#             */
/*   Updated: 2026/07/03 15:51:34 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include "spi_lib.h"
#include "GC9A01.h"

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
    GC9A01_cmd(0x36, screen); GC9A01_data(0x08);  // 0xC8 180 deg
 
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

    // Display on 0x29
    GC9A01_cmd(0x28, 1); // display off
    _delay_ms(20);
}


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
