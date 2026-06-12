#include "../headers/uart.h"

#define INPUT 3
#define OUTPUT 4
#define INPUT_PULLUP 2
#define HIGH 1
#define LOW 0

void ft_pinMode(uint8_t pin, uint8_t mode)
{
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
        else if (mode == INPUT_PULLUP)
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
    if (pin <= 7)
    {
        if (PIND & (1 << pin))
            return 1;
        return 0;
    }
    return 0;
}



// ligne pour le keyboard
#define ROW0 2
#define ROW1 3

// colonnes pour le keyboard
#define COL0 4
#define COL1 5
#define COL2 6

void keypad_init(void)
{
    // en output parce qu'on veut recevoir des infos
    ft_pinMode(ROW0, OUTPUT);
    ft_pinMode(ROW1, OUTPUT);

    ft_pinMode(COL0, INPUT_PULLUP);
    ft_pinMode(COL1, INPUT_PULLUP);
    ft_pinMode(COL2, INPUT_PULLUP);

    // on remt les lignes a 0
    ft_digitalWrite(ROW0, HIGH);
    ft_digitalWrite(ROW1, HIGH);
}

int keypad_read(void)
{
    // on scan la premiere ligne
    ft_digitalWrite(ROW0, LOW);
    ft_digitalWrite(ROW1, HIGH);

    // low ca veut dire qu'on a appuye
    if (ft_digitalRead(COL0) == LOW)
        return 0;

    if (ft_digitalRead(COL1) == LOW)
        return 1;

    if (ft_digitalRead(COL2) == LOW)
        return 2;

    // on scan la deuxieme ligne
    ft_digitalWrite(ROW0, HIGH);
    ft_digitalWrite(ROW1, LOW);

    if (ft_digitalRead(COL0) == LOW)
        return 3;

    if (ft_digitalRead(COL1) == LOW)
        return 4;

    if (ft_digitalRead(COL2) == LOW)
        return 5;

    return -1;
}

int main(void)
{
    int key;

    uart_init();
    uart_printstr("Test matrice 2x3\r\n");

    keypad_init();

    while (1)
    {
        key = keypad_read();

        if (key >= 0)
        {

            if (key == 0)
                uart_printstr("A");
            else if (key == 1)
                uart_printstr("B");
            else if (key == 2)
                uart_printstr("C");
            else if (key == 3)
                uart_printstr("O");
            else if (key == 4)
                uart_printstr("U");
            else if (key == 5)
                uart_printstr("\r\n");


            _delay_ms(200); // debouncing de boloss
        }
    }
}