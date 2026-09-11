#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

void spi_master_init(void);

/**
 * @brief Sends the data passed to it via SPI and returns the answer.
 *
 * @param data The data to send.
 * @return The response, it might be rubbish if you send something that does not
 * expect a response.
 */
uint8_t spi_txrx(uint8_t data);

#endif // !SPI_H
