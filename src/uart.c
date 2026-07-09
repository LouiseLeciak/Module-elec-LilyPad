#include "uart.h"

#include <avr/io.h>

void uart_init(const uint8_t ubrr) {
  UBRR0H = (ubrr >> 8);
  UBRR0L = ubrr;

  UCSR0A = _BV(U2X0);

  UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);

  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

void uart_tx(char data) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = data;
}

void uart_printstr(const char* str) {
  char* run = (char*)str;

  while (*run) {
    uart_tx(*run);
    run++;
  }
}

char uart_rx(void) {
  return (UDR0);
}
