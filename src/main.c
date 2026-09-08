#include "GC9A01.h"
#include "pinout.h"
#include "spi.h"
#include "state_machine.h"
#include <util/delay.h>

void testPINS(void);


t_state current_state = INIT;

void main_screen_init() {
  DDRH |= (MAIN_SCREEN_CS | MAIN_SCREEN_BL | MAIN_SCREEN_RST);

  PORTH |= (MAIN_SCREEN_BL);
}

void sd_init() { DDRH |= (SD_CS); }

void eyes_init() { DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST); }

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
  // TODO: Init I2C
  //  DDRD |= (SCL | SDA); // Not sure if necessary after i2c_init


  // DDRA |= (KB_C1 | KB_C2 | KB_C3 | KB_C4 | KB_C5);
  // DDRG |= (KB_C6);
  // DDRC |= (SDL_SW1 | SDL_SW2 | SDL_SW3 | KB_R4);
  // DDRD |= (SCL | SDA);
  // DDRH |= (MAIN_SCREEN_CS | MAIN_SCREEN_RST | MAIN_SCREEN_BL | SCREENS_DC | SD_CS);
  // DDRJ |= (KB_C7 | KB_C8 | KB_C9 | KB_C10 | KB_R1 | KB_R2 | KB_R3 );
  // DDRB |= (MISO);

  DDRB |= (CS | SCK | MOSI);
  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);



  spi_master_init();
  // spi_master_init

  screens_init();
  // sd_init();
  // keyboard_init();
}


#define GC9A01_WIDTH   240
#define GC9A01_HEIGHT  240

// RGB565 pure blue = 0x001F
#define COLOR_BLUE_HI  0x00
#define COLOR_BLUE_LO  0x1F

void GC9A01_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t screen) {
    // Column Address Set
    GC9A01_cmd(0x2A, screen);
    GC9A01_data(0x00, screen);
    GC9A01_data(x0, screen);
    GC9A01_data(0x00, screen);
    GC9A01_data(x1, screen);

    // Row Address Set
    GC9A01_cmd(0x2B, screen);
    GC9A01_data(0x00, screen);
    GC9A01_data(y0, screen);
    GC9A01_data(0x00, screen);
    GC9A01_data(y1, screen);

    // Memory Write (start RAM write)
    GC9A01_cmd(0x2C, screen);
}

// Fast raw pixel push: keeps CS low and DC high for the whole burst
static void GC9A01_pushColor(uint16_t color, uint32_t count, uint8_t screen) {
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    DC_HIGH();

    if (screen == LEFT_EYE) {
        CS_LEFT_EYE_LOW();
        while (count--) {
            spi_master_transmit(hi);
            spi_master_transmit(lo);
        }
        CS_LEFT_EYE_HIGH();
    } else if (screen == RIGHT_EYE) {
        CS_RIGHT_EYE_LOW();
        while (count--) {
            spi_master_transmit(hi);
            spi_master_transmit(lo);
        }
        CS_RIGHT_EYE_HIGH();
    }
}

void GC9A01_fillScreenBlue(uint8_t screen) {
    GC9A01_setAddrWindow(0, 0, GC9A01_WIDTH - 1, GC9A01_HEIGHT - 1, screen);
    GC9A01_pushColor(0x001F, (uint32_t)GC9A01_WIDTH * GC9A01_HEIGHT, screen);
}



int main(void) {
  init();

  GC9A01_init(LEFT_EYE);

    while (1) {
   GC9A01_fillScreenBlue(LEFT_EYE);
  }
}







void testPINS(void) {

  //  KEYBOARD
  // PORTA |= (KB_C1);
  // PORTA |= (KB_C2);
  // PORTA |= (KB_C3);
  // PORTA |= (KB_C4);
  // PORTA |= (KB_C5);
  // PORTG |= (KB_C6);
  // PORTJ |= (KB_C7);
  // PORTJ |= (KB_C8);
  // PORTJ |= (KB_C9);
  // PORTJ |= (KB_C10);
  // PORTJ |= (KB_R1);
  // PORTJ |= (KB_R2);
  // PORTJ |= (KB_R3);
  // PORTC |= (KB_R4);
  // PORTD |= (SCL);
  // PORTD |= (SDA);
  // PORTC |= (SDL_SW1);
  // PORTC |= (SDL_SW2);
  // PORTC |= (SDL_SW3);
  // PORTB |= (SCK);

  // SD_READER
  // PORTH &= ~(SD_CS);
  // PORTH |= (SD_CS);

  // MAIN SCREEN
  // PORTH |= (MAIN_SCREEN_RST); 
  // PORTH |= (MAIN_SCREEN_CS); 
  // PORTB |= (SCK | MISO | MOSI);
  // PORTH |= (SCREENS_DC);
  
  // EYE_RIGHT_SCREEN
  // PORTE |= (RIGHT_EYE_CS);
  // PORTE |= (EYES_RST);
  // PORTB |= (SCK);
  // PORTB |= (MOSI);
  // PORTH |= (SCREENS_DC);
  
  // EYE_LEFT_SCREEN
  // PORTE |= (LEFT_EYE_CS);
  // PORTE |= (EYES_RST);
  // PORTB |= (SCK);
  // PORTB |= (MOSI);
  // PORTH |= (SCREENS_DC);

}