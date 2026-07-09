#include "sd.h"

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "pins.h"
#include "sd_cmd.h"
#include "spi.h"
#include "uart.h"

uint8_t send_sd_frame(const sd_frame frame) {
  spi_master_transmit(frame.cmd);
  spi_master_transmit(frame.arg >> 24);
  spi_master_transmit(frame.arg >> 16);
  spi_master_transmit(frame.arg >> 8);
  spi_master_transmit(frame.arg & 0xFF);
  return (spi_master_transmit(frame.crc));
}

void itohs(const uint8_t nb, char *buf) {
  const char *base = "0123456789ABCDEF";
  buf[0] = '0';
  buf[1] = 'x';
  buf[2] = base[nb % 16];
  buf[3] = base[nb / 16];
}

uint8_t sd_spi_init(sd_card *card) {
  uart_printstr("SD SPI INIT: START\r\n");
  set_spi_freq(FOSC_DIV64);
  _delay_ms(100); // Let time for the voltage to stabilise

  spi_end_transaction(&PORTB, SD_SS);
  spi_master_transmit(0xFF);

  // 10 * 8 bits = 80 bits -> 80 SPI clock cycles
  for (uint8_t i = 0; i < 10; i++) {
    spi_master_transmit(0xFF);
  }

  for (uint8_t attempts = 0; attempts < 40; attempts++) {
    if (attempts == 39) {
      return (1);
    }
    if (sd_go_idle_state() == 0) {
      break;
    }
  }

  uart_printstr("SENDING CMD8\r\n");
  send_sd_frame((sd_frame){SEND_IF_COND, 0x000001AA, 0x87});
  while (spi_master_transmit(0xFF) != 0x01) {
    ;
  }
  uint32_t res = 0;
  for (uint8_t i = 0; i < 4; i++) {
    res <<= 8;
    res |= spi_master_transmit(0xFF);
  }

  if ((res & 0xFF) != 0xAA) {
    // Something wrong happened
    uart_printstr("CMD8 : [FAILED]\r\n");
    spi_end_transaction(&PORTB, SD_SS);
    spi_master_transmit(0xFF);
    return (1);
  }

  while (1) {
    uint8_t res = 0xFF;

    spi_start_transaction(&PORTB, SD_SS);
    spi_master_transmit(0xFF);

    uart_printstr("SENDING CMD55\r\n");
    send_sd_frame((sd_frame){APP_CMD, 0x00, 0x01});

    while (spi_master_transmit(0xFF) != 0x01) {
      ;
    }

    spi_end_transaction(&PORTB, SD_SS);
    spi_master_transmit(0xFF);

    spi_start_transaction(&PORTB, SD_SS);
    spi_master_transmit(0xFF);

    uart_printstr("SENDING ACMD41\r\n");
    send_sd_frame((sd_frame){SD_SEND_OP_COND, 0x40000000, 0x01});

    while (res == 0xFF) {
      res = spi_master_transmit(0xFF);
    }

    spi_end_transaction(&PORTB, SD_SS);
    spi_master_transmit(0xFF);

    if (res == 0x00)
      break;
  }

  uart_printstr("SENDING CMD58\r\n");
  send_sd_frame((sd_frame){READ_OCR, 0x00, 0x01});
  while (spi_master_transmit(0xFF) != 0x01) {
    ;
  }
  res = 0;
  for (uint8_t i = 0; i < 4; i++) {
    res <<= 8;
    res |= spi_master_transmit(0xFF);
  }
  if (((res >> 30) & 0x01) != 0) {
    // Card is high capacity -> block addressing
    card->is_high_capacity = true;
  } else {
    // Card is standard capacity -> byte addressing
    card->is_high_capacity = false;
  }

  spi_end_transaction(&PORTB, SD_SS);

  spi_master_transmit(0xFF);

  uart_printstr("SD SPI INIT: [OK]\r\n");

  return (0);
}
