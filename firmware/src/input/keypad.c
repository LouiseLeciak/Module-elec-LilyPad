#include "input/keypad.h"

#include <util/delay.h>

void keypad_init(void)
{
  // les 4 lignes, elles deviennent des sorties
  // 1 sortie 0 entree
  DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3);
  PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3);

  // les colonnes, on les met a 0
  // car on veut els lire
  DDRC &= ~((1 << PC4) | (1 << PC5) | (1 << PC6) | (1 << PC7));
  // pour les pull up
  PORTC |= ((1 << PC4) | (1 << PC5) | (1 << PC6) | (1 << PC7));


  DDRJ &= ~((1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3) | (1 << PJ4) | (1 << PJ5));

  PORTJ |= ((1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3) | (1 << PJ4) | (1 << PJ5));

}

// select row one by one to find where is the key
void select_row(uint8_t row)
{
  // je desactive toutes les lignes
  PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3)

  // j'active une seule lgine poru "monitorer"
  // j;active celle envoye en parametre
  switch (row)
  {
    case 0:
      PORTC &= ~(1 << PC0);  // R1
      break;

    case 1:
      PORTC &= ~(1 << PC1);  // R2
      break;

    case 2:
      PORTC &= ~(1 << PC2);  // R3
      break;

    case 3:
      PORTC &= ~(1 << PC3);  // R4
      break;
  }
}

// read column to find where is the key
int read_column(void)
{
  // si PINF = 0 alors c'est que c'est presse
  if (!(PINC & (1 << PC4)))
    return 0;
  if (!(PINC & (1 << PC5)))
    return 1;
  if (!(PINC & (1 << PC6)))
    return 2;
  if (!(PINC & (1 << PC7)))
    return 3;
  if (!(PINJ & (1 << PJ0)))
    return 4;
  if (!(PINJ & (1 << PJ1)))
    return 5;
  if (!(PINJ & (1 << PJ2)))
    return 6;
  if (!(PINJ & (1 << PJ3)))
    return 7;
  if (!(PINJ & (1 << PJ4)))
    return 8;
  if (!(PINJ & (1 << PJ5)))
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
