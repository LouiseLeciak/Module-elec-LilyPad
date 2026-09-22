#include "spi.h"

#include "pinout.h"

// NOTE: (p.195) In Master mode : In Master Mode, if SS pin is:
// - Output -> It's a regular GPIO pin, typically used to drive the slave's SS
// pin
// - Input -> The line must be held high, otherwise if pulled low, the SPI
// system interprets this as another master selecting the SPI as a slave
void spi_master_init(void) {
  DDRB |= (CS);
  PORTB |= (CS);

  DDRH |= (MAIN_SCREEN_CS | SD_CS);
  PORTH |= (MAIN_SCREEN_CS | SD_CS);

  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS);
  PORTE |= (LEFT_EYE_CS | RIGHT_EYE_CS);

  // Sets MOSI and SCK as outputs (p.172)
  DDRB |= (MOSI | SCK);

  PORTB |= (MISO);

  // Enables SPI, Master, set clock rate fck/16
  SPCR = ((1 << SPE) | (1 << MSTR) | (1 << SPR0));
  // double speed -> /2 = 8 MHz effective - speeds up the display time
  SPSR = (1 << SPI2X);
}

uint8_t spi_txrx(uint8_t data) {
  SPDR = data;
  while (!(SPSR & (1 << SPIF)));
  return SPDR;
}
