/**
 * @file uart.h
 * @brief UART driver for ATmega2560
 * @details This file provides functions to initialise the UART peripheral, and
 * send/receive characters and strings over serial.
 * @author lleciak, mpetitco, nrobinso
 */

#ifndef UART_H
#define UART_H

/* Reset */
#define ANSI_RESET "\033[0m"

/* Regular text colors */
#define ANSI_BLACK "\033[30m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_BLUE "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN "\033[36m"
#define ANSI_WHITE "\033[37m"

/* Bright/Bold text colors */
#define ANSI_BBLACK "\033[90m"
#define ANSI_BRED "\033[91m"
#define ANSI_BGREEN "\033[92m"
#define ANSI_BYELLOW "\033[93m"
#define ANSI_BBLUE "\033[94m"
#define ANSI_BMAGENTA "\033[95m"
#define ANSI_BCYAN "\033[96m"
#define ANSI_BWHITE "\033[97m"

/* Background colors */
#define ANSI_BG_BLACK "\033[40m"
#define ANSI_BG_RED "\033[41m"
#define ANSI_BG_GREEN "\033[42m"
#define ANSI_BG_YELLOW "\033[43m"
#define ANSI_BG_BLUE "\033[44m"
#define ANSI_BG_MAGENTA "\033[45m"
#define ANSI_BG_CYAN "\033[46m"
#define ANSI_BG_WHITE "\033[47m"

/* Text styles */
#define ANSI_BOLD "\033[1m"
#define ANSI_DIM "\033[2m"
#define ANSI_ITALIC "\033[3m"
#define ANSI_UNDERLINE "\033[4m"
#define ANSI_BLINK "\033[5m"
#define ANSI_REVERSE "\033[7m"
#define ANSI_HIDDEN "\033[8m"
#define ANSI_STRIKE "\033[9m"

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
