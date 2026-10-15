#include "input/keypad.h"

#include <util/delay.h>

#include "pinout.h"

void keypad_init(void)
{
  // les 4 lignes, elles deviennent des sorties
  // 1 sortie 0 entree
  DDRC |= KB_R1 | KB_R2 | KB_R3 | KB_R4;
  PORTC |= KB_R1 | KB_R2 | KB_R3 | KB_R4;

  // les colonnes, on les met en entree avec pull up
  // car on veut les lire
  DDRC &= ~(KB_C1 | KB_C2 | KB_C3 | KB_C4);
  PORTC |= KB_C1 | KB_C2 | KB_C3 | KB_C4;

  DDRJ &= ~(KB_C5 | KB_C6 | KB_C7 | KB_C8 | KB_C9 | KB_C10);
  PORTJ |= KB_C5 | KB_C6 | KB_C7 | KB_C8 | KB_C9 | KB_C10;
}

// select row one by one to find where is the key
void select_row(uint8_t row)
{
  // je desactive toutes les lignes
  PORTC |= KB_R1 | KB_R2 | KB_R3 | KB_R4;

  // j'active une seule ligne pour "monitorer"
  // j'active celle envoye en parametre
  switch (row)
  {
    case 0:
      PORTC &= ~KB_R1;
      break;

    case 1:
      PORTC &= ~KB_R2;
      break;

    case 2:
      PORTC &= ~KB_R3;
      break;

    case 3:
      PORTC &= ~KB_R4;
      break;
  }
}

// read column to find where is the key
int read_column(void)
{
  // si PINx = 0 alors c'est que c'est presse
  if (!(PINC & KB_C1))
    return 0;
  if (!(PINC & KB_C2))
    return 1;
  if (!(PINC & KB_C3))
    return 2;
  if (!(PINC & KB_C4))
    return 3;
  if (!(PINJ & KB_C5))
    return 4;
  if (!(PINJ & KB_C6))
    return 5;
  if (!(PINJ & KB_C7))
    return 6;
  if (!(PINJ & KB_C8))
    return 7;
  if (!(PINJ & KB_C9))
    return 8;
  if (!(PINJ & KB_C10))
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
