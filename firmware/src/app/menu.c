#include "app/menu.h"

#include "display/GC9A01.h"
#include "display/ili9488.h"
#include "display/screen_text.h"
#include "storage/fatfs.h"
#include "storage/sd_streaming.h"
#include "system/globals.h"
#include "system/uart.h"
#include "utils/keyboard_utils.h"
#include "utils/utils.h"

// to display the different choicies
void show_menu(void)
{
  ili9488_fill_screen(BG_COLOUR);
  if (language == LANG_FR)
  {
    draw_string(100, 20, "Menu", FG_COLOUR, BG_COLOUR, 5, 2);

    if (menu_choice == 0)
    {
      draw_string(10, 120, ">", ACCENT_COLOUR, BG_COLOUR, 4, 0);
    }
    draw_string(60, 120, "Traduction", FG_COLOUR, BG_COLOUR, 4, 2);

    if (menu_choice == 1)
    {
      draw_string(10, 220, ">", ACCENT_COLOUR, BG_COLOUR, 4, 0);
    }
    draw_string(60, 220, "Alphabet", FG_COLOUR, BG_COLOUR, 4, 2);

    if (menu_choice == 2)
    {
      draw_string(10, 320, ">", ACCENT_COLOUR, BG_COLOUR, 4, 0);
    }
    draw_string(60, 320, "Jeux", FG_COLOUR, BG_COLOUR, 4, 2);
  }
  else if (language == LANG_EN)
  {  // POUR ANGLAIS
    draw_string(100, 20, "Menu", FG_COLOUR, BG_COLOUR, 5, 2);

    if (menu_choice == 0)
    {
      draw_string(10, 120, ">", ACCENT_COLOUR, BG_COLOUR, 4, 0);
    }
    draw_string(60, 120, "Translation", FG_COLOUR, BG_COLOUR, 4, 2);
    if (menu_choice == 1)
    {
      draw_string(10, 220, ">", ACCENT_COLOUR, BG_COLOUR, 4, 0);
    }
    draw_string(60, 220, "Alphabet", FG_COLOUR, BG_COLOUR, 4, 2);

    if (menu_choice == 2)
    {
      draw_string(10, 320, ">", ACCENT_COLOUR, BG_COLOUR, 4, 0);
    }
    draw_string(60, 320, "Games", FG_COLOUR, BG_COLOUR, 4, 2);
  }

  app_state = MENU;
}

void update_menu_cursor(uint8_t old_choice)
{
  // on enelve lancien curseur et on met un espace
  if (old_choice == 0)
  {
    draw_string(10, 120, "  ", FG_COLOUR, BG_COLOUR, 4, 2);
  }
  else if (old_choice == 1)
  {
    draw_string(10, 220, "  ", FG_COLOUR, BG_COLOUR, 4, 2);
  }
  else
  {
    draw_string(10, 320, "  ", FG_COLOUR, BG_COLOUR, 4, 2);
  }

  // nouveau curseur
  if (menu_choice == 0)
  {
    draw_string(10, 120, "> ", ACCENT_COLOUR, BG_COLOUR, 4, 2);
  }
  else if (menu_choice == 1)
  {
    draw_string(10, 220, "> ", ACCENT_COLOUR, BG_COLOUR, 4, 2);
  }
  else
  {
    draw_string(10, 320, "> ", ACCENT_COLOUR, BG_COLOUR, 4, 2);
  }
}

void show_bonjour()
{
  uart_printstr("PRINT: BONJOUR IMAGE\n\r");
  char file_name[16] = {0};

  ft_strcat(file_name, "BONJOUR BMP");

  for (uint8_t i = 0; i < IMG_LUT_MAX_SIZE && image_lut[i].address != 0; i++)
  {
    if (!ft_strncmp(file_name, image_lut[i].name, FILE_NAME_SIZE))
    {
      sd_stream_bmp_to_screen(cluster_to_lba(image_lut[i].address));
      break;
    }
  }
}

void show_hello()
{
  uart_printstr("PRINT: HELLO IMAGE\n\r");
  char file_name[16] = {0};

  ft_strcat(file_name, "HELLO   BMP");

  for (uint8_t i = 0; i < IMG_LUT_MAX_SIZE && image_lut[i].address != 0; i++)
  {
    if (!ft_strncmp(file_name, image_lut[i].name, FILE_NAME_SIZE))
    {
      sd_stream_bmp_to_screen(cluster_to_lba(image_lut[i].address));
      break;
    }
  }
}
