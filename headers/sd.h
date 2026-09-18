#ifndef SD_H
#define SD_H

// Useful information about how to write to a SD card over SPI is there:
// https://onlinedocs.microchip.com/oxy/GUID-F9FE1ABC-D4DD-4988-87CE-2AFD74DEA334-en-US-3/GUID-48879CB2-9C60-4279-8B98-E17C499B12AF.html
// This is also useful ("How to Use MMC/SDC"):
// https://elm-chan.org/docs/mmc/mmc_e.html

#include <avr/io.h>

#define SD_R1_IDLE(r) ((r).r1 & 0x01)
#define SD_R1_ILLEGAL_CMD(r) ((r).r1 & 0x04)
#define SD_R1_CRC_ERROR(r) ((r).r1 & 0x08)

#define SD_OCR_BUSY(r) (((r).data[0] & 0x80) != 0)
#define SD_OCR_CCS(r) (((r).data[0] & 0x40) != 0)

#define SD_CS_LOW() (PORTH &= ~(SD_CS))
#define SD_CS_HIGH() (PORTH |= (SD_CS))

typedef enum {
  GO_IDLE_STATE = 0,
  SEND_OP_COND = 1,
  SEND_IF_COND = 8,
  SET_BLOCKLEN = 16,
  APP_CMD = 55,
  READ_OCR = 58,
  SD_SEND_OP_COND = 41
} SD_CMD_INDEX;

typedef enum {
  SD_RESP_R1,
  SD_RESP_R1B,
  SD_RESP_R2,
  SD_RESP_R3,
  SD_RESP_R7
} SD_RESP_KIND;

typedef struct __attribute__((packed)) {
  uint8_t cmd;
  uint8_t arg[4];
  uint8_t crc7_and_end_bit;
} sd_cmd;

typedef struct __attribute__((packed)) {
  uint8_t r1;
  uint8_t data[4];  // R2: data[0] used
                    // R3/R7: OCR/echo, all 4 bytes;
                    // unused for R1/R1b
} sd_resp;

// UTILS
void sd_crc7_gen(sd_cmd* cmd, SD_CMD_INDEX index);
sd_resp sd_send_cmd(sd_cmd* cmd, SD_RESP_KIND kind);
sd_resp sd_read_response(SD_RESP_KIND kind);

// STUFF
void sd_init(void);

// PRIMITIVE COMMANDS
/**
 * @brief Resets the SD Memory Card (CMD0)
 *
 * @param arg0 Stuff byte.
 * @param arg1 Stuff byte.
 * @param arg2 Stuff byte.
 * @param arg3 Stuff byte.
 * @return R1 response.
 */
sd_resp sd_go_idle_state(uint8_t arg0, uint8_t arg1, uint8_t arg2,
                         uint8_t arg3);

/**
 * @brief Sends host capacity support information and activates the card's
 * initialisation process.
 * HCS is effective when card receives SEND_IF_COND command.
 * Reserved bits shall be set to '0'.
 *
 * @param arg0 [31]: Reserved bit ; [30]: HCS, [29:24]: Reserved bits.
 * @param arg1 [23:16]: Reserved bits
 * @param arg2 [15:8]: Reserved bits.
 * @param arg3 [7:0]: Reserved bits.
 * @return R1 response.
 */
sd_resp sd_send_op_cond(uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3);

sd_resp sd_send_if_cond(uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3);

sd_resp sd_app_cmd(uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3);

sd_resp sd_set_blocklen(uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3);

sd_resp sd_read_ocr(uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t arg3);

sd_resp sd_sd_send_op_cond(uint8_t arg0, uint8_t arg1, uint8_t arg2,
                           uint8_t arg3);

#endif  // !SD_H
