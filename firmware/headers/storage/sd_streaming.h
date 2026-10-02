/**
 * @file sd_streaming.h
 * @brief API to display images from an SD card.
 * @author lleciak, mpetitco, nrobinso
 */

#ifndef SD_STREAMING_H
#define SD_STREAMING_H

#include <avr/io.h>

typedef enum
{
  MAIN_LCD = (1 << 0),
  LEFT_EYE_LCD = (1 << 1),
  RIGHT_EYE_LCD = (1 << 2),
  EYES = (LEFT_EYE_LCD | RIGHT_EYE_LCD),
  ALL = (MAIN_LCD | LEFT_EYE_LCD | RIGHT_EYE_LCD)
} device_screen;

uint8_t display_sd_image(const char* filename, const device_screen target);

#endif  // !SD_STREAMING_H
