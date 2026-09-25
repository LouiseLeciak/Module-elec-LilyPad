#ifndef STRUCTS_H
#define STRUCTS_H

#include <avr/io.h>

#define FILE_NAME_SIZE 16

/**
 * @class s_position
 * @brief Represents a position on the X and Y axis.
 *
 */
typedef struct s_position
{
  uint32_t _pos_x;  ///< X coordinate.
  uint32_t _pos_y;  ///< Y coordinate.
} position;

/**
 * @class s_rgb
 * @brief RGB colour, stored on 8 bits for each value.
 *
 */
typedef struct s_rgb
{
  uint8_t _red;    ///< Red value.
  uint8_t _green;  ///< Green value.
  uint8_t _blue;   ///< Blue value.
} rgb;

/**
 * @class s_window
 * @brief Represents a window of pixels. Must be rectangular.
 *
 */
typedef struct s_window
{
  position
      _start;  ///< Origin of the window (X;Y). Is usually the top-left corner.
  position
      _end;  ///< End of the window (X;Y). Is usually the bottom-right corner.
} window;

typedef struct s_img
{
  uint32_t address;
  char name[FILE_NAME_SIZE];
} img;

#endif  // !STRUCTS_H
