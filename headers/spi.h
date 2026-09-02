#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

void spi_master_init(void);
void spi_master_transmit(char data);

void spi_start_transmit(void);
void spi_end_transmit(void);

#endif // !SPI_H
