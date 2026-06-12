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
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}


#define BUTTON_PIN 4

int main(void)
{
    uart_init();

    uart_printstr("Test bouton AVR\r\n");

    // bouton en pull-up
    ft_pinMode(BUTTON_PIN, INPUT_PULLUP);

    while (1)
    {
        uint8_t state = ft_digitalRead(BUTTON_PIN);

        if (state == LOW)
        {
            uart_printstr("BOUTON PRESSE\r\n");
        }
        else
        {
            uart_printstr("RELACHE\r\n");
        }

        _delay_ms(200);
    }
}

