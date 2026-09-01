#include "pinout.h"
#include "spi.h"

#include <util/delay.h>

void init(void) {
  DDRB |= (CS | SCK | MOSI | MISO);
  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
  DDRH |= (SCREENS_DC);

  PORTB |= (CS | SCK | MOSI | MISO);
  PORTE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
  PORTH &= ~(SCREENS_DC);
}

int main(void) {
  init();

  while (1) {
    _delay_ms(1000);
    PORTB ^= (CS | SCK | MOSI | MISO);
    PORTE ^= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
    PORTH ^= (SCREENS_DC);
  }
}
