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

#include "utils.h"

#define ROTARY_CLK 6
#define ROTARY_SW 5
#define ROTARY_DT 7

static uint8_t eye_state = 0;
static uint8_t prev_sw = 1; // etat precedent du bouton


void change_eye(void)
{
    if (eye_state == 0)
    {
        GC9A01_blink(Eye_look_Right, 1);
        eye_state = 1;
    }
    else
    {
        GC9A01_blink(Eye_Front, 1);
        eye_state = 0;
    }
}


void rotary_button_update(void)
{
    uint8_t sw;
    uint8_t gpio = mcp_read_register(MCP_GPIOA);

    sw = (gpio >> ROTARY_SW) & 1;

    if (sw != prev_sw)
    {
        _delay_ms(5);

        gpio = mcp_read_register(MCP_GPIOA);
        sw = (gpio >> ROTARY_SW) & 1;

        if (sw != prev_sw)
        {
            if (sw == 0)
            {
                if (eye_state == 0)
                {
                    GC9A01_blink(Eye_look_Right, 1);
                    eye_state = 1;
                }
                else
                {
                    GC9A01_blink(Eye_Front, 1);
                    eye_state = 0;
                }
            }

            prev_sw = sw;
        }
    }
}

int main(void) {
  init();
  i2c_init();
  mcp_init();

  GC9A01_fillScreen_eyes(GC9A01A_COLOR_GREEN);
  _delay_ms(1000);
  GC9A01_fillScreen(GC9A01A_COLOR_PINK, RIGHT_EYE);
  _delay_ms(1000);
  GC9A01_fillScreen(GC9A01A_COLOR_PINK, LEFT_EYE);

  while (1){
    rotary_button_update();
  }
 
  //ili9488_fill_screen(0xF800);
  // while (1) {
  //   GC9A01_blink(Eye_look_Right, 1);
  //   GC9A01_blink(Eye_Front, 1);
  //   ;

  //   // testPINS();
  // }
}






