#include "system/power_save.h"

#include <util/delay.h>

#include "display/GC9A01.h"
#include "display/ili9488.h"
#include "pinout.h"
#include "system/uart.h"

// 30000 ~= 23s
#define INACTIVITY_THRESHOLD 30000

uint32_t inactivity_counter = 0;
uint8_t inac = 0;

// timer to know since when nothing is happening
void power_save_update(void)
{
  if (inactivity_counter < INACTIVITY_THRESHOLD)
    inactivity_counter++;

  if (inactivity_counter >= INACTIVITY_THRESHOLD)
  {
    if (inac == 0)
#ifdef DEBUG
      uart_printstr("[PWR] Screen sleeping (inactivity timeout)\r\n");
#endif /* ifdef DEBUG */
    ili9488_sleep_in();
    PORTE &= ~MAIN_SCREEN_BL;  // on eteint
    CS_LEFT_EYE_LOW();
    CS_RIGHT_EYE_LOW();
    GC9A01_eye_Lids_down();
    GC9A01_setAddrWindow_eyes(0, 0, 239, 219);
    GC9A01_draw_eye_Lids(FULLSCREEN, GC9A01A_COLOR_GREEN);
    CS_LEFT_EYE_HIGH();
    CS_RIGHT_EYE_HIGH();
    inac = 1;
  }
  return;
}

// ping if activity
void power_save_activity(void)
{
  inactivity_counter = 0;

  if (inac == 1)
  {
#ifdef DEBUG
    uart_printstr("[PWR] Screen waking up\r\n");
#endif /* ifdef DEBUG */
    ili9488_sleep_out();
    _delay_ms(50);
    PORTE |= MAIN_SCREEN_BL;
    eyes_action();  // pose un pb je crois, c;est pas toujours bine
    //   responsive, a tester
    inac = 0;
  }
}
