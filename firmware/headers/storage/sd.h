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

typedef struct __attribute__((packed)) {
  uint8_t r1;
  uint8_t data[4];  // R2: data[0] used
                    // R3/R7: OCR/echo, all 4 bytes;
                    // unused for R1/R1b
} sd_resp;

// STUFF
uint8_t sd_init(void);

sd_resp sd_read_single_block(uint8_t arg0, uint8_t arg1, uint8_t arg2,
                             uint8_t arg3, uint8_t* buf);

sd_resp sd_read_multiple_block_start(uint8_t arg0, uint8_t arg1, uint8_t arg2,
                                     uint8_t arg3);
void sd_read_multiple_block_next(uint8_t* buf);
sd_resp sd_read_multiple_block_stop(void);

#endif  // !SD_H
