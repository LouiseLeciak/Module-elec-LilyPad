#include <stdint.h>

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

static void version_info_string(void)
{
  uart_printstr("\r\nLily's Pad v.1\r\n");
  uart_printstr(
      "Made by Louise \"lleciak\" Leciak, Madeline \"apetitco\" Petitcollin & "
      "Nigel \"nrobinso\" Robinson\r\n");
  uart_printstr("Module Électronique 2026 @ 42 Paris\r\n\r\n");
}

static void eyes_init()
{
  uart_printstr("Initialising eyes...");

  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
  GC9A01_init(LEFT_EYE);
  GC9A01_init(RIGHT_EYE);

  uart_printstr(ANSI_GREEN "OK!" ANSI_RESET "\r\n");
}

static void screens_init()
{
  DDRH |= (SCREENS_DC);
  main_screen_init();
  eyes_init();
}

static DSTATUS sd_init()
{
  if (disk_initialize(0) != RES_OK)
    return RES_NOTRDY;
  if (parse_mbr() == RES_OK)
  {
    if (parse_vbr() == RES_OK)
      scan_root_dir();
    else
      uart_printstr("Failed to parse VBR\r\n");
  }
  else
    uart_printstr("Failed to parse MBR\r\n");
  return RES_OK;
}

void init(void)
{
  uart_init(MYUBRR);
  uart_printstr(
      "\r\nInitialising UART Module...");  // I mean, if it fails... you
                                           // will not be able to read it
                                           // on the serial port
  uart_printstr(ANSI_GREEN "OK!" ANSI_RESET "\r\n");

  version_info_string();

  uart_printstr("Initialising SPI module...");
  spi_master_init();
  uart_printstr(ANSI_GREEN "OK!" ANSI_RESET "\r\n");

#ifdef DEBUG
  uart_printstr("DEBUG MODE: ON\r\n");
#endif

  uart_printstr("Initialising SD Card...");
  sd_init();  // UART print "OK!" is done inside

  _delay_ms(150);
  // Set SPI clock to highest speed after SD initialisation
  SPSR |= (1 << SPI2X);
  SPCR &= ~(1 << SPR0);
  SPCR &= ~(1 << SPR1);

  screens_init();

  uart_printstr("Initialising Two wires (I2C) module...");
  i2c_init();
  uart_printstr(ANSI_GREEN "OK!" ANSI_RESET "\r\n");

  uart_printstr("Initialising MCP23017...");
  mcp_init();
  uart_printstr(ANSI_GREEN "OK!" ANSI_RESET "\r\n");

  keypad_init();
  rotary_init();
  language_switch_init();
}
