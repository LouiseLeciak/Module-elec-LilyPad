/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 13:56:57 by nrobinso          #+#    #+#             */
/*   Updated: 2026/06/09 15:02:00 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_H
    # define TOOLS_H
    #include <avr/io.h>


    uint8_t is_upper(unsigned char c);
    unsigned char to_upper(unsigned char c); 
    unsigned char to_lower(unsigned char c); 
    unsigned char inverse_char(unsigned char c); 
    uint8_t notPrintable(unsigned char c);
    uint8_t checkChar(unsigned char c);
    int ft_strlen(unsigned char *str);
    uint8_t ft_strcmp(unsigned char *str, unsigned char *name);
    uint8_t is_len_size(unsigned char *str, int size);
    uint8_t is_first_char(unsigned char *str, unsigned char c);
    uint8_t is_valid_hex_str(unsigned char *str);
    void split_hex(unsigned char *str, unsigned char *hex, int start_pos);
    uint8_t is_digit(unsigned char c);
    uint8_t convert_from_hex(unsigned char *str);
    void toHex(unsigned char c);
    void ft_itoa(uint32_t nbr, int index);
    void putnbr(uint16_t nbr);
    uint16_t ft_nbrlen(volatile char *str);
    void nbr_to_str (uint32_t nbr);
    void putnbr(uint16_t nbr);
    void putnbr_32t(uint32_t nbr);
    void print_hex_value(char c);
    void toHex_0xFFF(uint16_t c);
    uint8_t Printable(unsigned char c);
    uint16_t hexStr_to_dec(char* hexString);
    void loop_for_one_millisecond(volatile uint16_t loop_Max);
    void pause_in_milliseconds(uint16_t time_in_ms);
    uint16_t nbrStr_to_dec(const char* nbrString);
    uint8_t is_digit_str(volatile char* str);
    uint32_t nbrStr_to_dec_32t(const char* nbrString);
    int16_t nbrStr_to_dec_signed(const char* nbrString);
    void putnbr_32t_signed(int16_t nbr);

#endif 