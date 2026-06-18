# --- MICROCONTROLLER & FLASHING CONFIG ---
MCU					:=	atmega2560
F_CPU				:=	16000000UL
PORT				:= /dev/cu.usbmodem101 #Change this to match your configuration's port
PROGRAMMER	:=	wiring
BD_RATE			:=	115200

# --- DIRECTORIES ---
SRC_DIR	:=	src/
INC_DIR	:=	headers/
BUI_DIR	:=	build/
TST_DIR	:=	test/
UNI_DIR	:=	unity/

# --- TOOLCHAIN ---
CC			:=	avr-gcc
OBJCOPY	:=	avr-objcopy
DUDE		:=	avrdude

# --- COMPILER FLAGS ---
CFLAGS	=	-Wall -Wextra -Werror -I$(INC_DIR) -I$(INC_DIR)/ST7796 -I/opt/homebrew/opt/avr-gcc/avr/include -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -MMD -MP

# --- SOURCE & OBJECT FILES ---
SRC_FIL	=	main \
					main_screen \
					main_screen_text \
					mads \
					spi

SRC	= $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FIL)))
OBJ	= $(addprefix $(BUI_DIR), $(addsuffix .o, $(SRC_FIL)))
DEP	=	$(OBJ.o=.d)

# --- OUTPUT FILES ---
TARGET	=	$(BUI_DIR)main
HEX	= $(TARGET).hex

# --- BUILD RULES ---
.PHONY: all hex flash clean test

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
	$(DUDE) -v -p $(MCU) -c $(PROGRAMMER) -D -P $(PORT) -b $(BD_RATE) -U flash:w:$<:i

clean:
	rm -rf $(BUI_DIR)

TST_FIL	= test_add.c
TST_EXE = test_runner

test:
	$(cd unity ; mkdir build ; cd build ; cmake .. ; make ; mv libunity.a ..)
	avr-gcc -I$(INC_DIR) -L$(UNI_DIR) -I$(UNI_DIR)/src -lunity $(SRC) $(TST_DIR)/*.c -o $(TST_EXE)

-include $(DEP)
