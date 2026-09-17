#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <avr/io.h>

#define ROWS_NB 4
#define COLS_NB 10
#define ROTARY_CLK 6
#define ROTARY_SW 5
#define ROTARY_DT 7

static uint8_t rotaryclk_prev;
// static uint8_t sw_prev = 1;

// comme ca juste a donner la position et renvois
// la lettre qui va avec
static const char keymap[ROWS_NB][COLS_NB] = {
    {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
    {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
    {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '\n'},   // enter
    {'Z', 'X', 'C', 'V', 'B', 'N', 'M', ' ', '\0', '\0'}}; // add supp and home

void select_row(uint8_t row);
int read_column(void);
int keypad_read(void);
void rotary_update(void);
void rotary_button_update(void);

void draw_menu(void);
void print_hex_value(char c);

#endif // !KEYBOARD_H
