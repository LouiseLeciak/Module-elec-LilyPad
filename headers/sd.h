#ifndef ST7796_SD_H
#define ST7796_SD_H

// NOTE: Commands must have the sixth bit set to 1, so for each command yu have
// to add 40, that's WHY CMD0 is equal to 0x40.

typedef enum s_sd_cmd {
  GO_IDLE_STATE = 0x40, ///<
} SD_CMD;

#endif // !ST7796_SD_H
