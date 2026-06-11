// ili9341 - spi_lib.h
// Draws "s" using a 5x7 bitmap font over hardware SPI
//
// Pin Map: ON ARDUINO MEGA.
// _____________________________________________________ 1 - SCK  -> D50 (PB3).
// |. BREADBOARD. |   Position of Wires to MCU  2560      | 2 - MOSI -> D51
// (PB2)          |              |    1    2    3    4     5             | 3 -
// DC   ->  D12 (PB6)         |     LCD      |  -[R1]-[R1]-[R1]-[R1]-[R1]--
// center   | 4 - RST  ->  D13 (PB7)         |              |
// -[R2]-[R2]-[R2]-[R2]-[R2]--          | 5 - CS   -> D11 (PB5)          | |
// GND                        |
//                                 ------------------------------------------------------
// Power pins:
//   VCC  -> 3.3V -> TOP LINE + BREAD BOARD
//   VCC  -> 5V -> BOTTOM LINE + BREAD BOARD
//   GND  -> GND

#include <avr/io.h>
#include <util/delay.h>

#include "ili9341.h"
#include "small_font.h"
#include "spi_lib.h"
#include "tools.h"
#include "uart_lib.h"

// ─── Pin macros
// ───────────────────────────────────────────────────────────────

#define CS_LOW() PORTB &= ~(1 << PB4)
#define CS_HIGH() PORTB |= (1 << PB4)
#define DC_LOW() PORTB &= ~(1 << PB5) // command mode
#define DC_HIGH() PORTB |= (1 << PB5) // data mode
#define RST_LOW() PORTB &= ~(1 << PB6)
#define RST_HIGH() PORTB |= (1 << PB6)

// ─── Colors (RGB565)
// ─────────────────────────────────────────────────────────

#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_BLUE 0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_RED 0x001F
#define COLOR_YELLOW 0xFFE0

// --- FONT SIZE
// ---------------------------------------------------------------

#define SMALL 1
// --- FONT LCD (ORDERED) SIZE 320 x 480 - 4 TFT ILI9341

// --- FONT LCD SIZE 240 x 320 - 3,2 TFT ILI9341

#define SMALL_MAX_CHAR_WIDTH 40
#define MAX_PIXEL_WIDTH 240
#define MAX_PIXEL_HIGH 320

volatile uint8_t SCALE = 1;
volatile char hex[4];              // global for function toHex()
volatile char nbr_in_a_string[33]; // global variable for function nbr_to_str()

// ─── ILI9341 low-level
// ───────────────────────────────────────────────────────

// ─── Drawing primitives
// ──────────────────────────────────────────────────────

uint8_t
ft_main_strlen (const char *str)
{
  uint8_t len = 0;
  while (str && *str)
    {
      len++;
      str++;
    }
  return (len);
}

void
draw_pixel (uint16_t x, uint16_t y, uint16_t color)
{
  set_window (x, y, (MAX_PIXEL_WIDTH)-1, MAX_PIXEL_HIGH - 1);
  DC_HIGH ();
  CS_LOW ();
  spi_master_transmit (color >> 8);
  spi_master_transmit (color & 0xFF);
  CS_HIGH ();
}

// ─── 10x14 Font
// ──────────────────────────────────────────────────────────────── Each char =
// 5 bytes (one per column) x 4 CH0 | CH1
// ---------
// CH3 | CH4
static const uint8_t font10x14[95][4][5] = {
    // [0] = {
    //     { 0x40, 0x60, 0x30, 0x30, 0x30 }, // CH0
    //     { 0x30, 0x30, 0x30, 0x30, 0x30 }, // CH1
    //     { 0x61, 0x63, 0x66, 0x66, 0x66 }, // CH2
    //     { 0x66, 0x66, 0x66, 0x3E, 0x18 },  // CH3
    // } // 115 's'

    ['s' - 32] =
        {
            {0x00, 0x30, 0x30, 0x30, 0x30}, // CH1  (was CH0 reversed)
            {0x30, 0x30, 0x30, 0x60, 0x40}, // CH0  (was CH1 reversed)
            {0x18, 0x3E, 0x66, 0x66, 0x66}, // CH3  (was CH2 reversed)
            {0x66, 0x66, 0x66, 0x63, 0x01}, // CH2  (was CH3 reversed)
        } // 115 's'

};

void
draw_color_screen (uint16_t color)
{
  set_window (0, 0, MAX_PIXEL_WIDTH - 1, MAX_PIXEL_HIGH - 1);

  DC_HIGH ();
  CS_LOW ();

  uint8_t hi = color >> 8;
  uint8_t lo = color & 0xFF;

  for (uint32_t i = 0; i < (uint32_t)MAX_PIXEL_WIDTH * MAX_PIXEL_HIGH; i++)
    {
      spi_master_transmit (hi);
      spi_master_transmit (lo);
    }

  CS_HIGH ();
}

// ─── Text rendering
// ──────────────────────────────────────────────────────────

void
draw_char (uint16_t x, uint16_t y, char c, uint16_t fg, uint16_t bg)
{
  if (c < 32 || c > 126)
    c = '?';
  // const uint8_t *glyph = font5x7[(uint8_t)c - 32];

  const uint8_t (*glyph)[5] = font10x14[(uint8_t)c - 32];

  uint16_t xTemp = x;
  uint16_t yTemp = y;

  for (int8_t ch = 0; ch < 4; ch++)
    {

      if (ch == 0)
        {
          x = xTemp, y = yTemp;
          // continue;
        }
      if (ch == 1)
        {
          x += (5 * SCALE), y = yTemp;
          // continue;
        }
      if (ch == 2)
        {
          x = xTemp, y = yTemp + (7 * SCALE);
          // continue;
        }
      if (ch == 3)
        {
          x += (5 * SCALE), y = yTemp + (7 * SCALE);
          // continue;
        }
      for (int8_t col = 0; col < 5; col++)
        {
          for (int8_t row = 0; row < 7; row++)
            {
              uint16_t color;
              if (glyph[ch][col] & (1 << row))
                {
                  color = fg;
                }
              else
                {
                  color = bg;
                  continue;
                }
              for (uint8_t i = 0; i < SCALE; i++)
                {
                  for (uint8_t z = 0; z < SCALE; z++)
                    {
                      draw_pixel ((x + (col * SCALE) + z),
                                  (y + (row * SCALE)) + i, color);
                    }
                }
            }
        }
    }
}

// ─── Text rendering
// ──────────────────────────────────────────────────────────

void
draw_char_small (uint16_t x, uint16_t y, uint8_t c, uint16_t fg, uint16_t bg)
{
  if (c < 32 || c > 126)
    c = '?';

  // const volatile uint8_t *glyph = font5x7[c - 32];
  const volatile uint8_t *glyph = font5x7[(uint8_t)c - 32];

  //    const uint8_t *glyph = font5x7[33];
  uart_printstr ("\n\rtableax Num : ");
  putnbr (c - 32);
  uart_printstr (" pointer: ");
  putnbr ((uint16_t)glyph);
  uart_printstr (" pointer: ");
  uart_tx (c);
  uart_printstr (" ptr address: ");

  uart_print_addr ((void *)(&font5x7[c - 32][0]));
  uart_printstr ("\r\n");

  for (int8_t col = 0; col < 5; col++)
    {
      for (int8_t row = 0; row < 7; row++)
        {
          uint16_t color;

          if (glyph[col] & (1 << row))
            {
              // putnbr(1);
              color = fg;
            }
          else
            {
              color = bg;
              // putnbr(0);

              continue;
            }
          for (int8_t i = 0; i < SCALE; i++)
            {
              for (int8_t z = 0; z < SCALE; z++)
                {
                  draw_pixel ((x + (col * SCALE) + z), (y + (row * SCALE)) + i,
                              color);
                }
            }
        }
      // uart_printstr(" ");
    }
}

void
draw_string (uint16_t x, uint16_t y, const char *str, uint16_t fg, uint16_t bg)
{

  uint8_t len = ft_main_strlen (str);

  while (len > 0)
    {

      if (SCALE >= 2)
        {
          draw_char (x, y, str[len - 1], fg, bg);
          x += 1 + (5 * SCALE) + 5 * SCALE; // 5px glyph + 1px gap
        }
      else
        {

          draw_char_small (x, y, str[len - 1], fg, bg);
          x += (5 * SCALE) + 1; // 5px glyph + 1px gap
        }

      // str++;
      len--;
    }
}

// ─── Arduino entry points
// ────────────────────────────────────────────────────

void
setup (void)
{
  uart_init ();
  spi_master_init ();
  ili9341_init ();
}

int
main (void)
{
  setup ();
  uart_printstr ("Trying to display GREEN\r\n");
  fill_screen (COLOR_GREEN);

  while (1)
    ;

  return (0);
}
