#include "input/rotary.h"

#include "app/alphabet.h"
#include "app/game.h"
#include "app/menu.h"
#include "app/traduction.h"
#include "display/GC9A01.h"
#include "display/ili9488.h"
#include "display/screen_text.h"
#include "input/i2c_rotary.h"
#include "system/globals.h"
#include "system/power_save.h"
#include "system/uart.h"
#include "utils/keyboard_utils.h"
#include "utils/utils.h"

// https://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf

/////////////////////////////////////
/// Rotation manager of the rotary///
/////////////////////////////////////

void rotary_update(void)
{
  uint8_t clk;
  uint8_t dt;

  rotary_read(&clk, &dt);

  if (rotary_has_changed(&clk, &dt))  // added pointers  and poniter to dt
  {
    if (inac == 1)
    {
      power_save_activity();
      return;
    }
    else
      power_save_activity();
    rotary_manage_state(clk, dt);
  }
}

// read mcp23017 and get clk and dt
void rotary_read(uint8_t* clk, uint8_t* dt)
{
  uint8_t gpio;

  gpio = mcp_read_register(MCP_GPIOA);

  *clk = (gpio >> ROTARY_CLK) & 1;
  *dt = (gpio >> ROTARY_DT) & 1;
}

// Returns the rotary direction
// Returns +1 (CW), -1 (CCW) or 0 (no move / invalid jump)
int8_t rotary_get_direction(uint8_t prev_state, uint8_t current_state)
{
  uint8_t changed = prev_state ^ current_state;  // 1 bit = that pin changed
  uint8_t prev_clk =
      (prev_state >> 1) & 1;  // clk BEFORE the step (bit 1 of old state)
  uint8_t cur_dt = current_state & 1;  // dt AFTER the step (bit 0 of new state)

  //  find which pins changed  (prev_state ^ current_state)
  //  changed | meaning                  | (prev -> current) pairs
  // --------+--------------------------+---------------------------------------
  //   0     | nothing moved            | 00->00  01->01  10->10  11->11
  //   1     | only dt flipped (valid)  | 00->01  01->00  10->11  11->10
  //   2     | only clk flipped (valid) | 00->10  10->00  01->11  11->01
  //   3     | both flipped (invalid)   | 00->11  11->00  01->10  10->01
  //
  if (changed == 0 || changed == 3)
    return 0;

  //  check which has changed (2 possibilities changed = 10)
  //
  if (prev_clk == cur_dt)
  {
    // the levels are the same: clockwise
    return 1;
  }
  else
  {
    // the levels are different: counter-clockwise
    return -1;
  }
}

// Returns 1 when a full click was completed, 0 otherwise.
// On a click, clk and dt are rewritten to what rotary_manage_state() expects:
// clk = 0, dt = 1 for clockwise
// clk = 0, dt = 0 for counter-clockwise
// Updates Globals rotaryclk_prev, rotarydt_prev and rotary_steps.

uint8_t rotary_has_changed(uint8_t* clk, uint8_t* dt)
{
  uint8_t prev_state = 0;
  uint8_t current_state = 0;
  int8_t rotary_direction = 0;
  uint8_t clk_bit = *clk << 1;  // clk moved to bit 1
  uint8_t dt_bit = *dt;         // dt stays in bit 0

  if (rotaryclk_prev == 0 && rotarydt_prev == 0)
    prev_state = 0;
  else if (rotaryclk_prev == 0 && rotarydt_prev == 1)
    prev_state = 1;
  else if (rotaryclk_prev == 1 && rotarydt_prev == 0)
    prev_state = 2;
  else
    prev_state = 3;

  current_state = clk_bit | dt_bit;  // combine them

  // Pins are the same as last time: nothing moved
  if (current_state == prev_state)
    return 0;

  // Pins changed: CW (+1), CCW (-1) or invalid jump (0)
  rotary_direction = rotary_get_direction(prev_state, current_state);

  // Add this step to the count and remember the new position
  rotary_steps += rotary_direction;
  rotaryclk_prev = *clk;
  rotarydt_prev = *dt;

  // Not enough steps wait for a full click
  if (rotary_steps > -ROTARY_STEPS_PER_CLICK &&
      rotary_steps < ROTARY_STEPS_PER_CLICK)
    return 0;

  // Full click: report it in the format rotary_manage_state() expects
  *clk = 0;
  if (rotary_steps > 0)
    *dt = 1;  // clockwise
  else
    *dt = 0;  // counter-clockwise

  rotary_steps = 0;
  return 1;
}

// to dertermine if rotary is clockwise or not
uint8_t rotary_is_clockwise(uint8_t clk, uint8_t dt)
{
  if (dt == clk)
    return (0);
  return (1);
}

// manage the principal menu
void rotary_menu(uint8_t clockwise)
{
  uint8_t old_choice;

  old_choice = menu_choice;

  menu_choice = rotary_change_choice(menu_choice, MENU_SIZE, clockwise);
  // take away the cursor of the previous choice
  // to avoid having 2 at the same time
  update_menu_cursor(old_choice);
}

// manage the game menu
void rotary_game_menu(uint8_t clockwise)
{
  uint8_t old_choice;

  old_choice = game_choice;

  game_choice = rotary_change_choice(game_choice, 2, clockwise);

  update_game_cursor(old_choice);
}

// manage answer of the game
void rotary_game_answer(uint8_t clockwise)
{
  uint8_t old_choice;

  old_choice = game_answer;

  game_answer = rotary_change_choice(game_answer, GAME_CHOICES, clockwise);

  update_game_answer_cursor(old_choice);
}

// manage alphabet
void rotary_alphabet(uint8_t clockwise)
{
  uint8_t old_choice;

  old_choice = alphabet_choice;

  alphabet_choice =
      rotary_change_choice(alphabet_choice, ALPHABET_SIZE, clockwise);

  update_alphabet_cursor(old_choice);
}

void rotary_manage_state(uint8_t clk, uint8_t dt)
{
  uint8_t clockwise;

  clockwise = rotary_is_clockwise(clk, dt);

#ifdef DEBUG
  uart_printstr("[ROT] ");
  uart_printstr(clockwise ? "CW" : "CCW");
  uart_printstr(" | state=");
  if (app_state == MENU)
    uart_printstr("MENU");
  else if (app_state == TRADUCTION)
    uart_printstr("TRADUCTION");
  else if (app_state == ALPHABET)
    uart_printstr("ALPHABET");
  else if (app_state == JEU)
    uart_printstr("JEU");
  uart_printstr("\r\n");
#endif

  if (app_state == MENU)
  {
    rotary_menu(clockwise);
  }
  else if (app_state == JEU && game_state == GAME_MENU)
  {
    rotary_game_menu(clockwise);
  }
  else if (app_state == JEU && game_state == GAME_FIND_LETTER)
  {
    rotary_game_answer(clockwise);
  }
  else if (app_state == ALPHABET && alphabet_state == ALPHABET_LIST)
  {
    rotary_alphabet(clockwise);
  }
  else if (app_state == JEU && game_state == GAME_LETTER_SELECT)
  {
    rotary_game_answer(clockwise);
  }
  else if (app_state == JEU && game_state == GAME_SIGN_SELECT)
  {
    rotary_game_answer(clockwise);
  }
}

// fonction to move dforward the rotary
uint8_t rotary_change_choice(uint8_t choice, uint8_t max_choice,
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
    _delay_ms(5);  // debounce

    sw = rotary_button_read();

    if (sw != prev_sw)
    {
      prev_sw = sw;
      return 1;
    }
  }

  return 0;
}

// menu management
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
    if (word_len == 0)
      return;
    else
      validate_word();

    translation_index = 0;

    if (word_len > 0)
    {
      word_state = SHOW_TRANSLATION;
      display_translation_letter();
    }
  }
  else if (word_state == SHOW_TRANSLATION)
  {
    translation_index++;

    if (translation_index < word_len)
    {
      display_translation_letter();
    }
    else
    {
      start_new_word();
    }
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

  ////////////////////////////////
  /////////// JEU 1 //////////////
  ////////////////////////////////

  else if (game_state == GAME_FIND_LETTER)
  {
    game_state = GAME_SHOW_LETTER;
    display_game_letter();
  }

  else if (game_state == GAME_SHOW_LETTER)
  {
    game_state = GAME_LETTER_SELECT;
    game_answer = 0;
    display_letter_answers();
  }

  else if (game_state == GAME_LETTER_SELECT)
  {
    if (game_answers[game_answer] == game_target)
    {
      game_state = GAME_RESULT_YES_1;
      display_game_yes();
    }
    else
    {
      game_state = GAME_RESULT_NO_1;
      display_game_no();
    }
  }

  else if (game_state == GAME_RESULT_YES_1)
  {
    eyes_action();
    game_answer = 0;
    start_find_letter();
  }

  else if (game_state == GAME_RESULT_NO_1)
  {
    eyes_action();
    game_answer = 0;
    game_state = GAME_FIND_LETTER;
    display_find_letter();
  }

  ////////////////////////////////
  /////////// JEU 2 //////////////
  ////////////////////////////////

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
      game_state = GAME_RESULT_YES_2;
      display_game_yes();
    }
    else
    {
      game_state = GAME_RESULT_NO_2;
      display_game_no();
    }
  }
  else if (game_state == GAME_RESULT_YES_2)
  {
    eyes_action();
    game_answer = 0;
    start_find_sign();
  }
  else if (game_state == GAME_RESULT_NO_2)
  {
    eyes_action();
    game_answer = 0;
    game_state = GAME_FIND_SIGN;
    display_find_sign();
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
    // display_alphabet();
    show_alphabet();
  }
}

// where to go when the button is pressed in the menu
void rotary_button_handle_press(void)
{
  if (inac == 1)
  {
    power_save_activity();
    return;
  }
  else
    power_save_activity();

  if (app_state == MENU)
  {
    uart_printstr("[BTN] MENU -> ");
    if (menu_choice == 0)
      uart_printstr("TRADUCTION");
    else if (menu_choice == 1)
      uart_printstr("ALPHABET");
    else
      uart_printstr("JEU");
    uart_printstr("\r\n");
    rotary_button_menu();
  }
  else if (app_state == TRADUCTION)
  {
    uart_printstr("[BTN] TRADUCTION -> ");
    uart_printstr(word_state == INPUT ? "VALIDATED" : "NEW WORD");
    uart_printstr("\r\n");
    rotary_button_traduction();
  }
  else if (app_state == JEU)
  {
    uart_printstr("[BTN] JEU press\r\n");
    rotary_button_game();
  }
  else if (app_state == ALPHABET)
  {
    uart_printstr("[BTN] ALPHABET -> ");
    uart_printstr(alphabet_state == ALPHABET_LIST ? "LETTER VIEW" : "LIST");
    uart_printstr("\r\n");
    rotary_button_alphabet();
  }
}

void display_game_answer(void)
{
  ili9488_fill_screen(BG_COLOUR);

  if (language == LANG_FR)
  {
    draw_string(20, 20, "JEU", FG_COLOUR, BG_COLOUR, 4, 2);

    draw_string(20, 80, "Choisis :", FG_COLOUR, BG_COLOUR, 3, 2);
  }
  else
  {
    draw_string(20, 20, "GAME", FG_COLOUR, BG_COLOUR, 4, 2);

    draw_string(20, 80, "Choose:", FG_COLOUR, BG_COLOUR, 3, 2);
  }

  // choice n1
  draw_string(10, 150, ">", ACCENT_COLOUR, BG_COLOUR, 4, 2);

  draw_string(80, 150, "1", FG_COLOUR, BG_COLOUR, 4, 2);

  // choice n2
  draw_string(10, 220, " ", FG_COLOUR, BG_COLOUR, 4, 2);

  draw_string(80, 220, "2", FG_COLOUR, BG_COLOUR, 4, 2);

  // choice n3
  draw_string(10, 290, " ", FG_COLOUR, BG_COLOUR, 4, 2);

  draw_string(80, 290, "3", FG_COLOUR, BG_COLOUR, 4, 2);
}
