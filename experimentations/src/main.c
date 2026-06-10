#include <avr/io.h>
#include <util/delay.h>

#include <stdlib.h>

#define BAUD 115200
#define UBRR ((F_CPU / (8 * BAUD)) - 1)
#define RE_SW (1 << PD0)
#define RE_OUTA (1 << PD1)
#define RE_OUTB (1 << PD2)
#define RE_MSK (RE_SW | RE_OUTA | RE_OUTB)

void init_uart(unsigned int ubrr) {
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)(ubrr);

  UCSR0A = (1 << U2X0);

  UCSR0B = (1 << RXEN0) | (1 << TXEN0);

  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(char data) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = data;
}

void uart_printstr(const char *str) {
  char *run = (char *)str;

  while (*run) {
    uart_tx(*run);
    run++;
  }
}

char uart_rx(void) { return (UDR0); }

int counter = 0;
int stateOutA = 0;
int lastStateOutA = 0;

void init_rotary_encoder(void) {
  PORTD &= ~(RE_MSK);
  PORTD |= RE_MSK;

  lastStateOutA = (PIND & RE_OUTA) >> PD1;
}

int main(void) {
  // init_uart((unsigned int)UBRR);
  // init_rotary_encoder();
  //
  // while (1) {
  //   char tmp[32] = {0};
  //
  //   stateOutA = (PIND & RE_OUTA) >> PD1;
  //   if (stateOutA != lastStateOutA) {
  //     if (((PIND & RE_OUTB) >> PD2) != stateOutA) {
  //       counter--;
  //     } else {
  //       counter++;
  //     }
  //     lastStateOutA = stateOutA;
  //   }
  //
  //   uart_printstr("Rotary encoder's position: ");
  //   itoa(counter, tmp, 10);
  //   uart_printstr(tmp);
  //   uart_printstr("\r\n");
  // _delay_ms(500);

  // if ((PIND)&RE_SW) {
  //   uart_printstr("Rotary encoder's button has been pressed\r\n");
  // }
  // if ((PIND)&RE_DT) {
  //   uart_printstr("Rotary encoder's position: ");
  //   uart_printstr(PIND0);
  //   uart_printstr("\r\n");
  // };
  // }
  return (0);
}
