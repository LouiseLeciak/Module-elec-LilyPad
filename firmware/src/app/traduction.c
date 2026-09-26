#include "app/menu.h"
#include "display/GC9A01.h"
#include "display/main_screen.h"
#include "display/screen_text.h"
#include "input/keypad.h"
#include "system/globals.h"
#include "system/power_save.h"
#include "utils/keyboard_utils.h"
#include "storage/fatfs.h"
#include "storage/sd.h"
#include "storage/sd_streaming.h"
#include "utils/utils.h"

// Pour commencer un nouveau mot
void start_new_word(void)
{
  word_len = 0;
  word[0] = '\0';

  ili9488_fill_screen(GC9A01A_COLOR_OLIVE);

  draw_string(10, 20, "Entre un mot !", GC9A01A_COLOR_GREEN,
              GC9A01A_COLOR_OLIVE, 4, 2);

  word_state = INPUT;
}

// when you validate your word
void validate_word(void)
{
  word_state = VALIDATED;
}

void display_word(void)
{
  draw_string(10, 80, word, GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 4, 2);
}

void delete_last_char(void)
{
  if (word_len == 0)
    return;

  word_len--;
  word[word_len] = '\0';

  draw_char_small(10 + word_len * 22, 80, ' ', GC9A01A_COLOR_GREEN,
                  GC9A01A_COLOR_OLIVE, 4);
}

void traduction(void)
{
  if (word_state != INPUT)
    return;
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
    else if (c != '\0' && c != '\n' && word_len < WORD_MAX_LEN)
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

// le menu entre un mot
void start_translation(void)
{
  word_len = 0;
  word[0] = '\0';

  ili9488_fill_screen(GC9A01A_COLOR_OLIVE);
  if (language == LANG_FR)
  {
    draw_string(10, 20, "Entre un mot !", GC9A01A_COLOR_GREEN,
                GC9A01A_COLOR_OLIVE, 4, 2);
  }
  else if (language == LANG_EN)
  {
    draw_string(10, 20, "Choose a word !", GC9A01A_COLOR_GREEN,
                GC9A01A_COLOR_OLIVE, 4, 2);
  }

  word_state = INPUT;
  app_state = TRADUCTION;
}

//display image of a letter
// if letter is C then looking for C_LSF etc..
void display_translation_letter(void)
{
    char letter[2];
    char file_name[16] = {0};

    letter[0] = word[translation_index];
    letter[1] = '\0';

    ft_strcat(file_name, letter);

    if (language == LANG_FR)
        ft_strcat(file_name, "_LSF");
    else
        ft_strcat(file_name, "_BSL");

    ft_strcat(file_name, "   BMP");

    ili9488_fill_screen(GC9A01A_COLOR_OLIVE);

    for (uint8_t i = 0;
         i < IMG_LUT_MAX_SIZE && image_lut[i].address != 0;
         i++)
    {
        if (!ft_strncmp(file_name,
                        image_lut[i].name,
                        FILE_NAME_SIZE))
        {
            sd_stream_bmp_to_screen(
                cluster_to_lba(image_lut[i].address));
            break;
        }
    }
}