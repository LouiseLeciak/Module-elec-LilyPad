# --- MICROCONTROLLER & FLASHING CONFIG ---
MCU					:=	atmega2560
F_CPU				:=	16000000UL
-include config.mk
PORT ?= /dev/ttyUSB0 # fallback if config.mk absent
PROGRAMMER	:=	wiring
BD_RATE			:=	115200

# --- THEME CONFIG ---
# ALT=0 (or unset) default | ALT=1 halloween
ALT ?= 0
ifeq ($(ALT),0)
THEME_NAME := default
THEME_SUFFIX :=
else ifeq ($(ALT),1)
THEME_NAME := halloween
THEME_SUFFIX := _halloween
else ifeq ($(ALT), 2)
THEME_NAME := candy
THEME_SUFFIX := _candy
else ifeq ($(ALT), 3)
THEME_NAME := bw
THEME_SUFFIX := _bw
else
$(error Unknown ALT value "$(ALT)": use 0 (default), 1 (halloween), 2 (candy) or 3 (black & white)
endif
THEME_FLAGS := $(if $(THEME_SUFFIX),-DALT=$(ALT),)

# --- DIRECTORIES ---
SRC_DIR	:=	src/
INC_DIR	:=	headers/
BUI_DIR	:=	build/$(THEME_NAME)/

# --- TOOLCHAIN ---
CC			:=	avr-gcc
OBJCOPY	:=	avr-objcopy
DUDE		:=	avrdude

# --- COMPILER FLAGS ---
INC_FLAGS	=	-I$(INC_DIR) -I/opt/homebrew/opt/avr-gcc@15/avr/include
CFLAGS		=	-Wall -Wextra -Werror $(INC_FLAGS) -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(THEME_FLAGS) -MMD -MP


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
TARGET	=	$(BUI_DIR)main$(THEME_SUFFIX)
HEX	= $(TARGET).hex

# --- BUILD RULES ---
.PHONY: all hex flash clean re doc debug size default halloween candy bw releases

all: hex

hex: $(HEX)

# Theme shortcuts: make [default|halloween]
# Re-invoke make so ALT is visible at parse time (prerequisites are
# expanded before target-specific variables take effect).
halloween:
	$(MAKE) ALT=1 hex

candy:
	$(MAKE) ALT=2 hex

bw:
	$(MAKE) ALT=3 hex

default: hex


# Build one hex per theme and collect them in releases/
ALT_LIST := 0 1 2 3
REL_DIR := releases/
releases:
	@mkdir -p $(REL_DIR)
	@for alt in $(ALT_LIST); do \
		echo "==> Building theme ALT=$$alt"; \
		$(MAKE) --no-print-directory ALT=$$alt hex || exit 1; \
	done
	cp build/*/main*.hex build/*/main*.bin $(REL_DIR)

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
	rm -rf build/

re: clean all

debug: clean debug_compile
debug_compile: CFLAGS += -DDEBUG
debug_compile: all flash

-include $(DEP)
