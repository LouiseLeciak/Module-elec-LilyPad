#include <util/delay.h>

#include "GC9A01.h"
#include "main_screen.h"
#include "pinout.h"
#include "spi.h"
#include "state_machine.h"
#include "uart.h"
#include "eye_imgs.h"
#include "small_font.h"


// void testPINS(void);
// void GC9A01_fillScreen(uint16_t color, uint8_t screen);



#define CS_MAIN_LOW()  PORTH &= ~(1 << PH0);
#define CS_MAIN_HIGH() PORTH |=(1 << PH0);
#define MAX_PIXEL_WIDTH 240
#define MAX_PIXEL_HIGH 320

#define SCALE 2
#define GAP SCALE

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


uint8_t ft_main_strlen(const char *str) {
     uint8_t len = 0;
    while (str && *str) {
        len++;
        str++;
    }
    return (len);    
}



void ili9341_cmd(uint8_t cmd) {
    DC_LOW();
    CS_MAIN_LOW();
    spi_txrx(cmd);
    CS_MAIN_HIGH();
}

void ili9341_data(uint8_t data) {
    DC_HIGH();
    CS_MAIN_LOW();
    spi_txrx(data);
    CS_MAIN_HIGH();
}


void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    ili9341_cmd(0x2A);
    ili9341_data(x0 >> 8);
    ili9341_data(x0 & 0xFF);
    ili9341_data(x1 >> 8);
    ili9341_data(x1 & 0xFF);
    ili9341_cmd(0x2B);
    ili9341_data(y0 >> 8);
    ili9341_data(y0 & 0xFF);
    ili9341_data(y1 >> 8);
    ili9341_data(y1 & 0xFF);
    ili9341_cmd(0x2C);
}


void draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    
    uint8_t r = ((color >> 11) & 0x1F) << 3; // 5 bits -> 8 bits (bits hauts)
    uint8_t g = ((color >> 5) & 0x3F) << 2;  // 6 bits -> 8 bits (bits hauts)
    uint8_t b = (color & 0x1F) << 3;   
    set_window(x, y, x, y);
    DC_HIGH();
    CS_MAIN_LOW();
    spi_txrx(r);
    spi_txrx(g);
    spi_txrx(b);
    CS_MAIN_HIGH();
}




// ─── Text rendering ──────────────────────────────────────────────────────────
// ─── Text rendering ──────────────────────────────────────────────────────────

void draw_char_small(uint16_t x, uint16_t y, uint8_t c, uint16_t fg, uint16_t bg) {
    if (c < 32 || c > 126) c = '?';

    const uint8_t *char_in_array = font5x7[c - 32];   // pointer to the 5-byte char_in_array

    for (int8_t col = 0; col < 5; col++) {
        uint8_t columnsBits = pgm_read_byte(&char_in_array[col]);   // drop pgm_read_byte if font5x7 isn't in PROGMEM

        for (int8_t row = 0; row < 7; row++) {
            uint16_t color = (columnsBits & (1 << row)) ? fg : bg;

            for (int8_t i = 0; i < SCALE; i++) {
                for (int8_t z = 0; z < SCALE; z++) {
                    // if (!(columnsBits & (1 << row))) continue;   //OPTION add if you want to have transparentbackground skip pixels

                    draw_pixel((x + (col * SCALE) + z), (y + (row * SCALE) + i), color);
                }
            }
        }
    }
}

void draw_string(uint16_t x, uint16_t y, const char *str, uint16_t fg, uint16_t bg) {

    uint8_t len = ft_main_strlen(str);
    uint8_t i = 0;
    while (i < len) {
            draw_char_small(x, y, str[i], fg, bg);
            x += (5 * SCALE) + GAP;     // 5px glyph + gap
        i++;
    }
}




int main(void) {
  init();

  //GC9A01_fillScreen_eyes(GC9A01A_COLOR_GREEN);
  _delay_ms(1000);
  // GC9A01_fillScreen(GC9A01A_COLOR_BLUE, RIGHT_EYE);
  // _delay_ms(1000);
  // GC9A01_fillScreen(GC9A01A_COLOR_PINK, LEFT_EYE);

 
  //ili9488_fill_screen(0xF800);
  ili9488_fill_screen(GC9A01A_COLOR_RED);
  // _delay_ms(500);

  //ili9488_pixel_to_screen(180, 180, GC9A01A_COLOR_WHITE);


  //draw_string("ABC 123", GC9A01A_COLOR_WHITE, GC9A01A_COLOR_BLACK); // MAX H CHARS
  draw_pixel(320/2, 480/2, GC9A01A_COLOR_GREEN );
// draw_char_small(120,120, 38,GC9A01A_COLOR_GREEN, GC9A01A_COLOR_RED );

    draw_string(30, 120,  "Hello World", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_RED); // MAX H CHARS

  while (1) {
    //GC9A01_blink(Eye_look_Right, 1);
    //GC9A01_blink(Eye_Front, 1);

    for (uint8_t i = 32; i < 127; i++) {

        // draw_char_small(120,120, i,GC9A01A_COLOR_GREEN, GC9A01A_COLOR_RED );

        _delay_ms(300);
    }

    ;


    




    // testPINS();
  }
}






