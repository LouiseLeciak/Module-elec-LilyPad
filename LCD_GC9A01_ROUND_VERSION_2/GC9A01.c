/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GC9A01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 13:53:11 by nige42            #+#    #+#             */
/*   Updated: 2026/07/27 11:04:58 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include "spi_lib.h"
#include "GC9A01.h"



#define MADCTL_MY 0x80  ///< Bottom to top
#define MADCTL_MX 0x40  ///< Right to left
#define MADCTL_MV 0x20  ///< Reverse Mode
#define MADCTL_ML 0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00 ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08 ///< Blue-Green-Red pixel order
#define MADCTL_MH 0x04  ///< LCD refresh right to left


#define GC9A01A_TFTWIDTH 240  ///< Display width in pixels
#define GC9A01A_TFTHEIGHT 240 ///< Display height in pixels

#define GC9A01A_SWRESET 0x01   ///< Software Reset (maybe, not documented)
#define GC9A01A_RDDID 0x04     ///< Read display identification information
#define GC9A01A_RDDST 0x09     ///< Read Display Status
#define GC9A01A_SLPIN 0x10     ///< Enter Sleep Mode
#define GC9A01A_SLPOUT 0x11    ///< Sleep Out
#define GC9A01A_PTLON 0x12     ///< Partial Mode ON
#define GC9A01A_NORON 0x13     ///< Normal Display Mode ON
#define GC9A01A_INVOFF 0x20    ///< Display Inversion OFF
#define GC9A01A_INVON 0x21     ///< Display Inversion ON
#define GC9A01A_DISPOFF 0x28   ///< Display OFF
#define GC9A01A_DISPON 0x29    ///< Display ON
#define GC9A01A_CASET 0x2A     ///< Column Address Set
#define GC9A01A_RASET 0x2B     ///< Row Address Set
#define GC9A01A_RAMWR 0x2C     ///< Memory Write
#define GC9A01A_PTLAR 0x30     ///< Partial Area
#define GC9A01A_VSCRDEF 0x33   ///< Vertical Scrolling Definition
#define GC9A01A_TEOFF 0x34     ///< Tearing Effect Line OFF
#define GC9A01A_TEON 0x35      ///< Tearing Effect Line ON
#define GC9A01A_MADCTL 0x36    ///< Memory Access Control
#define GC9A01A_VSCRSADD 0x37  ///< Vertical Scrolling Start Address
#define GC9A01A_IDLEOFF 0x38   ///< Idle mode OFF
#define GC9A01A_IDLEON 0x39    ///< Idle mode ON
#define GC9A01A_COLMOD 0x3A    ///< Pixel Format Set
#define GC9A01A_CONTINUE 0x3C  ///< Write Memory Continue
#define GC9A01A_TEARSET 0x44   ///< Set Tear Scanline
#define GC9A01A_GETLINE 0x45   ///< Get Scanline
#define GC9A01A_SETBRIGHT 0x51 ///< Write Display Brightness
#define GC9A01A_SETCTRL 0x53   ///< Write CTRL Display
#define GC9A01A1_POWER7 0xA7   ///< Power Control 7
#define GC9A01A_TEWC 0xBA      ///< Tearing effect width control
#define GC9A01A1_POWER1 0xC1   ///< Power Control 1
#define GC9A01A1_POWER2 0xC3   ///< Power Control 2
#define GC9A01A1_POWER3 0xC4   ///< Power Control 3
#define GC9A01A1_POWER4 0xC9   ///< Power Control 4
#define GC9A01A_RDID1 0xDA     ///< Read ID 1
#define GC9A01A_RDID2 0xDB     ///< Read ID 2
#define GC9A01A_RDID3 0xDC     ///< Read ID 3
#define GC9A01A_FRAMERATE 0xE8 ///< Frame rate control
#define GC9A01A_SPI2DATA 0xE9  ///< SPI 2DATA control
#define GC9A01A_INREGEN2 0xEF  ///< Inter register enable 2
#define GC9A01A_GAMMA1 0xF0    ///< Set gamma 1
#define GC9A01A_GAMMA2 0xF1    ///< Set gamma 2
#define GC9A01A_GAMMA3 0xF2    ///< Set gamma 3
#define GC9A01A_GAMMA4 0xF3    ///< Set gamma 4
#define GC9A01A_IFACE 0xF6     ///< Interface control
#define GC9A01A_INREGEN1 0xFE  ///< Inter register enable 1




void GC9A01_version2_init(uint8_t screen) {

    // --- POWER STABILIZATION ---
    // _delay_ms(300);

      RST_HIGH();
    _delay_ms(20);
    // --- HARDWARE RESET ---
    RST_LOW();
    _delay_ms(20);
    RST_HIGH();
    _delay_ms(120);
    
    // // --- SOFTWARE RESET ---
    GC9A01_cmd(0x01, screen);
    _delay_ms(150);
    // --- BEGIN TABLE CONVERSION ---

    GC9A01_cmd(GC9A01A_INREGEN2, screen);

    GC9A01_cmd(0xEB, screen); GC9A01_data(0x14, screen);

    GC9A01_cmd(GC9A01A_INREGEN1, screen);
    GC9A01_cmd(GC9A01A_INREGEN2, screen);

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

    GC9A01_cmd(0xB6, screen);
    GC9A01_data(0x00, screen);
    GC9A01_data(0x00, screen);

    GC9A01_cmd(GC9A01A_MADCTL, screen);
    GC9A01_data(MADCTL_MX | MADCTL_BGR, screen);

    GC9A01_cmd(GC9A01A_COLMOD, screen);
    GC9A01_data(0x05, screen);

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

    GC9A01_cmd(GC9A01A1_POWER2, screen); GC9A01_data(0x13, screen);
    GC9A01_cmd(GC9A01A1_POWER3, screen); GC9A01_data(0x13, screen);
    GC9A01_cmd(GC9A01A1_POWER4, screen); GC9A01_data(0x22, screen);

    GC9A01_cmd(0xBE, screen); GC9A01_data(0x11, screen);

    GC9A01_cmd(0xE1, screen);
    GC9A01_data(0x10, screen);
    GC9A01_data(0x0E, screen);

    GC9A01_cmd(0xDF, screen);
    GC9A01_data(0x21, screen);
    GC9A01_data(0x0C, screen);
    GC9A01_data(0x02, screen);

    // GAMMA SETS
    GC9A01_cmd(GC9A01A_GAMMA1, screen);
    GC9A01_data(0x45, screen); GC9A01_data(0x09, screen);
    GC9A01_data(0x08, screen); GC9A01_data(0x08, screen);
    GC9A01_data(0x26, screen); GC9A01_data(0x2A, screen);

    GC9A01_cmd(GC9A01A_GAMMA2, screen);
    GC9A01_data(0x43, screen); GC9A01_data(0x70, screen);
    GC9A01_data(0x72, screen); GC9A01_data(0x36, screen);
    GC9A01_data(0x37, screen); GC9A01_data(0x6F, screen);

    GC9A01_cmd(GC9A01A_GAMMA3, screen);
    GC9A01_data(0x45, screen); GC9A01_data(0x09, screen);
    GC9A01_data(0x08, screen); GC9A01_data(0x08, screen);
    GC9A01_data(0x26, screen); GC9A01_data(0x2A, screen);

    GC9A01_cmd(GC9A01A_GAMMA4, screen);
    GC9A01_data(0x43, screen); GC9A01_data(0x70, screen);
    GC9A01_data(0x72, screen); GC9A01_data(0x36, screen);
    GC9A01_data(0x37, screen); GC9A01_data(0x6F, screen);

    GC9A01_cmd(0xED, screen);
    GC9A01_data(0x1B, screen);
    GC9A01_data(0x0B, screen);

    GC9A01_cmd(0xAE, screen); GC9A01_data(0x77, screen);
    GC9A01_cmd(0xCD, screen); GC9A01_data(0x63, screen);

    GC9A01_cmd(GC9A01A_FRAMERATE, screen);
    GC9A01_data(0x34, screen);

    // SOURCE TIMING
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

    // TE ON
    GC9A01_cmd(GC9A01A_TEON, screen);

    // INVERSION ON
    GC9A01_cmd(GC9A01A_INVON, screen);

    // SLEEP OUT
    GC9A01_cmd(GC9A01A_SLPOUT, screen);
    _delay_ms(150);

    // DISPLAY ON
    GC9A01_cmd(GC9A01A_DISPON, screen);
    _delay_ms(150);
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
