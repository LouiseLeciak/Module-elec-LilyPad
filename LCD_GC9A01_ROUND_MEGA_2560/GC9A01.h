/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GC9A01.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 13:53:22 by nige42            #+#    #+#             */
/*   Updated: 2026/07/15 13:00:29 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC9A01_H
 #define GC9A01_H

    // ─── Pin macros ───────────────────────────────────────────────────────────────
    
    #define CS_LOW() PORTL &= ~(1 << PL0) // CS MAIN LCD
    #define CS_HIGH()  PORTL |=  (1 << PL0) // CD MAIN LCD
    
    #define CS_LEFT_EYE_LOW()   PORTL &= ~(1 << PL1) // D10 PB4
    #define CS_LEFT_EYE_HIGH()  PORTL |=  (1 << PL1) // D10 PB4
    #define CS_RIGHT_EYE_LOW()   PORTL &= ~(1 << PL6)  // D53 PB0
    #define CS_RIGHT_EYE_HIGH()  PORTL |=  (1 << PL6) // D53 PB0
    #define DC_LOW()   PORTL &= ~(1 << PL4) // D11 PB5
    #define DC_HIGH()  PORTL |=  (1 << PL4) // D11 PB5
    #define RST_LOW()  PORTL &= ~(1 << PL2) // D13 PB7
    #define RST_HIGH() PORTL |=  (1 << PL2) // D13 PB7
    #define MAIN_LCD_RST_LOW()  PORTL &= ~(1 << PL7) // D12 PB6
    #define MAIN_LCD_RST_HIGH() PORTL |=  (1 << PL7) // D12 PB6
    
    
    
    // --- FONT LCD SIZE 240 x 320 - 3,2 TFT GC9A01
    
    #define MAX_PIXEL_WIDTH 240
    #define MAX_PIXEL_HIGH 240
    
    #define LEFT_EYE 1
    #define RIGHT_EYE 2
    
    

    
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
    #define GC9A01A_COLOR_EYE      0XD77A

    
    
    typedef unsigned char uint8_t;      // needed because not using stdlib
    typedef unsigned int uint16_t;      // needed because not using stdlib
    typedef uint8_t bool;
    void GC9A01_init(uint8_t screen);
    void GC9A01_cmd(uint8_t cmd, uint8_t screen);
    void GC9A01_data(uint8_t data, uint8_t screen);


#endif