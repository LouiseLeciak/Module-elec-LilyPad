#include "display/GC9A01.h"
#include "display/main_screen.h"
#include "display/screen_text.h"
#include "system/globals.h"
#include "utils/keyboard_utils.h"

// to display the different choicies
void show_menu(void)
{
  ili9488_fill_screen(GC9A01A_COLOR_OLIVE);
  if (language == LANG_FR)
  {
    draw_string(100, 20, "Menu", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 5,
                2);

    if (menu_choice == 0)
    {
      draw_string(10, 120, "> Traduction", GC9A01A_COLOR_GREEN,
                  GC9A01A_COLOR_OLIVE, 4, 2);
    }
    else
    {
      draw_string(10, 120, "  Traduction", GC9A01A_COLOR_GREEN,
                  GC9A01A_COLOR_OLIVE, 4, 2);
    }

    if (menu_choice == 1)
    {
      draw_string(10, 220, "> Alphabet", GC9A01A_COLOR_GREEN,
                  GC9A01A_COLOR_OLIVE, 4, 2);
    }
    else
    {
      draw_string(10, 220, "  Alphabet", GC9A01A_COLOR_GREEN,
                  GC9A01A_COLOR_OLIVE, 4, 2);
    }

    if (menu_choice == 2)
    {
      draw_string(10, 320, "> Jeu", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 4,
                  2);
    }
    else
    {
      draw_string(10, 320, "  Jeu", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 4,
                  2);
    }
  }
  else if (language == LANG_EN)
  {  // POUR ANGLAIS
    draw_string(100, 20, "Menu", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 5,
                2);

    if (menu_choice == 0)
    {
      draw_string(10, 120, "> Traduction", GC9A01A_COLOR_GREEN,
                  GC9A01A_COLOR_OLIVE, 4, 2);
    }
    else
    {
      draw_string(10, 120, "  Traduction", GC9A01A_COLOR_GREEN,
                  GC9A01A_COLOR_OLIVE, 4, 2);
    }

    if (menu_choice == 1)
    {
      draw_string(10, 220, "> Alphabet", GC9A01A_COLOR_GREEN,
                  GC9A01A_COLOR_OLIVE, 4, 2);
    }
    else
    {
      draw_string(10, 220, "  Alphabet", GC9A01A_COLOR_GREEN,
                  GC9A01A_COLOR_OLIVE, 4, 2);
    }

    if (menu_choice == 2)
    {
      draw_string(10, 320, "> Game", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE,
                  4, 2);
    }
    else
    {
      draw_string(10, 320, "  Game", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE,
                  4, 2);
    }
  }

  app_state = MENU;
}

void update_menu_cursor(uint8_t old_choice)
{
  // on enelve lancien curseur et on met un espace
  if (old_choice == 0)
  {
    draw_string(10, 120, "  ", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 4, 2);
  }
  else if (old_choice == 1)
  {
    draw_string(10, 220, "  ", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 4, 2);
  }
  else
  {
    draw_string(10, 320, "  ", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 4, 2);
  }

  // nouveau curseur
  if (menu_choice == 0)
  {
    draw_string(10, 120, "> ", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 4, 2);
  }
  else if (menu_choice == 1)
  {
    draw_string(10, 220, "> ", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 4, 2);
  }
  else
  {
    draw_string(10, 320, "> ", GC9A01A_COLOR_GREEN, GC9A01A_COLOR_OLIVE, 4, 2);
  }
}
