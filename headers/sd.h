#ifndef SD_H
#define SD_H

// Useful information about how to write to a SD card over SPI is there:
// https://onlinedocs.microchip.com/oxy/GUID-F9FE1ABC-D4DD-4988-87CE-2AFD74DEA334-en-US-3/GUID-48879CB2-9C60-4279-8B98-E17C499B12AF.html
// This is also useful ("How to Use MMC/SDC"):
// https://elm-chan.org/docs/mmc/mmc_e.html

#include <avr/io.h>
#include <stdint.h>

typedef enum e_sd_cmd {
  GO_IDLE_STATE = 0b01000000,
  CMD2,
  CMD3,
  CMD4,
  CMD6, //?
  CMD7,
  CMD8, //?
  CMD9,
  CM10,
  CMD11, //?
  CMD12,
  CMD13,
  CMD15,
  CMD16,
  CMD17,
  CMD18,
  CMD19, //?
  CMD20, //?
  CMD22, //?
  CMD23, //?
  CMD24,
  CMD25,
  CMD27,
  CMD32,
  CMD33,
  CMD55,
  CMD56,
  ACMD6,
  ACMD13,
  ACMD41,
  ACMD42,
  ACMD51,

} SD_CMD;

struct __attribute__((packed)) s_sd_cmd {
  uint8_t cmd;
  uint8_t arg[4];
  uint8_t crc7_and_end_bit;
};

// UTILS
void sd_crc7_gen(struct s_sd_cmd *cmd);

// COMMANDS
void sd_go_idle_state(uint8_t arg[4]);

#endif // !SD_H
