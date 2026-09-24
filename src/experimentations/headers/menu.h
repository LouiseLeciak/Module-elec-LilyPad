#ifndef MENU_H
#define MENU_H

#include <avr/io.h>

void show_menu(void);
void start_translation(void);
void show_alphabet(void);
void show_game(void);
void update_menu_cursor(uint8_t old_choice);
void traduction();
void alphabet();
void game();
void validate_word(void);
void start_new_word(void);



#endif