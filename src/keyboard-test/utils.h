
#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdbool.h>

#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)

#define READ 0
#define WRITE 1

#define AHT20_ADDR_W (0b01110000)
#define AHT20_ADDR_R (0b01110001)


void uart_init();
void uart_tx(char c);
char uart_rx(void);
void uart_printstr(const char* str);
void uart_printhex(uint8_t value);
void uart_printint(int val);

void draw_menu(void);

void i2c_init(void);
void i2c_start(uint8_t state);
void i2c_stop(void);
void i2c_write(unsigned char data);
void i2c_read(void);



#endif