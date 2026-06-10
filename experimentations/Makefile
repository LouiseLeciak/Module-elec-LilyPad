#####################
## MICROCONTROLLER ##
##        &        ##
## FLASHING CONFIG ##
#####################

MCU					:=	atmega2560
F_CPU				:=	16000000UL
PORT				:=	/dev/cu.usbserial-110
PROGRAMMER	:=	wiring
BD_RATE			:=	115200

#################
## DIRECTORIES ##
#################

SRC_DIR	:=	src/
INC_DIR	:=	include/
BUI_DIR	:=	build/

###############
## TOOLCHAIN ##
###############

CC			:=	avr-gcc
OBJCOPY	:=	avr-objcopy
DUDE		:=	avrdude

####################
## COMPILER FLAGS ##
####################

CFLAGS	:=	-Wall -Wextra -Werror -I$(INC_DIR) -I/opt/homebrew/opt/avr-gcc/avr/include -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS	+=	-MMD -MP
# CFLAGS	+=	-DDEBUG

#####################
## SOURCE & OBJECT ##
##      FILES      ##
#####################

SRC_FIL	:=	main

SRC	= $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FIL)))
OBJ	= $(addprefix $(BUI_DIR), $(addsuffix .o, $(SRC_FIL)))
DEP	=	$(OBJ.o=.d)

##################
## OUTPUT FILES ##
##################

TARGET	=	$(BUI_DIR)main
HEX	= $(TARGET).hex

#################
## BUILD RULES ##
#################

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
