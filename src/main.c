#include <util/delay.h>

#include "GC9A01.h"
#include "main_screen.h"
#include "pinout.h"
#include "spi.h"
#include "state_machine.h"
#include "uart.h"
#include "eye_imgs.h"

// void testPINS(void);
// void GC9A01_fillScreen(uint16_t color, uint8_t screen);




t_state current_state = INIT;

void sd_init() { DDRH |= (SD_CS); }

void eyes_init() {

  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
  GC9A01_init(LEFT_EYE);
  GC9A01_init(RIGHT_EYE);
}

void screens_init() {
  DDRH |= (SCREENS_DC);
  main_screen_init();
  eyes_init();
}

void keyboard_init(void) {
  DDRA |= (KB_C1 | KB_C2 | KB_C3 | KB_C4 | KB_C5);
  DDRC |= (KB_R4);
  DDRG |= (KB_C6);
  DDRJ |= (KB_R1 | KB_R2 | KB_R3 | KB_C7 | KB_C8 | KB_C9 | KB_C10);

  PORTA &= ~(KB_C1 | KB_C2 | KB_C3 | KB_C4 | KB_C5);
  PORTC &= ~(KB_R4);
  PORTG &= ~(KB_C6);
  PORTJ &= ~(KB_R1 | KB_R2 | KB_R3 | KB_C7 | KB_C8 | KB_C9 | KB_C10);
}

void rotary_encoder_init(void) { DDRC |= (SDL_SW1 | SDL_SW2 | SDL_SW3); }

void init(void) {

  spi_master_init();
  uart_init(MYUBRR);

  screens_init();
  // sd_init();
  // keyboard_init();
}

int main(void) {
  init();

  GC9A01_fillScreen_eyes(GC9A01A_COLOR_RED);
  _delay_ms(1000);
  GC9A01_fillScreen(GC9A01A_COLOR_BLUE, RIGHT_EYE);
  _delay_ms(1000);

 
  GC9A01_drawImg_eyes(Eye_look_Right, 240, 30, GC9A01A_COLOR_WHITE, GC9A01A_COLOR_BLACK);
 


  GC9A01_fillScreen(GC9A01A_COLOR_GREEN, LEFT_EYE);
  ili9488_fill_screen(0xF800);
  while (1) {
    ;

    // testPINS();
  }
}






