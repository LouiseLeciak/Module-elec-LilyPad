#include <util/delay.h>

#include "app/alphabet.h"
#include "app/game.h"
#include "app/language.h"
#include "app/menu.h"
#include "display/GC9A01.h"
#include "input/i2c_rotary.h"
#include "input/keypad.h"
#include "input/rotary.h"
#include "system/globals.h"
#include "system/init.h"
#include "system/power_save.h"
#include "utils/keyboard_utils.h"

int main(void)
{
  init();
  language_update();

  GC9A01_fillScreen_eyes(GC9A01A_COLOR_GREEN);
  GC9A01_fillScreen(GC9A01A_COLOR_PINK, RIGHT_EYE);
  GC9A01_fillScreen(GC9A01A_COLOR_PINK, LEFT_EYE);

  show_menu();  // display the main menu
  // eyes_action();

  while (1)
  {
    language_update();
    random_update();
    rotary_update();
    rotary_button_update();
    power_save_update();

    // listening to the keyboard for the power save
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
