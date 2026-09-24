
#include "keyboard_utils.h"
#include "globals.h"
#include "screen_text.h"
#include "GC9A01.h"
#include "main_screen.h"
#include "keypad.h"
#include "power_save.h"

// Pour commencer un nouveau mot
void start_new_word(void)
{
    word_len = 0;
    word[0] = '\0';

    ili9488_fill_screen(GC9A01A_COLOR_OLIVE);

    draw_string(
        10, 20, "Entre un mot !",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    word_state = INPUT;
}

// when you validate your word
void validate_word(void)
{
    ili9488_fill_screen(GC9A01A_COLOR_OLIVE);

    draw_string(
        10, 20, "Ton mot est:",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    draw_string(
        10, 80, word,
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    word_state = VALIDATED;
}

// to display the different choicies
void show_menu(void)
{
    ili9488_fill_screen(GC9A01A_COLOR_OLIVE);

    draw_string(
        100, 20, "Menu",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        5, 2);

    if (menu_choice == 0)
    {
        draw_string(
            10, 120, "> Traduction",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    else
    {
        draw_string(
            10, 120, "  Traduction",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }

    if (menu_choice == 1)
    {
        draw_string(
            10, 220, "> Alphabet",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    else
    {
        draw_string(
            10, 220, "  Alphabet",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }

    if (menu_choice == 2)
    {
        draw_string(
            10, 320, "> Jeu",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    else
    {
        draw_string(
            10, 320, "  Jeu",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }

    app_state = MENU;
}

void update_menu_cursor(uint8_t old_choice)
{
    // on enelve lancien curseur et on met un espace
    if (old_choice == 0)
    {
        draw_string(
            10, 120, "  ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    else if (old_choice == 1)
    {
        draw_string(
            10, 220, "  ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    else
    {
        draw_string(
            10, 320, "  ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }

    // nouveau curseur
    if (menu_choice == 0)
    {
        draw_string(
            10, 120, "> ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    else if (menu_choice == 1)
    {
        draw_string(
            10, 220, "> ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    else
    {
        draw_string(
            10, 320, "> ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
}

// le menu entre un mot
void start_translation(void)
{
    word_len = 0;
    word[0] = '\0';

    ili9488_fill_screen(GC9A01A_COLOR_OLIVE);

    draw_string(
        10, 20, "Entre un mot !",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    word_state = INPUT;
    app_state = TRADUCTION;
}

// a modifier jaffiche juste une string
void show_alphabet(void)
{
    ili9488_fill_screen(GC9A01A_COLOR_OLIVE);

    draw_string(
        10, 20, "Choix : alphabet",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        3, 2);

    app_state = ALPHABET;
}

// idem
void show_game(void)
{
    ili9488_fill_screen(GC9A01A_COLOR_OLIVE);

    draw_string(
        10, 20, "Choix : jeu",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        3, 2);

    app_state = JEU;
}

void display_word(void)
{
    draw_string(
        10, 80, word,
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);
}

void delete_last_char(void)
{
    if (word_len == 0)
        return;

    word_len--;
    word[word_len] = '\0';

    draw_char_small(
        10 + word_len * 22,
        80,
        ' ',
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4);
}

void traduction(void)
{
    int key = keypad_read();

    if (key >= 0)
    {
        power_save_activity();
        uint8_t row = key / COLS_NB;
        uint8_t col = key % COLS_NB;
        char c = keymap[row][col];

        if (c == HOME)
        {
            menu_choice = 0;
            show_menu();
        }
        else if (c == DEL)
            delete_last_char();
        else if (c != '\0' &&
                 c != '\n' &&
                 word_len < WORD_MAX_LEN)
        {
            word[word_len] = c;
            word_len++;

            word[word_len] = '\0';

            display_word();
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
        power_save_activity();
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
        power_save_activity();
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