/**
 * @file uart.h
 * @brief UART driver for ATmega2560
 * @details This file provides functions to initialise the UART peripheral, and
 * send/receive characters and strings over serial.
 * @author lleciak, mpetitco, nrobinso
 */

#ifndef UART_H
#define UART_H

#include <avr/io.h>

/**
 * @defgroup UART_Driver UART Peripheral Driver
 * @brief    Hardware driver for UART communication.
 * @{
 */

#define UART_BAUDRATE 115200  ///< Target baud rate for serial communication

/**
 * @brief Macro to calculate the UBRR register value based on CPU frequency
 */
#define MYUBRR (F_CPU / (8 * UART_BAUDRATE) - 1)

/**
 * @brief Initialises the UART peripheral with the given baud rate.
 *
 * @param[in] ubrr The calculated baud rate register value.
 *
 * @note This configures the UART for 8-bit data, no parity, 1 stop bit.
 */
void uart_init(const uint8_t ubrr);

/**
 * @brief Sends the given data over serial.
 *
 * @param[in] data The character to send over serial.
 */
void uart_tx(char data);

/**
 * @brief Send the given string over serial.
 *
 * @param[in] str The string to send over serial.
 */
void uart_printstr(const char* str);

/**
 * @brief Reads a character sent over serial.
 *
 * @return The read character.
 */
char uart_rx(void);

/** @} */

#endif  // !UART_H
