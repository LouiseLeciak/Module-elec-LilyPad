#include "pinout.h"
#include "GC9A01.h"
#include "spi.h"
#include "main_screen.h"
#include "keyboard_utils.h"
#include "keypad.h"
#include "i2c_rotary.h"
#include "language.h"

void sd_init() { DDRH |= (SD_CS); }

void eyes_init()
{

    DDRE |= (LEFT_EYE_CS | RIGHT_EYE_CS | EYES_RST);
    GC9A01_init(LEFT_EYE);
    GC9A01_init(RIGHT_EYE);
}

void screens_init()
{
    DDRH |= (SCREENS_DC);
    main_screen_init();
    eyes_init();
}

void init(void)
{

    spi_master_init();

    screens_init();
    i2c_init();
    mcp_init();

    keypad_init();
    rotary_init();
    language_switch_init();

}