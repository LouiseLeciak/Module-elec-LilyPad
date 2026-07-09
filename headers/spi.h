#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdint.h>

/**
 * @typedef s_spi_sck_freq
 * @brief Dividers for the SPI Clock frequency.
 *
 */
typedef enum s_spi_sck_freq {
  FOSC_DIV2 = 2,         ///< F_CPU / 2
  FOSC_DIV4 = 4,         ///< F_CPU / 4
  FOSC_DIV8 = 8,         ///< F_CPU / 8
  FOSC_DIV16 = 16,       ///< F_CPU / 16
  FOSC_DIV32 = 32,       ///< F_CPU / 32
  FOSC_DIV64 = 64,       ///< F_CPU / 64
  FOSC_DIV64_BIS = -64,  ///< F_CPU / 64
  FOSC_DIV128 = 128      ///< F_CPU / 128
} spi_sck_freq;

/**
 * @brief Initialises the SPI module of the ATmega2560 with the provided clock
 * frequency divider.
 *
 * @param fspi SPI Clock Frequency divider.
 */
void spi_master_init(const spi_sck_freq fspi);

/**
 * @brief Transmits data over SPI.
 *
 * @param data The data to be transmitted.
 */
uint8_t spi_master_transmit(uint8_t data);

/**
 * @brief Starts a SPI transaction, lowering the provided Slave Select pin on
 * the provided port.
 *
 * @param port Port of the Slave Select pin.
 * @param ss_pin Slave Select pin to be pulled LOW.
 */
void spi_start_transaction(volatile uint8_t* port, const uint16_t ss_pin);

/**
 * @brief Ends a SPI transaction, pulling to HIGH the provided Slave Select pin
 * on the provided port.
 *
 * @param port Port of the Slave Select pin.
 * @param ss_pin Slave Select pin to be pulled HIGH.
 */
void spi_end_transaction(volatile uint8_t* port, const uint16_t ss_pin);

#endif  // !SPI_H
