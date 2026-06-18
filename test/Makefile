# --- MICROCONTROLLER & FLASHING CONFIG ---
MCU					:=	atmega2560
F_CPU				:=	16000000UL

# --- DIRECTORIES ---
SRC_DIR	:=	../src/
INC_DIR	:=	../headers/
BUI_DIR	:=	build/
UNI_DIR	:=	../unity/

# --- EXTERNAL LIBRARIES ---
LIBUNITY	:=	libunity.a

# --- TOOLCHAIN ---
CC			:=	avr-gcc
OBJCOPY	:=	avr-objcopy

# --- COMPILER FLAGS ---
CFLAGS	=	-Wall -Wextra -Werror -I$(INC_DIR) $(SIMAVR_INC) -I$(UNI_DIR)src -L. -lunity -mmcu=$(MCU) -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -MMD -MP -Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000

# --- OS DEPENDENT SIMAVR INCLUDES ---
UNAME_S	:=	$(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	SIMAVR_INC := -I/opt/homebrew/opt/simavr/include
else
	SIMAVR_INC := -I/usr/include/simavr
endif

# --- SOURCE & OBJECT FILES ---
vpath %.c $(SRC_DIR) $(UNI_DIR)src .

PROD_SRCS   := $(filter-out $(SRC_DIR)main.c, $(wildcard $(SRC_DIR)*.c))

SRC_FIL     := main_test.c \
							 unity.c \
							 $(notdir $(PROD_SRCS))

OBJ         := $(addprefix $(BUI_DIR), $(SRC_FIL:.c=.o))
DEP	=	$(OBJ:.o=.d)

# --- OUTPUT FILES ---
TARGET	=	test_runner

# --- BUILD RULES ---
.PHONY: all clean fclean re

all: $(TARGET).hex
	simavr -m $(MCU) -f $(F_CPU) ./$(TARGET).hex

$(TARGET).hex: $(TARGET)
	$(OBJCOPY) -O ihex $< $@

$(TARGET): $(OBJ) $(LIBUNITY)
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(LIBUNITY):
	mkdir -p $(UNI_DIR)build
	cd $(UNI_DIR)build && cmake .. && make
	mv $(UNI_DIR)build/libunity.a .

$(BUI_DIR)unity.o: $(UNI_DIR)/src/unity.c | $(BUI_DIR)
	$(CC) $(CFLAGS) -Wno-error=clobbered -c $< -o $@

$(BUI_DIR)%.o: %.c | $(BUI_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUI_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUI_DIR) $(TARGET).hex

fclean:
	rm -rf $(TARGET) $(LIBUNITY) $(UNI_DIR)build

re: fclean all

-include $(DEP)
