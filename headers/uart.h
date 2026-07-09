#ifndef UART_H
#define UART_H

#include <avr/io.h>

#define UART_BAUDRATE 115200
#define MYUBRR (F_CPU / (8 * UART_BAUDRATE) - 1)

void uart_init(const uint8_t ubrr);
void uart_tx(char data);
void uart_printstr(const char* str);
char uart_rx(void);

#endif  // !UART_H
