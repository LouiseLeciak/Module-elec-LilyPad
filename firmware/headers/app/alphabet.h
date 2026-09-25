#ifndef ALPHABET_H
#define ALPHABET_H

#include <avr/io.h>

typedef enum
{
  ALPHABET_LIST,
  ALPHABET_LETTER
} alphabet_state_t;

#define ALPHABET_SIZE 36

static const char alphabet_characters[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

void show_alphabet(void);
void alphabet(void);
void display_alphabet(void);
void update_alphabet_cursor(uint8_t old_choice);
void display_alphabet_letter(void);

#endif
