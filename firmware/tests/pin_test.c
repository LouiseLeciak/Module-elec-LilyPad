#include <avr/io.h>

#include "pinout.h"

int main(void)
{
  DDRA |= (SDL_SW3);
  DDRB |= (CS | SCK | MOSI | MISO);
  DDRC |= (KB_R1 | KB_R2 | KB_R3 | KB_R4 | KB_C1 | KB_C2 | KB_C3 | KB_C4);
  DDRD |= (SCL | SDA);
  DDRE |= (MAIN_SCREEN_CS | MAIN_SCREEN_RST | MAIN_SCREEN_BL | SD_CS);
  DDRH |= (SCREENS_DC | LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
  DDRG |= (SDL_SW2);
  DDRJ |= (KB_C5 | KB_C6 | KB_C7 | KB_C8 | KB_C9 | KB_C10 | SDL_SW1);

  PORTA &= ~(SDL_SW3);
  PORTB &= ~(CS | SCK | MOSI | MISO);
  PORTC |= ~(KB_R1 | KB_R2 | KB_R3 | KB_R4 | KB_C1 | KB_C2 | KB_C3 | KB_C4);
  PORTD &= ~(SCL | SDA);
  PORTE &= ~(MAIN_SCREEN_CS | MAIN_SCREEN_RST | MAIN_SCREEN_BL | SD_CS);
  PORTH &= ~(SCREENS_DC | LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
  PORTG &= ~(SDL_SW2);
  PORTJ &= ~(KB_C5 | KB_C6 | KB_C7 | KB_C8 | KB_C9 | KB_C10 | SDL_SW1);

  // SCREENS
  // PORTE |= (MAIN_SCREEN_CS);
  // PORTE |= (MAIN_SCREEN_RST);
  // PORTB |= (MOSI);
  // PORTE |= (MAIN_SCREEN_BL);
  // PORTE |= (SD_CS);
  // PORTH |= (LEFT_EYE_CS);
  // PORTH |= (EYES_RST);
  // PORTH |= (RIGHT_EYE_CS);

  // KEYBOARD
  // PORTC |= (KB_R1);
  // PORTC |= (KB_R2);
  // PORTC |= (KB_R3);
  // PORTC |= (KB_R4);
  // PORTC |= (KB_C1);
  // PORTC |= (KB_C2);
  // PORTC |= (KB_C3);
  // PORTC |= (KB_C4);
  // PORTJ |= (KB_C5);
  // PORTJ |= (KB_C6);
  // PORTJ |= (KB_C7);
  // PORTJ |= (KB_C8);
  // PORTJ |= (KB_C9);
  // PORTJ |= (KB_C10);
  // PORTJ |= (SDL_SW1);
  // PORTG |= (SDL_SW2);
  // PORTA |= (SDL_SW3);
  // PORTD |= (SCL);
  // PORTD |= (SDA);

  // TEST AFTER FIXING
  // PORTH |= (SCREENS_DC);
  // DDRB |= (SCK);
  // DDRB |= (MISO);
}
