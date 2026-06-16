#ifndef MADS_H
#define MADS_H

#include <stdint.h>

// TODO: For now, this array is stored inside the MCU SRAM. It takes n char * 8
// bytes, meaning it can churn up memory really quickly, we should move this in
// PROGMEM (flash memory) but that's a layer of complexity we should aim for
// only after having mastered text displaying.
extern const uint8_t mads8x8[][8];

#endif  // !MADS_H
