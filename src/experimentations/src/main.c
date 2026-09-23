#include <util/delay.h>

#include "GC9A01.h"
#include "main_screen.h"
#include "pinout.h"
#include "spi.h"
#include "state_machine.h"
#include "eye_imgs.h"
#include "screen_text.h"
#include "keyboard_utils.h"
#include "globals.h"
#include "init.h"
#include "keypad.h"
#include "menu.h"
#include "i2c_rotary.h"
#include "power_save.h"

int main(void)
{
  init();

  GC9A01_fillScreen_eyes(GC9A01A_COLOR_GREEN);
  GC9A01_fillScreen(GC9A01A_COLOR_PINK, RIGHT_EYE);

  show_menu();

  while (1)
  {
    rotary_update();
    rotary_button_update();
    power_save_update();
    // eyes_action();
    
    int key = keypad_read();
    if (key >= 0)
      power_save_activity();
    if (app_state == TRADUCTION && word_state == INPUT)
    {
      traduction();
    }

    else if (app_state == ALPHABET)
    {
      alphabet();
    }

    else if (app_state == JEU)
    {
      game();
    }

  }
  return 0;
}