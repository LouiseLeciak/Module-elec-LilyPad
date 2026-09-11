<<<<<<< HEAD
#include "GC9A01.h"
#include "ili9488.h"
#include "pinout.h"
#include "spi.h"
#include "state_machine.h"
#include "dev_tools.h"
#include <util/delay.h>

// void testPINS(void);
void GC9A01_fillScreen(uint16_t color, uint8_t screen);


t_state current_state = INIT;



void sd_init() { DDRH |= (SD_CS); }

void eyes_init() { 

  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST); 
  GC9A01_init(LEFT_EYE);
  GC9A01_init(RIGHT_EYE);
=======
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#include "pins.h"
#include "uart.h"

#define CS_DDR DDRH
#define CS_PORT PORTH
#define CS_PIN PH0

#define DC_DDR DDRH
#define DC_PORT PORTH
#define DC_PIN PH6

#define RST_DDR DDRH
#define RST_PORT PORTH
#define RST_PIN PH1

static inline void cs_low(void) { CS_PORT &= ~(1 << CS_PIN); }
static inline void cs_high(void) { CS_PORT |= (1 << CS_PIN); }
static inline void dc_cmd(void) { DC_PORT &= ~(1 << DC_PIN); }
static inline void dc_data(void) { DC_PORT |= (1 << DC_PIN); }

typedef struct {
  uint8_t cmd;
  uint8_t n_args;
  uint8_t args[16];
  uint16_t delay_ms; // délai après la commande, 0 si aucun
} lcd_init_cmd_t;

void spi_init(void) {
  DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB0); // SCK, MOSI, CS as output
  DDRB &= ~(1 << PB3);                          // MISO as input
  CS_DDR |= (1 << CS_PIN);
  DC_DDR |= (1 << DC_PIN);
  RST_DDR |= (1 << RST_PIN);

  PORTB |= (1 << PB3);

  // Master mode, mode 0, fosc/16 to start conservative
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
  cs_high();
}

uint8_t spi_txrx(uint8_t data) {
  SPDR = data;
  while (!(SPSR & (1 << SPIF)))
    ;
  return SPDR;
}

void lcd_reset(void) {
  RST_PORT |= (1 << RST_PIN);
  _delay_ms(5);
  RST_PORT &= ~(1 << RST_PIN);
  _delay_ms(20);
  RST_PORT |= (1 << RST_PIN);
  _delay_ms(150); // wait out internal reset per most datasheets
}

// Send a command byte, then read back N response bytes (with the
// mandatory dummy clock most of these controllers require on the
// first byte after the command).
void lcd_read_reg(uint8_t cmd, uint8_t *buf, uint8_t n, uint8_t has_dummy) {
  cs_low();
  dc_cmd();
  spi_txrx(cmd);
  dc_data();
  if (has_dummy)
    spi_txrx(0x00); // dummy clock, needed by RDID1/2/3 etc.
  for (uint8_t i = 0; i < n; i++) {
    buf[i] = spi_txrx(0x00);
  }
  cs_high();
}

void lcd_run_init_sequence(const lcd_init_cmd_t *seq, uint8_t n_cmds) {
  for (uint8_t i = 0; i < n_cmds; i++) {
    cs_low();
    dc_cmd();
    spi_txrx(seq[i].cmd);
    if (seq[i].n_args > 0) {
      dc_data();
      for (uint8_t j = 0; j < seq[i].n_args; j++) {
        spi_txrx(seq[i].args[j]);
      }
    }
    cs_high();
    if (seq[i].delay_ms)
      for (uint16_t k = 0; k < seq[i].delay_ms; k++)
        _delay_ms(1);
  }
}

void lcd_fill_screen(uint16_t color565) {
  // Convertit le RGB565 en RGB666 (18-bit), format attendu par
  // l'ILI9486 sur son interface SPI : chaque composante sur 6 bits
  // utiles, alignée dans les bits hauts d'un octet.
  uint8_t r = ((color565 >> 11) & 0x1F) << 3; // 5 bits -> 8 bits (bits hauts)
  uint8_t g = ((color565 >> 5) & 0x3F) << 2;  // 6 bits -> 8 bits (bits hauts)
  uint8_t b = (color565 & 0x1F) << 3;         // 5 bits -> 8 bits (bits hauts)

  // Column Address Set (CASET, 0x2A)
  cs_low();
  dc_cmd();
  spi_txrx(0x2A);
  dc_data();
  spi_txrx(0x00);
  spi_txrx(0x00);
  spi_txrx(0x01);
  spi_txrx(0xDF);
  cs_high();

  // Page/Row Address Set (PASET, 0x2B)
  cs_low();
  dc_cmd();
  spi_txrx(0x2B);
  dc_data();
  spi_txrx(0x00);
  spi_txrx(0x00);
  spi_txrx(0x01);
  spi_txrx(0x3F);
  cs_high();

  // Memory Write (RAMWR, 0x2C) — 3 octets par pixel maintenant
  cs_low();
  dc_cmd();
  spi_txrx(0x2C);
  dc_data();
  uint32_t n_pixels = 480UL * 320UL;
  for (uint32_t i = 0; i < n_pixels; i++) {
    spi_txrx(r);
    spi_txrx(g);
    spi_txrx(b);
  }
  cs_high();
}

static const lcd_init_cmd_t ili9486_init[] = {
    {0x01, 0, {0}, 120},  // Software reset + délai
    {0x11, 0, {0}, 120},  // Sleep out
    {0x3A, 1, {0x66}, 0}, // COLMOD: 16-bit/pixel (RGB565)
    {0x36, 1, {0x48}, 0}, // MADCTL: orientation + BGR
    {0xC0, 2, {0x0e, 0x0e}, 0},
    {0xC1, 2, {0x41, 0x00}, 0},
    {0xC2, 1, {0x55}, 0},
    {0xC5, 4, {0x00, 0x00, 0x00, 0x00}, 0},
    {0xE0,
     15,
     {0x0F, 0x1F, 0x1c, 0x0c, 0x0F, 0x08, 0x48, 0x98, 0x37, 0x0A, 0x13, 0x04,
      0x11, 0x0D, 0x00},
     0},
    {0xE1,
     15,
     {0x0F, 0x32, 0X2E, 0X0B, 0x0D, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10, 0x03,
      0x24, 0X20, 0x00},
     0},
    {0x20, 0, {0}, 0},
    {0x36, 1, {0x48}, 0},
    {0x29, 0, {0}, 20}, // Display ON
};

#define ILI9486_N (sizeof(ili9486_init) / sizeof(ili9486_init[0]))

static const lcd_init_cmd_t st7796_init[] = {
    {0x01, 0, {0}, 120},
    {0x11, 0, {0}, 120},
    {0xF0, 1, {0xC3}, 0},
    {0xF0, 1, {0x96}, 0},
    {0x36, 1, {0x48}, 0},
    {0x3A, 1, {0x55}, 0},
    {0xB4, 1, {0x01}, 0},
    {0xB6, 3, {0x80, 0x02, 0x3B}, 0},
    {0xE8, 8, {0x40, 0x8A, 0x00, 0x00, 0x29, 0x19, 0xA5, 0x33}, 0},
    {0xC1, 1, {0x06}, 0},
    {0xC2, 1, {0xA7}, 0},
    {0xC5, 1, {0x18}, 120},
    {0xE0,
     14,
     {0xF0, 0x09, 0x0B, 0x06, 0x04, 0x15, 0x2f, 0x54, 0x42, 0x3C, 0x17, 0x14,
      0x18, 0x1B},
     120},
    {0xE1,
     14,
     {0xE0, 0x09, 0x0B, 0x06, 0x04, 0x03, 0x2B, 0x43, 0x42, 0x3B, 0x16, 0x14,
      0x17, 0x1B},
     120},
    {0xF0, 1, {0x3C}, 0},
    {0xF0, 1, {0x69}, 120},
    {0x29, 0, {0}, 0}};

#define ST7796_N (sizeof(st7796_init) / sizeof(st7796_init[0]))

typedef enum {
  DRV_ILI9486,
  DRV_ST7796,
  // DRV_ILI9488,
  DRV_COUNT
} driver_t;

void test_driver(driver_t d) {
  lcd_reset();
  switch (d) {
  case DRV_ILI9486:
    lcd_run_init_sequence(ili9486_init, ILI9486_N);
    break;
  case DRV_ST7796:
    lcd_run_init_sequence(st7796_init, ST7796_N);
    break;
  // case DRV_ILI9488:
  //   lcd_run_init_sequence(ili9488_init, ILI9488_N);
  //   break;
  default:
    return;
  }
  lcd_fill_screen(0xF800); // rouge plein écran, par ex. en RGB565
>>>>>>> 16db5fa (feat(screens): Makes TFT01 screen work.)
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
    screens_init();
    // spi_master_init
    // sd_init();
    // keyboard_init();
}



int main(void) {
<<<<<<< HEAD
  init();
=======
  DDRH |= MAIN_SCREEN_BL;
  PORTH |= MAIN_SCREEN_BL;

  uart_init(MYUBRR);
  spi_init();
  lcd_reset();
  lcd_run_init_sequence(ili9486_init, ILI9486_N);
  lcd_fill_screen(0xF800); // rouge plein écran, par ex. en RGB565
>>>>>>> 16db5fa (feat(screens): Makes TFT01 screen work.)

  GC9A01_fillScreen(GC9A01A_COLOR_BLUE, RIGHT_EYE);
  GC9A01_fillScreen(GC9A01A_COLOR_GREEN, LEFT_EYE);
  while (1) {
<<<<<<< HEAD
    ;
  
   // testPINS();
=======
    //   char c = uart_rx();
    //   if (c >= '1' && c <= '4') {
    //     uart_tx(c);
    //     test_driver((driver_t)(c - '1'));
    //   }
>>>>>>> 16db5fa (feat(screens): Makes TFT01 screen work.)
  }
}
