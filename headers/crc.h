#ifndef CRC_H
#define CRC_H

#include <avr/io.h>

// CRC7 Generator
extern const uint8_t G;

uint8_t generate_crc7(uint8_t* data, uint8_t len);

#endif  // !CRC_H
