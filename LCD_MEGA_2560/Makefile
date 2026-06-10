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
TARGET	=	$(BUI_DIR)main
HEX			=	$(TARGET).hex

SRC_DIR	=	src/
INC_DIR	=	include/
BUI_DIR	=	build/

# ------------------------- CPU frequency and baud rate -----------------------
MCU					=	atmega2560
F_CPU				=	16000000UL
PROGRAMMER	=	wiring
BD_RATE			=	115200
PORT				=	/dev/cu.usbserial-110 #There has to be a way to determine port through a command

# ------------------------- compiler ------------------------------------------
CC			=	avr-gcc
CFLAGS	=	-Wall -Wextra -Werror -I$(INC_DIR) -I/opt/homebrew/opt/avr-gcc/avr/include -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -MMD -MP

SRC_FIL	=	main.c \
					spi_lib.c \
					small_font.c \
					uart_lib.c \
					tools.c \
					#led_lib.c \
					config.h 

SRC			=	$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FIL)))
OBJ			=	$(addprefix $(BUI_DIR), $(addsuffix .o, $(SRC_FIL)))
DEP			=	$(OBJ.o=.d)

# ------------------------- Conversion .bin -> .hex ---------------------------
OBJCOPY	=	avr-objcopy
HEX_OUT	=	ihex

# ------------------------- Flasher --------------- ----------------------------
DUDE					=	avrdude
CARD					=	-p m2560
# PORT				=	-P /dev/cu.usbserial-1430
FLASH_IN_OUT	=	-U flash:w:$(NAME).hex:i
PORT					=	-P /dev/cu.usbmodem14301


# ------------------------- Rules ---------------------------------------------
.PHONY: all hex flash clean

all: hex

hex: $(HEX)

$(BUI_DIR):
	mkdir -p $@

$(BUI_DIR)%.o: $(SRC_DIR)%.c | $(BUI_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).bin: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(HEX) : $(TARGET).bin
	$(OBJCOPY) -O ihex $< $@

flash: $(HEX)
	$(DUDE) -v -D -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BD_RATE) -U flash:w:$<:i

clean:
	rm -rf $(BUI_DIR)

-include $(DEP)
