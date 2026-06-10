#ifndef SPI_LIB_H
#define SPI_LIB_H

// See ATmega2560 datasheet pinout on p.2
#define SS_LINES (1 << PB0)
#define SCK (1 << PB1)
#define MOSI (1 << PB2)
#define MISO (1 << PB3)

/**
 * @brief Initialises SPI module for ATMega2560.
 */
void spi_master_init (void);

/**
 * @brief Send data through SPI bus.
 *
 * @param data Data to be sent.
 */
void spi_master_transmit (char data);

#endif
