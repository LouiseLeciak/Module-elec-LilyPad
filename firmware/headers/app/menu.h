#ifndef MENU_H
#define MENU_H

#include <avr/io.h>

#include "display/GC9A01.h"

#ifdef ALT
#define BG_COLOUR GC9A01A_COLOR_BLACK
#define FG_COLOUR GC9A01A_COLOR_ORANGE
#define ACCENT_COLOUR GC9A01A_COLOR_WHITE
#define EYELID_COLOUR GC9A01A_COLOR_MAROON
#define IRIS_COLOUR GC9A01A_COLOR_RED
#define SCLERA_COLOUR GC9A01A_COLOR_MAROON
#define EYE_ACCENT_COLOUR GC9A01A_COLOR_RED
#else
#define BG_COLOUR GC9A01A_COLOR_OLIVE
#define FG_COLOUR GC9A01A_COLOR_GREEN
#define ACCENT_COLOUR GC9A01A_COLOR_WHITE
#define EYELID_COLOUR GC9A01A_COLOR_GREEN
#define IRIS_COLOUR GC9A01A_COLOR_WHITE
#define SCLERA_COLOUR GC9A01A_COLOR_BLACK
#define EYE_ACCENT_COLOUR GC9A01A_COLOR_CYAN
#endif  // ALT

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
void show_bonjour();
void show_hello();

#endif
