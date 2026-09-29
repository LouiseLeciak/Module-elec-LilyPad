#include "display/ili9488.h"

#include <util/delay.h>

#include "pinout.h"
#include "system/spi.h"
#include "system/uart.h"

/**
 * @typedef e_ili9488_cmd
 * @brief List of the II9488's commands, as per the ILI9488's datasheet
 * p.140-148
 *
 */
typedef enum
{
  NOP = 0x00,                   ///< No operation
  SWRESET = 0x01,               ///< Software Reset
  RDDID = 0x04,                 ///< Read display ID
  RD_NB_ERR_DSI = 0x05,         ///< Read Number of the Errors on DSI
  RDDST = 0x09,                 ///< Read Display Status
  RDDPM = 0x0A,                 ///< Read Display Power
  RDD_MADCTL = 0x0B,            ///< Read Display
  RDD_INTRFC_PX_FORMAT = 0x0C,  ///< RDD Interface Pixel Format
  RDDIM = 0x0D,                 ///< Read Display Image
  RDDSM = 0x0E,                 ///< Read Display Signal
  RDDSDR = 0x0F,                ///< Read Display Self-Diagnostic Result
  SLPIN = 0x10,                 ///< Sleep In
  SLPOUT = 0x11,                ///< Sleep Out
  PTLON = 0x12,                 ///< Partial Mode ON
  NORON = 0x13,                 ///< Partial Mode OFF (Normal)
  INVOFF = 0x20,                ///< Display Inversion OFF
  INVON = 0x21,                 ///< Display Inversion ON
  DISPOFF = 0x28,               ///< Display OFF
  DISPON = 0x29,                ///< Display ON
  CASET = 0x2A,                 ///< Column Address Set
  RASET = 0x2B,                 ///< Row Address Set
  RAMWR = 0x2C,                 ///< Memory Write
  RAMRD = 0x2E,                 ///< Memory Read
  PTLAR = 0x30,                 ///< Partial Start/End Address Set
  VSCRDEF = 0x33,               ///< Vertical Scrolling Definition
  TEOFF = 0x34,                 ///< Tearing Effect Line OFF
  TEON = 0x35,                  ///< Tearing Effect Line ON
  MADCTL = 0x36,                ///< Memory Data Access Control
  VSCRSADD = 0x37,              ///< Vertical Scrolling Start Address
  IDMOFF = 0x38,                ///< Idle Mode OFF
  IDMON = 0x39,                 ///< Idle Mode ON
  COLMOD = 0x3A,                ///< Interface Pixel Format
  RAMWRC = 0x3C,                ///< Memory Write Continue
  RAMRDC = 0x3E,                ///< Memory Read Continue
  TESCAN = 0x44,                ///< Set Tear Scanline
  RDTESCAN = 0x45,              ///< Get Scanline
  WRDISBV = 0x51,               ///< Write Display Brightness
  RDDISBV = 0x52,               ///< Read Display Brightness Value
  WRCTRLD = 0x53,               ///< Write CTRL Display
  RDCTRLD = 0x54,               ///< Read CTRL Value Display
  WRCABC = 0x55,                ///< Write Content Adaptive Brightness Control
  RDCABC = 0x56,                ///< Read Content Adaptive Brightness Control
  WRCABCMB = 0x5E,              ///< Write CABC Minimum Brightness
  RDCABCMB = 0x5F,              ///< Read CABC Minimum Brightness
  RDAUTB = 0x68,                ///< Read automatic brightness
  RDFCHKSUM = 0xAA,             ///< Read First Checksum
  RDCCHKSUM = 0xAF,             ///< Read Continue Checksum
  IFMODE = 0xB0,                ///< Interface Mode Control
  FRMCTR1 = 0xB1,     ///< Frame Rate Control (In Normal Mode / Full Colours)
  FRMCTR2 = 0xB2,     ///< Frame Rate Control (In Idle Mode / 8 Colours)
  FRMCTR3 = 0xB3,     ///< Frame Rate Control (In Partial Mode / Full Colours)
  INVTR = 0xB4,       ///< Display Inversion Control
  BPC = 0xB5,         ///< Blanking Porch Control
  DFC = 0xB6,         ///< Display Function Control
  EM = 0xB7,          ///< Entry Mode Set
  PWR1 = 0xC0,        ///< Power Control 1
  PWR2 = 0xC1,        ///< Power Control 2
  PWR3 = 0xC2,        ///< Power Control 3
  VCMPCTL = 0xC5,     ///< VCom Control
  VCM_OFFSET = 0xC6,  ///< VCom Offset Register
  NVMADW = 0xD0,      ///< NVM Address/Data
  NVMBPROG = 0xD1,    ///< NVM Byte Program Control
  NVMSTRD = 0xD2,     ///< NVM Status Read
  RDID4 = 0xD3,       ///< Read ID4
  RDID1 = 0xDA,       ///< Read ID1
  RDID2 = 0xDB,       ///< Read ID2
  RDID3 = 0xDC,       ///< Read ID3
  PGC = 0xE0,         ///< Positive Gamma Control
  NGC = 0xE1,         ///< Negative Gamma Control
  DGC1 = 0xE2,        ///< Digital Gamma Control1
  DGC2 = 0xE3,        ///< Digital Gamma Control2
  DOCA = 0xE8,        ///< Display Output
  CSCON = 0xF0,       ///< Command Set Control
  ADJC3 = 0xF7,       ///< Adjust Control 3
  SPIRC = 0xFB        ///< SPI Read Control
} ili9488_cmd;

/**
 * @typedef e_colmod_arg
 * @brief Arguments for COLMOD.
 *
 */
typedef enum
{
  CI_16B = 0x05,   ///< Colour Interface 16bits/pixel colour format.
  CI_18B = 0x06,   ///< Colour Interface 18bits/pixel colour format.
  CI_24B = 0x07,   ///< Colour Interface 24bits/pixel colour format.
  RGB_16B = 0x50,  ///< RGB Interface 16bits/pixel colour format.
  RGB_18B = 0x60   ///< RGB Interface 18bits/pixel colour format.
} colmod_arg;

static void ili9488_positive_gamma_control(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx((char)PGC);
  DC_DATA();
  // Anchor values for the positive gamma correction curve
  spi_txrx(0x00);
  spi_txrx(0x03);
  spi_txrx(0x09);
  spi_txrx(0x08);
  spi_txrx(0x16);
  spi_txrx(0x0A);
  spi_txrx(0x3F);
  spi_txrx(0x78);
  spi_txrx(0x4C);
  spi_txrx(0x09);
  spi_txrx(0x0A);
  spi_txrx(0x08);
  spi_txrx(0x16);
  spi_txrx(0x1A);
  spi_txrx(0x0F);
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_negative_gamma_control(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx((char)NGC);
  DC_DATA();
  // Anchor values for the negative gamma correction curve
  spi_txrx(0x00);
  spi_txrx(0x16);
  spi_txrx(0x19);
  spi_txrx(0x03);
  spi_txrx(0x0F);
  spi_txrx(0x05);
  spi_txrx(0x32);
  spi_txrx(0x45);
  spi_txrx(0x46);
  spi_txrx(0x04);
  spi_txrx(0x0E);
  spi_txrx(0x0D);
  spi_txrx(0x35);
  spi_txrx(0x37);
  spi_txrx(0x0F);
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_power_control_1(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx((char)PWR1);
  DC_DATA();
  spi_txrx(0x17);
  spi_txrx(0x15);
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_power_control_2(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx((char)PWR2);
  DC_DATA();
  spi_txrx(0x41);
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_vcom_control(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx((char)VCMPCTL);
  DC_DATA();
  spi_txrx(0x00);
  spi_txrx(0x12);
  spi_txrx(0x80);
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_memory_access_control(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx(MADCTL);
  DC_DATA();
  // spi_txrx(0x48);  // Calibrated for TFT02 panel orientation
  spi_txrx(0x88);  // Calibrated for TF01 panel orientation
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_interface_pixel_format(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx(COLMOD);
  DC_DATA();
  spi_txrx(0x66);  // 18-bit/pixel (RGB666)
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_interface_mode_control(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx((char)IFMODE);
  DC_DATA();
  spi_txrx(0x00);
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_frame_rate_control_normal(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx((char)FRMCTR1);
  DC_DATA();
  spi_txrx(0xA0);  // 60 Hz
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_display_inversion_control(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx((char)INVTR);
  DC_DATA();
  spi_txrx(0x02);
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_display_function_control(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx((char)DFC);
  DC_DATA();
  spi_txrx(0x02);
  spi_txrx(0x02);
  spi_txrx(0x3B);
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_entry_mode_set(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx((char)EM);
  DC_DATA();
  spi_txrx(0xC6);
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_adjust_control_3(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx((char)ADJC3);
  DC_DATA();
  spi_txrx(0xA9);
  spi_txrx(0x51);
  spi_txrx(0x2C);
  spi_txrx(0x82);
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_sleep_out(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx(SLPOUT);
  _delay_ms(120);  // Mandatory delay — see ILI9488 datasheet
  MAIN_SCREEN_CS_HIGH();
}

static void ili9488_display_on(void)
{
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx(DISPON);
  _delay_ms(50);
  MAIN_SCREEN_CS_HIGH();
}

void ili9488_reset(void)
{
  PORTE |= MAIN_SCREEN_RST;
  _delay_ms(5);
  PORTE &= ~(MAIN_SCREEN_RST);
  _delay_ms(20);
  PORTE |= MAIN_SCREEN_RST;
  _delay_ms(150);  // Wait out internal reset — see ILI9488 datasheet
}

// static void main_screen_swreset(void)
// {
//   DC_CMD();
//   spi_txrx(SWRESET);
// }
//
// static void main_screen_slpin(void)
// {
//   DC_CMD();
//   spi_txrx(SLPIN);
//   _delay_ms(5);  // See 9.2.12 (p.159), Restrictions, paragraph 2
// }
//
// static void main_screen_slpout(void)
// {
//   DC_CMD();
//   spi_txrx(SLPOUT);
//   _delay_ms(120);  // See 9.2.13 (p.161), Restrictions, paragraph 3
// }
//
// static void main_screen_dispon(void)
// {
//   DC_CMD();
//   spi_txrx(DISPON);
// }

// TODO: Maybe leverage a MAIN_SCREEN struct to fill in the width and height
// of the screen so we can check if col_start/col_end are [0,<SCREEN WIDTH>[
static void main_screen_caset(const uint16_t col_start, const uint16_t col_end)
{
  DC_CMD();
  spi_txrx(CASET);

  DC_DATA();
  spi_txrx(col_start >> 8);
  spi_txrx(col_start & 0xFF);
  spi_txrx(col_end >> 8);
  spi_txrx(col_end & 0xFF);
}

// TODO: Maybe leverage a MAIN_SCREEN struct to fill in the width and height
// of the screen so we can check if row_start/row_end are [0,<SCREEN HEIGHT>[
static void main_screen_raset(const uint16_t row_start, const uint16_t row_end)
{
  DC_CMD();
  spi_txrx(RASET);

  DC_DATA();
  spi_txrx(row_start >> 8);
  spi_txrx(row_start & 0xFF);
  spi_txrx(row_end >> 8);
  spi_txrx(row_end & 0xFF);
}

void main_screen_ramwr(void)
{
  DC_CMD();
  spi_txrx(RAMWR);
  DC_DATA();
}

// For a table of the different parameters for this command, refer to table at
// p.183
// static void main_screen_madctl(const uint8_t arg)
// {
//   DC_CMD();
//   spi_txrx(MADCTL);
//   DC_DATA();
//   spi_txrx(arg);
// }

// static void main_screen_colmod(const colmod_arg arg)
// {
//   DC_CMD();
//   spi_txrx(COLMOD);
//   DC_DATA();
//   spi_txrx(arg);
// }

// Initialisation sequence from:
// https://github.com/Bodmer/TFT_eSPI/blob/master/TFT_Drivers/ILI9488_Init.h
static void ili9488_init_driver(void)
{
  ili9488_positive_gamma_control();
  ili9488_negative_gamma_control();
  ili9488_power_control_1();
  ili9488_power_control_2();
  ili9488_vcom_control();
  ili9488_memory_access_control();
  ili9488_interface_pixel_format();
  ili9488_interface_mode_control();
  ili9488_frame_rate_control_normal();
  ili9488_display_inversion_control();
  ili9488_display_function_control();
  ili9488_entry_mode_set();
  ili9488_adjust_control_3();
  ili9488_sleep_out();
  ili9488_display_on();
}

// Setup commands
void main_screen_init()
{
  DDRH |= (MAIN_SCREEN_CS);
  MAIN_SCREEN_CS_HIGH();

  DDRH |= (SCREENS_DC);
  PORTH |= (SCREENS_DC);

  DDRH |= (MAIN_SCREEN_RST);
  MAIN_SCREEN_RST_HIGH();

  DDRH |= (MAIN_SCREEN_BL);
  MAIN_SCREEN_BL_HIGH();

  uart_printstr("Initialising main screen...");

  ili9488_reset();
  ili9488_init_driver();

  uart_printstr(ANSI_GREEN "OK!" ANSI_RESET "\r\n");
}

// ------ Drawing commands ------------------------------------------------
void ili9488_fill_screen(uint16_t color565)
{
  // Convertit le RGB565 en RGB666 (18-bit), format attendu par
  // l'ILI9486 sur son interface SPI : chaque composante sur 6 bits
  // utiles, alignée dans les bits hauts d'un octet.
  uint8_t r = ((color565 >> 11) & 0x1F) << 3;  // 5 bits -> 8 bits (bits hauts)
  uint8_t g = ((color565 >> 5) & 0x3F) << 2;   // 6 bits -> 8 bits (bits hauts)
  uint8_t b = (color565 & 0x1F) << 3;          // 5 bits -> 8 bits (bits hauts)

  // Column Address Set (CASET, 0x2A)
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx(0x2A);
  DC_DATA();
  spi_txrx(0x00);
  spi_txrx(0x00);
  spi_txrx(0x01);
  spi_txrx(0x3F);
  MAIN_SCREEN_CS_HIGH();

  // Page/Row Address Set (PASET, 0x2B)
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx(0x2B);
  DC_DATA();
  spi_txrx(0x00);
  spi_txrx(0x00);
  spi_txrx(0x01);
  spi_txrx(0xDF);
  MAIN_SCREEN_CS_HIGH();

  // Memory Write (RAMWR, 0x2C) — 3 octets par pixel maintenant
  MAIN_SCREEN_CS_LOW();
  DC_CMD();
  spi_txrx(0x2C);
  DC_DATA();
  uint32_t n_pixels = 480UL * 320UL;
  for (uint32_t i = 0; i < n_pixels; i++)
  {
    spi_txrx(r);
    spi_txrx(g);
    spi_txrx(b);
  }
  MAIN_SCREEN_CS_HIGH();
}

void main_screen_draw_pixel(const position pos, const rgb rgb)
{
  window win = {{pos._pos_x, pos._pos_y}, {pos._pos_x + 1, pos._pos_y + 1}};
  main_screen_set_window(win);
  main_screen_ramwr();
  DC_DATA();
  spi_txrx(pack_rgb565(rgb));
}

void main_screen_draw_rectangle(const window win, const rgb rgb)
{
  // uint16_t color = pack_rgb565(rgb);

  main_screen_set_window(win);
  main_screen_ramwr();
  DC_DATA();
  for (uint32_t i = 0; i < (win._end._pos_x - win._start._pos_x + 1) *
                               (win._end._pos_y - win._start._pos_y + 1);
       i++)
  {
    spi_txrx(rgb._red & 0xFC);
    spi_txrx(rgb._green & 0xFC);
    spi_txrx(rgb._blue & 0xFC);
  }
}

// ------ Utilitaries commands --------------------------------------------
// NOTE: Interesting bit on 16-bit pixel SPI transmission at MAIN_SCREEN's
// datasheet p.88
uint16_t pack_rgb565(const rgb colour)
{
  // For the red, we need to isolate the 5 most significant bits:
  //  1 1 1 1 | 1 0 0 0 (the `1` indicate the most significant bits)
  //  As the first half is full of 1, we know our mask starts with 0xF
  //  Then, only a 1 for 2^3=8, so the end of the mask is 8
  //  The mask thus is 0xF8

  // For the green, we need to isolate the 6 most significant bits:
  //  1 1 1 1 | 1 1 0 0 (the `1` indicate the most significant bits)
  //  As the first half is full of 1, we know our mask starts with 0xF
  //  Then, only a 1 for 2^3=8 and 2^2=4, so the end of the mask is 12 in
  //  decimal and C in hex The mask thus is 0xFC

  // For the blue we just bitshift 3 times to the right as it would be
  // essentially the same as applying 0xF8 on it.

  // In the end, this is the structure of the data to send :
  // R  R  R  R  R  G  G G G G G B B B B B
  // 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
  return (((colour._red & 0xF8) << 8) | ((colour._green & 0xFC) << 3) |
          (colour._blue >> 3));
}

uint16_t pack_rgb666(const rgb colour)
{
  // For the red, we need to isolate the 5 most significant bits:
  //  1 1 1 1 | 1 0 0 0 (the `1` indicate the most significant bits)
  //  As the first half is full of 1, we know our mask starts with 0xF
  //  Then, only a 1 for 2^3=8, so the end of the mask is 8
  //  The mask thus is 0xF8

  // For the green, we need to isolate the 6 most significant bits:
  //  1 1 1 1 | 1 1 0 0 (the `1` indicate the most significant bits)
  //  As the first half is full of 1, we know our mask starts with 0xF
  //  Then, only a 1 for 2^3=8 and 2^2=4, so the end of the mask is 12 in
  //  decimal and C in hex The mask thus is 0xFC

  // For the blue we just bitshift 3 times to the right as it would be
  // essentially the same as applying 0xF8 on it.

  // In the end, this is the structure of the data to send :
  // R  R  R  R  R  G  G G G G G B B B B B
  // 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
  return (((colour._red & 0xF8) << 8) | ((colour._green & 0xFC) << 3) |
          (colour._blue >> 3));
}

void main_screen_set_window(const window win)
{
  main_screen_caset(win._start._pos_y, win._end._pos_y);
  main_screen_raset(win._start._pos_x, win._end._pos_x);
}
