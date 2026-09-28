#include "display/ili9488.h"

#include <util/delay.h>

#include "system/spi.h"

static inline void dc_cmd(void) { DC_PORT &= ~(DC_PIN); }
static inline void dc_data(void) { DC_PORT |= (DC_PIN); }
static inline void cs_low(void) { CS_PORT &= ~(CS_PIN); }
static inline void cs_high(void) { CS_PORT |= (CS_PIN); }

void ili9488_positive_gamma_control(void)
{
  cs_low();
  dc_cmd();
  spi_txrx((char)PGC);
  dc_data();
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
  cs_high();
}

void ili9488_negative_gamma_control(void)
{
  cs_low();
  dc_cmd();
  spi_txrx((char)NGC);
  dc_data();
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
  cs_high();
}

void ili9488_power_control_1(void)
{
  cs_low();
  dc_cmd();
  spi_txrx((char)PWR1);
  dc_data();
  spi_txrx(0x17);
  spi_txrx(0x15);
  cs_high();
}

void ili9488_power_control_2(void)
{
  cs_low();
  dc_cmd();
  spi_txrx((char)PWR2);
  dc_data();
  spi_txrx(0x41);
  cs_high();
}

void ili9488_vcom_control(void)
{
  cs_low();
  dc_cmd();
  spi_txrx((char)VCMPCTL);
  dc_data();
  spi_txrx(0x00);
  spi_txrx(0x12);
  spi_txrx(0x80);
  cs_high();
}

void ili9488_memory_access_control(void)
{
  cs_low();
  dc_cmd();
  spi_txrx(MADCTL);
  dc_data();
  // spi_txrx(0x48);  // Calibrated for TFT02 panel orientation
  spi_txrx(0x88);  // Calibrated for TF01 panel orientation
  cs_high();
}

void ili9488_interface_pixel_format(void)
{
  cs_low();
  dc_cmd();
  spi_txrx(COLMOD);
  dc_data();
  spi_txrx(0x66);  // 18-bit/pixel (RGB666)
  cs_high();
}

void ili9488_interface_mode_control(void)
{
  cs_low();
  dc_cmd();
  spi_txrx((char)IFMODE);
  dc_data();
  spi_txrx(0x00);
  cs_high();
}

void ili9488_frame_rate_control_normal(void)
{
  cs_low();
  dc_cmd();
  spi_txrx((char)FRMCTR1);
  dc_data();
  spi_txrx(0xA0);  // 60 Hz
  cs_high();
}

void ili9488_display_inversion_control(void)
{
  cs_low();
  dc_cmd();
  spi_txrx((char)INVTR);
  dc_data();
  spi_txrx(0x02);
  cs_high();
}

void ili9488_display_function_control(void)
{
  cs_low();
  dc_cmd();
  spi_txrx((char)DFC);
  dc_data();
  spi_txrx(0x02);
  spi_txrx(0x02);
  spi_txrx(0x3B);
  cs_high();
}

void ili9488_entry_mode_set(void)
{
  cs_low();
  dc_cmd();
  spi_txrx((char)EM);
  dc_data();
  spi_txrx(0xC6);
  cs_high();
}

void ili9488_adjust_control_3(void)
{
  cs_low();
  dc_cmd();
  spi_txrx((char)ADJC3);
  dc_data();
  spi_txrx(0xA9);
  spi_txrx(0x51);
  spi_txrx(0x2C);
  spi_txrx(0x82);
  cs_high();
}

void ili9488_sleep_out(void)
{
  cs_low();
  dc_cmd();
  spi_txrx(SLPOUT);
  _delay_ms(120);  // Mandatory delay — see ILI9488 datasheet
  cs_high();
}

void ili9488_display_on(void)
{
  cs_low();
  dc_cmd();
  spi_txrx(DISPON);
  _delay_ms(50);
  cs_high();
}

void ili9488_reset(void)
{
  RST_PORT |= MAIN_SCREEN_RST;
  _delay_ms(5);
  RST_PORT &= ~(MAIN_SCREEN_RST);
  _delay_ms(20);
  RST_PORT |= MAIN_SCREEN_RST;
  _delay_ms(150);  // Wait out internal reset — see ILI9488 datasheet
}

// Initialisation sequence from:
// https://github.com/Bodmer/TFT_eSPI/blob/master/TFT_Drivers/ILI9488_Init.h
void ili9488_init_driver(void)
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
