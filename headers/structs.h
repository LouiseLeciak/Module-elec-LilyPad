#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>

typedef struct s_position {
  uint32_t _pos_x;
  uint32_t _pos_y;
} position;

typedef struct s_rgb {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} rgb;

typedef struct s_window {
  position _start;
  position _end;
} window;

#endif  // !STRUCTS_H
