/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen_text.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:21:21 by nige42            #+#    #+#             */
/*   Updated: 2026/09/15 11:57:12 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCREEN_TEXT_H 
 #define SCREEN_TEXT_H

#include <avr/io.h>


 #define CS_MAIN_LOW()  PORTH &= ~(1 << PH0);
 #define CS_MAIN_HIGH() PORTH |=(1 << PH0);
 #define DC_LOW()   PORTH &= ~SCREENS_DC
 #define DC_HIGH()  PORTH |=  SCREENS_DC


 /// @brief Standard strlen
 /// @param str pointer to string to length tested
 /// @return number of chars in the string
 uint8_t ft_text_len(const char *str);
 
 /// @brief sends data to main screen
 /// @param data uint8_t packets 
 void ili9341_data(uint8_t data);

 /// @brief sends cmd to main screen
 /// @param cmd uint8_t packets 
 void ili9341_cmd(uint8_t cmd);

 /// @brief Sets window size to write to a square / rectangle
 /// @param x0 X position of top left point
 /// @param y0 Y position of top left point
 /// @param x1 X position of bottom right point
 /// @param y1 Y position of bottom right point
 void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

 /// @brief draws one pixel of a "color" to main screen at position x y
 /// @param x X position on screen
 /// @param y Y position on screen
 /// @param color in format RGB 565 and parsed in the function
 void draw_pixel(uint16_t x, uint16_t y, uint16_t color);

 /// @brief Draws to main screen one character 'c' at position x, y in color fg and bg using scale factor
 /// @param x X position on screen
 /// @param y Y position on screen
 /// @param c character to be written
 /// @param fg foreground color
 /// @param bg background color
 /// @param ScaleFactor Scale factor 1, 2, 3, 4 etc
 void draw_char_small(uint16_t x, uint16_t y, uint8_t c, uint16_t fg, uint16_t bg, uint8_t ScaleFactor);

 /// @brief 
 /// @param x X position on screen
 /// @param y Y position on screen
 /// @param str pointer to a string to be written 
 /// @param fg foreground color
 /// @param bg background color
 /// @param ScaleFactor Scale factor 1, 2, 3, 4 etc
 /// @param HGap horizontal spacing. suggestion Scale 3 HGap 3, Scale 1 HGap 1
 void draw_string(uint16_t x, uint16_t y, const char *str, uint16_t fg, uint16_t bg, uint8_t ScaleFactor, uint8_t HGap);

 
#endif