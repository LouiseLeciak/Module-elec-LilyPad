# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nige42 <nige42@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/13 13:45:55 by nrobinso          #+#    #+#              #
#    Updated: 2026/06/10 15:46:41 by nige42           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ------------------------- project -------------------------------------------
NAME	=	main
TOOLS	=	#tools.c
LIB		=	spi_lib.c small_font.c uart_lib.c tools.c #led_lib.c config.h 

# ------------------------- compiler ------------------------------------------
CC			=	avr-gcc
CFLAGS	=	-mmcu=atmega328p -Wall -Wextra -Werror -Os

# ------------------------- CPU frequency and baud rate -----------------------
F_CPU					=	16000000UL
BAUD					=	-b 115200
UART_BAUDRATE	=	115200
# ------------------------- Conversion .bin -> .hex ---------------------------
HEX			=	avr-objcopy
HEX_OUT	=	ihex

# ------------------------- Flasher --------------- ----------------------------
FLASH					=	avrdude
PROGRAMMER		=	-c wiring
CARD					=	-p m2560
# PORT				=	-P /dev/cu.usbserial-1430
FLASH_IN_OUT	=	-U flash:w:$(NAME).hex:i
PORT					=	-P /dev/cu.usbmodem14301


# ------------------------- Rules ---------------------------------------------
all: hex flash

hex:
	$(CC) $(CFLAGS) -DF_CPU=$(F_CPU) -Os -o $(NAME).bin $(NAME).c $(TOOLS) $(LIB)
	$(HEX) -O $(HEX_OUT) $(NAME).bin $(NAME).hex
	
flash:
	$(FLASH) $(PROGRAMMER) $(CARD) $(PORT) $(BAUD) $(FLASH_IN_OUT)

clean:
	rm -rf $(NAME).bin
	rm -rf $(NAME).hex


.PHONY: all hex flash clean
