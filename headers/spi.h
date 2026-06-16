#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

typedef enum s_spi_sck_freq {
  FOSC_DIV2 = 2,
  FOSC_DIV4 = 4,
  FOSC_DIV8 = 8,
  FOSC_DIV16 = 16,
  FOSC_DIV128 = 28,
  FOSC_DIV32 = 32,
  FOSC_DIV64 = 64,
  FOSC_DIV64_BIS = -64,
} spi_sck_freq;

void spi_master_init(const spi_sck_freq fspi);
void spi_master_transmit(char data);

void spi_start_transaction(volatile uint8_t* port, const uint16_t ss_pin);
void spi_end_transaction(volatile uint8_t* port, const uint16_t ss_pin);

#endif  // !SPI_H
