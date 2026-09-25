/**
 * @file sd_streaming.h
 * @brief API to display images from an SD card.
 * @author lleciak, mpetitco, nrobinso
 */

#ifndef SD_STREAMING_H
#define SD_STREAMING_H

#include <avr/io.h>

/**
 * @brief Streams a BMP image from the SD card directly to the screen.
 *
 * @param[in] start_sector The starting sector on the SD card where the image is
 * stored.
 *
 * @warning This is a blocking function and will halt other processes until the
 *          entire image is drawn.
 */
void sd_stream_bmp_to_screen(uint32_t start_sector);

#endif  // !SD_STREAMING_H
