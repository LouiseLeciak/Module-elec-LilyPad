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

# --- STATIC ANALYSIS FLAGS ---
# Used only by `make analyze` (see below); the normal build never sees them.
# -flto makes unused-static detection whole-program, -fstack-usage writes one
# .su file per TU with per-function stack usage.
ANALYZE_WARN	=	-Wmissing-prototypes -Wmissing-declarations \
					-Wredundant-decls -Wunused-macros -Wshadow -Wpadded \
					-flto -fstack-usage
ANALYZE_CFLAGS	=	$(filter-out -Werror,$(CFLAGS)) $(ANALYZE_WARN)


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
.PHONY: all hex flash clean re doc debug size analyze gc-report cppcheck

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

# --- STATIC ANALYSIS TARGETS ---

# Full compile + link with the extended warning flags, in a separate build
# dir so the normal objects/hex are never touched. Warnings only, no -Werror.
# Per-function stack usage lands in build/analyze/*.su
analyze: clean
	$(MAKE) BUI_DIR=$(BUI_DIR)analyze/ CFLAGS="$(ANALYZE_CFLAGS)" hex

# Dead-code link report: prints every section the linker garbage-collects,
# writes a full map with a symbol cross-reference table to build/gc-report.map
gc-report:
	$(CC) $(filter-out -Werror,$(CFLAGS)) -ffunction-sections -fdata-sections \
		-Wl,--gc-sections,--print-gc-sections,--cref,-Map,$(BUI_DIR)gc-report.map \
		-o $(BUI_DIR)gc-report.bin $(SRC)

# Cross-file unused function/variable analysis (host tool, no avr-gcc needed).
# unusedVariable was merged into default checking in cppcheck >= 2.11, so only
# unusedFunction remains in --enable. __AVR_ATmega2560__ is normally defined
# by avr-gcc's -mmcu; cppcheck needs it explicitly to resolve <avr/io.h>.
CPPCHECK_FLAGS	=	--enable=unusedFunction --suppress=missingIncludeSystem --std=c11 \
					-D__AVR_ATmega2560__=1 -DF_CPU=16000000UL
cppcheck:
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck $(CPPCHECK_FLAGS) $(INC_FLAGS) $(SRC); \
	else echo "cppcheck not found: brew install cppcheck"; fi

-include $(DEP)
