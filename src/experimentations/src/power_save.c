
#include "power_save.h"

#include "globals.h"
#include "pinout.h"
#include "GC9A01.h"

uint32_t inactivity_counter = 0;
uint8_t inac = 0;

// timer to know since when nothing is happening
void power_save_update(void) {
  // 30000 ~= 23s
  if (inactivity_counter < 30000) inactivity_counter++;

  if (inactivity_counter >= 30000) {
    PORTH &= ~MAIN_SCREEN_BL;  // on eteint
    GC9A01_fillScreen_eyes(GC9A01A_COLOR_GREEN);
    inac = 1;
  }
  return;
}

// ping if activity
void power_save_activity(void) {
  inactivity_counter = 0;

  if (inac == 1)
  {
    PORTH |= MAIN_SCREEN_BL;
    eyes_action(); // pose un pb je crois, c;est pas toujours bine responsive, a tester
    inac = 0;
  }
}
