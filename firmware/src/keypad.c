
#include "keyboard_utils.h"
#include "keypad.h"


void keypad_init(void)
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

// select row one by one to find where is the key
void select_row(uint8_t row)
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

// read column to find where is the key
int read_column(void)
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

// return the key after reading row and column
int keypad_read(void)
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