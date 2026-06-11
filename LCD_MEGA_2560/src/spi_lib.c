#include <avr/io.h>

#include "spi_lib.h"

// NOTE: Every page mention is referring to ATmega2460's datasheet until if
// mentionned otherwise.

// --- Main LCD Screen defines ---------------------------------------------
// Pin on which we hooked the main LCD screen's Data/Command pin.
// WARNING: This doesn't follow the pinout on the specifications !!!
#define LCD_MAIN_DC (1 << PB5)
#define LCD_MAIN_RST (1 << PB6)
// !-- Main LCD Screen defines ---------------------------------------------

void
spi_master_init (void)
{
  // In this context (MCU with only one screen driver on the SPI line), it's
  // useless to use MISO as the screen driver should never send us anything.
  // That's why we don't set MISO here.
  DDRB |= (SS_LINES | SCK | MOSI | LCD_MAIN_DC | LCD_MAIN_RST);

  PORTB |= (SS_LINES | LCD_MAIN_DC | LCD_MAIN_RST);

  // NOTE: SPI clock frequency should be determined relative to the driver's
  // maximum supported frequency, the ILI9488 supports up to 20MHz (see
  // ILI9488's datasheet p.55).

  SPCR = (1 << SPE) | (1 << MSTR);
}

void
spi_master_transmit (char data)
{
  SPDR = data;
  while (!(SPSR & (1 << SPIF)))
    ;
}
