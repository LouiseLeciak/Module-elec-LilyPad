#ifndef ROTARY_H
#define ROTARY_H

#include <avr/io.h>

// rotary funcions
void rotary_read(uint8_t* clk, uint8_t* dt);
uint8_t rotary_has_changed(uint8_t clk);
uint8_t rotary_is_clockwise(uint8_t clk, uint8_t dt);
uint8_t rotary_change_choice(uint8_t choice, uint8_t max_choice,
                             uint8_t clockwise);
void rotary_menu(uint8_t clockwise);
void rotary_game_menu(uint8_t clockwise);
void rotary_game_answer(uint8_t clockwise);
void rotary_alphabet(uint8_t clockwise);
void rotary_manage_state(uint8_t clk, uint8_t dt);

// rotary button functions
uint8_t rotary_button_read(void);
uint8_t rotary_button_has_changed(uint8_t sw);
void rotary_button_menu(void);
void rotary_button_traduction(void);
void rotary_button_game(void);
void rotary_button_alphabet(void);
void rotary_button_handle_press(void);
void rotary_update(void);
void rotary_button_update(void);
void display_game_answer(void);

#endif
