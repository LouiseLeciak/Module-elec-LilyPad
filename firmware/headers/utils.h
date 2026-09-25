#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>

#include "storage/fatfs.h"
#include "types.h"

void dump_buf(uint8_t* buf, uint16_t len);

void dump_mbr(void);
void dump_partition_entry(PARTITION_ENTRY_NO entry_no);

void uart_printhex_32(uint32_t value);
void uart_printhex(uint8_t value);

int ft_strncmp(const char* s1, const char* s2, size_t n);
size_t ft_strlcat(char* dst, const char* src, size_t size);
size_t ft_strlen(const char* str);

#endif  // !UTILS_H
