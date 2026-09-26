#ifndef GAME_H
#define GAME_H

#define GAME_CHAR_COUNT 36
#define GAME_CHOICES 3

#include <avr/io.h>

#define GAME_1 0
#define GAME_2 1

// diff states of the game menu
typedef enum
{
    GAME_MENU,
    GAME_FIND_LETTER,
    GAME_LETTER_SELECT,
    GAME_FIND_SIGN,
    GAME_SIGN_CHOICE_1,
    GAME_SIGN_CHOICE_2,
    GAME_SIGN_CHOICE_3,
    GAME_SIGN_SELECT,
    GAME_SHOW_LETTER,
    GAME_RESULT_YES_1,
    GAME_RESULT_NO_1,
    GAME_RESULT_YES_2,
    GAME_RESULT_NO_2
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
void display_sign_choice(uint8_t choice);
void display_find_sign(void);
void display_letter_answers(void);
void display_game_letter(void);

#endif
