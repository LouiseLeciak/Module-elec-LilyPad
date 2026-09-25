#ifndef SD_STREAMING_H
#define SD_STREAMING_H

#include <avr/io.h>

void sd_stream_bmp_to_screen(uint32_t start_sector);

#endif  // !SD_STREAMING_H
