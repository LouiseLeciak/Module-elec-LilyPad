# --- MICROCONTROLLER & FLASHING CONFIG ---
MCU			:=	atmega2560
F_CPU		:=	16000000UL
-include ../config.mk
PORT ?= /dev/ttyUSB0 # fallback if config.mk absent
PROGRAMMER	:=	wiring
BD_RATE		:=	115200

# --- DIRECTORIES ---
SRC_DIR	:=	../src/
INC_DIR	:=	../headers/
BUI_DIR	:=	build/

# --- TOOLCHAIN ---
CC		:=	avr-gcc
OBJCOPY	:=	avr-objcopy
DUDE	:=	avrdude

# --- COMPILER FLAGS ---
INC_FLAGS	=	-I$(INC_DIR) -I/opt/homebrew/opt/avr-gcc@15/avr/include
CFLAGS		=	-Wall -Wextra -Werror $(INC_FLAGS) -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -MMD -MP

# --- SOURCE & OBJECT FILES ---
TEST_SRC	=	pin_test.c
LIB_SRC		=	$(filter-out $(SRC_DIR)main.c, $(wildcard $(SRC_DIR)*.c))

OBJ	=	$(addprefix $(BUI_DIR), $(TEST_SRC:.c=.o)) \
		$(addprefix $(BUI_DIR)src/, $(notdir $(LIB_SRC:.c=.o)))
DEP	=	$(OBJ:.o=.d)

# --- OUTPUT FILES ---
TARGET	=	$(BUI_DIR)pin_tester
HEX		=	$(TARGET).hex

# --- BUILD RULES ---
.PHONY: all hex flash clean re size

all: hex

hex: $(HEX)

# test file (tests/pin_tester.c -> tests/build/pin_tester.o)
$(BUI_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# project sources (../src/foo.c -> tests/build/src/foo.o)
$(BUI_DIR)src/%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).bin: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(HEX): $(TARGET).bin
	$(OBJCOPY) -O ihex $< $@

flash: $(HEX)
	$(DUDE) -v -p $(MCU) -c $(PROGRAMMER) -D -P $(PORT) -b $(BD_RATE) -U flash:w:$<:i

size: $(HEX)
	avr-size --format=avr --mcu=$(MCU) $(TARGET).bin

clean:
	rm -rf $(BUI_DIR)

re: clean all

-include $(DEP)