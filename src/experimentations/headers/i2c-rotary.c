
#include "utils.h"

void i2c_init(void)
{
    // TWI control register, TWI Enable Bit p240
    // TWEN - TWI enable bit
    TWCR |= (1 << TWEN);

    // prescaler = 1
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0));

    // p221/222 bit rate generator unit
    //((F_CPU / 100kHz)- 16) / 2 * prescaler
    //  TWI bit rate register
    TWBR = 72;
}

void mcp_init(void)
{
    // pour configurer tous les pins du port A en entree
    // quand IODIRA = 1 alors entree
    // https://www.alldatasheet.com/datasheet-pdf/download/195324/MICROCHIP/MCP23017.html
    // p12
    mcp_write_register(MCP_IODIRA, 0xFF);

    // pareil mais avec GPPUA pour les pullup internes
    mcp_write_register(MCP_GPPUA, 0xFF);
}

void i2c_stop(void)
{
    // p225 7
    // twsto = stop condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    while (TWCR & (1 << TWSTO))
        ;
}

uint8_t i2c_read_byte(void)
{
    // lecture d'un seul octet sans ACK (dernier octet)
    TWCR = (1 << TWEN) | (1 << TWINT);

    while (!(TWCR & (1 << TWINT)))
        ;

    return TWDR;
}

// to write the content of the TWDR register and send it to the temp sensor
// p225 5-2 -> 7-1
// Master Transmitter
void i2c_write(unsigned char data)
{
    // Load data into TWDR register
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    // wait for the TWINT flag set
    while (!(TWCR & (1 << TWINT)))
    {
    }

    if (TW_STATUS != TW_MT_DATA_ACK)
    {
        uart_printstr("error MT_DATA_ACK\n\r");
        i2c_stop();
    }
}

void print_hex_value(char c)
{
    char hex[] = "0123456789abcdef";
    char buf[3];

    unsigned char value = (unsigned char)c;

    buf[0] = hex[value / 16];
    buf[1] = hex[value % 16];
    buf[2] = '\0';

    uart_printstr(buf);
}

uint8_t i2c_start(uint8_t addr)
{
    uint32_t timeout = 100000;

    // Envoi de la condition START
    // twsta demande l'emission du start
    // et twint lance l'operayion
    // p225
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // on attend la fin du start
    while (!(TWCR & (1 << TWINT)))
    {
        if (--timeout == 0)
        {
            uart_printstr("START TIMEOUT\r\n");
            return 0;
        }
    }

    // on check si le starts ou repeted start a bien ete envoye
    if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START)
    {
        uart_printstr("START ERROR : ");
        print_hex_value(TW_STATUS);
        uart_printstr("\r\n");
        return 0;
    }

    // Envoi de l'adresse + bit R/W
    TWDR = addr;

    // lancer la transmisison
    TWCR = (1 << TWINT) | (1 << TWEN);

    timeout = 100000;

    while (!(TWCR & (1 << TWINT)))
    {
        if (--timeout == 0)
        {
            uart_printstr("ADDR TIMEOUT\r\n");
            return 0;
        }
    }

    // Verifier que le slave dit ack
    if ((addr & 1) == WRITE)
    {
        if (TW_STATUS != TW_MT_SLA_ACK)
        {
            uart_printstr("ADDRESS ERROR : ");
            print_hex_value(TW_STATUS);
            uart_printstr("\r\n");
            return 0;
        }
    }
    else
    {
        if (TW_STATUS != TW_MR_SLA_ACK)
        {
            uart_printstr("ADDRESS ERROR : ");
            print_hex_value(TW_STATUS);
            uart_printstr("\r\n");
            return 0;
        }
    }

    return 1;
}

//! + de commentaires
uint8_t mcp_read_register(uint8_t reg)
{
    uint8_t value;

    // commence une ecriture, commencer par ecrire pour dire
    // quel registre interne du mcp sera lu
    if (!i2c_start((MCP23017_ADDR << 1) | WRITE))
        return 0;

    // on envois l'adresse du registre
    i2c_write(reg);

    // passe en lecture
    if (!i2c_start((MCP23017_ADDR << 1) | READ))
    {
        i2c_stop();
        return 0;
    }

// comme TWEA = 0 alors un nack est envoye automatiquement
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)))
        ;

    // valeur du registre
    value = TWDR;

    i2c_stop();

    return value;
}

void mcp_write_register(uint8_t reg, uint8_t value)
{
    uart_printstr("mcp start\r\n");

    // debut de transaction i2c
    if (!i2c_start((MCP23017_ADDR << 1) | WRITE))
        return;

    uart_printstr("addr sent\r\n");

    // on envois l'adresse du registre a modifier
    // 1.3.2.1 p5 mcp datasheet
    i2c_write(reg);

    uart_printstr("reg sent\r\n");

    // ecrire la nouvelle valeur dans le registre en question
    i2c_write(value);

    uart_printstr("value sent\r\n");

    i2c_stop();

    uart_printstr("stop\r\n");
}