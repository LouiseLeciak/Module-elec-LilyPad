#ifndef ILI9341_CMDS_H
#define ILI9341_CMDS_H

#include <stdint.h>

// NOTE: The following commands are listed in the ILI9341's datasheet p.83-87.
typedef enum e_ili9341_cmd
{
  NO_OPERATION = 0x00,                       // Not implemented
  SOFTWARE_RESET = 0x01,                     // Not implemented
  RD_DISPLAY_ID_INFO = 0x04,                 // Not implemented
  RD_DISPLAY_STATUS = 0x09,                  // Not implemented
  RD_DISPLAY_PWR_MODE = 0x0A,                // Not implemented
  RD_DISPLAY_MADCTL = 0x0B,                  // Not implemented
  RD_DISPLAY_PIXEL_FORMAT = 0x0C,            // Not implemented
  RD_DISPLAY_IMG_FORMAT = 0x0D,              // Not implemented
  RD_DISPLAY_SIGNAL_MODE = 0x0E,             // Not implemented
  RD_DISPLAY_SELF_DIAG_RESULT = 0x0F,        // Not implemented
  ENTER_SLP_MODE = 0x10,                     // Not implemented
  SLEEP_OUT = 0x11,                          // Not implemented
  PARTIAL_MODE_ON = 0x12,                    // Not implemented
  NORMAL_DISPLAY_MODE_ON = 0x13,             // Not implemented
  DISPLAY_INVERSION_OFF = 0x20,              // Not implemented
  DISPLAY_INVERSION_ON = 0x21,               // Not implemented
  GAMMA_SET = 0x26,                          // Not implemented
  DISPLAY_OFF = 0x28,                        // Not implemented
  DISPLAY_ON = 0x29,                         // Not implemented
  COL_ADDRESS_SET = 0x2A,                    // Not implemented
  PAGE_ADDRESS_SET = 0x2B,                   // Not implemented
  MEMORY_WRITE = 0x2C,                       // Not implemented
  COLOR_SET = 0x2D,                          // Not implemented
  MEMORY_READ = 0x2E,                        // Not implemented
  PARTIAL_AREA = 0x30,                       // Not implemented
  VERTICAL_SCROLL_DEF = 0x33,                // Not implemented
  TEARING_EFFECT_LINE_OFF = 0x34,            // Not implemented
  TEARING_EFFECT_LINE_ON = 0x35,             // Not implemented
  MEMORY_ACCESS_CONTROL = 0x36,              // Not implemented
  VERTICAL_SCROLLING_START_ADDRESS = 0x37,   // Not implemented
  IDLE_MODE_OFF = 0x38,                      // Not implemented
  IDLE_MODE_ON = 0x39,                       // Not implemented
  PIXEL_FORMAT_SET = 0x3A,                   // Not implemented
  WRITE_MEMORY_CONTINUE = 0x3C,              // Not implemented
  RD_MEMORY_CONTINUE = 0x3E,                 // Not implemented
  SET_TEAR_SCANLINE = 0x44,                  // Not implemented
  GET_SCANLINE = 0x45,                       // Not implemented
  WRITE_DISPLAY_BRIGHT = 0x51,               // Not implemented
  RD_DISPLAY_BRIGHTNESS = 0x52,              // Not implemented
  WRITE_CTRL_DISPLAY = 0x53,                 // Not implemented
  RD_CTRL_DISPLAY = 0x54,                    // Not implemented
  WRITE_CONTENT_ADAPTIVE_BRIGHT_CTRL = 0x55, // Not implemented
  RD_CONTENT_ADAPTIVE_BRIGHT_CTRL = 0x56,    // Not implemented
  WRITE_CABC_MIN_BRIGHT = 0x5E,              // Not implemented
  RD_CABC_MIN_BRIGHT = 0x5F,                 // Not implemented
  READ_ID1 = 0xDA,                           // Not implemented
  READ_ID2 = 0xDB,                           // Not implemented
  READ_ID3 = 0xDC,                           // Not implemented
} ILI9341_CMDS;

void ili9341_col_address_set (const uint16_t page_start,
                              const uint16_t page_end);

void ili9341_page_address_set (const uint16_t page_start,
                               const uint16_t page_end);

void ili9341_memory_write (void);

#endif // !ILI9341_CMDS_H
