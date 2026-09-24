#ifndef GAME_H
#define GAME_H

#define GAME_CHAR_COUNT 36
#define GAME_CHOICES 3

#include <stdint.h>

// diff states of the game menu
typedef enum
{
    GAME_MENU,
    GAME_FIND_LETTER,
    GAME_RESULT_YES,
    GAME_RESULT_NO,
    GAME_FIND_SIGN
} game_state_t;

uint32_t simple_random(void);
void show_game(void);
void update_game_cursor(uint8_t old_choice);
void start_find_letter(void);
void start_find_sign(void);
void random_update(void);
void update_game_answer_cursor(uint8_t old_choice);
void display_find_letter(void);
void update_game_answer_cursor(uint8_t old_choice);
void display_game_yes(void);
void display_game_no(void);

#endif