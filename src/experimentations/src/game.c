#include "game.h"
#include "GC9A01.h"
#include "globals.h"
#include "main_screen.h"
#include "screen_text.h"
#include "power_save.h"

uint8_t game_choice = 0;
game_state_t game_state = GAME_MENU;
char game_target;
// the different choices you have 
char game_answers[GAME_CHOICES];
uint8_t game_answer = 0;// player answer
uint32_t random_state = 0;

static const char game_characters[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// display teh game menu
void show_game(void)
{
    ili9488_fill_screen(GC9A01A_COLOR_OLIVE);
    if (language == LANG_FR){
        draw_string(
            125, 20, "JEU",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    
        draw_string(
            10, 120, "> ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    
        draw_string(
            70, 120, "Lettre",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    
        draw_string(
            70, 220, "Signe",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    else if (language == LANG_EN){
        draw_string(
            125, 20, "GAME",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    
        draw_string(
            10, 120, "> ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    
        draw_string(
            70, 120, "Letter game",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    
        draw_string(
            70, 220, "Sign game",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    game_choice = 0;
    game_state = GAME_MENU;
    app_state = JEU;
}

// to print the cursor of the game menu
// similar of the main menu one
void update_game_cursor(uint8_t old_choice)
{
    power_save_activity();

    if (old_choice == 0)
    {
        draw_string(
            10, 120, "  ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    else
    {
        draw_string(
            10, 220, "  ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }

    if (game_choice == 0)
    {
        draw_string(
            10, 120, "> ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
    else
    {
        draw_string(
            10, 220, "> ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);
    }
}

void display_find_letter(void)
{
    char target_text[2];
    char answer_text[2];

    ili9488_fill_screen(GC9A01A_COLOR_OLIVE);

    target_text[0] = game_target;
    target_text[1] = '\0';

    if (language == LANG_FR){
        draw_string(
            20, 20, "JEU",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);

        draw_string(
            20, 80, "Ton signe: ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            3, 2);
    }
    if (language == LANG_EN){
        draw_string(
            20, 20, "GAME",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);

        draw_string(
            20, 80, "Your sign: ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            3, 2);
    }


    draw_string(
        200, 80, target_text,
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        3, 2);

    // choix 1

    answer_text[0] = game_answers[0];
    answer_text[1] = '\0';

    draw_string(
        10, 150, "> ",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    draw_string(
        70, 150, answer_text,
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    // choix 2

    answer_text[0] = game_answers[1];

    draw_string(
        70, 220, answer_text,
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    //choix 3

    answer_text[0] = game_answers[2];

    draw_string(
        70, 290, answer_text,
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);
}

void update_game_answer_cursor(uint8_t old_choice)
{
    power_save_activity();
    uint16_t old_y;
    uint16_t new_y;

    old_y = 150 + old_choice * 70;
    new_y = 150 + game_answer * 70;

    draw_string(
        10, old_y, "  ",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    draw_string(
        10, new_y, "> ",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);
}

void random_update(void){
    random_state++;
    // if (random_state >= 4294967290UL)
    //     random_state = 0;
}

//https://stackoverflow.com/questions/32225896/pseudo-random-function-in-c
// this helped me
uint32_t simple_random(void)
{
    random_state = random_state * 1103515245UL + 12345UL;

    return random_state;
}
///////////////////////////////////////////////////////////////////
//////////////////////////  GAME 1  ///////////////////////////////
///////////////////////////////////////////////////////////////////
void start_find_letter(void)
{
    uint8_t correct_position;
    uint8_t target_position;
    uint8_t first_position;// position of the first bad answer
    uint8_t second_position;// pos of the second one

    game_state = GAME_FIND_LETTER;

    // choose the letter to find
    target_position = simple_random() % GAME_CHAR_COUNT;// % bcs we have on 36 choices

    game_target = game_characters[target_position];

    // where the good answer will be
    correct_position = simple_random() % GAME_CHOICES;

    game_answers[correct_position] = game_target;

    // where is the first bad asnwer
    first_position = simple_random() % GAME_CHAR_COUNT;

    if (first_position == target_position)
    {
        first_position++;

        if (first_position >= GAME_CHAR_COUNT)
            first_position = 0;
    }

    // where is the second bad answer
    second_position = simple_random() % GAME_CHAR_COUNT;

    // check if similar of the good answer
    if (second_position == target_position)
    {
        second_position++;

        if (second_position >= GAME_CHAR_COUNT)
            second_position = 0;
    }

    // checking if similar of the other bad answer
    if (second_position == first_position)
    {
        second_position++;

        if (second_position >= GAME_CHAR_COUNT)
            second_position = 0;
    }

    // positions
    if (correct_position == 0)
    {
        game_answers[1] = game_characters[first_position];
        game_answers[2] = game_characters[second_position];
    }
    else if (correct_position == 1)
    {
        game_answers[0] = game_characters[first_position];
        game_answers[2] = game_characters[second_position];
    }
    else
    {
        game_answers[0] = game_characters[first_position];
        game_answers[1] = game_characters[second_position];
    }

    game_answer = 0;// set the player choice at 0

    display_find_letter();
}

///////////////////////////////////////////////////////////////////
//////////////////////////  GAME 2  ///////////////////////////////
///////////////////////////////////////////////////////////////////
void display_find_sign(void)
{
    char target_text[2];
    char answer_text[2];

    ili9488_fill_screen(GC9A01A_COLOR_OLIVE);

    target_text[0] = game_target;
    target_text[1] = '\0';

    if (language == LANG_FR){
        draw_string(
            20, 20, "JEU",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);

        draw_string(
            20, 80, "Ta lettre: ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            3, 2);
    }
    if (language == LANG_EN){
        draw_string(
            20, 20, "GAME",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            4, 2);

        draw_string(
            20, 80, "Your letter: ",
            GC9A01A_COLOR_GREEN,
            GC9A01A_COLOR_OLIVE,
            3, 2);
    }


    draw_string(
        200, 80, target_text,
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        3, 2);

    // choix 1

    answer_text[0] = game_answers[0];
    answer_text[1] = '\0';

    draw_string(
        10, 150, "> ",
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    draw_string(
        70, 150, answer_text,
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    // choix 2

    answer_text[0] = game_answers[1];

    draw_string(
        70, 220, answer_text,
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);

    //choix 3

    answer_text[0] = game_answers[2];

    draw_string(
        70, 290, answer_text,
        GC9A01A_COLOR_GREEN,
        GC9A01A_COLOR_OLIVE,
        4, 2);
}
// you have to found which sign is the letter
void start_find_sign(void)
{
    uint8_t correct_position;
    uint8_t target_position;
    uint8_t first_position;// position of the first bad answer
    uint8_t second_position;// pos of the second one

    game_state = GAME_FIND_SIGN;

    // choose the letter to find
    target_position = simple_random() % GAME_CHAR_COUNT;// % bcs we have on 36 choices

    game_target = game_characters[target_position];

    // where the good answer will be
    correct_position = simple_random() % GAME_CHOICES;

    game_answers[correct_position] = game_target;

    // where is the first bad asnwer
    first_position = simple_random() % GAME_CHAR_COUNT;

    if (first_position == target_position)
    {
        first_position++;

        if (first_position >= GAME_CHAR_COUNT)
            first_position = 0;
    }

    // where is the second bad answer
    second_position = simple_random() % GAME_CHAR_COUNT;

    // check if similar of the good answer
    if (second_position == target_position)
    {
        second_position++;

        if (second_position >= GAME_CHAR_COUNT)
            second_position = 0;
    }

    // checking if similar of the other bad answer
    if (second_position == first_position)
    {
        second_position++;

        if (second_position >= GAME_CHAR_COUNT)
            second_position = 0;
    }

    // positions
    if (correct_position == 0)
    {
        game_answers[1] = game_characters[first_position];
        game_answers[2] = game_characters[second_position];
    }
    else if (correct_position == 1)
    {
        game_answers[0] = game_characters[first_position];
        game_answers[2] = game_characters[second_position];
    }
    else
    {
        game_answers[0] = game_characters[first_position];
        game_answers[1] = game_characters[second_position];
    }

    game_answer = 0;// set the player choice at 0
}

void display_game_yes(void)
{
    power_save_activity();
    ili9488_fill_screen(GC9A01A_COLOR_GREEN);

    draw_string(
        110, 170, "O",
        GC9A01A_COLOR_WHITE,
        GC9A01A_COLOR_GREEN,
        20, 2);
}

void display_game_no(void)
{
    power_save_activity();
    ili9488_fill_screen(GC9A01A_COLOR_RED);

    draw_string(
        110, 170, "X",
        GC9A01A_COLOR_WHITE,
        GC9A01A_COLOR_RED,
        20, 2);
}