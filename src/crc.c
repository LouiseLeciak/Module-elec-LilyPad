#include "crc.h"

const uint8_t G = 0b10001001;  // Best value for error detection as per way
                               // smarter folks than us.

uint8_t generate_crc7(uint8_t* data, uint8_t len) {
  uint8_t g = (G & 0b01111111);
  uint8_t r = 0;

  for (uint8_t n = 0; n < len; n++) {  // Byte-level loop
    for (uint8_t i = 0; i < 8; i++) {  // Loop to process each bit
      uint8_t r_msb = (r >> 6) & 1;    // Discards all bits besides the MSB
      uint8_t feedback = r_msb ^ (data[n] >> (7 - i) & 1);

      r = (r << 1) & 0b01111111;
      if (feedback != 0) {
        r ^= g;
      }
    }
  }

  return r;
}
