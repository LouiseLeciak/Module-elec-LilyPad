NAME	= main
PROG	= arduino
MICROCONTROLLER	= atmega328p
CC	= avr-gcc
OBJCOPY	= avr-objcopy
DUDE	= avrdude
FILES	= test-keyboard.c uart.c

# mmcu to tell what microcontroller we have
# -Os for optimisation
FLAGS	= -Wall -Wextra -Werror -Os -mmcu=$(MICROCONTROLLER)
BAUD_RATE	= 115200
PORT	= /dev/ttyUSB0

# how fast is the CPU
F_CPU	= 16000000 # pdf -> quartz 16Mhz

all:	hex flash

hex:
	$(CC) $(FLAGS) -DF_CPU=$(F_CPU) -o $(NAME).bin $(FILES)
	$(OBJCOPY) -O ihex $(NAME).bin $(NAME).hex
# -O to give a target

flash:
	$(DUDE) -p $(MICROCONTROLLER) -c $(PROG) -b $(BAUD_RATE) -P $(PORT) -U flash:w:$(NAME).hex:i
# -p to specify which microcontroller we use
# -c to specify a programmer (how to talk with the board)
# -b to specify the baud rate
# -P to give a port
# -U which memory operation you want to do, i at the end is the format

clean:
	rm -f $(NAME).hex $(NAME).bin

.PHONY: all hex flash clean
