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
INC_FLAGS	=	-I$(INC_DIR) -I/opt/homebrew/opt/avr-gcc@15/avr/include
CFLAGS		=	-Wall -Wextra -Werror $(INC_FLAGS) -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -MMD -MP


# --- SOURCE & OBJECT FILES ---
SRC_FIL	=	main \
					app/alphabet \
					app/game \
					app/language \
					app/menu \
					app/traduction \
					display/bmp \
					display/eye_imgs \
					display/GC9A01 \
					display/ili9488 \
					display/screen_text \
					display/small_font \
					input/i2c_rotary \
					input/keypad \
					input/rotary \
					storage/fatfs \
					storage/sd \
					storage/sd_streaming \
					system/globals \
					system/init \
					system/power_save \
					system/spi \
					system/uart \
					utils/crc \
					utils/mem_utils \
					utils/utils

SRC	= $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FIL)))
OBJ	= $(addprefix $(BUI_DIR), $(addsuffix .o, $(SRC_FIL)))
DEP	=	$(OBJ:.o=.d)

# --- OUTPUT FILES ---
TARGET	=	$(BUI_DIR)main
HEX	= $(TARGET).hex

# --- BUILD RULES ---
.PHONY: all hex flash clean re doc debug size

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

size: $(HEX)
	avr-size --format=avr --mcu=$(MCU) $(TARGET).bin

clean:
	rm -rf $(BUI_DIR)

re: clean all

debug: clean debug_compile
debug_compile: CFLAGS += -DDEBUG
debug_compile: all flash

-include $(DEP)
