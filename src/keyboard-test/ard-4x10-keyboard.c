#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"

// sur la arduino du lab les pins
// sont orga un peu diff
// et certains de nos pins sont innaccessibles
// donc j'utilise d'autres pin pour la version proto
// Colonnes
//
// C1  -> PF0
// C2  -> PF1
// C3  -> PF2
// C4  -> PF3
// C5  -> PF4
// C6  -> PF5
// C7  -> PF6
// C8  -> PF7
// C9  -> PK0
// C10 -> PK1
//
// Lignes
//
// R1 -> PK2
// R2 -> PK3
// R3 -> PK4
// R4 -> PK5
//
// pour le rotary
// CLK -> PC6
// DT -> PC5
// SW -> PC4

#define ROWS_NB 4
#define COLS_NB 10
#define ROTARY_CLK 5
#define ROTARY_SW 6
#define ROTARY_DT 7

static uint8_t rotaryclk_prev;
// static uint8_t sw_prev = 1;
static uint8_t menu_index = 0;

// comme ca juste a donner la position et renvois
// la lettre qui va avec
static const char keymap[ROWS_NB][COLS_NB] =
    {
        {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
        {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
        {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '\n'},   // enter
        {'Z', 'X', 'C', 'V', 'B', 'N', 'M', ' ', '\0', '\0'}}; // add supp and home

const char *menu[] = {
    "OPTION NUMERO 1",
    "OPTION NUMERO 2",
    "OPTION NUMERO 3"};

static void keypad_init(void)
{
    // les 4 lignes, elles deviennent des sorties
    // 1 sortie 0 entree
    DDRK |= (1 << PK2) | (1 << PK3) | (1 << PK4) | (1 << PK5);

    PORTK |= (1 << PK2) | (1 << PK3) | (1 << PK4) | (1 << PK5);

    // les colonnes, on les met a 0
    // car on veut els lire
    DDRF &= ~(
        (1 << PF0) |
        (1 << PF1) |
        (1 << PF2) |
        (1 << PF3) |
        (1 << PF4) |
        (1 << PF5) |
        (1 << PF6) |
        (1 << PF7));

    // pour les pull up
    PORTF |= ((1 << PF0) |
              (1 << PF1) |
              (1 << PF2) |
              (1 << PF3) |
              (1 << PF4) |
              (1 << PF5) |
              (1 << PF6) |
              (1 << PF7));

    DDRK &= ~(
        (1 << PK0) |
        (1 << PK1));

    PORTK |= ((1 << PK0) |
              (1 << PK1));

    // pour le rotary encoder
    DDRC &= ~(
        (1 << ROTARY_CLK) |
        (1 << ROTARY_DT) |
        (1 << ROTARY_SW));

    PORTC |= ((1 << ROTARY_CLK) |
              (1 << ROTARY_DT) |
              (1 << ROTARY_SW));

    // on check comment on est pour savoir comment changer
    if (PINC & (1 << ROTARY_CLK))
        rotaryclk_prev = 1;
    else
        rotaryclk_prev = 0;
}

// je veux selectionner qu'une seule ligne a la fois
static void select_row(uint8_t row)
{
    // je desactive toutes les lignes
    PORTK |= (1 << PK2) | (1 << PK3) | (1 << PK4) | (1 << PK5);

    // j'active une seule lgine poru "monitorer"
    // j;active celle envoye en parametre
    switch (row)
    {
    case 0:
        PORTK &= ~(1 << PK5);
        break;

    case 1:
        PORTK &= ~(1 << PK4);
        break;

    case 2:
        PORTK &= ~(1 << PK3);
        break;

    case 3:
        PORTK &= ~(1 << PK2);
        break;
    }
}

static int read_column(void)
{
    // si PINF = 0 alors c'est que c'est presse
    if (!(PINF & (1 << PF0)))
        return 0;
    if (!(PINF & (1 << PF1)))
        return 1;
    if (!(PINF & (1 << PF2)))
        return 2;
    if (!(PINF & (1 << PF3)))
        return 3;
    if (!(PINF & (1 << PF4)))
        return 4;
    if (!(PINF & (1 << PF5)))
        return 5;
    if (!(PINF & (1 << PF6)))
        return 6;
    if (!(PINF & (1 << PF7)))
        return 7;
    if (!(PINK & (1 << PK0)))
        return 8;
    if (!(PINK & (1 << PK1)))
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

// FONCTION POUR LE ROTARY
void rotary_update(void)
{
    uint8_t clk; // etat actuel de la clock
    uint8_t dt;  // etat actuel de la pin data

    uint8_t gpio = mcp_read_register(MCP_GPIOA);

    clk = (gpio >> ROTARY_CLK) & 1;

    // si la clk a change
    if (rotaryclk_prev == 1 && clk == 0)
    {
        dt = (gpio >> ROTARY_DT) & 1;

        // si dt est diff de clk alors on tourne dans un sens
        if (dt != clk)
        {
            if (menu_index == 0)
                menu_index = 2;
            else
                menu_index--;
            draw_menu();
        }
        // sinon c'est qu'on tourne dans l'autre
        else
        {
            menu_index++;
            if (menu_index > 2)
                menu_index = 0;
            draw_menu();
        }
    }

    rotaryclk_prev = clk;
}

//? Probablement pas opti comme verification, probablement a retaper
void rotary_button_update(void)
{
    static uint8_t prev_sw = 1; // etat precedent du bouton
    uint8_t sw;          // etat actuel

    uint8_t gpio = mcp_read_register(MCP_GPIOA);

    sw = (gpio >> ROTARY_SW) & 1;

    // si l'etat du bouton a change
    if (sw != prev_sw)
    {
        _delay_ms(5);

        gpio = mcp_read_register(MCP_GPIOA);
        sw = (gpio >> ROTARY_SW) & 1;
        // si sw a bien change alors on print des trucs pour le moment en uart
        if (sw != prev_sw)
        {
            if (sw == 0)
            {
                switch (menu_index)
                {
                case 0:
                    uart_printstr("You choose option 1\n\r");
                    break;
                case 1:
                    uart_printstr("You choose option 2\n\r");
                    break;
                case 2:
                    uart_printstr("You choose option 3\n\r");
                    break;
                }
            }
            // et on met a jout le prev ducoup
            prev_sw = sw;
            _delay_ms(20);
        }
    }
}

void draw_menu(void)
{
    uart_printstr("\033[2J"); // ANSI pour effacer l'ecran
    uart_printstr("\033[H");  // ANSI pour se mettre a l1 c1
    uart_printstr("========== MENU ==========\r\n");
    uart_printstr("\r\n");
    for (uint8_t i = 0; i < 3; i++)
    {
        if (i == menu_index)
            uart_printstr("> ");
        else
            uart_printstr("  ");
        uart_printstr(menu[i]);
        uart_printstr("\n\r");
    }
}

#define SWITCH_PIN PL0
#define LED_LEFT PB0  // D53
#define LED_RIGHT PB2 // D51

static void switch_led_init(void)
{
    // LEDs en sortie
    DDRB |= (1 << LED_LEFT) | (1 << LED_RIGHT);

    // Eteindre les LEDs
    PORTB &= ~((1 << LED_LEFT) | (1 << LED_RIGHT));

    // Switch en entree
    DDRL &= ~(1 << SWITCH_PIN);

    // Pas de pull-up
    PORTL &= ~(1 << SWITCH_PIN);
}

static void switch_led_update(void)
{
    if (PINL & (1 << SWITCH_PIN))
    {
        // Le switch est cote 3,3 V
        PORTB |= (1 << LED_LEFT);
        PORTB &= ~(1 << LED_RIGHT);
    }
    else
    {
        // Le switch est cote GND
        PORTB &= ~(1 << LED_LEFT);
        PORTB |= (1 << LED_RIGHT);
    }
}

int main(void)
{
    int key;
    uint8_t row;
    uint8_t col;

    uart_init();

    i2c_init();

    mcp_init();

    //! print un semi menu - 1 mot au clavier, 2 image to mot et 3 jeu
    //! par exemple

    draw_menu();

    keypad_init();

    switch_led_init();

    while (1)
    {
        rotary_update();

        rotary_button_update();

        switch_led_update();

        key = keypad_read();

        if (key >= 0)
        {
            // ici je recuper la dizaine et l'unite
            // comme je renvois en version aditione dans keypad read
            // donc si on reprend l'exemple de 23 comme a la ligne 151
            // on recupere bien row = 2 et col = 3
            row = key / COLS_NB;
            col = key % COLS_NB;

            //! TO ADD comportement dans le cas ou on appuie sur plusieurs touches
            //! en meme temps
            if (keymap[row][col] == '\n')
                uart_printstr("\n\r");
            else
                uart_tx(keymap[row][col]);

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