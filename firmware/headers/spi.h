/**
 * @file spi.h
 * @brief SPI driver for ATmega2560.
 * @author lleciak, mpetitco, nrobinso
 */

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

/**
 * @defgroup SPI_Driver SPI Peripheral Driver
 * @brief    Hardware driver for SPI communication.
 * @{
 */

/**
 * @brief Initializes the microcontroller as an SPI Master.
 *
 * @note This function must be called before any SPI transmissions occur.
 */
void spi_master_init(void);

/**
 * @brief Transmits and receives a byte over the SPI bus.
 *
 * @param[in] data The byte to transmit.
 *
 * @return The received byte. If the transaction does not expect a response,
 *         this value will be dummy data and should be ignored.
 *
 * @warning This function is blocking and waits for the transmission to
 * complete.
 */
uint8_t spi_txrx(uint8_t data);

/** @} */

#endif  // !SPI_H
