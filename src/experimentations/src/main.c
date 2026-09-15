#include <util/delay.h>

#include "GC9A01.h"
#include "main_screen.h"
#include "pinout.h"
#include "spi.h"
#include "state_machine.h"
#include "uart.h"
#include "eye_imgs.h"
#include "screen_text.h"

// void testPINS(void);
// void GC9A01_fillScreen(uint16_t color, uint8_t screen);

t_state current_state = INIT;

void sd_init() { DDRH |= (SD_CS); }

void eyes_init()
{

  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
  GC9A01_init(LEFT_EYE);
  GC9A01_init(RIGHT_EYE);
}

void screens_init()
{
  DDRH |= (SCREENS_DC);
  main_screen_init();
  eyes_init();
}

void keyboard_init(void)
{
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

void init(void)
{

  spi_master_init();
  uart_init(MYUBRR);

  screens_init();
  // sd_init();
  // keyboard_init();
}


///////////////////////////////////////////////////////////////////////
////////////////////////// ROTARY ENCODER//////////////////////////////
///////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////
///////////////////////////// KEYBOARD ////////////////////////////////
///////////////////////////////////////////////////////////////////////


int main(void)
{
  init();

  // GC9A01_fillScreen_eyes(GC9A01A_COLOR_GREEN);
  // GC9A01_fillScreen(GC9A01A_COLOR_BLUE, RIGHT_EYE);
  // GC9A01_fillScreen(GC9A01A_COLOR_PINK, LEFT_EYE);

  ili9488_fill_screen(GC9A01A_COLOR_PINK);

  draw_pixel(250, 320, GC9A01A_COLOR_GREEN);

  draw_string(10, 20, "Entre un mot !", GC9A01A_COLOR_PURPLE, GC9A01A_COLOR_PINK, 4, 2);
  // draw_string(30, 160, "Hello World !", GC9A01A_COLOR_PURPLE, GC9A01A_COLOR_PINK, 2, 2); // MAX H CHARS
  // draw_string(30, 220, "Hello World !", GC9A01A_COLOR_PURPLE, GC9A01A_COLOR_PINK, 3, 3); // MAX H CHARS
  // draw_string(30, 260, "Hello World !", GC9A01A_COLOR_PURPLE, GC9A01A_COLOR_PINK, 4, 4); // MAX H CHARS

  while (1)
  {
    // GC9A01_blink(Eye_look_Right, 1);
    // GC9A01_blink(Eye_Front, 1);
    // for (uint8_t i = 32; i < 127; i++)
    // {
    //   draw_char_small(120, 180, i, GC9A01A_COLOR_PURPLE, GC9A01A_COLOR_PINK, 5);
    //   // _delay_ms(300);
    // }

    draw_char_small(10, 80, 124, GC9A01A_COLOR_PURPLE, GC9A01A_COLOR_PINK, 4);
    _delay_ms(500);
    draw_char_small(10, 80, ' ', GC9A01A_COLOR_PURPLE, GC9A01A_COLOR_PINK, 4);
    _delay_ms(500);


    // testPINS();
  }
}
