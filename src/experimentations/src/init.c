#include "keyboard_utils.h"
#include "pinout.h"
#include "GC9A01.h"

void sd_init() { DDRH |= (SD_CS); }

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

  screens_init();
  // sd_init();
  // keyboard_init();
}