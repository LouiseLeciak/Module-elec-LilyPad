/**
 * @file sd.h
 * @brief SD card driver over SPI.
 * @author lleciak, mpetitco, nrobinso
 */

#ifndef SD_H
#define SD_H

// Useful information about how to write to a SD card over SPI is there:
// https://onlinedocs.microchip.com/oxy/GUID-F9FE1ABC-D4DD-4988-87CE-2AFD74DEA334-en-US-3/GUID-48879CB2-9C60-4279-8B98-E17C499B12AF.html
// This is also useful ("How to Use MMC/SDC"):
// https://elm-chan.org/docs/mmc/mmc_e.html

#include <avr/io.h>

/**
 * @defgroup SD_Card
 * @brief    API for the SD card.
 * @{
 */

#define SD_R1_IDLE(r) ((r).r1 & 0x01)
#define SD_R1_ILLEGAL_CMD(r) ((r).r1 & 0x04)
#define SD_R1_CRC_ERROR(r) ((r).r1 & 0x08)

#define SD_OCR_BUSY(r) (((r).data[0] & 0x80) != 0)
#define SD_OCR_CCS(r) (((r).data[0] & 0x40) != 0)

typedef enum
{
  GO_IDLE_STATE = 0,
  SEND_OP_COND = 1,
  SEND_IF_COND = 8,
  STOP_TRANSMISSION = 12,
  SET_BLOCKLEN = 16,
  READ_SINGLE_BLOCK = 17,
  READ_MULTIPLE_BLOCK = 18,
  APP_CMD = 55,
  READ_OCR = 58,
  SD_SEND_OP_COND = 41
} SD_CMD_INDEX;

typedef enum
{
  SD_RESP_R1,
  SD_RESP_R1B,
  SD_RESP_R2,
  SD_RESP_R3,
  SD_RESP_R7
} SD_RESP_KIND;

typedef struct __attribute__((packed))
{
  uint8_t cmd;
  uint8_t arg[4];
  uint8_t crc7_and_end_bit;
} sd_cmd;

typedef struct __attribute__((packed))
{
  uint8_t r1;
  uint8_t data[4];  // R2: data[0] used
                    // R3/R7: OCR/echo, all 4 bytes;
                    // unused for R1/R1b
} sd_resp;

// UTILS

// STUFF
uint8_t sd_init(void);

// PRIMITIVE COMMANDS

sd_resp sd_read_single_block(uint8_t arg0, uint8_t arg1, uint8_t arg2,
                             uint8_t arg3, uint8_t* buf);

sd_resp sd_read_multiple_block_start(uint8_t arg0, uint8_t arg1, uint8_t arg2,
                                     uint8_t arg3);
void sd_read_multiple_block_next(uint8_t* buf);
sd_resp sd_read_multiple_block_stop(void);

/** @} */

#endif  // !SD_H
