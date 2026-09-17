#include "sd.h"

#include "crc.h"
#include "pinout.h"
#include "spi.h"
#include "uart.h"

#define MAX_TRIES 8

void
sd_init (void)
{
  uart_printstr ("Initialising SD card...");

  // Set SPI clock between 100kHz and 400kHz (as per Elm-Chan guide)
  SPSR &= ~(1 << SPI2X);
  SPCR &= ~(1 << SPR0);
  SPCR |= (1 << SPR1);

  for (uint8_t i = 0; i < 74; i++) // Dummy clocks
    spi_txrx (0xFF);

  sd_resp cmd0 = sd_go_idle_state (0, 0, 0, 0);
  if (cmd0.r1 != 0x01)
    {
      uart_printstr ("ERROR\r\nSD: CMD0 failed, no card?\r\n");
      return;
    }

  // CMD08 args:
  // - VHS=0b0001 (2.7-3.6V, as per p.90 of SD spec)
  // - check pattern=0xAA
  sd_resp cmd8 = sd_send_if_cond (0, 0, 0x01, 0xAA);
  if (SD_R1_ILLEGAL_CMD (cmd8))
    {
      uart_printstr (
          "ERROR\r\nSD: v1 card or MMC detected, not supported!\r\n");
      return;
    }
  if (cmd8.data[3] != 0xAA) // Check echo pattern (last byte)
    {
      uart_printstr (
          "ERROR\r\nSD: CMD8 echo mismatch, voltage incompatible!\r\n");
      return;
    }

  sd_resp acmd41;
  uint16_t acmd41_tries = 0xFFFF; // May be too much
  do
    {
      acmd41 = sd_sd_send_op_cond (0x40, 0, 0, 0);
      acmd41_tries--;
    }
  while (SD_R1_IDLE (acmd41) && acmd41_tries);

  if (!acmd41_tries)
    {
      uart_printstr ("ERROR\r\nSD: ACMD41 timeout, card stuck in idle!\r\n");
      return;
    }

  sd_resp ocr = sd_read_ocr (0, 0, 0, 0); // CMD58

  // SD_OCR_BUSY: bit 31 of OCR = 1 means card is ready (confusingly named)
  if (!SD_OCR_BUSY (ocr))
    {
      uart_printstr ("ERROR\r\nSD: card not ready after ACMD41!\r\n");
      return;
    }

  // SD_OCR_CCS: bit 30 = 1 → SDHC/SDXC (block addressing)
  //                      = 0 → SDSC (byte addressing, needs CMD16)
  if (!SD_OCR_CCS (ocr))
    sd_set_blocklen (0, 0, 0x02,
                     0x00); // CMD16: set 512-byte blocks (only for SDSC)

  uart_printstr ("OK!\r\n");
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
  sd_resp resp = sd_read_response (kind);
  SD_CS_HIGH ();
  spi_txrx (0xFF); // 1 dummy byte to release the bus (SD spec)
  return resp;
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
sd_set_blocklen (uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3)
{
  sd_cmd cmd16 = { 0, { arg0, arg1, arg2, arg3 }, 0 };
  sd_crc7_gen (&cmd16, SET_BLOCKLEN);
  return sd_send_cmd (&cmd16, SD_RESP_R1);
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
sd_send_if_cond (uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3)
{
  sd_cmd cmd08 = { 0, { arg0, arg1, arg2, arg3 }, 0 };
  sd_crc7_gen (&cmd08, SEND_IF_COND);
  return sd_send_cmd (&cmd08, SD_RESP_R7);
}

sd_resp
sd_app_cmd (uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3)
{
  sd_cmd cmd55 = { 0, { arg0, arg1, arg2, arg3 }, 0 };
  sd_crc7_gen (&cmd55, APP_CMD);
  return sd_send_cmd (&cmd55, SD_RESP_R1);
}

sd_resp
sd_read_ocr (uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3)
{
  sd_cmd cmd58 = { 0, { arg0, arg1, arg2, arg3 }, 0 };
  sd_crc7_gen (&cmd58, READ_OCR);
  return sd_send_cmd (&cmd58, SD_RESP_R3);
}

sd_resp
sd_sd_send_op_cond (uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3)
{
  sd_resp response = sd_app_cmd (0, 0, 0, 0);
  if (SD_R1_ILLEGAL_CMD (response))
    return (response);
  sd_cmd acmd41 = { 0, { arg0, arg1, arg2, arg3 }, 0 };
  sd_crc7_gen (&acmd41, SD_SEND_OP_COND);
  return sd_send_cmd (&acmd41, SD_RESP_R1);
}
