
#include <avr/io.h>
#include "language.h"
#include "pinout.h"



language_t language = LANG_FR;

// init the slide switch
// pc1 <-> pc2 ; pc2 <-> pc3
void language_switch_init(void)
{
    // pc1 et pc3 avec pull up
    // entree a 1
    DDRC &= ~(SDL_SW1 | SDL_SW3);
    PORTC |= SDL_SW1 | SDL_SW3;

    // pc2, different comme c;est le point commun
    // sortie a 0
    DDRC |= SDL_SW2;
    PORTC &= ~SDL_SW2;
}

// change the language when moving the slide switch
void language_update(void)
{
    language_t new_language;

    // if on the left -> fr
    // pc1 = 0 car relie a pc2 qui est a 0
    if (!(PINC & SDL_SW1))
    {
        new_language = LANG_FR;
    }
    else if (!(PINC & SDL_SW3))// right -> en
    {
        new_language = LANG_EN;
    }
    else
    {
        return;
    }

    if (new_language != language)
    {
        language = new_language;
        // go back to the menu when we switch language
        menu_choice = 0;
        show_menu();
    }
}