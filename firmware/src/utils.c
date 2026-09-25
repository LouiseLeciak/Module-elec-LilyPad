#include "utils.h"

#include "storage/fatfs.h"
#include "uart.h"

#define COL_RED "\x1b[31m"
#define COL_GREEN "\x1b[32m"
#define COL_YELLOW "\x1b[33m"
#define COL_BLUE "\x1b[34m"
#define COL_PURPLE "\x1b[35m"
#define COL_CYAN "\x1b[36m"
#define COL_RESET "\x1b[0m"
#define COL_GRAY "\x1b[38;5;248m"

#define COL_BOLD "\x1b[1m"

#define COL_BGBLACK "\x1b[47m"

#define MBR_SIZE 512
#define PART_ENTRY_SIZE 16
#define BOOT_SIGNATURE 0x01FE
#define BOOT_SIGNATURE_SIZE 2

void uart_printhex(uint8_t value) {
  char hex[] = "0123456789abcdef";
  char buf[3];

  uint8_t first = value / 16;
  uint8_t sec = value % 16;

  buf[0] = hex[first];
  buf[1] = hex[sec];
  buf[2] = '\0';

  uart_printstr(buf);
}

void uart_printhex_32(uint32_t value) {
  char hex[] = "0123456789abcdef";
  char buf[9];

  uint8_t i1 = (value / 268435456) % 16;  // 16^7
  uint8_t i2 = (value / 16777216) % 16;   // 16^6
  uint8_t i3 = (value / 1048576) % 16;    // 16^5
  uint8_t i4 = (value / 65536) % 16;      // 16^4
  uint8_t i5 = (value / 4096) % 16;       // 16^3
  uint8_t i6 = (value / 256) % 16;        // 16^2
  uint8_t i7 = (value / 16) % 16;         // 16^1
  uint8_t i8 = value % 16;                // 16^0

  buf[0] = hex[i1];
  buf[1] = hex[i2];
  buf[2] = hex[i3];
  buf[3] = hex[i4];
  buf[4] = hex[i5];
  buf[5] = hex[i6];
  buf[6] = hex[i7];
  buf[7] = hex[i8];
  buf[8] = '\0';

  uart_printstr(buf);
}

static void dump_mbr_desc(void) {
  uart_printstr(COL_RED);
  uart_printstr("Partition entries (1,2,3,4)");
  uart_printstr(COL_RESET);
  uart_printstr(" ; ");
  uart_printstr(COL_CYAN);
  uart_printstr("Signature");
  uart_printstr(COL_RESET);
  uart_printstr(" ; ");

  uart_printstr(COL_RESET);
  uart_printstr("\r\n");
}

void dump_partition_entry(PARTITION_ENTRY_NO entry_no) {
  if (entry_no != PART_ENTRY_NO_1 && entry_no != PART_ENTRY_NO_2 &&
      entry_no != PART_ENTRY_NO_3 && entry_no != PART_ENTRY_NO_4)
    return;

  uint8_t mbr_buf[MBR_SIZE] = {0};
  DRESULT res = disk_read(0, mbr_buf, 0, 1);
  if (res != RES_OK) {
    uart_printstr("disk_read failed\r\n");
    return;
  }

  /*
   * Point to the selected 16-byte partition entry inside the MBR buffer.
   *
   * MBR Partition Entry layout (16 bytes):
   *  [0]     Status          (0x80 = bootable, 0x00 = not bootable)
   *  [1-3]   CHS of first sector
   *  [4]     Partition type
   *  [5-7]   CHS of last sector
   *  [8-11]  LBA of first sector  (little-endian DWORD)
   *  [12-15] Number of sectors    (little-endian DWORD)
   */
  uint8_t* pe = &mbr_buf[entry_no];

  uart_printstr(COL_BOLD);
  uart_printstr("Partition entry @ 0x");
  uart_printhex_32((uint32_t)entry_no);
  uart_printstr(COL_RESET);
  uart_printstr("\r\n");

  uart_printstr("  Status          : ");
  uart_printhex(pe[0]);
  if (pe[0] == 0x80)
    uart_printstr(" (bootable)");
  else if (pe[0] == 0x00)
    uart_printstr(" (not bootable)");
  uart_printstr("\r\n");

  uart_printstr("  CHS first sector: ");
  uart_printhex(pe[1]);
  uart_tx(' ');
  uart_printhex(pe[2]);
  uart_tx(' ');
  uart_printhex(pe[3]);
  uart_printstr("\r\n");

  uart_printstr("  Partition type  : ");
  uart_printhex(pe[4]);
  uart_printstr("\r\n");

  uart_printstr("  CHS last sector : ");
  uart_printhex(pe[5]);
  uart_tx(' ');
  uart_printhex(pe[6]);
  uart_tx(' ');
  uart_printhex(pe[7]);
  uart_printstr("\r\n");

  /* LBA first sector (little-endian 32-bit) */
  uint32_t lba_start = (uint32_t)pe[8] | (uint32_t)pe[9] << 8 |
                       (uint32_t)pe[10] << 16 | (uint32_t)pe[11] << 24;
  uart_printstr("  LBA first sector: 0x");
  uart_printhex_32(lba_start);
  uart_printstr("\r\n");

  /* Number of sectors (little-endian 32-bit) */
  uint32_t num_sectors = (uint32_t)pe[12] | (uint32_t)pe[13] << 8 |
                         (uint32_t)pe[14] << 16 | (uint32_t)pe[15] << 24;
  uart_printstr("  Num sectors     : 0x");
  uart_printhex_32(num_sectors);
  uart_printstr("\r\n");
}

void dump_mbr(void) {
  uint8_t mbr_buf[MBR_SIZE] = {0};
  DRESULT res = disk_read(0, mbr_buf, 0, 1);
  if (res != RES_OK) {
    uart_printstr("disk_read failed\r\n");
    return;
  }

  dump_mbr_desc();

  for (uint16_t addr = 0; addr < MBR_SIZE; addr += 16) {
    uart_printhex_32((uint32_t)addr);
    uart_tx(' ');

    for (uint8_t i = 0; i < 16 && (addr + i) < MBR_SIZE; i++) {
      uint16_t val = mbr_buf[addr + i];
      if (addr + i >= PART_ENTRY_NO_1 &&
          addr + i < PART_ENTRY_NO_1 + PART_ENTRY_SIZE) {
        uart_printstr(COL_RED);
        uart_printhex(val);
        uart_printstr(COL_RESET);
      } else if (addr + i >= PART_ENTRY_NO_2 &&
                 addr + i < PART_ENTRY_NO_2 + PART_ENTRY_SIZE) {
        uart_printstr(COL_RED);
        uart_printhex(val);
        uart_printstr(COL_RESET);
      } else if (addr + i >= PART_ENTRY_NO_3 &&
                 addr + i < PART_ENTRY_NO_3 + PART_ENTRY_SIZE) {
        uart_printstr(COL_RED);
        uart_printhex(val);
        uart_printstr(COL_RESET);
      } else if (addr + i >= PART_ENTRY_NO_4 &&
                 addr + i < PART_ENTRY_NO_4 + PART_ENTRY_SIZE) {
        uart_printstr(COL_RED);
        uart_printhex(val);
        uart_printstr(COL_RESET);
      } else if (addr + i >= BOOT_SIGNATURE &&
                 addr + i < BOOT_SIGNATURE + BOOT_SIGNATURE_SIZE) {
        uart_printstr(COL_CYAN);
        uart_printhex(val);
        uart_printstr(COL_RESET);
      } else {
        uart_printhex(val);
      }
      uart_tx(' ');
    }

    uart_tx('|');

    for (uint8_t i = 0; i < 16 && (addr + i) < MBR_SIZE; i++) {
      uint8_t value = mbr_buf[addr + i];
      if (value >= 32 && value <= 126)
        uart_tx(value);
      else
        uart_tx('.');
    }

    uart_tx('|');
    uart_printstr("\r\n");
  }
}

void dump_buf(uint8_t* buf, uint16_t len) {
  for (uint16_t addr = 0; addr < len; addr += 16) {
    uart_printhex_32((uint32_t)addr);
    uart_tx(' ');

    for (uint8_t i = 0; i < 16 && (addr + i) < len; i++) {
      uart_printhex(buf[addr + i]);
      uart_tx(' ');
    }

    uart_tx('|');

    for (uint8_t i = 0; i < 16 && (addr + i) < len; i++) {
      uint8_t value = buf[addr + i];
      if (value >= 32 && value <= 126)
        uart_tx(value);
      else
        uart_tx('.');
    }

    uart_tx('|');
    uart_printstr("\r\n");
  }
}

int    ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t    i;
    int        result;

    if (n == 0)
        return (0);
    i = 0;
    while (i < n && (s1[i] || s2[i]))
    {
        if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i])
        {
            result = ((unsigned char *)s1)[i] - ((unsigned char *)s2)[i];
            return (result);
        }
        i++;
    }
    return (0);
}

size_t    ft_strlcat(char *dst, const char *src, size_t size)
{
    size_t    src_len;
    size_t    dst_len;
    size_t    i;

    if (size == 0)
        return (ft_strlen((char *)src));
    src_len = ft_strlen((char *)src);
    dst_len = ft_strlen(dst);
    i = 0;
    if (size <= dst_len)
        return (size + src_len);
    while (src[i] && dst_len + i < size - 1)
    {
        dst[dst_len + i] = src[i];
        i++;
    }
    dst[dst_len + i] = 0;
    return (dst_len + src_len);
}