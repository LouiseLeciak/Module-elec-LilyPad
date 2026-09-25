#include "app/language.h"
#include "display/GC9A01.h"
#include "display/main_screen.h"
#include "input/i2c_rotary.h"
#include "input/keypad.h"
#include "pinout.h"
#include "storage/fatfs.h"
#include "system/spi.h"
#include "system/uart.h"
#include "utils/keyboard_utils.h"

// void sd_init() { DDRH |= (SD_CS); }

void eyes_init()
{
  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
  GC9A01_init(LEFT_EYE);
  GC9A01_init(RIGHT_EYE);
}

void screens_init()
{
  DDRH |= (SCREENS_DC);
  main_screen_init();
  eyes_init();
}

void init(void)
{
  spi_master_init();
  uart_init(MYUBRR);

  _delay_ms(150);
  disk_initialize(0);  // SD Initialisation
  if (parse_mbr() == RES_OK)
  {
    if (parse_vbr() == RES_OK)
    {
      scan_root_dir();
    }
    else
    {
      uart_printstr("Failed to parse VBR\r\n");
    }
  }
  else
  {
    uart_printstr("Failed to parse MBR\r\n");
  }
  // Set SPI clock to highest speed after SD initialisation
  SPSR |= (1 << SPI2X);
  SPCR &= ~(1 << SPR0);
  SPCR &= ~(1 << SPR1);

  screens_init();
  i2c_init();
  mcp_init();

  keypad_init();
  rotary_init();
  language_switch_init();
}
