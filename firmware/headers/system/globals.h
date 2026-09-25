#ifndef GLOBALS_H
#define GLOBALS_H

#include "app/alphabet.h"
#include "app/game.h"
#include "app/language.h"
#include "utils/keyboard_utils.h"

// word buffer, used in translation per example
extern char word[WORD_MAX_LEN + 1];

// 0 = Traduction
// 1 = Alphabet
// 2 = Jeu
extern uint8_t menu_choice;

// help to know if a word is valid or not
extern uint8_t word_len;

extern app_state_t app_state;

extern word_state_t word_state;

extern uint8_t eye_state;

// rotary encoder
extern uint8_t prev_sw;  // etat precedent du bouton
extern uint8_t rotaryclk_prev;

extern uint32_t last_key_time;
extern uint8_t screen_sleeping;

language_t language;

// game global
uint8_t game_choice;
game_state_t game_state;
char game_target;
// the different choices you have
char game_answers[GAME_CHOICES];
uint8_t game_answer;  // player answer
uint32_t random_state;

// alphabet
extern alphabet_state_t alphabet_state;
extern uint8_t alphabet_choice;

#endif
