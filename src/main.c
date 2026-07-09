#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "ili9488.h"
#include "main_screen.h"
#include "pins.h"
#include "sd.h"
#include "spi.h"
#include "structs.h"
#include "uart.h"

static void setup(void) {
  DDRB |= (SS_MASK | MAIN_SCREEN_DC | MAIN_SCREEN_RST | MCU_SPI_MASK);
  DDRB &= ~(MCU_MISO);
  PORTB |= (SS_MASK | MAIN_SCREEN_DC | MAIN_SCREEN_RST);

  spi_master_init(FOSC_DIV128);
  // spi_start_transaction(&PORTB, MAIN_SCREEN_SS);
  // ili9488_init_driver();
  // main_screen_init();
  // spi_end_transaction(&PORTB, MAIN_SCREEN_SS);
  uart_init(F_CPU / (8 * UART_BAUDRATE) - 1);
}

int main(void) {
  sd_card card = {0};
  setup();

  uart_printstr("UART MODULE : [OK]\r\n");

  if (sd_spi_init(&card)) {
    uart_printstr("CRITICAL ERROR\r\n");
    return (1);
  }

  spi_start_transaction(&PORTB, SD_SS);
  uint8_t data[512] = {0};
  uart_printstr("Reading SD Card's first block\r\n");
  sd_read_single_block(0x00, data);
  spi_end_transaction(&PORTB, SD_SS);

  uart_printstr("SD Card read, now displaying:\r\n\r\n");

  for (uint16_t i = 0; i < 512; i++) {
    uart_tx(data[i]);
    uart_printstr("\r\n");
  }

  while (1) {
    ;
  }

  return (0);
}
