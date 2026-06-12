#ifndef ST7796_CMDS_H
#define ST7796_CMDS_H

/**
 * @typedef e_st7796_cmd
 * @brief List of the ST7796's commands, as per the ST7796's datasheet
 * p.131-140
 *
 */
#include <stdint.h>
typedef enum e_st7796_cmd
{
  NOP = 0x00,                  // No operation
  SWRESET = 0x01,              // Software Reset
  RDDID = 0x04,                // Read display ID
  RD_NB_ERR_DSI = 0x05,        // Read Number of the Errors on DSI
  RDDST = 0x09,                // Read Display Status
  RDDPM = 0x0A,                // Read Display Power
  RDD_MADCTL = 0x0B,           // Read Display
  RDD_INTRFC_PX_FORMAT = 0x0C, // RDD Interface Pixel Format
  RDDIM = 0x0D,                // Read Display Image
  RDDSM = 0x0E,                // Read Display Signal
  RDDSDR = 0x0F,               // Read Display Self-Diagnostic Result
  SLPIN = 0x10,                // Sleep In
  SLPOUT = 0x11,               // Sleep Out
  PTLON = 0x12,                // Partial Mode ON
  NORON = 0x13,                // Partial Mode OFF (Normal)
  INVOFF = 0x20,               // Display Inversion OFF
  INVON = 0x21,                // Display Inversion ON
  DISPOFF = 0x28,              // Display OFF
  DISPON = 0x29,               // Display ON
  CASET = 0x2A,                // Column Address Set
  RASET = 0x2B,                // Row Address Set
  RAMWR = 0x2C,                // Memory Write
  RAMRD = 0x2E,                // Memory Read
  PTLAR = 0x30,                // Partial Start/End Address Set
  VSCRDEF = 0x33,              // Vertical Scrolling Definition
  TEOFF = 0x34,                // Tearing Effect Line OFF
  TEON = 0x35,                 // Tearing Effect Line ON
  MADCTL = 0x36,               // Memory Data Access Control
  VSCRSADD = 0x37,             // Vertical Scrolling Start Address
  IDMOFF = 0x38,               // Idle Mode OFF
  IDMON = 0x39,                // Idle Mode ON
  INTRFC_PX_FORMAT = 0x3A,     // Interface Pixel Format
  RAMWRC = 0x3C,               // Memory Write Continue
  RAMRDC = 0x3E,               // Memory Read Continue
  TESCAN = 0x44,               // Set Tear Scanline
  RDTESCAN = 0x45,             // Get Scanline
  WRDISBV = 0x51,              // Write Display Brightness
  RDDISBV = 0x52,              // Read Display Brightness Value
  WRCTRLD = 0x53,              // Write CTRL Display
  RDCTRLD = 0x54,              // Read CTRL Value Display
  WRCABC = 0x55,               // Write Content Adaptive Brightness Control
  RDCABC = 0x56,               // Read Content Adaptive Brightness Control
  WRCABCMB = 0x5E,             // Write CABC Minimum Brightness
  RDCABCMB = 0x5F,             // Read CABC Minimum Brightness
  RDFCHKSUM = 0xAA,            // Read First Checksum
  RDCCHKSUM = 0xAF,            // Read Continue Checksum
  IFMODE = 0xB0,               // Interface Mode Control
  FRMCTR1 = 0xB1,    // Frame Rate Control (In Normal Mode / Full Colours)
  FRMCTR2 = 0xB2,    // Frame Rate Control (In Idle Mode / 8 Colours)
  FRMCTR3 = 0xB3,    // Frame Rate Control (In Partial Mode / Full Colours)
  INVTR = 0xB4,      // Display Inversion Control
  BPC = 0xB5,        // Blanking Porch Control
  DFC = 0xB6,        // Display Function Control
  EM = 0xB7,         // Entry Mode Set
  PWR1 = 0xC0,       // Power Control 1
  PWR2 = 0xC1,       // Power Control 2
  PWR3 = 0xC2,       // Power Control 3
  VCMPCTL = 0xC5,    // VCom Control
  VCM_OFFSET = 0xC6, // VCom Offset Register
  NVMADW = 0xD0,     // NVM Address/Data
  NVMBPROG = 0xD1,   // NVM Byte Program Control
  NVMSTRD = 0xD2,    // NVM Status Read
  RDID4 = 0xD3,      // Read ID4
  RDID1 = 0xDA,      // Read ID1
  RDID2 = 0xDB,      // Read ID2
  RDID3 = 0xDC,      // Read ID3
  PGC = 0xE0,        // Positive Gamma Control
  NGC = 0xE1,        // Negative Gamma Control
  DGC1 = 0xE2,       // Digital Gamma Control1
  DGC2 = 0xE3,       // Digital Gamma Control2
  DOCA = 0xE8,       // Display Output
  CSCON = 0xF0,      // Command Set Control
  SPIRC = 0xFB       // SPI Read Control
} ST7796_cmd;

// --- Low-level commands ----------------------------------------------------

/**
 * @brief This command causes the commands and parameters to their S/W Reset
 * default values. After sending this command, it is necessary to wait 5ms
 * before sending another command.
 * If a software reset is sent during sleep in
 * mode, it will be necessary to wait 120ms before sending sleep out command.
 * Software Reset command can't be sent during sleep out sequence.
 *
 * SWRESET (0x01) p.141
 */
void st7796_swreset (void);

/**
 * @brief This command causes the LCD module to enter the minimum power
 * consumption mode.
 *
 * SLPIN (0x10) p.159
 */
void st7796_slpin (void);

/**
 * @brief This command turns off sleep mode.
 *
 * SLPOUT (0x11) p.160
 */
void st7796_slpout (void);

/**
 * @brief Sets the column address.
 *
 * @param col_start Start of the column (between 0 and the width of the screen
 * minus one).
 * @param col_end End of the column (between 0 and the width of the screen
 * minus one).
 *
 * CASET (0x2A) p.170
 */
void st7796_caset (const uint16_t col_start, const uint16_t col_end);

/**
 * @brief Sets the row address.
 *
 * @param row_start Start of the row (between 0 and the height of the screen
 * minus one).
 * @param row_end End of the row (between 0 and the height of the screen minus
 * one).
 *
 * RASET (0x2A) p.172
 */
void st7796_raset (const uint16_t row_start, const uint16_t row_end);

/**
 * @brief This command is used to transfer data from the MCU to frame memory.
 * When this command is set, both the column and row registers are reset to the
 * start column/row start positions. Sending any other command can stop frame
 * write.
 *
 * RAMWR (0x2C) p.173
 */
void st7796_ramwr (void);

#endif // !ST7796_CMDS_H
