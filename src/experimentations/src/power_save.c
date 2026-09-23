
#include "power_save.h"
#include "globals.h"
#include "pinout.h"

uint32_t inactivity_counter = 0;

// timer to know since when nothing is happening
void power_save_update(void)
{
    // 30000 ~= 23s
    if (inactivity_counter < 30000)
        inactivity_counter++;
        
    if (inactivity_counter >= 30000)
    {
        PORTH &= ~MAIN_SCREEN_BL; // on eteint
    }
    return;
}

// ping if activity
void power_save_activity(void)
{
    inactivity_counter = 0;

    PORTH |= MAIN_SCREEN_BL;
}
