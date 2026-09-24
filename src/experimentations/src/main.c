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
#include "game.h"
#include "language.h"

int main(void)
{
  init();
  language_update();

  GC9A01_fillScreen_eyes(GC9A01A_COLOR_GREEN);
  GC9A01_fillScreen(GC9A01A_COLOR_PINK, RIGHT_EYE);
  GC9A01_fillScreen(GC9A01A_COLOR_PINK, LEFT_EYE);

  show_menu();// display the main menu
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