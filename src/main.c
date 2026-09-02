#include "pinout.h"
#include "spi.h"
#include "state_machine.h"

#include <util/delay.h>

t_state current_state = INIT;

void main_screen_init() {
  DDRH |= (MAIN_SCREEN_CS | MAIN_SCREEN_BL | MAIN_SCREEN_RST);

  PORTH |= (MAIN_SCREEN_BL);
}

void sd_init() { DDRH |= (SD_CS); }

void eyes_init() { DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST); }

void screens_init() {
  DDRH |= (SCREENS_DC);

  main_screen_init();
  eyes_init();
}

void keyboard_init(void) {
  DDRA |= (KB_C1 | KB_C2 | KB_C3 | KB_C4 | KB_C5);
  DDRC |= (KB_R4);
  DDRG |= (KB_C6);
  DDRJ |= (KB_R1 | KB_R2 | KB_R3 | KB_C7 | KB_C8 | KB_C9 | KB_C10);

  PORTA &= ~(KB_C1 | KB_C2 | KB_C3 | KB_C4 | KB_C5);
  PORTC &= ~(KB_R4);
  PORTG &= ~(KB_C6);
  PORTJ &= ~(KB_R1 | KB_R2 | KB_R3 | KB_C7 | KB_C8 | KB_C9 | KB_C10);
}

void rotary_encoder_init(void) { DDRC |= (SDL_SW1 | SDL_SW2 | SDL_SW3); }

void init(void) {
  // TODO: Init I2C
  //  DDRD |= (SCL | SDA); // Not sure if necessary after i2c_init

  spi_master_init();
  // spi_master_init

  screens_init();
  sd_init();
  keyboard_init();
}

// WARNING: CHECKER PIN 12 ATMEGA2560 (Main Screen CS), PIN 60 ATMEGA2560
// (KB_R4)

int main(void) {
  // init();

  DDRA |= (KB_C1 | KB_C2 | KB_C3 | KB_C4 | KB_C5);
  DDRB |= (CS | SCK | MOSI | MISO);
  DDRC |= (SDL_SW1 | SDL_SW2 | SDL_SW3);
  DDRD |= (SCL | SDA);
  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
  DDRG |= (KB_C6);
  DDRH |=
      (MAIN_SCREEN_CS | MAIN_SCREEN_RST | MAIN_SCREEN_BL | SCREENS_DC | SD_CS);
  DDRJ |= (KB_C7 | KB_C8 | KB_C9 | KB_C10 | KB_R1 | KB_R2 | KB_R3 | KB_R4);

  // PORTA |= (KB_C1);
  // PORTB |= (SCK);
  // PORTC |= (SDL_SW3);
  // PORTD |= (SDA);
  // PORTE |= (LEFT_EYE_CS);
  // PORTG |= (KB_C6);
  // PORTH |= (SD_CS);
  // PORTJ |= (KB:_R1);

  while (1) {
    // PORTJ |= (KB_R3);
    // PORTC |= (SDL_SW3);
    // PORTB |= (CS | SCK | MISO | MOSI);
    // PORTE |= (RIGHT_EYE_CS | EYES_RST);
    // PORTH |= (SCREENS_DC);
  }
}
