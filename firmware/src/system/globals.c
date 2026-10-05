#include "utils/keyboard_utils.h"

/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////   GLOBALS
/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// word buffer, used in translation per example
char word[WORD_MAX_LEN + 1];

// 0 = Traduction
// 1 = Alphabet
// 2 = Jeu
uint8_t menu_choice = 0;

// help to know if a word is valid or not
uint8_t word_len = 0;

app_state_t app_state = MENU;
word_state_t word_state = INPUT;

// static uint8_t eye_state = 0;

uint8_t prev_sw;  // etat precedent du bouton
uint8_t rotaryclk_prev = 1;

// screen energy mode
uint32_t last_key_time = 0;
uint8_t screen_sleeping = 0;

uint8_t translation_index = 0;
