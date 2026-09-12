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




void GC9A01_draw_square(uint16_t pixels, uint16_t color) {
  GC9A01_pushColor_eyes(color, pixels);
}

void GC9A01_eye_Lids_up(void) {
    GC9A01_cmd_eyes(0x36);
    GC9A01_data_eyes(0xC8); 
          GC9A01_cmd_eyes(0x2C);     // force Memory Write before pixel push

}


void GC9A01_eye_Lids_down(void) {
    GC9A01_cmd_eyes(0x36);
    GC9A01_data_eyes(0x08);
          GC9A01_cmd_eyes(0x2C);     // force Memory Write before pixel push
 
}



void GC9A01_blink(int nbr) {


  for( int i = 0; i < nbr; i++) {

      CS_LEFT_EYE_LOW();
      CS_RIGHT_EYE_LOW();
      GC9A01_eye_Lids_down();
      GC9A01_setAddrWindow_eyes(0, 0, 239, 219);
      GC9A01_draw_square(52800, GC9A01A_COLOR_GREEN);
      
      CS_LEFT_EYE_HIGH();
      CS_RIGHT_EYE_HIGH();

      CS_LEFT_EYE_LOW();
      CS_RIGHT_EYE_LOW();
      GC9A01_eye_Lids_up();
      GC9A01_setAddrWindow_eyes(0, 0, 239, 239);
      GC9A01_drawImg_eyes(Eye_Front, 240, 30, GC9A01A_COLOR_WHITE, GC9A01A_COLOR_BLACK);
      CS_LEFT_EYE_HIGH();
      CS_RIGHT_EYE_HIGH();
    } 
}









int main(void) {
  init();

  GC9A01_fillScreen_eyes(GC9A01A_COLOR_GREEN);
  _delay_ms(1000);
  // GC9A01_fillScreen(GC9A01A_COLOR_BLUE, RIGHT_EYE);
  // _delay_ms(1000);
  // GC9A01_fillScreen(GC9A01A_COLOR_GREEN, LEFT_EYE);

 
  GC9A01_blink(4);
  //ili9488_fill_screen(0xF800);
  while (1) {
    ;

    // testPINS();
  }
}






