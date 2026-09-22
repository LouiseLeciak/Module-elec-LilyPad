
#ifndef KEYBOARD_UTILS_H
#define KEYBOARD_UTILS_H

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdbool.h>


/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////   DEFINES   //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

#define WRITE 0
#define READ 1

#define MCP23017_ADDR 0x20 // A0/A1/A2 = GND
#define MCP_IODIRA 0x00 // pour config les pin GPA0-7 
#define MCP_GPIOA 0x12 // lit ou ecrit l'etat actuel des pin GPA
#define MCP_GPPUA 0x0C // pour les resistances de pull up

// app
#define WORD_MAX_LEN 20

// keyboard
#define ROWS_NB 4
#define COLS_NB 10
#define MENU_SIZE 3

// rotary encoder
#define ROTARY_CLK 6
#define ROTARY_SW 5
#define ROTARY_DT 7

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////   ENUMS   ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
  INPUT,
  VALIDATED
} word_state_t;

typedef enum
{
  MENU,
  TRADUCTION,
  ALPHABET,
  JEU
} app_state_t;

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////   FONCTIONS   /////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// uart -> to be deprecated
void uart_init();
void uart_tx(char c);
char uart_rx(void);
void uart_printstr(const char *str);
void uart_printhex(uint8_t value);
void uart_printint(int val);

// display
void draw_menu(void);

// i2c - rotary encoder
void i2c_init(void);
void i2c_write(unsigned char data);
uint8_t i2c_start(uint8_t addr);
void i2c_stop(void);
void print_hex_value(char c);
uint8_t i2c_read_byte(void);
void mcp_init(void);
void mcp_write_register(uint8_t reg, uint8_t value);
uint8_t mcp_read_register(uint8_t reg);
void rotary_init(void);
void rotary_update(void);

// word management
void start_new_word(void);
void validate_word(void);

//DISPLAY
// menu management
void show_menu(void);
void start_translation(void);
void show_alphabet(void);
void show_game(void);
void update_menu_cursor(uint8_t old_choice);

#endif