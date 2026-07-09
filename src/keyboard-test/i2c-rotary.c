
#include "utils.h"


void i2c_init(void) {
    // TWI control register, TWI Enable Bit p240
    TWCR |= (1 << TWEN);

    // prescaler = 1
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0));

    // p221/222 bit rate generator unit
    //((F_CPU / 100kHz)- 16) / 2 * prescaler
    //  TWI bit rate register
    TWBR = 72;
}

void mcp_init(void){
    mcp_write_register(MCP_IODIRA, 0xFF);

    mcp_write_register(MCP_GPPUA, 0xFF);
}

void i2c_stop(void) {
    // p225 7
    TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

uint8_t i2c_read_byte(void)
{
    // lecture d'un seul octet sans ACK (dernier octet)
    TWCR = (1 << TWEN) | (1 << TWINT);

    while (!(TWCR & (1 << TWINT)));

    return TWDR;
}

// to write the content of the TWDR register and send it to the temp sensor
// p225 5-2 -> 7-1
// Master Transmitter
void i2c_write(unsigned char data) {
    // Load data into TWDR register
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    // wait for the TWINT flag set
    while (!(TWCR & (1 << TWINT))){}

    if (TW_STATUS != TW_MT_DATA_ACK) {
        uart_printstr("error MT_DATA_ACK\n\r");
        i2c_stop();
    }
}

void print_hex_value(char c) {
    char hex[] = "0123456789abcdef";
    char buf[3];

    unsigned char value = (unsigned char)c;

    buf[0] = hex[value / 16];
    buf[1] = hex[value % 16];
    buf[2] = '\0';

    uart_printstr(buf);
}

void i2c_start(uint8_t addr)
{
    uint32_t timeout = 100000;

    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT))){
        if (--timeout == 0){
            uart_printstr("START TIMEOUT\r\n");
            return;
        }
    }

    if (TW_STATUS != TW_START)
    {
        uart_printstr("START error !!\r\n");
        return;
    }

    TWDR = addr;

    TWCR = (1 << TWINT) | (1 << TWEN);

    timeout = 100000;
    while (!(TWCR & (1 << TWINT))){
        uart_printstr("ADDR TIMEOUT\r\n");
        return;
    }

    if (TW_STATUS != TW_MT_SLA_ACK)
    {
        uart_printstr("MCP ADDRESS ERROR !!\r\n");
        print_hex_value(TW_STATUS);
        uart_printstr("\r\n");
    }
}

//! + de commentaires
uint8_t mcp_read_register(uint8_t reg)
{
    uint8_t value;

    i2c_start((MCP23017_ADDR << 1) | WRITE);

    i2c_write(reg);

    i2c_start((MCP23017_ADDR << 1) | READ);

    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));

    value = TWDR;

    i2c_stop();

    return value;
}

void mcp_write_register(uint8_t reg, uint8_t value)
{
    uart_printstr("mcp start\r\n");

    i2c_start((MCP23017_ADDR << 1) | WRITE);

    uart_printstr("addr sent\r\n");

    i2c_write(reg);

    uart_printstr("reg sent\r\n");

    i2c_write(value);

    uart_printstr("value sent\r\n");

    i2c_stop();

    uart_printstr("stop\r\n");
}