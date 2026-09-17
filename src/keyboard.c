#include "keyboard.h"

#include <util/delay.h>

#include "MCP23017.h"
#include "pinout.h"
#include "uart.h"

static uint8_t menu_index = 0;
static uint8_t prev_sw = 1; // etat precedent du bouton
const char *menu[] = {"OPTION NUMERO 1", "OPTION NUMERO 2", "OPTION NUMERO 3"};

void print_hex_value(char c) {
  char hex[] = "0123456789abcdef";
  char buf[3];

  unsigned char value = (unsigned char)c;

  buf[0] = hex[value / 16];
  buf[1] = hex[value % 16];
  buf[2] = '\0';

  uart_printstr(buf);
}

void draw_menu(void) {
  uart_printstr("\033[2J"); // ANSI pour effacer l'ecran
  uart_printstr("\033[H");  // ANSI pour se mettre a l1 c1
  uart_printstr("========== MENU ==========\r\n");
  uart_printstr("\r\n");
  for (uint8_t i = 0; i < 3; i++) {
    if (i == menu_index)
      uart_printstr("> ");
    else
      uart_printstr("  ");
    uart_printstr(menu[i]);
    uart_printstr("\n\r");
  }
}

// je veux selectionner qu'une seule ligne a la fois
void select_row(uint8_t row) {
  // je desactive toutes les lignes
  PORTJ |= (KB_R1 | KB_R2 | KB_R3);
  PORTC |= (KB_R4);

  // j'active une seule lgine poru "monitorer"
  // j;active celle envoye en parametre
  switch (row) {
  case 0:
    PORTJ &= ~(KB_R1);
    break;

  case 1:
    PORTJ &= ~(KB_R2);
    break;

  case 2:
    PORTJ &= ~(KB_R3);
    break;

  case 3:
    PORTC &= ~(KB_R4);
    break;
  }
}

int read_column(void) {
  // si PINF = 0 alors c'est que c'est presse
  if (!(PINA & KB_C1))
    return 0;
  if (!(PINA & KB_C2))
    return 1;
  if (!(PINA & KB_C3))
    return 2;
  if (!(PINA & KB_C4))
    return 3;
  if (!(PINA & KB_C5))
    return 4;
  if (!(PING & KB_C6))
    return 5;
  if (!(PINJ & KB_C7))
    return 6;
  if (!(PINJ & KB_C8))
    return 7;
  if (!(PINJ & KB_C9))
    return 8;
  if (!(PINJ & KB_C10))
    return 9;

  return -1;
}

int keypad_read(void) {
  int row;
  int col;

  for (row = 0; row < ROWS_NB; row++) {
    select_row(row);

    _delay_us(5);

    col = read_column();

    if (col >= 0)
      return (row * COLS_NB + col);
    // en gros par exemple si c'est r2 c3
    // bah ca return 23, ca m'evite de reeefaire un tableau
    // a return etc
  }

  return -1;
}

// FONCTION POUR LE ROTARY
void rotary_update(void) {
  uint8_t clk; // etat actuel de la clock
  uint8_t dt;  // etat actuel de la pin data

  uint8_t gpio = mcp_read_register(MCP_GPIOA);

  clk = (gpio >> ROTARY_CLK) & 1;

  // si la clk a change
  if (rotaryclk_prev == 1 && clk == 0) {
    dt = (gpio >> ROTARY_DT) & 1;

    // si dt est diff de clk alors on tourne dans un sens
    if (dt != clk) {
      if (menu_index == 0)
        menu_index = 2;
      else
        menu_index--;
      draw_menu();
    }
    // sinon c'est qu'on tourne dans l'autre
    else {
      menu_index++;
      if (menu_index > 2)
        menu_index = 0;
      draw_menu();
    }
  }

  rotaryclk_prev = clk;
}

//? Probablement pas opti comme verification, probablement a retaper
void rotary_button_update(void) {
  uint8_t sw; // etat actuel

  uint8_t gpio = mcp_read_register(MCP_GPIOA);

  sw = (gpio >> ROTARY_SW) & 1;

  // si l'etat du bouton a change
  if (sw != prev_sw) {
    _delay_ms(5);

    gpio = mcp_read_register(MCP_GPIOA);
    sw = (gpio >> ROTARY_SW) & 1;
    // si sw a bien change alors on print des trucs pour le moment en uart
    if (sw != prev_sw) {
      if (sw == 0) {
        switch (menu_index) {
        case 0:
          uart_printstr("You choose option 1\n\r");
          break;
        case 1:
          uart_printstr("You choose option 2\n\r");
          break;
        case 2:
          uart_printstr("You choose option 3\n\r");
          break;
        }
      }
      // et on met a jout le prev ducoup
      prev_sw = sw;
      _delay_ms(20);
    }
  }
}
