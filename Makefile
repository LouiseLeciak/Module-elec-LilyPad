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
CFLAGS	=	-Wall -Wextra -Werror -I$(INC_DIR) -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -MMD -MP

# --- OS DEPENDENT SIMAVR INCLUDES ---
UNAME_S	:=	4(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	SIMAVR_INC := -I/opt/homebrew/opt/simavr/include
else
	SIMAVR_INC := -I/usr/include/simavr
endif

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

TST_FIL	= $(TST_DIR)test_add.c
TST_EXE = test_runner
TST_CFLAGS	=	-I$(INC_DIR) $(SIMAVR_INC) -I$(UNI_DIR)src -L. -lunity -mmcu=$(MCU) -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -MMD -MP -Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000


test:
	mkdir -p unity/build
	cd unity/build && cmake .. && make
	mv unity/build/libunity.a .
	avr-gcc $(TST_CFLAGS) $(SRC_DIR)blabla.c $(TST_FIL) $(UNI_DIR)/src/unity.c -o $(TST_EXE)
	simavr -m $(MCU) -f $(F_CPU) ./$(TST_EXE)

-include $(DEP)
