#ifndef KEYPAD_H
#define KEYPAD_H

#include <avr/io.h>

#define ROWS_NB 4
#define COLS_NB 10

extern const char keymap[ROWS_NB][COLS_NB];

void keypad_init(void);
int keypad_read(void);

#endif
