#include "pinout.h"
#include "spi.h"

void init(void) {
  DDRB |= (CS | SCK | MOSI | MISO);
  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);

  PORTB |= (CS | SCK | MOSI | MISO);
  PORTE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
}

int main(void) {
  init();

  while (1) {
    ;
  }
}
