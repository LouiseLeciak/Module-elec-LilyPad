/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 13:51:57 by nrobinso          #+#    #+#             */
/*   Updated: 2026/06/09 15:02:06 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include "tools.h"
#include "uart_lib.h"
typedef unsigned char uint8_t;      // needed because not using stdlib
typedef unsigned int uint16_t;      // needed because not using stdlib
typedef uint8_t bool;

extern volatile char hex[4];                // global variable for function toHex()
extern volatile char nbr_in_a_string[33];    // global variable for function nbr_to_str()



bool is_upper(unsigned char c) {

    if (c >= 'A' && c <= 'Z') {
        return (1);
    }
    return (0);
}

unsigned char to_upper(unsigned char c) {

    if (c >= 'a' && c <= 'z') {
        c -= 32;   
    }
    return c;
}

unsigned char to_lower(unsigned char c) {

    if (c >= 'A' && c <= 'Z') {
        c += 32;   
    }
    return c;
}

unsigned char inverse_char(unsigned char c) {

    if (is_upper(c))
        c = to_lower(c);
    else {
        c = to_upper(c);
    }
    return (c);
}


bool notPrintable(unsigned char c) {
    if (c == 0x7F || c == '\r')
        return (0);
    if (c < ' ' || c > '~')
        return (1);
    return (0);
    
}


bool Printable(unsigned char c) {
    if (c == 0x7F || c == '\r')
        return (0);
    if (c < ' ' || c > '~')
        return (0);
    return(1);
}



bool checkChar(unsigned char c) {
    
    if (c == '\t' || c =='\b')
        return (1);
    return (0);
}


int ft_strlen(unsigned char *str) {
    
    int i = 0;
    if (!str)
        return (0);
    while (str && str[i])
        i++;
    return (i);
}


bool ft_strcmp(unsigned char *str, unsigned char *name) {

    int i = 0;
    if (ft_strlen(str) != ft_strlen(name))
        return(0);
    if (str[0] == '\0') 
        return (1);

    while (name[i] != '\0') {

        if (str[i] != name[i])
            return (0);
        i++;
    } 
    
    return (1);
}

/// NOTE: function chhecks the size of the string aginst size
/// ARGS: char* str to be checked, unit8_t size to compare 
/// RETURNS: 0 - false, 1 - true if string is the length found in size 

bool is_len_size(unsigned char *str, int size) {

    if (!str)
        return (0);
    if (ft_strlen(str) == size)
        return (1);
    return (0);
}


/// NOTE: function checks first char in a string
/// ARGS: char* str to be checked, char to compare with
/// RETURNS: 0 - false, 1 - true

bool is_first_char(unsigned char *str, unsigned char c) {

    if (!str)
        return (0);
    if (str && str[0] == c)
        return (1);
    return (0);
}


/// NOTE: function checks the chars are valid HEX in the string
/// ARGS: char* str
/// RETURNS: 0 - false, 1 - true

bool is_valid_hex_str(unsigned char *str) {
    
    int i = 1;
    if (!str)
        return (0);

    while (str[i]) {

        if (!((str[i] >= '0' && str[i] <= '9') 
            || (str[i] >= 'a' 
            && str[i] <= 'f')))
                return (0);
        i++;
    }
    return (1);
}


/// NOTE: function splits a string into 2 Chars in function of stating position
/// ARGS: pointer string to split, pointer to hex dest, int start point
/// RETURNS: nothing 

void split_hex(unsigned char *str, unsigned char *hex, int start_pos) {

    int i = 0;
    int y = 1;
    int stop = start_pos + 2;
    unsigned char *dest;
    dest = hex;
    while (str && str[y]){
        
        dest[i] = str[start_pos + 1];
        i++;
        y++;
        start_pos++;
        if (start_pos == stop)
            break;   
    }
    dest[i] = '\0';
    // uart_printstr((volatile char*)"\n\rdest-split{");
    // uart_printstr((volatile char*)hex);
    // uart_printstr((volatile char*)"}\r\n");    
}

/// NOTE: function check if a digit
/// ARGS: a character
/// RETURNS: 0 - false, 1 - true

bool is_digit(unsigned char c) {

    if (c >= '0' && c <= '9') {
        return (1);
    }
    return (0);
}


/// NOTE: function converts from hex 0xFF to decimal (only two heximals FF)
/// ARG:  unsigned char* pointer
/// RETURNS: unit8_t 8 bits
/// REF: - str not protected in size. undefined result if under or over size string 

uint8_t convert_from_hex(unsigned char *str) {

    volatile int i = 0;
    volatile uint8_t high_end = 0;
    volatile uint8_t low_end = 0;
    volatile uint8_t res = 0;
    
    if (!str)
        return (0);

    while (str && str[i]) {

        if (i == 0) {
            if (is_digit(str[i])) {
                high_end = (str[i] - '0');      // high nibble × 16
            } else if (str[i] >= 'a' && str[i] <= 'f') {
                high_end = (str[i] - 'a') + 10;
            }
        }

        if  (i == 1) {
                if (is_digit(str[i]))
                    low_end += (str[i] - '0');            // low nibble, no multiplier
                else if (str[i] >= 'a' && str[i] <= 'f')
                    low_end += (str[i] - 'a') + 10;
            }
            i++;
        }
        res = (((high_end) * 16) + low_end);
    return res;
}



/// NOTE: function converts a dec number fronm 0 to 255 into 0xFF format
/// ARGS: the unsigned char to convert
/// RETURNS: None
/// INFO: need a global variable hex[3] to works

void toHex(unsigned char c) {

    volatile uint8_t low = 0;
    volatile uint8_t high = 0;

    hex[0] = '\0';
    hex[1] = '\0';
    hex[2] = '\0';
    
    if (c == 0) {
        (hex[0] = '0'); (hex[1] = '0'); (hex[2] = '\0');
        return ;
    }
    high = c / 16;
    low = c % 16;
    
    if (high <= 9 )
        hex[0] = high + '0';
    if (high > 9 && high <= 16)
        hex[0] = high + 87;

    if (low <= 9 )
        hex[1] = low + '0';
    if (low > 9 && low <= 16)
        hex[1] = low + 87;
     
}



void toHex_0xFFF(uint16_t c) {

    volatile uint8_t low = 0;
    volatile uint8_t mid = 0;
    volatile uint8_t high = 0;

    hex[0] = '\0';
    hex[1] = '\0';
    hex[2] = '\0';
    hex[3] = '\0';
    
    if (c == 0) {
        (hex[0] = '0'); (hex[1] = '0'); (hex[2] = '\0');
        return ;
    }
    high = (c >> 8) & 0xF;
    mid = (c >> 4)  & 0xF;
    low = c         &0xF;
    
    hex[0] = (high <= 9) ? (high + '0') : (high + 87); // 87 = 'a' - 10
    hex[1] = (mid  <= 9) ? (mid  + '0') : (mid  + 87);
    hex[2] = (low  <= 9) ? (low  + '0') : (low  + 87);
    hex[3] = '\0';
     
}



uint16_t ft_nbrlen(volatile char *str) {

    uint16_t len = 0;

    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void reverseNumString(volatile char *str) {

    uint16_t stop = 0;
    int i = 0;
    stop = ft_nbrlen(str);
    char newstr[stop + 1];

    while (str[i] != '\0') {
        newstr[i] = str[stop - 1];
        i++;
        stop--;
    }
    newstr[i] = '\0';
    i = 0;
    stop = ft_nbrlen(str);
    while (stop > 0) {
            str[i] = newstr[i]; 
            i++;
            stop--;
    }
    str[i] = '\0';
}

void nbr_to_str (uint32_t nbr) {
    
    uint8_t flag = 0;
    if (nbr_in_a_string[0] == '-') {
        flag = 1;
    }
    ft_itoa(nbr,-1);
    if (flag == 1){
        uint8_t len = ft_strlen((unsigned char*)nbr_in_a_string);
        nbr_in_a_string[len] = '-';
        len++;
        nbr_in_a_string[len] = '\0'; 
    }

    // uart_printstr("\r\n DEBUG nbr to STR 0 :");
    reverseNumString(nbr_in_a_string);
    // uart_printstr(nbr_in_a_string);
    // uart_printstr("\r\n DEBUG nbr to STR 1\r\n");
}

void ft_itoa(uint32_t nbr, int index){

    int i = index;

    if (nbr <= 9) {
        i++;
        nbr_in_a_string[i] = (nbr % 10) + '0';
        i++;
        nbr_in_a_string[i] = '\0';
        return;
    }
    i++;
    if (nbr > 9)  {
        ft_itoa(nbr / 10, i);
    }
    // uart_tx(((nbr % 10) + '0'));
    nbr_in_a_string[i] = (nbr % 10) + '0';
}



void putnbr(uint16_t nbr){

    if (nbr > 9)  {
        putnbr(nbr / 10);
    }
    uart_tx(((nbr % 10) + '0'));
}



// void putnbr_32t(uint32_t nbr){

//     if (nbr > 9)  {
//         putnbr_32t(nbr / 10);
//     }
//     uart_tx(((nbr % 10) + '0'));
// }



void putnbr_32t(uint32_t nbr) {
    char    buf[10];        // max 10 digits for uint32_t (fits in registers/stack once)
    uint8_t i = 0;

    if (nbr == 0) {
        uart_tx('0');
        return;
    }

    while (nbr > 0) {
        buf[i++] = (nbr % 10) + '0';
        nbr /= 10;
    }

    // digits are stored in reverse, print backwards
    while (i--) {
        uart_tx(buf[i]);
    }
}



/// NOTE: prints value in 0xFF to screen with uart lib
/// ARGS: char
/// RETURNS: None

void print_hex_value(char c) {
        toHex(c);
        uart_printstr(hex);
}


/// NOTE: string in hex to decimal -> FF -> 255
/// ARGS: string to convert
/// RETURNS: uint16_t value in decimal

uint16_t hexStr_to_dec(char* hexString) {

    uint8_t index = 0;
    uint16_t result = 0;
    
    while (hexString && hexString[index]) {
        uint8_t value = 0;
        if (hexString[index] >= '0' && hexString[index] <= '9') {
            value = hexString[index] - '0';            
        } else if (hexString[index] >= 'a' && hexString[index] <= 'f') {
            
            value = hexString[index] - 'a' + 10;                        
        } else {
            break;
        }
        result = (result << 4) | value;
        index++;
    }
    return (result);    
}


#define CALIBRATE_MS 8000

void loop_for_one_millisecond(volatile uint16_t loop_Max) {

    for (uint16_t i = 0; i < loop_Max; i++)
        ;
} 
void pause_in_milliseconds(uint16_t time_in_ms) {

    while (time_in_ms > 0) {
        loop_for_one_millisecond(F_CPU / CALIBRATE_MS);       
        time_in_ms--;
    }
} 



/// NOTE: string dec in to decimal -> "09" -> 9
/// ARGS: string to convert
/// RETURNS: uint16_t value in decimal

uint16_t nbrStr_to_dec(const char* nbrString) {

    uint32_t result = 0;
    uint8_t index = 0;

    while (nbrString && nbrString[index]) {

        if (nbrString[index] < '0' || nbrString[index] > '9') {
            break;  // stop on non-digit
        }

        uint8_t digit = nbrString[index] - '0';
        result = result * 10 + digit;

        index++;
    }

    return result;
}



bool is_digit_str(volatile char* str) {

    int i = 0;
    while (str && str[i]) {

        if(!is_digit(str[i]))
            return (0);
        i++;
    }

    return (1);
    
}



/// NOTE: string dec in to decimal -> "09" -> 9
/// ARGS: string to convert
/// RETURNS: uint16_t value in decimal

uint32_t nbrStr_to_dec_32t(const char* nbrString) {

    uint64_t result = 0;
    uint8_t index = 0;

    if (ft_strlen((unsigned char*)nbrString) > 10)
        return (0);

    while (nbrString && nbrString[index]) {

        if (nbrString[index] < '0' || nbrString[index] > '9') {
            break;  // stop on non-digit
        }

        uint8_t digit = nbrString[index] - '0';
        result = result * 10 + digit;
        index++;
        if (result > 4294967295)
            return (uart_printstr("OVER SIZED NUMBER\r\n"), 0);
    }

    return result;
}



/// NOTE: string dec in to decimal -> "09" -> 9
/// ARGS: string to convert
/// RETURNS: uint16_t value in decimal

int16_t nbrStr_to_dec_signed(const char *nbrString)
{
    int32_t result = 0;
    uint8_t index  = 0;
    int8_t  neg    = 1;

    if (!nbrString || nbrString[0] == '\0')
        return 0;

    if (nbrString[0] == '-') {
        neg = -1;
        index++;
    }

    while (nbrString[index]) {
        if (nbrString[index] < '0' || nbrString[index] > '9')
            break;

        result = result * 10 + (nbrString[index] - '0');

        if (neg == 1  && result >  32767) {return  32767; }
        if (neg == -1 && result >  32768) {return -32768; }

        index++;
    }

    return (int16_t)(result * neg);
}


void putnbr_32t_signed(int16_t nbr) {
    char    buf[10];        // max 10 digits for uint32_t (fits in registers/stack once)
    volatile uint8_t i = 0;
    int neg = 0;

    if (nbr < 0) {
        nbr *= -1;
        neg = 1;
    }

    while (nbr > 0) {
        buf[i++] = (nbr % 10) + '0';
        nbr /= 10;
    }
    if (neg == 1)
        buf[i++] = '-';
    // digits are stored in reverse, print backwards
    while (i--) {
        uart_tx(buf[i]);
    }
}
