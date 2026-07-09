#ifndef SD_CMD_H
#define SD_CMD_H

#include <avr/io.h>
#include <stdint.h>
#include "sd.h"

// NOTE: Commands must have the sixth bit set to 1, so for each command yu have
// to add 40, that's WHY CMD0 is equal to 0x40.

typedef enum s_sd_cmd {
  GO_IDLE_STATE = (0 | 0x40),       ///< CMD0
  SEND_IF_COND = (8 | 0x40),        ///< CMD8
  READ_SINGLE_BLOCK = (17 | 0x40),  ///< CMD17
  SD_SEND_OP_COND = (41 | 0x40),    ///< CMD41
  APP_CMD = (55 | 0x40),            ///< CMD55
  READ_OCR = (58 | 0x40),           ///< CMD58
} SD_CMD;

typedef struct s_r7_response {
  uint8_t _command_index;
  uint8_t _pcie_1_2V;
  uint8_t _pcie;
  uint8_t _voltage;
  uint8_t _echo_check;
  uint8_t _crc7;
} r7_response;

void get_r7_response(r7_response* response);

/**
 * @brief CMD0 - Resets all cards to idle state. When cards supports boot
 * functionalities and receives this commands as the first one in idle state
 * after power up, the argument is regarded as the bus mode in Fast Boot. Refer
 * to Section 4.21.3.3.
 */
uint8_t sd_go_idle_state(void);

/**
 * @brief CMD8 - Sends SD Memory Card interface condition, which includes host
 * supply voltage information ands asks the card whether card supports voltage.
 * Reserved bits shall be set to '0'.
 *
 * Arguments:
 * |                [31:12]                |[11:8] |     [7:0]     |
 * |               Reserved                |  VHS  | Check pattern |
 * |0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|X|X|X|X|X|X|X|X|X|X|X|X|
 *
 * Response: R7 (Card Interface Condition)
 * |Start Bit|Tansmission Bit| CMD Index |          Reserved bits            |PCIe 1.2V|PCIe Response|Voltage accepted|Check pattern echo|    CRC7     |End Bit |
 * |   47    |      46       |  [45:40]  |              [39:22]              |    21   |      20     |    [19:16]     |      [15:8]      |    [7:1]    |   1    |
 * |    0    |       0       |0|0|1|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|    X    |      X      | X | X | X | X  | X|X|X|X|X|X|X|X  |X|X|X|X|X|X|X|   1    |
 *
 * PCIe 1.2V -> 0 = Not supporting 1.2V ; 1 = Supporting 1.2V
 * PCIe Response -> 0 = Not accepted ; 1 = Accepted
 *
 * Voltage accepted:
 * |0|0|0|0| -> Not defined
 * |0|0|0|1| -> 2.7-3.6V
 * |0|0|1|0| -> Reserved for Low Voltage Range
 * |0|1|0|0| -> Reserved
 * |1|0|0|0| -> Reserved
 *  Others   -> Not defined
 */
uint8_t sd_send_if_cond(void);

/**
 * @brief CMD17 - In the case of a Standard Capacity SD Memory Card, this
 * commmand reads a block of the size selected by the SET_BLOCKLEN command. In
 * case of SDHC, SDXC and SDUC Cards, block length is fixed 512 bytes regardless
 * of the SET_BLOCKLEN command.
 *
 * @param arg Data address
 * @param buf Buffer to be filled with the read bytes.
 * @param buf_siz Max length of the buffer.
 */
void sd_read_single_block(const sd_card card,
                          const uint32_t arg,
                          uint8_t* buf,
                          const uint16_t buf_siz);

#endif  // !SD_CMD_H
