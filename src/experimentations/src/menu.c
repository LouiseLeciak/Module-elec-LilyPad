
#include "keyboard_utils.h"
#include "globals.h"
#include "screen_text.h"
#include "GC9A01.h"
#include "main_screen.h"
#include "keypad.h"

// Pour commencer un nouveau mot
void start_new_word(void)
{
    word_len = 0;
    word[0] = '\0';

    ili9488_fill_screen(GC9A01A_COLOR_PINK);

    draw_string(
        10, 20, "Entre un mot !",
        GC9A01A_COLOR_PURPLE,
        GC9A01A_COLOR_PINK,
        4, 2);

    word_state = INPUT;
}

// when you validate your word
void validate_word(void)
{
    ili9488_fill_screen(GC9A01A_COLOR_PINK);

    draw_string(
        10, 20, "Ton mot est:",
        GC9A01A_COLOR_PURPLE,
        GC9A01A_COLOR_PINK,
        4, 2);

    draw_string(
        10, 80, word,
        GC9A01A_COLOR_PURPLE,
        GC9A01A_COLOR_PINK,
        4, 2);

    word_state = VALIDATED;
}

// to display the different choicies
void show_menu(void)
{
    ili9488_fill_screen(GC9A01A_COLOR_PINK);

    draw_string(
        10, 20, "Menu",
        GC9A01A_COLOR_PURPLE,
        GC9A01A_COLOR_PINK,
        4, 2);

    if (menu_choice == 0)
    {
        draw_string(
            10, 80, "> Traduction",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }
    else
    {
        draw_string(
            10, 80, "  Traduction",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }

    if (menu_choice == 1)
    {
        draw_string(
            10, 140, "> Alphabet",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }
    else
    {
        draw_string(
            10, 140, "  Alphabet",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }

    if (menu_choice == 2)
    {
        draw_string(
            10, 200, "> Jeu",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }
    else
    {
        draw_string(
            10, 200, "  Jeu",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }

    app_state = MENU;
}

void update_menu_cursor(uint8_t old_choice)
{
    // on enelve lancien curseur et on met un espace
    if (old_choice == 0)
    {
        draw_string(
            10, 80, "  ",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }
    else if (old_choice == 1)
    {
        draw_string(
            10, 140, "  ",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }
    else
    {
        draw_string(
            10, 200, "  ",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }

    // nouveau curseur
    if (menu_choice == 0)
    {
        draw_string(
            10, 80, "> ",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }
    else if (menu_choice == 1)
    {
        draw_string(
            10, 140, "> ",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }
    else
    {
        draw_string(
            10, 200, "> ",
            GC9A01A_COLOR_PURPLE,
            GC9A01A_COLOR_PINK,
            3, 2);
    }
}

// le menu entre un mot
void start_translation(void)
{
    word_len = 0;
    word[0] = '\0';

    ili9488_fill_screen(GC9A01A_COLOR_PINK);

    draw_string(
        10, 20, "Entre un mot !",
        GC9A01A_COLOR_PURPLE,
        GC9A01A_COLOR_PINK,
        4, 2);

    word_state = INPUT;
    app_state = TRADUCTION;
}

// a modifier jaffiche juste une string
void show_alphabet(void)
{
    ili9488_fill_screen(GC9A01A_COLOR_PINK);

    draw_string(
        10, 20, "Choix : alphabet",
        GC9A01A_COLOR_PURPLE,
        GC9A01A_COLOR_PINK,
        3, 2);

    app_state = ALPHABET;
}

// idem
void show_game(void)
{
    ili9488_fill_screen(GC9A01A_COLOR_PINK);

    draw_string(
        10, 20, "Choix : jeu",
        GC9A01A_COLOR_PURPLE,
        GC9A01A_COLOR_PINK,
        3, 2);

    app_state = JEU;
}

void traduction()
{
    int key = keypad_read();

    if (key >= 0)
    {
        uint8_t row = key / COLS_NB;
        uint8_t col = key % COLS_NB;
        char c = keymap[row][col];

        // a modifier avec un define plus propre
        // pour comprendre directement qu'il s'agit du bouton HOME
        if (c == HOME)
        {
            menu_choice = 0;
            show_menu();
        }
        else if (c != '\0' &&
                 c != '\n' &&
                 word_len < WORD_MAX_LEN)
        {
            word[word_len] = c;
            word_len++;
            word[word_len] = '\0';

            draw_string(
                10, 80, word,
                GC9A01A_COLOR_PURPLE,
                GC9A01A_COLOR_PINK,
                4, 2);
        }

        _delay_ms(20);

        while (keypad_read() >= 0)
        {
            ;
        }
    }
}

void alphabet()
{
    int key = keypad_read();

    if (key >= 0)
    {
        uint8_t row = key / COLS_NB;
        uint8_t col = key % COLS_NB;
        char c = keymap[row][col];

        if (c == HOME)
        {
            show_menu();
        }

        _delay_ms(20);

        while (keypad_read() >= 0)
        {
            ;
        }
    }
}

void game()
{
    int key = keypad_read();

    if (key >= 0)
    {
        uint8_t row = key / COLS_NB;
        uint8_t col = key % COLS_NB;
        char c = keymap[row][col];

        if (c == HOME)
        {
            show_menu();
        }

        _delay_ms(20);

        while (keypad_read() >= 0)
        {
            ;
        }
    }
}