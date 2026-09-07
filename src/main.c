#include "ili9488.h"
#include "pinout.h"
#include "spi.h"

#include <util/delay.h>

int main(void) {
  spi_master_init();
  main_screen_init();
  ili9488_init_driver();

  while (1) {
    ;
  }
}
