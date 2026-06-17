#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"

// DANS LE PINOUT
// Colonnes
//
// C1  -> PA3
// C2  -> PA4
// C3  -> PA5
// C4  -> PA6
// C5  -> PA7
// C6  -> PG2
// C7  -> PJ6
// C8  -> PJ5
// C9  -> PJ4
// C10 -> PJ3
//
// Lignes
//
// R1 -> PJ2
// R2 -> PJ1
// R3 -> PJ0
// R4 -> PC7
//

#define ROWS_NB 4
#define COLS_NB 10


// comme ca juste a donner la position et renvois 
// la lettre qui va avec
static const char keymap[ROWS_NB][COLS_NB] =
{
    {'1','2','3','4','5','6','7','8','9','0'},
    {'Q','W','E','R','T','Y','U','I','O','P'},
    {'A','S','D','F','G','H','J','K','L','\n'},
    {'Z','X','C','V','B','N','M','\0','\0','\0'}
};

static void keypad_init(void)
{
    // les 4 lignes, elles deviennent des sorties
    // 1 sortie 0 entree
    DDRJ |= (1 << PJ2) | (1 << PJ1) | (1 << PJ0);
    DDRC |= (1 << PC7);

    PORTJ |= (1 << PJ2) | (1 << PJ1) | (1 << PJ0);
    PORTC |= (1 << PC7);

    // les colonnes, on les met a 0
    // car on veut els lire
    DDRA &= ~(
        (1 << PA3) |
        (1 << PA4) |
        (1 << PA5) |
        (1 << PA6) |
        (1 << PA7)
    );

    // pour les pull up
    PORTA |= (
        (1 << PA3) |
        (1 << PA4) |
        (1 << PA5) |
        (1 << PA6) |
        (1 << PA7)
    );

    DDRG &= ~(1 << PG2);
    PORTG |= (1 << PG2);

    DDRJ &= ~(
        (1 << PJ6) |
        (1 << PJ5) |
        (1 << PJ4) |
        (1 << PJ3)
    );

    PORTJ |= (
        (1 << PJ6) |
        (1 << PJ5) |
        (1 << PJ4) |
        (1 << PJ3)
    );
}

// je veux selectionner qu'une seule ligne a la fois
static void select_row(uint8_t row)
{
    // je desactive toutes les lignes
    PORTJ |= (1 << PJ2) | (1 << PJ1) | (1 << PJ0);
    PORTC |= (1 << PC7);

    // j'active une seule lgine poru "monitorer"
    // j;active celle envoye en parametre
    switch (row)
    {
        case 0:
            PORTJ &= ~(1 << PJ2);
            break;

        case 1:
            PORTJ &= ~(1 << PJ1);
            break;

        case 2:
            PORTJ &= ~(1 << PJ0);
            break;

        case 3:
            PORTC &= ~(1 << PC7);
            break;
    }
}

static int read_column(void)
{
    // si PINA/PING/PINJ = 0 alors c'est que c'est presse
    if (!(PINA & (1 << PA3))) return 0;
    if (!(PINA & (1 << PA4))) return 1;
    if (!(PINA & (1 << PA5))) return 2;
    if (!(PINA & (1 << PA6))) return 3;
    if (!(PINA & (1 << PA7))) return 4;

    if (!(PING & (1 << PG2))) return 5;

    if (!(PINJ & (1 << PJ6))) return 6;
    if (!(PINJ & (1 << PJ5))) return 7;
    if (!(PINJ & (1 << PJ4))) return 8;
    if (!(PINJ & (1 << PJ3))) return 9;

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

int main(void)
{
    int key;
    uint8_t row;
    uint8_t col;

    uart_init();

    uart_printstr("Keyboard 4x10 ready\r\n");

    keypad_init();

    while (1)
    {
        key = keypad_read();

        if (key >= 0)
        {
            // ici je recuper la dizaine et l'unite
            // comme je renvois en version aditione dans keypad read
            // donc si on reprend l'exemple de 23 comme a la ligne 151
            // on recupere bien row = 2 et col = 3
            row = key / COLS_NB;
            col = key % COLS_NB;

            uart_print_str(keymap[row][col]);

            // debounce
            _delay_ms(20);

            // le temps que ca relache le bouton
            // mais a voir si on autorise ou pas le fait
            // de pouvoir rester appuyer pour que ca fasse AAAAAA
            while (keypad_read() >= 0)
                ;
        }
    }

    return (0);
}