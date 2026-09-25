#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>

#include "storage/fatfs.h"

void dump_buf(uint8_t* buf, uint16_t len);

void dump_mbr(void);
void dump_partition_entry(PARTITION_ENTRY_NO entry_no);

void uart_printhex_32(uint32_t value);
void uart_printhex(uint8_t value);

#endif  // !UTILS_H
