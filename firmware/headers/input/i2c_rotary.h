
#ifndef I2C_ROTARY_H
#define I2C_ROTARY_H

#include <avr/io.h>

// i2c - rotary encoder
void i2c_init(void);
void mcp_init(void);
uint8_t mcp_read_register(uint8_t reg);
void rotary_init(void);

#endif
