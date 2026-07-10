
#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdbool.h>

#define BAUD 115200
// p182 table 20-1
#define MYUBRR ((F_CPU / (8 * BAUD)) - 1)

#define WRITE 0
#define READ 1


#define MCP23017_ADDR 0x20 // A0/A1/A2 = GND
#define MCP_IODIRA 0x00 // pour config les pin GPA0-7 
#define MCP_GPIOA 0x12 // lit ou ecrit l'etat actuel des pin GPA
#define MCP_GPPUA 0x0C // pour les resistances de pull up

void uart_init();
void uart_tx(char c);
char uart_rx(void);
void uart_printstr(const char *str);
void uart_printhex(uint8_t value);
void uart_printint(int val);

void draw_menu(void);

void i2c_init(void);
void i2c_write(unsigned char data);
uint8_t i2c_start(uint8_t addr);
void i2c_stop(void);
void print_hex_value(char c);
uint8_t i2c_read_byte(void);

void mcp_init(void);
void mcp_write_register(uint8_t reg, uint8_t value);
uint8_t mcp_read_register(uint8_t reg);

#endif