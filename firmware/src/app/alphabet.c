
#include "app/alphabet.h"

#include "app/menu.h"
#include "display/GC9A01.h"
#include "display/main_screen.h"
#include "display/screen_text.h"
#include "input/keypad.h"
#include "storage/fatfs.h"
#include "storage/sd_streaming.h"
#include "system/globals.h"
#include "system/power_save.h"
#include "system/uart.h"
#include "utils/utils.h"

alphabet_state_t alphabet_state = ALPHABET_LIST;
uint8_t alphabet_choice = 0;

// a modifier jaffiche juste une string
void show_alphabet(void)
{
  ili9488_fill_screen(GC9A01A_COLOR_OLIVE);

  if (language == LANG_FR)
  {
    draw_string(80, 20, "ALPHABET", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 4,
                2);

    draw_string(10, 80, "Choisis une lettre", GC9A01A_COLOR_GREEN,
                GC9A01A_COLOR_OLIVE, 3, 2);
  }
  else if (language == LANG_EN)
  {
    draw_string(80, 20, "ALPHABET", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 4,
                2);

    draw_string(10, 80, "Choose a letter:", GC9A01A_COLOR_GREEN,
                GC9A01A_COLOR_OLIVE, 3, 2);
  }
  alphabet_choice = 0;
  alphabet_state = ALPHABET_LIST;
  app_state = ALPHABET;

  display_alphabet();
}

// l'ecran avec la lettre nen grand
// a modifier pour image
void display_alphabet_letter(void)
{
  char letter[2];
  char file_name[16] = {0};

  letter[0] = alphabet_characters[alphabet_choice];
  letter[1] = '\0';

  /////////////////////////////////////////// nom du fichier

  ft_strcat(file_name, letter);

  if (language == LANG_FR)
  {
    ft_strcat(file_name, "_LSF");
  }
  else
  {
    ft_strcat(file_name, "_BSL");
  }
  ft_strcat(file_name, "   BMP");
  ////////////////////////////////////////////////////////////
  // aller regarder dans la lookup table
  for (uint8_t i = 0; i < IMG_LUT_MAX_SIZE && image_lut[i].address != 0; i++)
  {
    if (!ft_strncmp(file_name, image_lut[i].name, FILE_NAME_SIZE))
    {
      // uart_printstr("Queried file name: ");
      // uart_printstr(file_name);
      // uart_printstr("--\r\nMATCH avec cette image la:\r\nName: ");
      // uart_printstr(image_lut[i].name);
      // uart_printstr("--\r\nAddress: ");
      // uart_printhex_32(image_lut[i].address);
      // uart_printstr("\r\n");
      sd_stream_bmp_to_screen(cluster_to_lba(image_lut[i].address));
    }
  }
}
// manage keyboard in alphabet mode
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

// display letters from A to 0, 4 column
void display_alphabet(void)
{
  uint8_t i;
  uint8_t row;
  uint8_t col;
  uint16_t x;
  uint16_t y;
  char letter[2];

  for (i = 0; i < 36; i++)
  {
    row = i / 4;
    col = i % 4;

    x = 30 + col * 80;   // ecrat entre colonne
    y = 115 + row * 38;  // ecrat entre ligne

    letter[0] = alphabet_characters[i];
    letter[1] = '\0';

    if (i == alphabet_choice)
    {
      draw_string(x, y, letter, GC9A01A_COLOR_WHITE, GC9A01A_COLOR_OLIVE, 5, 2);
    }
    else
    {
      draw_string(x, y, letter, GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 3, 2);
    }
  }
}

// when cursor is moving the actual letter is bigger than the others
// this function is to update the new letter selected bigger and old smaller
void update_alphabet_cursor(uint8_t old_choice)
{
  uint8_t old_row;
  uint8_t old_col;
  uint8_t new_row;
  uint8_t new_col;

  uint16_t old_x;
  uint16_t old_y;
  uint16_t new_x;
  uint16_t new_y;

  char old_letter[2];
  char new_letter[2];

  old_row = old_choice / 4;
  old_col = old_choice % 4;

  new_row = alphabet_choice / 4;
  new_col = alphabet_choice % 4;

  old_x = 30 + old_col * 80;
  old_y = 115 + old_row * 38;

  new_x = 30 + new_col * 80;
  new_y = 115 + new_row * 38;

  old_letter[0] = alphabet_characters[old_choice];
  old_letter[1] = '\0';

  new_letter[0] = alphabet_characters[alphabet_choice];
  new_letter[1] = '\0';

  // Effacer complètement l'ancienne grosse lettre
  draw_string(old_x, old_y, old_letter, GC9A01A_COLOR_OLIVE,
              GC9A01A_COLOR_OLIVE, 5, 2);

  // Redessiner l'ancienne lettre en petit
  draw_string(old_x, old_y, old_letter, GC9A01A_COLOR_GREEN,
              GC9A01A_COLOR_OLIVE, 3, 2);

  // Nouvelle lettre en grand
  draw_string(new_x, new_y, new_letter, GC9A01A_COLOR_WHITE,
              GC9A01A_COLOR_OLIVE, 5, 2);
}
