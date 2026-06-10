#include <avr/io.h>

#include "spi_lib.h"

// NOTE: Every page mention is referring to ATmega2460's datasheet until if
// mentionned otherwise.

// --- Main LCD Screen defines ---------------------------------------------
// Pin on which we hooked the main LCD screen's Data/Command pin.
// NOTE: This doesn't follow the pinout on the specifications !!!
#define LCD_MAIN_DC (1 << PB5)
#define LCD_MAIN_RST (1 << PB6)
// !-- Main LCD Screen defines ---------------------------------------------

void
spi_master_init (void)
{
  // In this context (MCU with only one screen driver on the SPI line), it's
  // useless to use MISO as the screen driver should never send us anything.
  // That's why we don't set MISO here.
  DDRB |= (SS_LINES | SCK | MOSI | LCD_MAIN_DC);

  // NOTE: With only SPR0 set, we have a clock of F_CPU/16 = 1,000,000 (p.198).
  // SPI clock frequency should be determined relative to the driver's maximum
  // supported frequency, the ILI9488 supports up to 20MHz (see ILI9488's
  // datasheet p.55).

  // Perhaps setting DORD to 1 would fix the reverse displaying issue ?
  // (apetitco)
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void
spi_master_transmit (char data)
{
  SPDR = data;
  while (!(SPSR & (1 << SPIF)))
    ;
}
