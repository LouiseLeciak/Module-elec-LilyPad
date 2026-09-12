#include <util/delay.h>

#include "GC9A01.h"
#include "main_screen.h"
#include "pinout.h"
#include "spi.h"
#include "state_machine.h"
#include "uart.h"

// void testPINS(void);
// void GC9A01_fillScreen(uint16_t color, uint8_t screen);




void draw_pixel(uint16_t color) {
    spi_txrx(color >> 8);
    spi_txrx(color & 0xFF);
}



void drawEye_img(const uint8_t *file, uint8_t maxNbrLines, uint8_t hSizeBytes, uint16_t fg, uint16_t bg) {
    uint16_t newfg;
    DC_HIGH();
    CS_LEFT_EYE_LOW();
    CS_RIGHT_EYE_LOW();
    for (int line = 0; line < maxNbrLines; line++) {
        for (int byte = 0; byte < hSizeBytes; byte++) {
            uint8_t b = pgm_read_byte(&file[((maxNbrLines - 1) - line) * hSizeBytes + byte]); // read rows bottom-up
            for (int bit = 0; bit < 8; bit++) {
                uint8_t pixel = (b >> bit) & 1;
                if (b == 0x00)
                    newfg = fg;
                else
                    newfg = GC9A01A_COLOR_YELLOW;
                draw_pixel(pixel ? bg : newfg);
            }
        }
    }
    CS_LEFT_EYE_HIGH();
    CS_RIGHT_EYE_HIGH();
}







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

 
  drawEye_img(Eye_look_Right, 240, 30, GC9A01A_COLOR_WHITE, GC9A01A_COLOR_BLACK);
 


  GC9A01_fillScreen(GC9A01A_COLOR_GREEN, LEFT_EYE);
  ili9488_fill_screen(0xF800);
  while (1) {
    ;

    // testPINS();
  }
}






