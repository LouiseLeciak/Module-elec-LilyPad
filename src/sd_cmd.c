#include "sd_cmd.h"

#include <stdint.h>
#include <stdlib.h>

#include "pins.h"
#include "sd.h"
#include "spi.h"
#include "uart.h"

void get_r7_response(r7_response* response) {
  (void)response;
  uint32_t res = 0;
  (void)res;
  for (uint8_t i = 0; i < 4; i++) {
    res <<= 8;
    res |= spi_master_transmit(0xFF);
  }
}

uint8_t sd_go_idle_state(void) {
  uint8_t cmd_answer = 0xFF;

  spi_start_transaction(&PORTB, SD_SS);
  spi_master_transmit(0xFF);

  uart_printstr("SENDING CMD00\r\n");
  send_sd_frame((sd_frame){GO_IDLE_STATE, 0x00, (0x95 | 0x01)});

  while (cmd_answer == 0xFF) {
    cmd_answer = spi_master_transmit(0xFF);
  }

  spi_end_transaction(&PORTB, SD_SS);
  spi_master_transmit(0xFF);

  if (cmd_answer != 0x01) {
    char tmp[10] = {0};
    uart_printstr("CMD0 : [FAILED] (Wrong answer) : 0x");
    itoa(cmd_answer, tmp, 16);
    uart_printstr(tmp);
    uart_printstr("\r\n");
    return (1);
  }
  return (0);
}

uint8_t sd_send_if_cond(void) {
  spi_start_transaction(&PORTB, SD_SS);
  spi_master_transmit(0xFF);

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

    (void)res;
  }
}

void sd_read_single_block(const sd_card card,
                          const uint32_t arg,
                          uint8_t* buf,
                          const uint16_t buf_siz) {
  uint8_t ret = 0xFF;
  sd_frame frame = {READ_SINGLE_BLOCK, arg, 0x01};

  send_sd_frame(frame);

  while (ret != 0x00) {  // Command acknowledged
    ret = spi_master_transmit(0xFF);
  }
  while (spi_master_transmit(0xFF) != 0xFE)  // Waiting for card to fetch data
  {
    ;
  }
  for (uint16_t i = 0; i < card.block_len; i++) {
    if (i < buf_siz)
      buf[i] = spi_master_transmit(0xFF);
  }
  // Ignoring CRC because who cares
  for (uint8_t dummy_crc = 0; dummy_crc < 2; dummy_crc++) {
    spi_master_transmit(0xFF);
  }
}
