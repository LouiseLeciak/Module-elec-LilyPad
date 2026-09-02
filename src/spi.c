#include "spi.h"
#include "pinout.h"

// NOTE: (p.195) In Master mode : In Master Mode, is SS pin is:
// - Output -> It's a regular GPIO pin, typically used to drive the slave's SS
// pin
// - Input -> The line must be held high, otherwise if pulled low, the SPI
// system interprets this as another master selecting the SPI as a slave
void spi_master_init(void) {
  // Sets MOSI and SCK output, the rest is input (p.172)
  DDRB |= (MOSI | SCK);

  // Enables SPI, Master, set clock rate fck/16
  SPCR = ((1 << SPE) | (1 << MSTR) | (1 << SPR0));
}

void spi_master_transmit(char data) {
  // Starts transmission
  SPDR = data;
  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF)))
    ;
}
