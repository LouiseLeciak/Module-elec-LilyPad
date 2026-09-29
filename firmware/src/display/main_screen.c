#include "display/main_screen.h"

#include <util/delay.h>

#include "display/ili9488.h"
#include "structs.h"
#include "system/spi.h"
#include "system/uart.h"

// NOTE: I am puzzled about whether SPI transaction start/end should be set
// here or whether it should be even higher level. I'd wager it should be
// higher level even though instinctively I would want to put them here but
// what if we wanted to run several high/low level commands ? Doing the
// stqrt/end transaction here would just bounce the Slave Select SPI line.
// Still, I left the calls commented just in case, but I am pretty positive we
// will end up managing the transaction start/end by ourselves at a higher
// level. (apetitco)

// --- HIGH LEVEL COMMANDS -----------------------------------------------------
