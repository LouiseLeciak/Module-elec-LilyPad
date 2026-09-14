/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart_lib.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 08:24:09 by nrobinso          #+#    #+#             */
/*   Updated: 2026/06/09 17:15:38 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_LIB_H
# define UART_LIB_H


    void uart_init(void);
    void uart_interupt_tx(unsigned char c);
    unsigned char uart_interupt_rx(void);
    void uart_printstr(volatile char* string);
    void uart_Init_interupts(void);
    void uart_tx(volatile unsigned char c);
    unsigned char uart_rx(void);
    void uart_print_addr(void *ptr);
    
#endif