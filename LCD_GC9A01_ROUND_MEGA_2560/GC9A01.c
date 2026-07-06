/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GC9A01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 13:53:11 by nige42            #+#    #+#             */
/*   Updated: 2026/07/06 12:39:37 by nige42           ###   ########.fr       */
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
    GC9A01_cmd(0x3A, screen); GC9A01_data(0x05, screen);

    // Memory Access Control: default scan direction
    GC9A01_cmd(0x36, screen); GC9A01_data(0x08, screen);  // 0xC8 180 deg
 
    // Display Function Control
    GC9A01_cmd(0xB6, screen); GC9A01_data(0x00, screen); GC9A01_data(0x20, screen);

    // --- Analog / Source timing (panel-specific, do not change) ---
    GC9A01_cmd(0x62, screen);
    GC9A01_data(0x18, screen); GC9A01_data(0x0D, screen); GC9A01_data(0x71, screen); GC9A01_data(0xED, screen);
    GC9A01_data(0x70, screen); GC9A01_data(0x70, screen); GC9A01_data(0x18, screen); GC9A01_data(0x0F, screen);
    GC9A01_data(0x71, screen); GC9A01_data(0xEF, screen); GC9A01_data(0x70, screen); GC9A01_data(0x70, screen);

    GC9A01_cmd(0x63, screen);
    GC9A01_data(0x18, screen); GC9A01_data(0x11, screen); GC9A01_data(0x71, screen); GC9A01_data(0xF1, screen);
    GC9A01_data(0x70, screen); GC9A01_data(0x70, screen); GC9A01_data(0x18, screen); GC9A01_data(0x13, screen);
    GC9A01_data(0x71, screen); GC9A01_data(0xF3, screen); GC9A01_data(0x70, screen); GC9A01_data(0x70, screen);

    // Gamma voltage — omit if causes flicker on some panels
    GC9A01_cmd(0x64, screen);
    GC9A01_data(0x28, screen); GC9A01_data(0x29, screen); GC9A01_data(0xF1, screen);
    GC9A01_data(0x01, screen); GC9A01_data(0xF0, screen); GC9A01_data(0x00, screen); GC9A01_data(0x00, screen);

    GC9A01_cmd(0x66, screen);
    GC9A01_data(0x3C, screen); GC9A01_data(0x00, screen); GC9A01_data(0xCD, screen); GC9A01_data(0x67, screen);
    GC9A01_data(0x45, screen); GC9A01_data(0x45, screen); GC9A01_data(0x10, screen);
    GC9A01_data(0x00, screen); GC9A01_data(0x00, screen); GC9A01_data(0x00, screen);

    GC9A01_cmd(0x67, screen);
    GC9A01_data(0x00, screen); GC9A01_data(0x3C, screen); GC9A01_data(0x00, screen); GC9A01_data(0x00, screen);
    GC9A01_data(0x00, screen); GC9A01_data(0x01, screen); GC9A01_data(0x54, screen);
    GC9A01_data(0x10, screen); GC9A01_data(0x32, screen); GC9A01_data(0x98, screen);

    GC9A01_cmd(0x74, screen);
    GC9A01_data(0x10, screen); GC9A01_data(0x85, screen); GC9A01_data(0x80, screen);
    GC9A01_data(0x00, screen); GC9A01_data(0x00, screen); GC9A01_data(0x4E, screen); GC9A01_data(0x00, screen);

    GC9A01_cmd(0x98, screen); GC9A01_data(0x3E, screen); GC9A01_data(0x07, screen);
    // --- End analog / source timing ---

    // Frame rate: fastest (0x00 = slowest, 0x1F = fastest)
    GC9A01_cmd(0xC6, screen); GC9A01_data(0x01, screen);

    // Gate control
    GC9A01_cmd(0xB7, screen); GC9A01_data(0x35, screen);

    // VCOM voltage
    GC9A01_cmd(0xBB, screen); GC9A01_data(0x06, screen);

    // LCM control
    GC9A01_cmd(0xC0, screen); GC9A01_data(0x6C, screen);

    // VRH: GVDD voltage level
    GC9A01_cmd(0xC3, screen); GC9A01_data(0x0B, screen);

    // VDV: VCOM offset voltage
    GC9A01_cmd(0xC4, screen); GC9A01_data(0x0F, screen);

    // Power control
    GC9A01_cmd(0xD0, screen); GC9A01_data(0xA4, screen); GC9A01_data(0xA1, screen);

    // Display inversion ON (required for correct colors on this panel)
    GC9A01_cmd(0x21, screen);

    // Sleep out — must wait 120ms before display on
    GC9A01_cmd(0x11, screen);
    _delay_ms(120);

    // Display on 0x29
    GC9A01_cmd(0x28, screen); // display off
    _delay_ms(20);
}


void GC9A01_cmd(uint8_t cmd, uint8_t screen) {
    DC_LOW();    
    if (screen == LEFT_EYE)  {
        CS_LEFT_EYE_LOW();
        spi_send(cmd);
        CS_LEFT_EYE_HIGH();
    } else if (screen == RIGHT_EYE)  {
        CS_RIGHT_EYE_LOW();
        spi_send(cmd);
        CS_RIGHT_EYE_HIGH();
    }
}

void GC9A01_data(uint8_t data, uint8_t screen){
    DC_HIGH();

    if (screen == LEFT_EYE)  {

        CS_LEFT_EYE_LOW();
        spi_send(data);
        CS_LEFT_EYE_HIGH();
    } else if (screen == RIGHT_EYE)  {

        CS_RIGHT_EYE_LOW();
        spi_send(data);
        CS_RIGHT_EYE_HIGH();
    }
}
