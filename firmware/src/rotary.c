

#include "keyboard_utils.h"
#include "globals.h"
#include "menu.h"
#include "i2c_rotary.h"
#include "power_save.h"
#include "game.h"
#include "alphabet.h"
#include "rotary.h"
#include "GC9A01.h"
#include "main_screen.h"
#include "screen_text.h"

// https://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf

/////////////////////////////////////
/// Rotation manager of the rotary///
/////////////////////////////////////

void rotary_update(void)
{
    uint8_t clk;
    uint8_t dt;

    rotary_read(&clk, &dt);

    if (rotary_has_changed(clk))
    {
        power_save_activity();
        rotary_manage_state(clk, dt);
    }
}

// read mcp23017 and get clk and dt
void rotary_read(uint8_t *clk, uint8_t *dt)
{
    uint8_t gpio;

    gpio = mcp_read_register(MCP_GPIOA);

    *clk = (gpio >> ROTARY_CLK) & 1;
    *dt = (gpio >> ROTARY_DT) & 1;
}

// compare with previous clk because everything goes fast
uint8_t rotary_has_changed(uint8_t clk)
{
    // rotary prev started at 1
    if (rotaryclk_prev == 1 && clk == 0)
    {
        rotaryclk_prev = clk;
        return 1;
    }

    rotaryclk_prev = clk;
    return 0;
}

// to dertermine if rotary is clockwise or not
uint8_t rotary_is_clockwise(uint8_t clk, uint8_t dt)
{
    return (dt != clk);
}

// manage the principal menu
void rotary_menu(uint8_t clockwise)
{
    uint8_t old_choice;

    old_choice = menu_choice;

    menu_choice = rotary_change_choice(
        menu_choice,
        MENU_SIZE,
        clockwise);
    // take away the cursor of the previous choice
    // to avoid having 2 at the same time
    update_menu_cursor(old_choice);
}

// manage the game menu
void rotary_game_menu(uint8_t clockwise)
{
    uint8_t old_choice;

    old_choice = game_choice;

    game_choice = rotary_change_choice(
        game_choice,
        2,
        clockwise);

    update_game_cursor(old_choice);
}

// manage answer of the game
void rotary_game_answer(uint8_t clockwise)
{
    uint8_t old_choice;

    old_choice = game_answer;

    game_answer = rotary_change_choice(
        game_answer,
        GAME_CHOICES,
        clockwise);

    update_game_answer_cursor(old_choice);
}

// manage alphabet
void rotary_alphabet(uint8_t clockwise)
{
    uint8_t old_choice;

    old_choice = alphabet_choice;

    alphabet_choice = rotary_change_choice(
        alphabet_choice,
        ALPHABET_SIZE,
        clockwise);

    update_alphabet_cursor(old_choice);
}

void rotary_manage_state(uint8_t clk, uint8_t dt)
{
    uint8_t clockwise;

    clockwise = rotary_is_clockwise(clk, dt);

    if (app_state == MENU)
    {
        rotary_menu(clockwise);
    }
    else if (app_state == JEU &&
             game_state == GAME_MENU)
    {
        rotary_game_menu(clockwise);
    }
    else if (app_state == JEU &&
             game_state == GAME_FIND_LETTER)
    {
        rotary_game_answer(clockwise);
    }
    else if (app_state == ALPHABET &&
             alphabet_state == ALPHABET_LIST)
    {
        rotary_alphabet(clockwise);
    }
    else if (app_state == JEU &&
         game_state == GAME_SIGN_SELECT)
    {
        rotary_game_answer(clockwise);
    }
}

// fonction to move dforward the rotary
uint8_t rotary_change_choice(
    uint8_t choice,
    uint8_t max_choice,
    uint8_t clockwise)
{
    if (clockwise)
    {
        choice++;

        if (choice >= max_choice)
            choice = 0;
    }
    else
    {
        if (choice == 0)
            choice = max_choice - 1;
        else
            choice--;
    }

    return choice;
}




////////////////////////////////////////
/// Push button manager of the rotary///
////////////////////////////////////////

void rotary_button_update(void)
{
    uint8_t sw;

    sw = rotary_button_read();

    if (rotary_button_has_changed(sw))
    {
        if (sw == 0)
        {
            rotary_button_handle_press();
        }
    }
}

// MCP23017 -> GPIOA -> bit SW -> return 0 or 1
uint8_t rotary_button_read(void)
{
    uint8_t gpio;

    gpio = mcp_read_register(MCP_GPIOA);

    return (gpio >> ROTARY_SW) & 1;
}

// check if somoene pushed the button
uint8_t rotary_button_has_changed(uint8_t sw)
{
    if (sw != prev_sw)
    {
        _delay_ms(5);// debounce

        sw = rotary_button_read();

        if (sw != prev_sw)
        {
            prev_sw = sw;
            return 1;
        }
    }

    return 0;
}

//menu management
void rotary_button_menu(void)
{
    if (menu_choice == 0)
    {
        start_translation();
    }
    else if (menu_choice == 1)
    {
        show_alphabet();
    }
    else if (menu_choice == 2)
    {
        show_game();
    }
}

// traduction button management
void rotary_button_traduction(void)
{
    if (word_state == INPUT)
    {
        validate_word();
    }
    else
    {
        start_new_word();
    }
}

// game button management
void rotary_button_game(void)
{
    if (game_state == GAME_MENU)
    {
        if (game_choice == 0)
        {
            start_find_letter();
        }
        else
        {
            start_find_sign();
        }
    }
    else if (game_state == GAME_FIND_LETTER)
    {
        if (game_answers[game_answer] == game_target)
        {
            game_state = GAME_RESULT_YES;
            display_game_yes();
        }
        else
        {
            game_state = GAME_RESULT_NO;
            display_game_no();
        }
    }
    else if (game_state == GAME_RESULT_YES)
    {
        start_find_letter();
    }
    else if (game_state == GAME_RESULT_NO)
    {
        game_state = GAME_FIND_LETTER;
        display_find_letter();
    }
    else if (game_state == GAME_FIND_SIGN)
    {
        game_state = GAME_SIGN_CHOICE_1;
        display_sign_choice(0);
    }
    else if (game_state == GAME_SIGN_CHOICE_1)
    {
        game_state = GAME_SIGN_CHOICE_2;
        display_sign_choice(1);
    }
    else if (game_state == GAME_SIGN_CHOICE_2)
    {
        game_state = GAME_SIGN_CHOICE_3;
        display_sign_choice(2);
    }
    else if (game_state == GAME_SIGN_CHOICE_3)
    {
        game_state = GAME_SIGN_SELECT;

        game_answer = 0;

        display_game_answer();
    }
    else if (game_state == GAME_SIGN_SELECT)
    {
        if (game_answers[game_answer] == game_target)
        {
            game_state = GAME_RESULT_YES;
            display_game_yes();
        }
        else
        {
            game_state = GAME_RESULT_NO;
            display_game_no();
        }
    }
}

// alphabet button manger
void rotary_button_alphabet(void)
{
    if (alphabet_state == ALPHABET_LIST)
    {
        alphabet_state = ALPHABET_LETTER;
        display_alphabet_letter();
    }
    else if (alphabet_state == ALPHABET_LETTER)
    {
        alphabet_state = ALPHABET_LIST;
        display_alphabet();
    }
}

// where to go when the button is pressed in the menu
void rotary_button_handle_press(void)
{
    power_save_activity();

    if (app_state == MENU)
    {
        rotary_button_menu();
    }
    else if (app_state == TRADUCTION)
    {
        rotary_button_traduction();
    }
    else if (app_state == JEU)
    {
        rotary_button_game();
    }
    else if (app_state == ALPHABET)
    {
        rotary_button_alphabet();
    }
}

void display_game_answer(void)
{
    ili9488_fill_screen(GC9A01A_COLOR_OLIVE);
    if (language == LANG_FR){
        draw_string(
            100, 30,
            "JEU",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    else if (language == LANG_EN){
        draw_string(
            100, 30,
            "GAME",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    if (language == LANG_FR)
    {
        draw_string(
            80, 100,
            "Choisis :",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            3, 2);
    }
    else
    {
        draw_string(
            80, 100,
            "Choose:",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            3, 2);
    }

    draw_string(
        80, 150,
        "  1",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    draw_string(
        80, 220,
        "  2",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    draw_string(
        80, 290,
        "  3",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);
}