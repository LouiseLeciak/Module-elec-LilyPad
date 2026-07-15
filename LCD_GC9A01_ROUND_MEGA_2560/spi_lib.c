/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spi_lib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 10:31:22 by nige42            #+#    #+#             */
/*   Updated: 2026/07/15 12:48:15 by nige42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include "spi_lib.h"

// ─── SPI ---------------------------------------------------------------------
//   | (1 << PB4)   // CS
// | (1 << PB5)   // DC - PB5
// | (1 << PB7);  // RST

void spi_init(void) {
    DDRB |= (1 << PB2)   // MOSI
    | (1 << PB1)   // SCK
    | (1 << PB0); // SS 

    DDRL |= (1 << PL1) // CS LEFT EYE
        | (1 << PL4)   // DC - PB5
        | (1 << PL6);  // CS RIGHT EYE

    SPCR = (1 << SPE) | (1 << MSTR); // SPI enable, master, fosc/4
    SPSR = (1 << SPI2X);              // fosc/2 = 8MHz
}

// uint8_t spi_send(uint8_t data) {
void spi_send(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    // return SPDR;
}

