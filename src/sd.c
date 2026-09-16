#include "sd.h"

#include <avr/io.h>
#include <stdint.h>

#include "crc.h"
#include "pinout.h"
#include "spi.h"
#include "uart.h"

void sd_init(void) {
  uart_printstr("Initialising SD card...\r\n");

  // Set SPI clock between 100kHz and 400kHz (as per Elm-Chan guide)
  SPCR &= ~((1 << SPI2X) | (1 << SPR0));
  SPCR |= (1 << SPR1);

  DDRB |= (MOSI);
  PORTB |= (MOSI);
  DDRH |= (SD_CS);
  PORTH |= (SD_CS);

  for (uint8_t i = 0; i < 74; i++) { // Dummy clocks
    spi_txrx(0);
  }

  uart_printstr("SD card is currently in SD mode.\r\n");

  PORTH &= (SD_CS);
  sd_go_idle_state(0);
}

void sd_crc7_gen(struct s_sd_cmd *cmd) {
  uint8_t crc7 =
      generate_crc7((uint8_t *)cmd, 5); // 1 byte for CMD and 4 bytes for arg
  cmd->crc7_and_end_bit = (crc7 << 1) | 1;
}

uint8_t sd_send_cmd(struct s_sd_cmd *cmd) {
  uint8_t response = 0;
  for (uint8_t i = 0; i < (sizeof(*cmd) / sizeof(uint8_t)); i++) {
    response = spi_txrx((((uint8_t *)cmd)[i]));
  }
  return response;
}

void sd_go_idle_state(uint8_t arg[4]) {
  struct s_sd_cmd cmd00 = {GO_IDLE_STATE, {arg[0], arg[1], arg[2], arg[3]}, 0};
  sd_crc7_gen(&cmd00);
  sd_send_cmd(&cmd00);
}
