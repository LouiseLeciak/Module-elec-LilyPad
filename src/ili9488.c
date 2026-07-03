#include "ili9488.h"

#include <stddef.h>
#include <util/delay.h>

#include "main_screen.h"
#include "pins.h"
#include "spi.h"

// NOTE: Command functions have hardcoded arguments for now, will make it more
// modular later. -Maddie

void ili9488_positive_gamma_control() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(PGC);
  MAIN_SCREEN_DC_DATA();
  // The following arguments are the anchor values for the positive gamma scale
  spi_master_transmit(0x00);
  spi_master_transmit(0x03);
  spi_master_transmit(0x09);
  spi_master_transmit(0x08);
  spi_master_transmit(0x16);
  spi_master_transmit(0x0A);
  spi_master_transmit(0x3F);
  spi_master_transmit(0x78);
  spi_master_transmit(0x4C);
  spi_master_transmit(0x09);
  spi_master_transmit(0x0A);
  spi_master_transmit(0x08);
  spi_master_transmit(0x16);
  spi_master_transmit(0x1A);
  spi_master_transmit(0x0F);
}

void ili9488_negative_gamma_control() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(NGC);
  spi_master_transmit(0x00);
  spi_master_transmit(0x16);
  spi_master_transmit(0x19);
  spi_master_transmit(0x03);
  spi_master_transmit(0x0F);
  spi_master_transmit(0x05);
  spi_master_transmit(0x32);
  spi_master_transmit(0x45);
  spi_master_transmit(0x46);
  spi_master_transmit(0x04);
  spi_master_transmit(0x0E);
  spi_master_transmit(0x0D);
  spi_master_transmit(0x35);
  spi_master_transmit(0x37);
  spi_master_transmit(0x0F);
}

void ili9488_power_control_1() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(PWR1);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(0x17);
  spi_master_transmit(0x15);
}

void ili9488_power_control_2() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(PWR2);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(0x41);
}

void ili9488_vcom_control() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(VCMPCTL);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(0x00);
  spi_master_transmit(0x12);
  spi_master_transmit(0x80);
}

void ili9488_memory_access_control() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(MADCTL);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(0x48); // Might make display inverted
}

void ili9488_interface_pixel_format() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(COLMOD);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(0x66);
}

void ili9488_interface_mode_control() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(IFMODE);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(0x00);
}

void ili9488_frame_rate_control_normal() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(FRMCTR1);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(0xA0);
}

void il9488_display_inversion_control() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(INVTR);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(0x02);
}

void ili9488_display_function_control() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(DFC);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(0x02);
  spi_master_transmit(0x02);
  spi_master_transmit(0x3B);
}

void ili9488_entry_mode_set() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(EM);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(0xC6);
}

void ili9488_adjust_control_3() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(ADJC3);
  MAIN_SCREEN_DC_DATA();
  spi_master_transmit(0xA9);
  spi_master_transmit(0x51);
  spi_master_transmit(0x2C);
  spi_master_transmit(0x82);
}

void ili9488_sleep_out() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(SLPOUT);
  _delay_ms(120);
}

void ili9488_display_on() {
  MAIN_SCREEN_DC_COMMAND();
  spi_master_transmit(DISPON);
  _delay_ms(50);
}

void ili9488_init_driver() {
  // MAIN_SCREEN_RST_LOW();
  // _delay_ms(100);
  // MAIN_SCREEN_RST_HIGH();
  // _delay_ms(120);
  main_screen_swreset();
  ili9488_positive_gamma_control();
  // ili9488_negative_gamma_control(); // Blanks the screen.
  ili9488_power_control_1();
  ili9488_power_control_2();
  ili9488_vcom_control();
  ili9488_memory_access_control();
  ili9488_interface_pixel_format();
  ili9488_interface_mode_control();
  ili9488_frame_rate_control_normal();
  il9488_display_inversion_control();
  ili9488_display_function_control();
  ili9488_entry_mode_set();
  ili9488_adjust_control_3();
  ili9488_sleep_out();
  ili9488_display_on();
}

void main_screen_init(void) {
  MAIN_SCREEN_RST_LOW();
  _delay_ms(100);
  MAIN_SCREEN_RST_HIGH();
  _delay_ms(120);
  main_screen_swreset();
  _delay_ms(5);
  main_screen_slpout();
  _delay_ms(5);
  main_screen_madctl(0x48); // Not really elegant for now*.
  main_screen_colmod(CI_16B);
  main_screen_dispon();

  // *: The only bits that are different from its standard value are bit 3 and
  // bit 7.
  // Bit 3: Set to 1 so as to send the data in BGR as the LCD panel we
  // have (HSD-9190J-B3) seems to be wired with Red and Blue inverted.
  // Bit 7:
  // Set to 1 so as to have coorinate 0 be left-hand side of the screen instead
  // of right-hand side.
}
