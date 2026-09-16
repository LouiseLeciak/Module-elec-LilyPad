#include "sd.h"

#include <avr/io.h>
#include <stdint.h>

#include "crc.h"
#include "pinout.h"
#include "spi.h"
#include "uart.h"

#define MAX_TRIES 8

void
sd_init (void)
{
  uart_printstr ("Initialising SD card...\r\n");

  // Set SPI clock between 100kHz and 400kHz (as per Elm-Chan guide)
  SPSR &= ~(1 << SPI2X);
  SPCR &= ~(1 << SPR0);
  SPCR |= (1 << SPR1);

  DDRB |= (MOSI);
  PORTB |= (MOSI);
  DDRH |= (SD_CS);
  PORTH |= (SD_CS);

  for (uint8_t i = 0; i < 74; i++)
    { // Dummy clocks
      spi_txrx (0xFF);
    }

  uart_printstr ("SD card is currently in SD mode.\r\n");

  PORTH &= ~(SD_CS);
  sd_go_idle_state (0, 0, 0, 0);
}

void
sd_crc7_gen (sd_cmd *cmd, SD_CMD_INDEX index)
{
  cmd->cmd = 0b01000000 | index;
  uint8_t crc7 = generate_crc7 ((uint8_t *)cmd,
                                5); // 1 byte for CMD and 4 bytes for arg
  cmd->crc7_and_end_bit = (crc7 << 1) | 1;
}

sd_resp
sd_send_cmd (sd_cmd *cmd, SD_RESP_KIND kind)
{
  SD_CS_LOW ();
  for (uint8_t i = 0; i < (sizeof (*cmd) / sizeof (uint8_t)); i++)
    spi_txrx ((((uint8_t *)cmd)[i]));
  SD_CS_HIGH ();

  for (uint8_t i = 0; i < 8;
       i++) // TODO: Release SD CS properly with dummy clocks
    spi_txrx (0xFF);
  return sd_read_response (kind);
}

sd_resp
sd_send_acmd (SD_CMD_INDEX index, uint8_t arg0, uint8_t arg1, uint8_t arg2,
              uint8_t arg3, SD_RESP_KIND kind)
{
  sd_resp r55 = sd_app_cmd (0, 0, 0, 0); // r55 is currently unused

  if (SD_R1_ILLEGAL_CMD (r55) || SD_R1_CRC_ERROR (r55))
    return r55;

  sd_cmd cmd = { 0, { arg0, arg1, arg2, arg3 }, 0 };
  sd_crc7_gen (&cmd, index);
  return sd_send_cmd (&cmd, kind);
}

sd_resp
sd_read_response (SD_RESP_KIND kind)
{
  sd_resp resp = { 0 };
  uint8_t tries = MAX_TRIES;

  do
    {
      resp.r1 = spi_txrx (0xFF);
    }
  while ((resp.r1 & 0x80) && --tries);

  switch (kind)
    {
    case SD_RESP_R1:
      break;
    case SD_RESP_R2:
      resp.data[0] = spi_txrx (0xFF);
      break;
    case SD_RESP_R3:
    case SD_RESP_R7:
      for (uint8_t i = 0; i < 4; i++)
        {
          resp.data[i] = spi_txrx (0xFF);
        }
      break;
    case SD_RESP_R1B:
      while (spi_txrx (0xFF) != 0xFF)
        ;
      break;
    }
  return resp;
}

sd_resp
sd_go_idle_state (uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3)
{
  sd_cmd cmd00 = { 0, { arg0, arg1, arg2, arg3 }, 0 };
  sd_crc7_gen (&cmd00, GO_IDLE_STATE);
  return sd_send_cmd (&cmd00, SD_RESP_R1);
}

sd_resp
sd_send_op_cond (uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3)
{
  sd_cmd cmd01 = { 0, { arg0, arg1, arg2, arg3 }, 0 };
  sd_crc7_gen (&cmd01, SEND_OP_COND);
  return sd_send_cmd (&cmd01, SD_RESP_R1);
}

sd_resp
sd_app_cmd (uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3)
{
  sd_cmd cmd55 = { 0, { arg0, arg1, arg2, arg3 }, 0 };
  sd_crc7_gen (&cmd55, APP_CMD);
  return sd_send_cmd (&cmd55, SD_RESP_R1);
}
