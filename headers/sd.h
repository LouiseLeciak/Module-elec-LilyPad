#ifndef ST7796_SD_H
#define ST7796_SD_H

#include "stdbool.h"
#include <avr/io.h>
#include <stdint.h>

typedef struct s_sd {
  bool is_high_capacity; ///< Uses block addressing (1) or byte addressing (0)
  uint16_t block_len;    ///< SD Card block len
} sd_card;

typedef struct s_sd_frame {
  uint8_t cmd;
  uint32_t arg;
  uint8_t crc;
} sd_frame;

/**
 * @brief See p.44 for initialisation diagram
 *
 * @param card Card struct we want to initialise.
 */
uint8_t sd_spi_init(sd_card *card);

/**
 * @brief [TODO:description]
 *
 * @param frame [TODO:parameter]
 * @return [TODO:return]
 */
uint8_t send_sd_frame(const sd_frame frame);

#endif // !ST7796_SD_H
