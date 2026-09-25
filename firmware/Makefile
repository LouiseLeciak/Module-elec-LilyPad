# --- MICROCONTROLLER & FLASHING CONFIG ---
MCU					:=	atmega2560
F_CPU				:=	16000000UL
-include config.mk
PORT ?= /dev/ttyUSB0 # fallback if config.mk absent
PROGRAMMER	:=	wiring
BD_RATE			:=	115200

# --- DIRECTORIES ---
SRC_DIR	:=	src/
INC_DIR	:=	headers/
BUI_DIR	:=	build/

# --- TOOLCHAIN ---
CC			:=	avr-gcc
OBJCOPY	:=	avr-objcopy
DUDE		:=	avrdude


# --- COMPILER FLAGS ---
INC_FLAGS	=	-I$(INC_DIR) -I/opt/homebrew/opt/avr-gcc/avr/include
CFLAGS		=	-Wall -Wextra -Werror  $(INC_FLAGS) -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -MMD -MP


# --- SOURCE & OBJECT FILES ---
SRC_FIL	=	main \
					app/alphabet \
					display/bmp \
					utils/crc \
					display/eye_imgs \
					app/game \
					display/GC9A01 \
					system/globals \
					input/i2c_rotary \
					display/ili9488 \
					system/init \
					input/keypad \
					app/language \
					display/main_screen \
					utils/mem_utils \
					app/menu \
					system/power_save \
					input/rotary \
					display/screen_text \
					display/small_font \
					system/spi \
					storage/fatfs \
					storage/sd \
					storage/sd_streaming \
					app/traduction \
					system/uart \
					utils/utils

SRC	= $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FIL)))
OBJ	= $(addprefix $(BUI_DIR), $(addsuffix .o, $(SRC_FIL)))
DEP	=	$(OBJ:.o=.d)

# --- OUTPUT FILES ---
TARGET	=	$(BUI_DIR)main
HEX	= $(TARGET).hex

# --- BUILD RULES ---
.PHONY: all hex flash clean re doc lint fix-lint

all: hex

hex: $(HEX)

$(BUI_DIR):
	mkdir -p $@

$(BUI_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).bin: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(HEX) : $(TARGET).bin
	$(OBJCOPY) -O ihex $< $@

flash: $(HEX)
	$(DUDE) -v -p $(MCU) -c $(PROGRAMMER) -D -P $(PORT) -b $(BD_RATE) -U flash:w:$<:i

lint:
	find . \( -name '*.c' -o -name '*.h' \) | xargs clang-format --dry-run --Werror --style=file

fix-lint:
	find . \( -name '*.c' -o -name '*.h' \) | xargs clang-format -i --style=file

clean:
	rm -rf $(BUI_DIR)

re: clean all

-include $(DEP)
