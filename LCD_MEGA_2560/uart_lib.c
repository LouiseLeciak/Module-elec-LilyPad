/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart_lib.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 08:24:14 by nrobinso          #+#    #+#             */
/*   Updated: 2026/06/09 17:21:23 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include "uart_lib.h"

#define BAUDRATE 115200                                 // baud Com Speed
#define OSC_SPEED_MHZ 16                                // Oscillation Speed page
#define DBL_SPEED (OSC_SPEED_MHZ/2)                     // p 182 set bit UCSRnA -> U2Xn
#define MYUBRR (F_CPU / (DBL_SPEED * BAUDRATE) -1)      // p 182   UBRR =  16000000


/// NOTE: function sets up UART params 
/// ARGS: None
/// RETURNS: None
/// REFS: page 209 - Datasheet

void uart_init(void) {
    
    UBRR0H = (MYUBRR>>8);                   // Set BAUD RATE REGISTER HIGH side  -  page 204
    UBRR0L = MYUBRR;                        // Set BAUD RATE REGISTER LOW side  -  page 204
    UCSR0A = (1<<U2X0);                     // Set Double Speed Mode p182  - p200
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);       // Set frame format: 8 data bits, 1 stop bit, no parity - page 203 
    UCSR0B |= (1 << RXEN0);                 // Enable receiver USART - page 202
    UCSR0B |= (1 << TXEN0);                 // Enable transmitter - page 201
}


/// NOTE: function transmits a char to the UDRn two level buffer 
/// ARGS: char c
/// RETURNS: None
/// REFS: page 209 - Datasheet

void uart_interupt_tx(volatile unsigned char c) {

    UDR0 = c;    // Put data into buffer, sends the data - page 187
}


/// NOTE: function receives a char from UDRn  two level buffer 
/// ARGS: None
/// RETURNS: char
/// REFS: page 209 - Datasheet

unsigned char uart_interupt_rx(void) {
    
    char c;
    
    c = UDR0;          // Receive Data from char c
    return (c);         
}


/// NOTE: function transmits to URD0 buffer a string
/// ARGS: pointer 
/// RETURNS: None
/// REFS: page 209 - Datasheet

void uart_printstr(volatile char* string) {

    int i = 0;
    volatile unsigned char *str;
    str = (unsigned char *)string;
                                            // UDRE0 - page 200 indicates Data Register Empty
    while (str[i] != '\0') {                // UCSR0A - page 200 USART Control and status register
        while (!(UCSR0A & (1 << UDRE0))) ;  // Set Flag indicates if ready to tramit data 1 = Buf empty
        UDR0 = str[i];                      // add to output buffer
        i++; 
    }    
}


/// NOTE: function enables UART interupts
/// ARGS: None
/// RETURNS: None
/// REFS: page 201 - Datasheet
void uart_Init_interupts(void) {

    UCSR0B |= (1 << RXCIE0);                // Enable Complete Interupt Enable receiver - page 201
    SREG |= (1 << 7);                       // ENABLE Global interupts page 20
}




/// NOTE: function enables UART interupts
/// ARGS: None
/// RETURNS: None
/// REFS: page 201 - Datasheet

void uart_tx(volatile unsigned char c)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0))) ;                  // Set Flag indicates if ready to tramit data 1 = Buf empty
    UDR0 = c;                                           // Put data into buffer, sends the data
}



unsigned char uart_rx(void) {
    
    unsigned char c;
    
    while ((UCSR0A & (1 << RXC0)) == 0) ;               // Recieve data Flag - page 200
    c = UDR0;                                           // receive data
    return (c);                                         // return c
}





#include <stdint.h>

void uart_print_addr(void *ptr)
{
    uint16_t        addr;       // AVR: 16-bit address space
    char            buf[7];     // "0x" + 4 hex digits + '\0'
    unsigned char   i;
    unsigned char   nibble;

    addr = (uint16_t)ptr;       // cast the pointer itself, not dereference it
    buf[0] = '0';
    buf[1] = 'x';
    buf[6] = '\0';

    i = 5;
    while (i >= 2)
    {
        nibble = addr & 0xF;
        buf[i] = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
        addr >>= 4;
        i--;
    }
    uart_printstr(buf);
}