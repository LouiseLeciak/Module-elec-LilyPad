#ifndef KEYPAD_H
#define KEYPAD_H

#define ROWS_NB 4
#define COLS_NB 10

static const char keymap[ROWS_NB][COLS_NB] =
    {
        {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
        {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
        {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '-'},  // enter
        {'Z', 'X', 'C', 'V', 'B', 'N', 'M', '#', '/', '/'}}; // add supp and home


void keypad_init(void);
void select_row(uint8_t row);
int read_column(void);
int keypad_read(void);

#endif