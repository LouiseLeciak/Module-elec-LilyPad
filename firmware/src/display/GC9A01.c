/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GC9A01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:07:34 by nige42            #+#    #+#             */
/*   Updated: 2026/10/17 12:38:36 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "display/GC9A01.h"

#include <avr/io.h>
#include <avr/pgmspace.h>  //memory
#include <stdint.h>
#include <util/delay.h>

#include "app/menu.h"
#include "display/eye_imgs.h"
#include "pinout.h"
#include "system/spi.h"


/* ------------------------------------------------------------------ */
/* Level 1 commands (always available)             p.91-93, 97-152     */
/* ------------------------------------------------------------------ */
typedef enum
{
    GC9A01_CMD_RDDID        = 0x04, /* Read display identification   p.97  (3 bytes, default 00 9A 01) */
    GC9A01_CMD_RDDST        = 0x09, /* Read display status           p.99  */
    GC9A01_CMD_SLPIN        = 0x10, /* Enter sleep mode              p.101 */
    GC9A01_CMD_SLPOUT       = 0x11, /* Sleep out                     p.103 (wait 5 ms; 120 ms before next SLPIN->SLPOUT) */
    GC9A01_CMD_PTLON        = 0x12, /* Partial mode ON               p.105 */
    GC9A01_CMD_NORON        = 0x13, /* Normal display mode ON        p.106 */
    GC9A01_CMD_INVOFF       = 0x20, /* Display inversion OFF         p.107 */
    GC9A01_CMD_INVON        = 0x21, /* Display inversion ON          p.108 */
    GC9A01_CMD_DISPOFF      = 0x28, /* Display OFF                   p.109 */
    GC9A01_CMD_DISPON       = 0x29, /* Display ON                    p.110 */
    GC9A01_CMD_CASET        = 0x2A, /* Column address set (4 params) p.111 */
    GC9A01_CMD_RASET        = 0x2B, /* Row address set (4 params)    p.113 */
    GC9A01_CMD_RAMWR        = 0x2C, /* Memory write                  p.115 */
    GC9A01_CMD_PTLAR        = 0x30, /* Partial area (4 params)       p.117 */
    GC9A01_CMD_VSCRDEF      = 0x33, /* Vertical scrolling definition p.120 */
    GC9A01_CMD_TEOFF        = 0x34, /* Tearing effect line OFF       p.124 */
    GC9A01_CMD_TEON         = 0x35, /* Tearing effect line ON (1 param: M)  p.125 */
    GC9A01_CMD_MADCTL       = 0x36, /* Memory access control (1 param)      p.127 */
    GC9A01_CMD_VSCRSADD     = 0x37, /* Vertical scrolling start address     p.130 */
    GC9A01_CMD_IDMOFF       = 0x38, /* Idle mode OFF                 p.132 */
    GC9A01_CMD_IDMON        = 0x39, /* Idle mode ON (8 colors)       p.133 */
    GC9A01_CMD_COLMOD       = 0x3A, /* Pixel format set (1 param)    p.135 */
    GC9A01_CMD_RAMWRC       = 0x3C, /* Write memory continue         p.137 */
    GC9A01_CMD_SETTEARSCAN  = 0x44, /* Set tear scanline             p.140 */
    GC9A01_CMD_GETSCAN      = 0x45, /* Get scanline                  p.142 */
    GC9A01_CMD_WRDISBV      = 0x51, /* Write display brightness      p.143 */
    GC9A01_CMD_WRCTRLD      = 0x53, /* Write CTRL display            p.145 */
    GC9A01_CMD_RDID1        = 0xDA, /* Read ID1 (default 00)         p.147 */
    GC9A01_CMD_RDID2        = 0xDB, /* Read ID2 (default 9A)         p.149 */
    GC9A01_CMD_RDID3        = 0xDC  /* Read ID3 (default 01)         p.151 */
} gc9a01_cmd_t;
 
/* ------------------------------------------------------------------ */
/* Level 2 commands (extended)                     p.94, 153-163       */
/* ------------------------------------------------------------------ */
typedef enum
{
    GC9A01_CMD_RGBCTRL      = 0xB0, /* RGB interface signal control  p.153 */
    GC9A01_CMD_BPC          = 0xB5, /* Blanking porch control        p.156 */
    GC9A01_CMD_DFC          = 0xB6, /* Display function control      p.158 (2nd byte is read, 1st ignored) */
    GC9A01_CMD_TECTRL       = 0xBA, /* Tearing effect control        p.161 */
    GC9A01_CMD_IFCTRL       = 0xF6  /* Interface control             p.163 */
} gc9a01_cmd_l2_t;
 
/* ------------------------------------------------------------------ */
/* Level 3 commands ("inter" commands)             p.94-96, 164-181    */
/* Only usable after FEh then EFh were written (see note below).       */
/* ------------------------------------------------------------------ */
typedef enum
{
    GC9A01_CMD_PWR1         = 0xC1, /* Power control 1 (vcire)               p.167 */
    GC9A01_CMD_PWR2         = 0xC3, /* Power control 2 (vreg1_vbp_d, VREG1A/B) p.168 */
    GC9A01_CMD_PWR3         = 0xC4, /* Power control 3 (vreg1_vbn_d, VREG2A/B) p.169 */
    GC9A01_CMD_PWR4         = 0xC9, /* Power control 4 (vrh)                 p.170 */
    GC9A01_CMD_PWR7         = 0xA7, /* Power control 7 (vdd_ad)              p.171 */
    GC9A01_CMD_FRAMERATE    = 0xE8, /* Frame rate (DINV, RTN1, RTN2)         p.164 (layout p.95) */
    GC9A01_CMD_SPI2DATA     = 0xE9, /* SPI 2-data control                    p.166 */
    GC9A01_CMD_CHARGEPUMP   = 0xEC, /* Charge pump frequency control         p.95 only (no detail page) */
    GC9A01_CMD_INTER_EN1    = 0xFE, /* Inter register enable 1               p.172 */
    GC9A01_CMD_INTER_EN2    = 0xEF, /* Inter register enable 2               p.173 */
    GC9A01_CMD_GAMMA1       = 0xF0, /* Set gamma 1 (6 params)                p.174 */
    GC9A01_CMD_GAMMA2       = 0xF1, /* Set gamma 2 (6 params)                p.176 */
    GC9A01_CMD_GAMMA3       = 0xF2, /* Set gamma 3 (6 params)                p.178 */
    GC9A01_CMD_GAMMA4       = 0xF3  /* Set gamma 4 (6 params)                p.180 */
} gc9a01_cmd_l3_t;
 
/* ------------------------------------------------------------------ */
/* Sent by your init, NOT described in this datasheet                  */
/* ------------------------------------------------------------------ */
typedef enum
{
    /* 0x01: "software reset"*/
    GC9A01_CMD_SWRESET      = 0x01,
 
    /* No description anywhere in the datasheet. Values below come from
    // Found information here https://github.com/adafruit/Adafruit_GC9A01A/blob/main/Adafruit_GC9A01A.cpp
    // Initialization sequence came from some early code provided by the
    // manufacturer. Many of these registers are undocumented, some might
    // be unnecessary, just playing along...
    */
    GC9A01_VENDOR_62 = 0x62, GC9A01_VENDOR_63 = 0x63, GC9A01_VENDOR_64 = 0x64,
    GC9A01_VENDOR_66 = 0x66, GC9A01_VENDOR_67 = 0x67, GC9A01_VENDOR_74 = 0x74,
    GC9A01_VENDOR_84 = 0x84, GC9A01_VENDOR_85 = 0x85, GC9A01_VENDOR_86 = 0x86,
    GC9A01_VENDOR_87 = 0x87, GC9A01_VENDOR_88 = 0x88, GC9A01_VENDOR_89 = 0x89,
    GC9A01_VENDOR_8A = 0x8A, GC9A01_VENDOR_8B = 0x8B, GC9A01_VENDOR_8C = 0x8C,
    GC9A01_VENDOR_8D = 0x8D, GC9A01_VENDOR_8E = 0x8E, GC9A01_VENDOR_8F = 0x8F,
    GC9A01_VENDOR_90 = 0x90, GC9A01_VENDOR_98 = 0x98, GC9A01_VENDOR_AE = 0xAE,
    GC9A01_VENDOR_BC = 0xBC, GC9A01_VENDOR_BD = 0xBD, GC9A01_VENDOR_BE = 0xBE,
    GC9A01_VENDOR_CD = 0xCD, GC9A01_VENDOR_DF = 0xDF, GC9A01_VENDOR_E1 = 0xE1,
    GC9A01_VENDOR_EB = 0xEB, GC9A01_VENDOR_ED = 0xED, GC9A01_VENDOR_FF = 0xFF
} gc9a01_vendor_cmd_t;
 
/* ------------------------------------------------------------------ */
/* Parameter bits                                                      */
/* ------------------------------------------------------------------ */
 
/* MADCTL (36h) parameter, p.127. Default 0x00 (p.128). */
typedef enum
{
    GC9A01_MADCTL_MY  = 0x80, /* D7 row address order        */
    GC9A01_MADCTL_MX  = 0x40, /* D6 column address order     */
    GC9A01_MADCTL_MV  = 0x20, /* D5 row/column exchange      */
    GC9A01_MADCTL_ML  = 0x10, /* D4 vertical refresh order   */
    GC9A01_MADCTL_BGR = 0x08, /* D3 0 = RGB filter, 1 = BGR  */
    GC9A01_MADCTL_MH  = 0x04  /* D2 horizontal refresh order */
} gc9a01_madctl_t;
 
/* COLMOD (3Ah) parameter, p.135. Default 0x66 (18 bit both). */
typedef enum
{
    GC9A01_COLMOD_DBI_12BIT = 0x03, /* MCU interface, D2:D0 = 011 */
    GC9A01_COLMOD_DBI_16BIT = 0x05, /* MCU interface, D2:D0 = 101 (RGB565) */
    GC9A01_COLMOD_DBI_18BIT = 0x06, /* MCU interface, D2:D0 = 110 */
    GC9A01_COLMOD_DPI_16BIT = 0x50, /* RGB interface, D6:D4 = 101 */
    GC9A01_COLMOD_DPI_18BIT = 0x60  /* RGB interface, D6:D4 = 110 */
} gc9a01_colmod_t;
 
/* TEON (35h) parameter, p.125. */
typedef enum
{
    GC9A01_TEON_VBLANK_ONLY      = 0x00, /* M = 0: V-blanking only */
    GC9A01_TEON_VBLANK_AND_HBLANK = 0x01 /* M = 1: V and H blanking */
} gc9a01_teon_t;
 
/* Display Function Control (B6h), 2nd parameter, p.158. */
typedef enum
{
    GC9A01_DFC_GS = 0x40, /* D6: gate scan, 0 = G1->G32, 1 = G32->G1 */
    GC9A01_DFC_SS = 0x20  /* D5: source scan, 0 = S1->S360, 1 = S360->S1 */
} gc9a01_dfc_t;
 
/* Frame rate (E8h), 1st parameter DINV, p.164 (inversion table). */
typedef enum
{
    GC9A01_DINV_COLUMN = 0, /* default after reset is 1 (p.165) */
    GC9A01_DINV_1DOT   = 1,
    GC9A01_DINV_2DOT   = 2,
    GC9A01_DINV_4DOT   = 3,
    GC9A01_DINV_8DOT   = 4
} gc9a01_dinv_t;
 

void GC9A01_init(uint8_t screen)
{
  // 1. Hardware Reset
  RST_HIGH();
  _delay_ms(10);
  RST_LOW();
  _delay_ms(20);
  RST_HIGH();
  _delay_ms(150);
 
  // 2. Software Reset & Wake-up
  GC9A01_cmd(GC9A01_CMD_SWRESET, screen);   // not in datasheet command list
  _delay_ms(150);
 
  GC9A01_cmd(GC9A01_CMD_SLPOUT, screen);    // Exit Sleep first so charge pumps activate (p.103)
  _delay_ms(120);
 
  // 3. Enable internal commands (FEh then EFh, p.172-173)
  GC9A01_cmd(GC9A01_CMD_INTER_EN1, screen);
  GC9A01_cmd(GC9A01_CMD_INTER_EN2, screen);
 
  // Undocumented registers (not described in the datasheet)
  // Found information here https://github.com/adafruit/Adafruit_GC9A01A/blob/main/Adafruit_GC9A01A.cpp
  // // Initialization sequence came from some early code provided by the
  // manufacturer. Many of these registers are undocumented, some might
  // be unnecessary, just playing along...
  GC9A01_cmd(GC9A01_VENDOR_EB, screen);
  GC9A01_data(0x14, screen);
  GC9A01_cmd(GC9A01_VENDOR_84, screen);
  GC9A01_data(0x40, screen);
  GC9A01_cmd(GC9A01_VENDOR_85, screen);
  GC9A01_data(0xFF, screen);
  GC9A01_cmd(GC9A01_VENDOR_86, screen);
  GC9A01_data(0xFF, screen);
  GC9A01_cmd(GC9A01_VENDOR_87, screen);
  GC9A01_data(0xFF, screen);
  GC9A01_cmd(GC9A01_VENDOR_88, screen);
  GC9A01_data(0x0A, screen);
  GC9A01_cmd(GC9A01_VENDOR_89, screen);
  GC9A01_data(0x21, screen);
  GC9A01_cmd(GC9A01_VENDOR_8A, screen);
  GC9A01_data(0x00, screen);
  GC9A01_cmd(GC9A01_VENDOR_8B, screen);
  GC9A01_data(0x80, screen);
  GC9A01_cmd(GC9A01_VENDOR_8C, screen);
  GC9A01_data(0x01, screen);
  GC9A01_cmd(GC9A01_VENDOR_8D, screen);
  GC9A01_data(0x01, screen);
  GC9A01_cmd(GC9A01_VENDOR_8E, screen);
  GC9A01_data(0xFF, screen);
  GC9A01_cmd(GC9A01_VENDOR_8F, screen);
  GC9A01_data(0xFF, screen);
 
  // Display Function Control (p.158): 1st byte ignored, 2nd = GS/SS bits (both 0)
  GC9A01_cmd(GC9A01_CMD_DFC, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(0x00, screen);
 
  // Memory Access Control (p.127)
  GC9A01_cmd(GC9A01_CMD_MADCTL, screen);
  GC9A01_data(GC9A01_MADCTL_BGR, screen);   // BGR order (0x08)
 
  // Pixel Format (p.135): 16-bit RGB565 on the MCU interface
  GC9A01_cmd(GC9A01_CMD_COLMOD, screen);
  GC9A01_data(GC9A01_COLMOD_DBI_16BIT, screen);   // 0x05
 
  // VREG / Power Setup
  GC9A01_cmd(GC9A01_VENDOR_90, screen);
  GC9A01_data(0x08, screen);
  GC9A01_data(0x08, screen);
  GC9A01_data(0x08, screen);
  GC9A01_data(0x08, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_BD, screen);
  GC9A01_data(0x06, screen);
  GC9A01_cmd(GC9A01_VENDOR_BC, screen);
  GC9A01_data(0x00, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_FF, screen);
  GC9A01_data(0x60, screen);
  GC9A01_data(0x01, screen);
  GC9A01_data(0x04, screen);
 
  // Power control 2/3/4 (p.168-170)
  GC9A01_cmd(GC9A01_CMD_PWR2, screen);      // vreg1_vbp_d = 0x13
  GC9A01_data(0x13, screen);
  GC9A01_cmd(GC9A01_CMD_PWR3, screen);      // vreg1_vbn_d = 0x13
  GC9A01_data(0x13, screen);
  GC9A01_cmd(GC9A01_CMD_PWR4, screen);      // vrh = 0x22
  GC9A01_data(0x22, screen);
 
  // Found information here https://github.com/adafruit/Adafruit_GC9A01A/blob/main/Adafruit_GC9A01A.cpp
  // // Initialization sequence came from some early code provided by the
  // manufacturer. Many of these registers are undocumented, some might
  // be unnecessary, just playing along...
  GC9A01_cmd(GC9A01_VENDOR_BE, screen);
  GC9A01_data(0x11, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_E1, screen);
  GC9A01_data(0x10, screen);
  GC9A01_data(0x0E, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_DF, screen);
  GC9A01_data(0x21, screen);
  GC9A01_data(0x0C, screen);
  GC9A01_data(0x02, screen);
 
  // Gamma Curves (p.174-181)
  GC9A01_cmd(GC9A01_CMD_GAMMA1, screen);
  GC9A01_data(0x45, screen);
  GC9A01_data(0x09, screen);
  GC9A01_data(0x08, screen);
  GC9A01_data(0x08, screen);
  GC9A01_data(0x26, screen);
  GC9A01_data(0x2A, screen);
 
  GC9A01_cmd(GC9A01_CMD_GAMMA2, screen);
  GC9A01_data(0x43, screen);
  GC9A01_data(0x70, screen);
  GC9A01_data(0x72, screen);
  GC9A01_data(0x36, screen);
  GC9A01_data(0x37, screen);
  GC9A01_data(0x6F, screen);
 
  GC9A01_cmd(GC9A01_CMD_GAMMA3, screen);
  GC9A01_data(0x45, screen);
  GC9A01_data(0x09, screen);
  GC9A01_data(0x08, screen);
  GC9A01_data(0x08, screen);
  GC9A01_data(0x26, screen);
  GC9A01_data(0x2A, screen);
 
  GC9A01_cmd(GC9A01_CMD_GAMMA4, screen);
  GC9A01_data(0x43, screen);
  GC9A01_data(0x70, screen);
  GC9A01_data(0x72, screen);
  GC9A01_data(0x36, screen);
  GC9A01_data(0x37, screen);
  GC9A01_data(0x6F, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_ED, screen);
  GC9A01_data(0x1B, screen);
  GC9A01_data(0x0B, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_AE, screen);
  GC9A01_data(0x77, screen);
  GC9A01_cmd(GC9A01_VENDOR_CD, screen);
  GC9A01_data(0x63, screen);
 
  // Frame rate (p.164): DINV = 4-dot inversion, RTN1 = 4 (0x34). RTN2 not sent.
  GC9A01_cmd(GC9A01_CMD_FRAMERATE, screen);
  GC9A01_data((GC9A01_DINV_4DOT << 4) | 0x04, screen);
 
  // Source Timing Configuration (undocumented)
  // Found information here https://github.com/adafruit/Adafruit_GC9A01A/blob/main/Adafruit_GC9A01A.cpp
  // // Initialization sequence came from some early code provided by the
  // manufacturer. Many of these registers are undocumented, some might
  // be unnecessary, just playing along...
  
  GC9A01_cmd(GC9A01_VENDOR_62, screen);
  GC9A01_data(0x18, screen);
  GC9A01_data(0x0D, screen);
  GC9A01_data(0x71, screen);
  GC9A01_data(0xED, screen);
  GC9A01_data(0x70, screen);
  GC9A01_data(0x70, screen);
  GC9A01_data(0x18, screen);
  GC9A01_data(0x0F, screen);
  GC9A01_data(0x71, screen);
  GC9A01_data(0xEF, screen);
  GC9A01_data(0x70, screen);
  GC9A01_data(0x70, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_63, screen);
  GC9A01_data(0x18, screen);
  GC9A01_data(0x11, screen);
  GC9A01_data(0x71, screen);
  GC9A01_data(0xF1, screen);
  GC9A01_data(0x70, screen);
  GC9A01_data(0x70, screen);
  GC9A01_data(0x18, screen);
  GC9A01_data(0x13, screen);
  GC9A01_data(0x71, screen);
  GC9A01_data(0xF3, screen);
  GC9A01_data(0x70, screen);
  GC9A01_data(0x70, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_64, screen);
  GC9A01_data(0x28, screen);
  GC9A01_data(0x29, screen);
  GC9A01_data(0xF1, screen);
  GC9A01_data(0x01, screen);
  GC9A01_data(0xF1, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(0x07, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_66, screen);
  GC9A01_data(0x3C, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(0xCD, screen);
  GC9A01_data(0x67, screen);                // data byte (not a command)
  GC9A01_data(0x45, screen);
  GC9A01_data(0x45, screen);
  GC9A01_data(0x10, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(0x00, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_67, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(0x3C, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(0x01, screen);
  GC9A01_data(0x54, screen);
  GC9A01_data(0x10, screen);
  GC9A01_data(0x32, screen);
  GC9A01_data(0x98, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_74, screen);
  GC9A01_data(0x10, screen);
  GC9A01_data(0x85, screen);
  GC9A01_data(0x80, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(0x4E, screen);
  GC9A01_data(0x00, screen);
 
  GC9A01_cmd(GC9A01_VENDOR_98, screen);
  GC9A01_data(0x3E, screen);
  GC9A01_data(0x07, screen);
 
  // Inversion (p.108)
  GC9A01_cmd(GC9A01_CMD_INVON, screen);    

  // 4. Software Reset (Not Documented found at https://github.com/adafruit/Adafruit_GC9A01A/blob/main/Adafruit_GC9A01A.h )
  GC9A01_cmd(GC9A01_CMD_SWRESET, screen);
  _delay_ms(120);

  // 5. Display ON (p.110)
  GC9A01_cmd(GC9A01_CMD_DISPON, screen);
  _delay_ms(120);
}
 
void GC9A01_cmd(uint8_t cmd, uint8_t screen)
{
  DC_LOW();
  if (screen == LEFT_EYE)
  {
    CS_LEFT_EYE_LOW();
    spi_txrx(cmd);
    CS_LEFT_EYE_HIGH();
    return;
  }
  else if (screen == RIGHT_EYE)
  {
    CS_RIGHT_EYE_LOW();
    spi_txrx(cmd);
    CS_RIGHT_EYE_HIGH();
    return;
  }
}

void GC9A01_data(uint8_t data, uint8_t screen)
{
  DC_HIGH();

  if (screen == LEFT_EYE)
  {
    CS_LEFT_EYE_LOW();
    spi_txrx(data);
    CS_LEFT_EYE_HIGH();
    return;
  }
  else if (screen == RIGHT_EYE)
  {
    CS_RIGHT_EYE_LOW();
    spi_txrx(data);
    CS_RIGHT_EYE_HIGH();
    return;
  }
}

void GC9A01_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
                          uint8_t screen)
{
  // Column Address Set
  GC9A01_cmd(0x2A, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(x0, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(x1, screen);

  // Row Address Set
  GC9A01_cmd(0x2B, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(y0, screen);
  GC9A01_data(0x00, screen);
  GC9A01_data(y1, screen);

  // Memory Write (start RAM write)
  GC9A01_cmd(0x2C, screen);
}

void gc9a01_sleep_in(void)
{
  GC9A01_cmd_eyes(GC9A01_CMD_SLPIN);
  _delay_ms(5);
}

void gc9a01_idle_mode_off(void) { GC9A01_cmd_eyes(GC9A01_CMD_IDMOFF); }

void gc9a01_idle_mode_on(void) { GC9A01_cmd_eyes(GC9A01_CMD_IDMON); }

void gc9a01_sleep_out(void)
{
  GC9A01_cmd_eyes(GC9A01_CMD_SLPOUT);
  _delay_ms(120);
}

void GC9A01_fillScreen(uint16_t color, uint8_t screen)
{
  GC9A01_setAddrWindow(0, 0, GC9A01_WIDTH - 1, GC9A01_HEIGHT - 1, screen);
  GC9A01_pushColor(color, FULLSCREEN, screen);
}

// pixel push
void GC9A01_pushColor(uint16_t color, uint16_t pixels, uint8_t screen)
{
  uint8_t hi = color >> 8;
  uint8_t lo = color & 0xFF;

  if (screen == LEFT_EYE)
  {
    DC_HIGH();
    CS_LEFT_EYE_LOW();
    while (pixels--)
    {
      spi_txrx(hi);
      spi_txrx(lo);
    }
    CS_LEFT_EYE_HIGH();
    return;
  }
  else if (screen == RIGHT_EYE)
  {
    DC_HIGH();

    CS_RIGHT_EYE_LOW();
    while (pixels--)
    {
      spi_txrx(hi);
      spi_txrx(lo);
    }
    CS_RIGHT_EYE_HIGH();
    return;
  }
}

/// Alls Eyes together functions

void GC9A01_cmd_eyes(uint8_t cmd)
{
  DC_LOW();
  spi_txrx(cmd);
  return;
}

void GC9A01_data_eyes(uint8_t data)
{
  DC_HIGH();
  spi_txrx(data);
  return;
}

void GC9A01_setAddrWindow_eyes(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  GC9A01_cmd_eyes(GC9A01_CMD_CASET);
  GC9A01_data_eyes(0x00);
  GC9A01_data_eyes(x0);
  GC9A01_data_eyes(0x00);
  GC9A01_data_eyes(x1);

  GC9A01_cmd_eyes(GC9A01_CMD_RASET);
  GC9A01_data_eyes(0x00);
  GC9A01_data_eyes(y0);
  GC9A01_data_eyes(0x00);
  GC9A01_data_eyes(y1);
  // Memory Write (start RAM write)
  GC9A01_cmd_eyes(0x2C);
}

// pixel push
void GC9A01_pushColor_eyes(uint16_t color, uint16_t count)
{
  uint8_t hi = color >> 8;
  uint8_t lo = color & 0xFF;

  DC_HIGH();
  while (count--)
  {
    spi_txrx(hi);
    spi_txrx(lo);
  }
  return;
}

void GC9A01_fillScreen_eyes(uint16_t color)
{
  CS_LEFT_EYE_LOW();
  CS_RIGHT_EYE_LOW();

  GC9A01_setAddrWindow_eyes(0, 0, GC9A01_WIDTH - 1, GC9A01_HEIGHT - 1);
  GC9A01_pushColor_eyes(color, FULLSCREEN);

  CS_LEFT_EYE_HIGH();
  CS_RIGHT_EYE_HIGH();
}

// Helper function of GC9A01_drawEye_img()
// Sends the color to be drawn

void GC9A01_draw_pixel(uint16_t color)
{
  spi_txrx(color >> 8);
  spi_txrx(color & 0xFF);
}

void GC9A01_drawImg_eyes(const uint8_t* file, uint8_t maxNbrLines,
                         uint8_t hSizeBytes, uint16_t fg, uint16_t bg)
{
  uint16_t newfg;
  DC_HIGH();
  CS_LEFT_EYE_LOW();
  CS_RIGHT_EYE_LOW();
  for (int line = 0; line < maxNbrLines; line++)
  {
    for (int byte = 0; byte < hSizeBytes; byte++)
    {
      uint8_t b = pgm_read_byte(&file[((maxNbrLines - 1) - line) * hSizeBytes +
                                      byte]);  // read rows bottom-up
      for (int bit = 0; bit < 8; bit++)
      {
        uint8_t pixel = (b >> bit) & 1;
        if (b == 0x00)
          newfg = fg;
        else
          newfg = EYE_ACCENT_COLOUR;
        GC9A01_draw_pixel(pixel ? bg : newfg);
      }
    }
  }
  CS_LEFT_EYE_HIGH();
  CS_RIGHT_EYE_HIGH();
}

void GC9A01_draw_eye_Lids(uint16_t pixels, uint16_t color)
{
  GC9A01_pushColor_eyes(color, pixels);
}

void GC9A01_eye_Lids_up(void)
{
  GC9A01_cmd_eyes(GC9A01_CMD_MADCTL);
  GC9A01_data_eyes(0xC8);
  GC9A01_cmd_eyes(GC9A01_CMD_RAMWR);  // force Memory Write before pixel push
}

void GC9A01_eye_Lids_down(void)
{
  GC9A01_cmd_eyes(GC9A01_CMD_MADCTL);
  GC9A01_data_eyes(0x08);
  GC9A01_cmd_eyes(GC9A01_CMD_RAMWR);  // force Memory Write before pixel push
}

void GC9A01_blink(const uint8_t* file, int nbr)
{
  for (int i = 0; i < nbr; i++)
  {
    CS_LEFT_EYE_LOW();
    CS_RIGHT_EYE_LOW();
    GC9A01_eye_Lids_down();
    GC9A01_setAddrWindow_eyes(0, 0, 239, 219);
    GC9A01_draw_eye_Lids(LID_80_PERCENT, EYELID_COLOUR);
    CS_LEFT_EYE_HIGH();
    CS_RIGHT_EYE_HIGH();

    CS_LEFT_EYE_LOW();
    CS_RIGHT_EYE_LOW();
    GC9A01_eye_Lids_up();
    GC9A01_setAddrWindow_eyes(0, 0, 239, 239);
    GC9A01_drawImg_eyes(file, 240, 30, SCLERA_COLOUR, IRIS_COLOUR);
    CS_LEFT_EYE_HIGH();
    CS_RIGHT_EYE_HIGH();
  }
}

uint8_t change_eye(uint8_t es)
{
  uint8_t eye_state = es;
  if (eye_state == 0)
  {
    // GC9A01_blink(Eye_look_Right, 1);
    eye_state = 1;
  }
  else
  {
    // GC9A01_blink(Eye_Front, 1);
    eye_state = 0;
  }
  return eye_state;
}

void eyes_action()
{
  GC9A01_blink(Eye_look_Right, 1);
  GC9A01_blink(Eye_Front, 1);
}

void blink_front() { GC9A01_blink(Eye_Front, 1); }

void win() { GC9A01_blink(Eye_Heart, 1); }

void loose() { GC9A01_blink(Eye_no, 1); }
