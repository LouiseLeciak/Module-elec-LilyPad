#include <util/delay.h>

#include "GC9A01.h"
#include "main_screen.h"
#include "pinout.h"
#include "spi.h"
#include "state_machine.h"
#include "uart.h"
#include "eye_imgs.h"
#include "screen_text.h"

// void testPINS(void);
// void GC9A01_fillScreen(uint16_t color, uint8_t screen);
#define WORD_MAX_LEN 20
#define ROWS_NB 4
#define COLS_NB 10
#define MENU_SIZE 3

static char word[WORD_MAX_LEN + 1];

static uint8_t menu_choice = 0; // savoir ou on est dans le menu
// 0 = Traduction
// 1 = Alphabet
// 2 = Jeu

// pour checker si on valide un mot ou pas
static uint8_t word_len = 0;

typedef enum
{
  INPUT,
  VALIDATED
} word_state_t;

typedef enum
{
  MENU,
  TRADUCTION,
  ALPHABET,
  JEU
} app_state_t;

static app_state_t app_state = MENU;

//////////////////////

static word_state_t word_state = INPUT;

t_state current_state = INIT;

void sd_init() { DDRH |= (SD_CS); }

void eyes_init()
{

  DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
  GC9A01_init(LEFT_EYE);
  GC9A01_init(RIGHT_EYE);
}

void screens_init()
{
  DDRH |= (SCREENS_DC);
  main_screen_init();
  eyes_init();
}

void rotary_encoder_init(void) { DDRC |= (SDL_SW1 | SDL_SW2 | SDL_SW3); }

void init(void)
{

  spi_master_init();
  uart_init(MYUBRR);

  screens_init();
  // sd_init();
  // keyboard_init();
}

///////////////////////////////////////////////////////////////////////
////////////////////////// ROTARY ENCODER//////////////////////////////
///////////////////////////////////////////////////////////////////////
#include "utils.h"

#define ROTARY_CLK 6
#define ROTARY_SW 5
#define ROTARY_DT 7

static uint8_t eye_state = 0;
static uint8_t prev_sw; // etat precedent du bouton
static uint8_t rotaryclk_prev = 1;

void change_eye(void)
{
  if (eye_state == 0)
  {
    GC9A01_blink(Eye_look_Right, 1);
    eye_state = 1;
  }
  else
  {
    GC9A01_blink(Eye_Front, 1);
    eye_state = 0;
  }
}

// Pour commencer un nouveau mot
static void start_new_word(void)
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
static void validate_word(void)
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

static void rotary_init(void)
{
  uint8_t gpio;

  gpio = mcp_read_register(MCP_GPIOA);

  rotaryclk_prev = (gpio >> ROTARY_CLK) & 1;
  prev_sw = (gpio >> ROTARY_SW) & 1;
}

// to display the different choicies
static void show_menu(void)
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

static void update_menu_cursor(uint8_t old_choice)
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

// gestion de la rotation
void rotary_update(void)
{
  uint8_t gpio;
  uint8_t clk;
  uint8_t dt;
  uint8_t old_choice;

  gpio = mcp_read_register(MCP_GPIOA);

  clk = (gpio >> ROTARY_CLK) & 1;

  if (rotaryclk_prev == 1 && clk == 0)
  {
    dt = (gpio >> ROTARY_DT) & 1;

    if (app_state == MENU)
    {
      old_choice = menu_choice;

      //sens du tournage
      if (dt != clk)
      {
        menu_choice++;

        if (menu_choice >= MENU_SIZE)
        {
          menu_choice = 0;
        }
      }
      else
      {
        if (menu_choice == 0)
        {
          menu_choice = MENU_SIZE - 1;
        }
        else
        {
          menu_choice--;
        }
      }

      update_menu_cursor(old_choice);
    }
  }

  rotaryclk_prev = clk;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////// KEYBOARD ////////////////////////////////
///////////////////////////////////////////////////////////////////////

// comme ca juste a donner la position et renvois
// la lettre qui va avec
static const char keymap[ROWS_NB][COLS_NB] =
    {
        {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
        {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
        {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'd'},  // enter
        {'Z', 'X', 'C', 'V', 'B', 'N', 'M', '#', 'i', 'p'}}; // add supp and home

static void keypad_init(void)
{
  // les 4 lignes, elles deviennent des sorties
  // 1 sortie 0 entree
  DDRJ |= (1 << PJ0) | (1 << PJ1) | (1 << PJ2);
  PORTJ |= (1 << PJ0) | (1 << PJ1) | (1 << PJ2);

  DDRC |= (1 << PC7);
  PORTC |= (1 << PC7);

  // les colonnes, on les met a 0
  // car on veut els lire
  DDRA &= ~(
      (1 << PA3) |
      (1 << PA4) |
      (1 << PA5) |
      (1 << PA6) |
      (1 << PA7));
  // pour les pull up
  PORTA |= ((1 << PA3) |
            (1 << PA4) |
            (1 << PA5) |
            (1 << PA6) |
            (1 << PA7));

  DDRG &= ~(1 << PG2);
  PORTG |= (1 << PG2);

  DDRJ &= ~(
      (1 << PJ3) |
      (1 << PJ4) |
      (1 << PJ5) |
      (1 << PJ6));

  PORTJ |= ((1 << PJ3) |
            (1 << PJ4) |
            (1 << PJ5) |
            (1 << PJ6));

  // // on check comment on est pour savoir comment changer
  // if (PINC & (1 << ROTARY_CLK))
  //     rotaryclk_prev = 1;
  // else
  //     rotaryclk_prev = 0;
}

// je veux selectionner qu'une seule ligne a la fois
static void select_row(uint8_t row)
{
  // je desactive toutes les lignes
  PORTJ |= (1 << PJ0) | (1 << PJ1) | (1 << PJ2);
  PORTC |= (1 << PC7);

  // j'active une seule lgine poru "monitorer"
  // j;active celle envoye en parametre
  switch (row)
  {
  case 0:
    PORTJ &= ~(1 << PJ2); // R1
    break;

  case 1:
    PORTJ &= ~(1 << PJ1); // R2
    break;

  case 2:
    PORTJ &= ~(1 << PJ0); // R3
    break;

  case 3:
    PORTC &= ~(1 << PC7); // R4
    break;
  }
}

static int read_column(void)
{
  // si PINF = 0 alors c'est que c'est presse
  if (!(PINA & (1 << PA3)))
    return 0;
  if (!(PINA & (1 << PA4)))
    return 1;
  if (!(PINA & (1 << PA5)))
    return 2;
  if (!(PINA & (1 << PA6)))
    return 3;
  if (!(PINA & (1 << PA7)))
    return 4;
  if (!(PING & (1 << PG2)))
    return 5;
  if (!(PINJ & (1 << PJ6)))
    return 6;
  if (!(PINJ & (1 << PJ5)))
    return 7;
  if (!(PINJ & (1 << PJ4)))
    return 8;
  if (!(PINJ & (1 << PJ3)))
    return 9;

  return -1;
}

static int keypad_read(void)
{
  int row;
  int col;

  for (row = 0; row < ROWS_NB; row++)
  {
    select_row(row);

    _delay_us(5);

    col = read_column();

    if (col >= 0)
      return (row * COLS_NB + col);
    // en gros par exemple si c'est r2 c3
    // bah ca return 23, ca m'evite de reeefaire un tableau
    // a return etc
  }

  return -1;
}

///////////////////////////////////////////////////////////////////////
//////////////////////////////AFFICHAGE////////////////////////////////
///////////////////////////////////////////////////////////////////////

// le menu entre un mot
static void start_translation(void)
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
static void show_alphabet(void)
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
static void show_game(void)
{
  ili9488_fill_screen(GC9A01A_COLOR_PINK);

  draw_string(
      10, 20, "Choix : jeu",
      GC9A01A_COLOR_PURPLE,
      GC9A01A_COLOR_PINK,
      3, 2);

  app_state = JEU;
}

// static void select_menu_choice(void)
// {
//   if (menu_choice == 0)
//   {
//     start_translation();
//   }
//   else if (menu_choice == 1)
//   {
//     show_alphabet();
//   }
//   else if (menu_choice == 2)
//   {
//     show_game();
//   }
// }

// ou on ets dans le menu, valider ou pas
void rotary_button_update(void)
{
  uint8_t gpio;
  uint8_t sw;

  gpio = mcp_read_register(MCP_GPIOA);
  sw = (gpio >> ROTARY_SW) & 1;

  if (sw != prev_sw)
  {
    _delay_ms(5);

    gpio = mcp_read_register(MCP_GPIOA);
    sw = (gpio >> ROTARY_SW) & 1;

    if (sw != prev_sw)
    {
      if (sw == 0)
      {
        if (app_state == MENU)
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
        else if (app_state == TRADUCTION)
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
      }

      prev_sw = sw;
    }
  }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////// MAIN ////////////////////////////////////
///////////////////////////////////////////////////////////////////////

int main(void)
{
  init();

  i2c_init();
  mcp_init();

  keypad_init();
  rotary_init();

  show_menu();

  while (1)
  {
    rotary_update();
    rotary_button_update();

    if (app_state == TRADUCTION && word_state == INPUT)
    {
      int key = keypad_read();

      if (key >= 0)
      {
        uint8_t row = key / COLS_NB;
        uint8_t col = key % COLS_NB;
        char c = keymap[row][col];

        if (c == '#')
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

    else if (app_state == ALPHABET)
    {
      int key = keypad_read();

      if (key >= 0)
      {
        uint8_t row = key / COLS_NB;
        uint8_t col = key % COLS_NB;
        char c = keymap[row][col];

        if (c == '#')
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

    else if (app_state == JEU)
    {
      int key = keypad_read();

      if (key >= 0)
      {
        uint8_t row = key / COLS_NB;
        uint8_t col = key % COLS_NB;
        char c = keymap[row][col];

        if (c == '#')
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
  }

  return 0;
}