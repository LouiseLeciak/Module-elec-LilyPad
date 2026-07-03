#include "spi.h"

#include <avr/io.h>
#include <stdint.h>

static void set_spi_freq(const spi_sck_freq fspi) {
  switch (fspi) {
  case FOSC_DIV4:
    SPCR &= ~((1 << SPR1) | (1 << SPR0));
    SPSR &= ~(1 << SPI2X);
    break;
  case FOSC_DIV16:
    SPCR &= ~(1 << SPR1);
    SPCR |= (1 << SPR0);
    SPSR &= ~(1 << SPI2X);
    break;
  case FOSC_DIV64:
    SPCR &= ~(1 << SPR0);
    SPCR |= (1 << SPR1);
    SPSR &= ~(1 << SPI2X);
    break;
  case FOSC_DIV128:
    SPCR |= ((1 << SPR1) | (1 << SPR0));
    SPSR &= ~(1 << SPI2X);
    break;
  case FOSC_DIV2:
    SPCR &= ~((1 << SPR1) | (1 << SPR0));
    SPSR |= (1 << SPI2X);
    break;
  case FOSC_DIV8:
    SPCR &= ~(1 << SPR1);
    SPCR |= (1 << SPR0);
    SPSR |= (1 << SPI2X);
    break;
  case FOSC_DIV32:
    SPCR &= ~(1 << SPR0);
    SPCR |= (1 << SPR1);
    SPSR |= (1 << SPI2X);
    break;
  case FOSC_DIV64_BIS:
    SPCR |= ((1 << SPR1) | (1 << SPR0));
    SPSR |= (1 << SPI2X);
    break;
  default:
    break;
  }
}

void spi_master_init(const spi_sck_freq fspi) {
  SPCR |= ((1 << SPE) | (1 << MSTR));
  set_spi_freq(fspi);
}

// TODO: Maybe change function signature to
// void spi_master_transmit(uint8_t* data, size_t size)
// Where data is a pointer to data serialised to 8bit parts (by typecast) and
// size is the result of sizeof(data) That could allow to give this function
// anything and be sure it will transmit the data correctly
void spi_master_transmit(uint8_t data) {
  // Starts transmission
  SPDR = data;
  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF)))
    ;
}

void spi_start_transaction(volatile uint8_t *port, const uint16_t ss_pin) {
  *port &= ~(ss_pin);
}

void spi_end_transaction(volatile uint8_t *port, const uint16_t ss_pin) {
  *port |= ss_pin;
}
