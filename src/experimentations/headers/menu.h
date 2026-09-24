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

void show_menu_en(void);
void start_translation_en(void);
// void show_alphabet_en(void);
// void show_game_en(void);
void update_menu_cursor_en(uint8_t old_choice);
void traduction_en();
void alphabet_en();
void game_en();
void validate_word_en(void);
void start_new_word_en(void);

#endif