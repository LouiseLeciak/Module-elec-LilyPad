
#include "../headers/uart.h"
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define HIGH 3
#define LOW 4

// screen /dev/ttyUSB0 115200
// organisation des boutons, ligne et colonnes
// https://github.com/EanNewton/Awesome-Keebs/blob/main/tutorials/How%20to%20make%20a%20keyboard%20-%20the%20matrix.md
// https://hilpcb.com/fr/blog/keyboard-matrix-design/
// les lignes, a activer 1 par 1
// je commence par 2 car sur la arduino que j'utilise
// pour mon test a les pins 0 et 1 deja utilisee
const int rowPins[2] = {2, 3};

// les colonnes
const int colPins[3] = {4, 5, 6};

// Tableau pour dire quel bouton est quel lettre en gros
// pratique si je veux changer les lettres
char keys[2][3] = {
    {'A', 'B', 'C'},
    {'D', 'E', 'F'}};

void ft_pinMode(uint8_t pin, uint8_t mode)
{

    // du 0 a 7 sur la arduino uno c;est le port D
    if (pin <= 7)
    {
        if (mode == OUTPUT)
        {
            DDRD |= (1 << pin);
        }
        else if (mode == INPUT)
        {
            DDRD &= ~(1 << pin);
            PORTD &= ~(1 << pin);
        }
        if (mode == INPUT_PULLUP)
        {
            DDRD &= ~(1 << pin);
            PORTD |= (1 << pin);
        }
    }
}

void ft_digitalWrite(uint8_t pin, uint8_t value)
{

    if (pin <= 7)
    {
        if (value)
            PORTD |= (1 << pin);
        else
            PORTD &= ~(1 << pin);
    }
}

uint8_t ft_digitalRead(uint8_t pin)
{

    uint8_t value;

    if (pin <= 7)
    {
        if (PIND & (1 << pin))
            value = 1;
        else
            value = 0;
        return value;
    }

    return 0;
}

void setup()
{

    uart_init();

    uart_printstr("Coucou je suis connecte et c'est super !\r\n");

    for (int i = 0; i < 2; i++)
    {

        // je veux controler ce pin et pouvoir le mettre a low ou high
        // en gros output = je veux parler
        // on le fait que sur les lignes parce que on veut scanner les lignes
        ft_pinMode(rowPins[i], OUTPUT);

        // HIGH = inactif
        ft_digitalWrite(rowPins[i], HIGH);
    }

    for (int i = 0; i < 3; i++)
    {

        // set up colonne
        // pull-up = HIGH desactive
        // LOW = bouton active
        // input le keyboard doit ecouter
        ft_pinMode(colPins[i], INPUT_PULLUP);
    }
}

char read_keyboard()
{

    // on check d'abord les lignes
    for (int row = 0; row < 2; row++)
    {

        // on active une ligne par une ligne
        ft_digitalWrite(rowPins[row], LOW);
        // LOW = courant envoye sur la ligne donc active

        // ensuite on check les colonnes
        for (int col = 0; col < 3; col++)
        {

            // si colonne est LOW alors le bouton est appuye
            if (ft_digitalRead(colPins[col]) == LOW)
            {
                uart_printstr("DANS READ KEYBOARD\r\n");

                // debounce un peu sale
                _delay_ms(30);

                // while temporaire pour attendre le temps que l'user relache le boutpon
                while (ft_digitalRead(colPins[col]) == LOW)
                    ;

                // je desac ma ligne
                ft_digitalWrite(rowPins[row], HIGH);

                // je return mon bouton/ ma touche
                return keys[row][col];
            }
        }

        // je desac la ligne actuelle avant de tester la prochaine
        ft_digitalWrite(rowPins[row], HIGH);
    }

    // pas de touche
    // pas sure que return '\0' soit la bonne strat ceci dit
    return '\0';
}

int main()
{
    uart_init();
    setup();
    while (1)
    {
        char key = read_keyboard();

        // si on appui sur une touche
        if (key != '\0')
        {
            uart_printstr("Key: ");
            // uart_tx(key);
            uart_printstr("bouton\r\n");
        }
    }
}
