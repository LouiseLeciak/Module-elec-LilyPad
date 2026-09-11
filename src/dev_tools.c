/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dev_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 10:43:35 by nige42            #+#    #+#             */
/*   Updated: 2026/09/11 10:51:19 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include "dev_tools.h"
#include "pinout.h"
#include "GC9A01.h"

void testPINS(void) {

  //  KEYBOARD
  // PORTA |= (KB_C1);
  // PORTA |= (KB_C2);
  // PORTA |= (KB_C3);
  // PORTA |= (KB_C4);
  // PORTA |= (KB_C5);
  // PORTG |= (KB_C6);
  // PORTJ |= (KB_C7);
  // PORTJ |= (KB_C8);
  // PORTJ |= (KB_C9);
  // PORTJ |= (KB_C10);
  // PORTJ |= (KB_R1);
  // PORTJ |= (KB_R2);
  // PORTJ |= (KB_R3);
  // PORTC |= (KB_R4);
  // PORTD |= (SCL);
  // PORTD |= (SDA);
  // PORTC |= (SDL_SW1);
  // PORTC |= (SDL_SW2);
  // PORTC |= (SDL_SW3);
  // PORTB |= (SCK);

  // SD_READER
  PORTH &= ~(SD_CS);
  PORTH |= (SD_CS);

  // MAIN SCREEN
  // PORTH |= (MAIN_SCREEN_RST); 
  PORTH |= (MAIN_SCREEN_BL);
  PORTH |= (MAIN_SCREEN_CS); 
  // PORTB |= (SCK | MISO | MOSI);
  // PORTH |= (SCREENS_DC);
  
  // EYE_RIGHT_SCREEN
  PORTE |= (RIGHT_EYE_CS);
  PORTE |= (EYES_RST);
  PORTB |= (SCK);
  PORTB |= (MOSI);
  PORTH |= (SCREENS_DC);
  
  // EYE_LEFT_SCREEN
  PORTE |= (LEFT_EYE_CS);
  PORTE |= (EYES_RST);
  PORTB |= (SCK);
  PORTB |= (MOSI);
  PORTH |= (SCREENS_DC);

}