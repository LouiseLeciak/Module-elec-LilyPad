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

#define CS_MAIN_LOW() PORTE &= ~(MAIN_SCREEN_CS);
#define CS_MAIN_HIGH() PORTE |= MAIN_SCREEN_CS;
#define DC_LOW() PORTH &= ~SCREENS_DC
#define DC_HIGH() PORTH |= SCREENS_DC

/// @brief Draws to main screen one character 'c' at position x, y in color fg
/// and bg using scale factor
/// @param x X position on screen
/// @param y Y position on screen
/// @param c character to be written
/// @param fg foreground color
/// @param bg background color
/// @param ScaleFactor Scale factor 1, 2, 3, 4 etc
void draw_char_small(uint16_t x, uint16_t y, uint8_t c, uint16_t fg,
                     uint16_t bg, uint8_t ScaleFactor);

/// @brief
/// @param x X position on screen
/// @param y Y position on screen
/// @param str pointer to a string to be written
/// @param fg foreground color
/// @param bg background color
/// @param ScaleFactor Scale factor 1, 2, 3, 4 etc
/// @param HGap horizontal spacing. suggestion Scale 3 HGap 3, Scale 1 HGap 1
void draw_string(uint16_t x, uint16_t y, const char* str, uint16_t fg,
                 uint16_t bg, uint8_t ScaleFactor, uint8_t HGap);

#endif
