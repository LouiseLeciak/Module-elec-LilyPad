#ifndef ST7796_SD_H
#define ST7796_SD_H

#include <avr/io.h>
#include <stdint.h>
#include "stdbool.h"

typedef struct s_sd {
  bool is_high_capacity;  ///< Uses block addressing (1) or byte addressing (0)
} sd_card;

typedef struct s_sd_frame {
  uint8_t cmd;
  uint32_t arg;
  uint8_t crc;
} sd_frame;

// NOTE: Commands must have the sixth bit set to 1, so for each command yu have
// to add 40, that's WHY CMD0 is equal to 0x40.

typedef enum s_sd_cmd {
  GO_IDLE_STATE = (0 | 0x40),       ///< CMD0
  SEND_IF_COND = (8 | 0x40),        ///< CMD8
  READ_SINGLE_BLOCK = (17 | 0x40),  ///< CMD17
  SD_SEND_OP_COND = (41 | 0x40),    ///< CMD41
  APP_CMD = (55 | 0x40),            ///< CMD55
  READ_OCR = (58 | 0x40),           ///< CMD58
} SD_CMD;

/**
 * @brief [TODO:description]
 *
 * @param card [TODO:parameter]
 */
uint8_t sd_spi_init(sd_card* card);

/**
 * @brief [TODO:description]
 */
uint8_t sd_go_idle_state(void);

/**
 * @brief Reads a block of the size selected by the SET_BLOCKLEN command.
 *
 * @param arg Data address
 */
void sd_read_single_block(uint32_t arg, uint8_t buf[512]);

#endif  // !ST7796_SD_H
