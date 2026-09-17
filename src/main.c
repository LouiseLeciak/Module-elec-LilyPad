#include <stdint.h>
#include <util/delay.h>

#include "GC9A01.h"
#include "i2c.h"
#include "keyboard.h"
#include "main_screen.h"
#include "pinout.h"
#include "spi.h"
#include "state_machine.h"
#include "uart.h"

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
  uart_printstr("Starting keyboard initialisation...");

  // Initialises rows as output
  DDRJ |= (KB_R1 | KB_R2 | KB_R3);
  DDRC |= (KB_R4);
  PORTJ |= (KB_R1 | KB_R2 | KB_R3);
  PORTC |= (KB_R4);

  // Initialises columns as input
  DDRA &= ~(KB_C1 | KB_C2 | KB_C3 | KB_C4 | KB_C5);
  DDRG &= ~(KB_C6);
  DDRJ &= ~(KB_C7 | KB_C8 | KB_C9 | KB_C10);
  PORTA |= (KB_C1 | KB_C2 | KB_C3 | KB_C4 | KB_C5);
  PORTG |= (KB_C6);
  PORTJ |= (KB_C7 | KB_C8 | KB_C9 | KB_C10);

  uart_printstr("OK!\r\n");

  // TODO: Translate this to our proto, this was made for the old pinout

  // on check comment on est pour savoir comment changer
  if (PINC & (1 << ROTARY_CLK))
    rotaryclk_prev = 1;
  else
    rotaryclk_prev = 0;
}

void rotary_encoder_init(void) { DDRC |= (SDL_SW1 | SDL_SW2 | SDL_SW3); }

void init(void) {

  uart_init(MYUBRR);
  spi_master_init();
  i2c_init();

  // screens_init();
  // sd_init();
  keyboard_init();
}

int main(void) {
  int key = 0;
  uint8_t row;
  uint8_t col;

  init();

  // GC9A01_fillScreen_eyes(GC9A01A_COLOR_GREEN);
  // _delay_ms(1000);
  // GC9A01_fillScreen(GC9A01A_COLOR_BLUE, RIGHT_EYE);
  // _delay_ms(1000);
  // GC9A01_fillScreen(GC9A01A_COLOR_PINK, LEFT_EYE);

  // ili9488_fill_screen(0xF800);
  while (1) {
    // GC9A01_blink(Eye_look_Right, 1);
    // GC9A01_blink(Eye_Front, 1);

    key = keypad_read();

    if (key >= 0) {
      // ici je recuper la dizaine et l'unite
      // comme je renvois en version aditione dans keypad read
      // donc si on reprend l'exemple de 23 comme a la ligne 151
      // on recupere bien row = 2 et col = 3
      row = key / COLS_NB;
      col = key % COLS_NB;

      //! TO ADD comportement dans le cas ou on appuie sur plusieurs touches
      //! en meme temps
      if (keymap[row][col] == '\n')
        uart_printstr("\n\r");
      else
        uart_tx(keymap[row][col]);

      // debounce
      _delay_ms(20);

      // le temps que ca relache le bouton
      // mais a voir si on autorise ou pas le fait
      // de pouvoir rester appuyer pour que ca fasse AAAAAA
      while (keypad_read() >= 0)
        ;
    }

    // testPINS();
  }
}
