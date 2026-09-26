#ifndef TRADUCTION_H
#define TRADUCTION_H

#include <avr/io.h>

void show_menu_en(void);
void start_translation_en(void);
void update_menu_cursor_en(uint8_t old_choice);
void traduction_en();
void alphabet_en();
void game_en();
void validate_word_en(void);
void start_new_word_en(void);
void display_translation_letter(void);
#endif
