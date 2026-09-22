
#ifndef I2C_ROTARY_H
#define I2C_ROTARY_H

#include <avr/io.h>

// i2c - rotary encoder
void i2c_init(void);
void i2c_write(unsigned char data);
uint8_t i2c_start(uint8_t addr);
void i2c_stop(void);
void print_hex_value(char c);
uint8_t i2c_read_byte(void);
void mcp_init(void);
void mcp_write_register(uint8_t reg, uint8_t value);
uint8_t mcp_read_register(uint8_t reg);
void rotary_init(void);
void rotary_update(void);
void rotary_button_update(void);



#endif